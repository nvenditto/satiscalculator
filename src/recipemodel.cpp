#include "recipemodel.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QIcon>

constexpr quintptr RecipeModel::KeyID;

RecipeModel::RecipeModel(std::unordered_map<QString, QIcon*>& iconDB, QObject *parent) :
    QAbstractItemModel(parent),
    iconDatabase(iconDB)
{
    loadRecipes();
}

int RecipeModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
    {
        // if this is not a valid index, then it's the root node
        // so we return the number of outputs
        return static_cast<int>(recipeKeys.size());
    }
    else
    {
        if(parent.internalId() == KeyID)
        {
            // If this index is an output key, then return the number of recipes
            try {
                const auto keyString = recipeKeys.at(static_cast<size_t>(parent.row()));
                auto recipeList = recipeDB.at(keyString);

                return static_cast<int>(recipeList.size());
            }
            catch (std::exception e)
            {
                qWarning("Unable to retrieve row count for index with row = %s", qUtf8Printable(QString::number(parent.row())));
                return 0;
            }
        }
        else
        {
            // Otherwise, this is a recipe. Recipes have no children
            return 0;
        }
    }
}

int RecipeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant RecipeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        // This is the root node, you get NOTHING
        return {};
    }

    try
    {
        if(index.internalId() == KeyID)
        {
            // this is an output name
            auto key = recipeKeys.at(static_cast<size_t>(index.row()));

            if(role == Qt::DisplayRole)
            {
                return {key};
            }
            else if(role == Qt::DecorationRole)
            {
                auto iconPtr = iconDatabase.at(key);

                QVariant retVar = QVariant::fromValue(*iconPtr);

                return retVar;
            }
        }
        else
        {
            // this is a recipe entry
            const auto key = recipeKeys.at(index.internalId()-1);
            const auto recipeList = recipeDB.at(key);
            const auto theRecipe = recipeList.at(static_cast<size_t>(index.row()));

            QIcon* iconPtr {nullptr};
            QVariant retVar{};
            Qt::CheckState recipeCheckState {Qt::Unchecked};

            switch(role)
            {
            case Qt::DisplayRole:
                return {theRecipe->outputName};

            case Qt::DecorationRole:
                iconPtr = iconDatabase.at(theRecipe->outputName);
                retVar = QVariant::fromValue(*iconPtr);
                return retVar;

            case Qt::CheckStateRole:
                recipeCheckState = (theRecipe->active) ? Qt::Checked : Qt::Unchecked;
                retVar = QVariant::fromValue(recipeCheckState);
                return retVar;

            case RecipeRole:
                retVar = QVariant::fromValue(theRecipe);
                return retVar;
            }
        }
    } catch(std::exception e)
    {
        qWarning("Unable to retrieve data for index with row = %s, internalID = %s",
                 qUtf8Printable(QString::number(index.row())),
                 qUtf8Printable(QString::number(index.internalId())));
        return {};
    }
    return {};
}

bool RecipeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // The only thing that can be set is the checkbox for a recipe
    if(index.isValid() && index.internalId() != KeyID && role == Qt::CheckStateRole && value == Qt::Checked)
    {
        try {
            const auto key = recipeKeys.at(index.internalId()-1);
            auto recipeList = recipeDB.at(key);

            int loopIndex = 0;
            for(auto* recipeEntry : recipeList)
            {
                recipeEntry->active = (loopIndex == index.row());
                ++loopIndex;
            }

            auto firstRecipe = createIndex(0, index.column(), index.internalId());
            auto lastRecipe = createIndex(static_cast<int>(recipeList.size()) - 1, index.column(), index.internalId());

            emit dataChanged(firstRecipe, lastRecipe);
            return true;
        }
        catch(std::exception e)
        {
            qWarning("Unable to set data for index with row = %s, internalID = %s",
                     qUtf8Printable(QString::number(index.row())),
                     qUtf8Printable(QString::number(index.internalId())));
        }
    }

    return false;
}

Qt::ItemFlags RecipeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        // This is the root node, you get NOTHING
        return Qt::NoItemFlags;
    }
    else if(index.internalId() == KeyID)
    {
        // this is an output name
        return Qt::ItemIsEnabled;
    }
    else
    {
        // this is a recipe entry
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    }
}

QVariant RecipeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
    {
        return {"Recipe"};
    }

    return {};
}

QModelIndex RecipeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    if(!parent.isValid())
    {
        // The parent is the root item, so this is an output
        return createIndex(row, column, KeyID);
    }
    else if(parent.internalId() == KeyID)
    {
        // The parent is an output, so this is a recipe
        try {
            const auto keyString = recipeKeys.at(static_cast<size_t>(parent.row()));
            const auto recipeList = recipeDB.at(keyString);

            if(row < static_cast<int>(recipeList.size()))
            {
                auto childID = static_cast<quintptr>(parent.row() + 1);
                return createIndex(row, column, childID);
            }
        }
        catch (std::exception e)
        {
            qWarning("Error Creating Index for row = %s, ParentRow = %s",
                     qUtf8Printable(QString::number(row)),
                     qUtf8Printable(QString::number(parent.row())));
        }
    }

    return QModelIndex();
}

QModelIndex RecipeModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
    {
        return QModelIndex();
    }

    if(child.internalId() == KeyID)
    {
        return QModelIndex();
    }

    int newRow = static_cast<int>(child.internalId()) - 1;

    return createIndex(newRow, 0, KeyID);
}

void RecipeModel::loadRecipes()
{
    QFile recipesFile(":/builtin/data/recipes.json");

    if(!recipesFile.open(QIODevice::ReadOnly))
    {
        qCritical("Unable to load recipe listing!");
        return;
    }

    QByteArray recipesData = recipesFile.readAll();

    QJsonDocument recipesDoc(QJsonDocument::fromJson(recipesData));
    QJsonObject jsonRoot = recipesDoc.object();

    for(auto jsonIt = jsonRoot.constBegin(); jsonIt != jsonRoot.constEnd(); ++jsonIt)
    {
        const auto entryKey = jsonIt.key();
        const auto entryValue = jsonIt.value();

        // new recipe
        if(entryValue.isObject())
        {
            addRecipe(entryKey, entryValue.toObject());
        }
        else if(entryValue.isArray())
        {
            auto recipeList = entryValue.toArray();
            for(const auto recipeEntry : recipeList)
            {
                if(recipeEntry.isObject())
                {
                    addRecipe(entryKey, recipeEntry.toObject());
                }
                else
                {
                    qWarning("Unrecognized recipe entry found for key %s", qUtf8Printable(entryKey));
                }
            }
        }
        else
        {
            qWarning("Unrecognized recipe entry found for key %s", qUtf8Printable(entryKey));
        }
    }

    // Activate the first recipe entry for all outputs
    for(auto& outputEntry : recipeDB)
    {
        auto& recipeList = outputEntry.second;
        recipeList[0]->active = true;
    }

}

void RecipeModel::addRecipe(QString outputName, QJsonObject recipeJsonObj)
{
    auto newRecipe = new Recipe;
    newRecipe->outputName = outputName;

    const auto entryMap = recipeJsonObj.toVariantMap();

    if(!entryMap.contains("Qty") || !entryMap.contains("Time") || !entryMap.contains("Building") || !entryMap.contains("Inputs"))
    {
        qCritical("Recipe missing required fields for key %s", qUtf8Printable(outputName));
        return;
    }

    newRecipe->outputQty = entryMap.value("Qty").toUInt();
    newRecipe->productionTime = entryMap.value("Time").toUInt();
    newRecipe->buildingName = entryMap.value("Building").toString();

    if(!entryMap.contains("Inputs"))
    {
        qCritical("Recipe is missing inputs for key %s", qUtf8Printable(outputName));
        return;
    }

    auto inputsMap = entryMap.value("Inputs").toMap();

    for(auto inputEntry = inputsMap.cbegin(); inputEntry != inputsMap.cend(); ++inputEntry)
    {
        Recipe::RecipeInput newInput;
        newInput.name = inputEntry.key();
        newInput.qty = inputEntry.value().toUInt();

        newRecipe->inputs.push_back(newInput);
    }


    // If we don't have the key already, add it to the key list
    if(recipeDB.count(outputName) == 0)
    {
        recipeKeys.push_back(outputName);
        recipeDB.insert(std::make_pair(outputName, std::vector<Recipe*>()));
    }

    // Add recipe to recipe list
    recipeDB[outputName].push_back(newRecipe);
}
