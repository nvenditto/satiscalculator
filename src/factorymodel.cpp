#include "factorymodel.h"

#include "factorynode.h"
#include "recipemodel.h"

#include <QIcon>

void FactoryModel::deleteTree(FactoryNode* nodePtr)
{
    if(nodePtr->getSources().size() > 0)
    {
        for(auto nodeEntry : nodePtr->getSources())
        {
            deleteTree(nodeEntry);
        }
    }
    else
    {
        delete nodePtr;
    }
}

FactoryNode* FactoryModel::addNode(const Recipe *newRecipe, double nodeRate, FactoryNode* parentNode)
{
    double singleProdRate = newRecipe->outputQty * (60.0 / newRecipe->productionTime);

    double numNodes = nodeRate / singleProdRate;

    auto newFactoryNode = new FactoryNode(newRecipe->outputName,
                                   numNodes,
                                   newRecipe->buildingName,
                                   newRecipe);

    if(parentNode != nullptr)
    {
        parentNode->addSourceNode(newFactoryNode);
    }

    if(newRecipe->inputs.size() > 0)
    {
        for(const auto& input : newRecipe->inputs)
        {
            double singleInputRate = input.qty * (60.0 / newRecipe->productionTime);
            double totalInputRate = singleInputRate * numNodes;

            auto childRecipe = modelRef.LookupRecipe(input.name);

            if(childRecipe != nullptr)
            {
                addNode(childRecipe, totalInputRate, newFactoryNode);
            }
        }
    }

    return newFactoryNode;
}

void FactoryModel::consolidateNode(FactoryNode* currentNode)
{
    // If node is already in the set, update instance count
    if(buildingSet.count(currentNode->getOutputName()) > 0)
    {
        auto existingNode = buildingSet[currentNode->getOutputName()];
        auto prevNodeCount = existingNode->getInstanceCount();
        existingNode->setInstanceCount(prevNodeCount + currentNode->getInstanceCount());
    }
    // otherwise, add node to list
    else
    {
        auto newNode = new FactoryNode(*currentNode);
        buildingSet[currentNode->getOutputName()] = newNode;
    }

    // Now cycle through each of this node's inputs
    for(const auto& inputNode : currentNode->getSources())
    {
        consolidateNode(inputNode);
    }
}

FactoryModel::FactoryModel(RecipeModel &recipeModelRef, std::unordered_map<QString, QIcon*>& iconDB, QObject* parent) :
    QAbstractItemModel(parent),
    iconDatabase(iconDB),
    modelRef(recipeModelRef)

{

}

QModelIndex FactoryModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!parent.isValid())
    {
        if(static_cast<size_t>(row) < OutputNodes.size() && column == 0)
        {
            auto nodePtr = OutputNodes.at(static_cast<size_t>(row));

            return createIndex(row, column, nodePtr);
        }

    }
    else
    {
        auto parentNode = static_cast<FactoryNode*>(parent.internalPointer());

        if(parentNode != nullptr && static_cast<size_t>(row) < parentNode->getSources().size() && column == 0)
        {
            auto nodePtr = parentNode->getSources().at(static_cast<size_t>(row));

            return createIndex(row, column, nodePtr);
        }
    }

    qWarning("Unable to create index for row = %s, col = %s, parent = %s",
             qUtf8Printable(QString::number(row)),
             qUtf8Printable(QString::number(column)),
             qUtf8Printable(QString::number(parent.internalId())));
    return {};
}

QModelIndex FactoryModel::parent(const QModelIndex &child) const
{
    if(!child.isValid() || child.internalPointer() == nullptr)
    {
        return QModelIndex();
    }

    auto childNode = static_cast<FactoryNode*>(child.internalPointer());

    auto parentNode = childNode->getDestination();

    if(parentNode == nullptr)
    {
        return {};
    }

    auto grandParent = parentNode->getDestination();

    if(grandParent == nullptr)
    {
        int row = 0;
        for(const auto& outputEntry : OutputNodes)
        {
            if(outputEntry == parentNode)
            {
                return createIndex(row, 0, parentNode);
            }

            ++row;
        }
    }
    else
    {
        int row = 0;
        for(const auto& outputEntry : grandParent->getSources())
        {
            if(outputEntry == parentNode)
            {
                return createIndex(row, 0, parentNode);
            }

            ++row;
        }
    }


    qWarning("Unable to find parent for index with row = %s, col = %s, parent = %s",
             qUtf8Printable(QString::number(child.row())),
             qUtf8Printable(QString::number(child.column())),
             qUtf8Printable(QString::number(child.internalId())));
    return {};
}

int FactoryModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
    {
        return static_cast<int>(OutputNodes.size());
    }
    else
    {
        auto currentNode = static_cast<FactoryNode*>(parent.internalPointer());

        return static_cast<int>(currentNode->getSources().size());
    }
}

int FactoryModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 1;
}

QVariant FactoryModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.column() > 0 || index.internalPointer() == nullptr)
    {
        return {};
    }

    auto node = static_cast<FactoryNode*>(index.internalPointer());

    if(role == Qt::DisplayRole)
    {
        return {node->getOutputName()};
    }
    else if(role == Qt::DecorationRole)
    {
        auto icon = iconDatabase.at(node->getOutputName());
        return {*icon};
    }

    return {};
}

void FactoryModel::addOutput(const QString& newOutput, double outputRate)
{
    beginResetModel();

    auto outputRecipe = modelRef.LookupRecipe(newOutput);

    if(outputRecipe != nullptr)
    {
        auto outputNode = addNode(outputRecipe, outputRate, nullptr);

        OutputNodes.push_back(outputNode);
    }

    endResetModel();
}

void FactoryModel::consolidate()
{
    for(const auto& outputNode : OutputNodes)
    {
        consolidateNode(outputNode);
    }
}

void FactoryModel::clear()
{
    beginResetModel();

    for(auto nodeEntry : OutputNodes)
    {
        deleteTree(nodeEntry);
    }

    OutputNodes.clear();

    buildingSet.clear();

    endResetModel();
}
