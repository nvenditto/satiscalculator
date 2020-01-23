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
    if(parent.isValid())
    {
        if(parent.internalId() == 0)
        {
            // output names have one or more recipes
            auto key = recipeKeys.at(static_cast<size_t>(parent.row()));

            auto keyCount = static_cast<int>(recipeDB.count(key));

            return keyCount;
        }
        else
        {
            // recipe entries have no children
            return 0;
        }
    }

    return static_cast<int>(recipeKeys.size());
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
        return {};
    }

    try
    {
        if(index.internalId() == 0)
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
            else
            {
                return {};
            }
        }
        else
        {
            // this is a recipe entry
            auto key = recipeKeys.at(index.internalId()-1);
            auto recipes = recipeDB.equal_range(key);

            int entryIndex = 0;
            for(auto& recipeEntry = recipes.first; recipeEntry != recipes.second; ++recipeEntry)
            {
                if(entryIndex == index.row())
                {
                    auto theRecipe = recipeEntry->second;
                    QIcon* iconPtr {nullptr};
                    QVariant retVar{};

                    switch(role)
                    {
                    case Qt::DisplayRole:
                        return {theRecipe->outputName};

                    case Qt::DecorationRole:
                        iconPtr = iconDatabase.at(theRecipe->outputName);
                        retVar = QVariant::fromValue(*iconPtr);
                        return retVar;

                    case OutputQtyRole:
                        return {theRecipe->outputQty};

                    case ProductionTimeRole:
                        return {theRecipe->productionTime};

                    case BuildingNameRole:
                        return {theRecipe->buildingName};

                    case Input1NameRole:
                        return {theRecipe->inputs[0].name};

                    case Input1IconRole:
                        iconPtr = iconDatabase.at(theRecipe->inputs[0].name);
                        retVar = QVariant::fromValue(*iconPtr);
                        return retVar;

                    case Input1QtyRole:
                        return {theRecipe->inputs[0].qty};


                    case Input2NameRole:
                        if(theRecipe->inputs.size() > 1)
                        {
                            return {theRecipe->inputs[1].name};
                        }
                        else
                        {
                            return {};
                        }

                    case Input2IconRole:
                        if(theRecipe->inputs.size() > 1)
                        {
                            iconPtr = iconDatabase.at(theRecipe->inputs[1].name);
                            retVar = QVariant::fromValue(*iconPtr);
                            return retVar;
                        }
                        else
                        {
                            return {};
                        }

                    case Input2QtyRole:
                        if(theRecipe->inputs.size() > 1)
                        {
                            return {theRecipe->inputs[1].qty};
                        }
                        else
                        {
                            return {};
                        }


                    case Input3NameRole:
                        if(theRecipe->inputs.size() > 2)
                        {
                            return {theRecipe->inputs[2].name};
                        }
                        else
                        {
                            return {};
                        }

                    case Input3IconRole:
                        if(theRecipe->inputs.size() > 2)
                        {
                            iconPtr = iconDatabase.at(theRecipe->inputs[2].name);
                            retVar = QVariant::fromValue(*iconPtr);
                            return retVar;
                        }
                        else
                        {
                            return {};
                        }

                    case Input3QtyRole:
                        if(theRecipe->inputs.size() > 2)
                        {
                            return {theRecipe->inputs[2].qty};
                        }
                        else
                        {
                            return {};
                        }


                    case Input4NameRole:
                        if(theRecipe->inputs.size() > 3)
                        {
                            return {theRecipe->inputs[3].name};
                        }
                        else
                        {
                            return {};
                        }

                    case Input4IconRole:
                        if(theRecipe->inputs.size() > 3)
                        {
                            iconPtr = iconDatabase.at(theRecipe->inputs[3].name);
                            retVar = QVariant::fromValue(*iconPtr);
                            return retVar;
                        }
                        else
                        {
                            return {};
                        }

                    case Input4QtyRole:
                        if(theRecipe->inputs.size() > 3)
                        {
                            return {theRecipe->inputs[3].qty};
                        }
                        else
                        {
                            return {};
                        }

                    default:
                        return {};
                    }
                }
                ++entryIndex;
            }
        }
    } catch(std::exception e)
    {
        return {};
    }
    return {};
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
        // The parent is the root item
        return createIndex(row, column, KeyID);
    }
    else if(parent.internalId() == 0)
    {
        auto key = recipeKeys.at(static_cast<size_t>(parent.row()));

        auto recipes = recipeDB.equal_range(key);

        int entryIndex = 0;

        for(auto& recipeEntry = recipes.first; recipeEntry != recipes.second; ++recipeEntry)
        {
            if(entryIndex == row)
            {
                auto childID = static_cast<quintptr>(parent.row() + 1);
                return createIndex(row, column, childID);
            }
            ++entryIndex;
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

    if(child.internalId() == 0)
    {
        return QModelIndex();
    }

    int newRow = static_cast<int>(child.internalId()) - 1;

    return createIndex(newRow, 0, KeyID);
}

void RecipeModel::addRecipe(const RecipeModel::Recipe &newRecipe)
{
    auto recordedRecipe = new Recipe(newRecipe);

    if(recipeDB.count(newRecipe.outputName) > 0)
    {
        // We already have at a key for this recipe, so add an additional child
        recipeDB.insert({newRecipe.outputName, recordedRecipe});
    }
    else
    {
        // No key exists yet for this recipe, so add new row
        int newRowCount = static_cast<int>(recipeKeys.size()) + 1;
        beginInsertRows(QModelIndex(), newRowCount, newRowCount);

            recipeKeys.push_back(newRecipe.outputName);

            recipeDB.insert({newRecipe.outputName, recordedRecipe});

        endInsertRows();
    }
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
        RecipeInput newInput;
        newInput.name = inputEntry.key();
        newInput.qty = inputEntry.value().toUInt();

        newRecipe->inputs.push_back(newInput);
    }

    // If we don't have the key already, add it to the key list
    if(recipeDB.count(outputName) == 0)
    {
        recipeKeys.push_back(outputName);
    }

    recipeDB.insert({newRecipe->outputName, newRecipe});
}
