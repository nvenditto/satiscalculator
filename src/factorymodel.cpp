#include "factorymodel.h"

#include "factorynode.h"
#include "recipemodel.h"


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

FactoryModel::FactoryModel(RecipeModel &recipeModelRef) :
    modelRef(recipeModelRef)
{

}

FactoryModel::~FactoryModel()
{
    clear();
}

void FactoryModel::addOutput(const QString& newOutput, double outputRate)
{
    auto outputRecipe = modelRef.LookupRecipe(newOutput);

    if(outputRecipe != nullptr)
    {
        auto outputNode = addNode(outputRecipe, outputRate, nullptr);

        OutputNodes.push_back(outputNode);
    }
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
    for(auto nodeEntry : OutputNodes)
    {
        deleteTree(nodeEntry);
    }

    buildingSet.clear();
}
