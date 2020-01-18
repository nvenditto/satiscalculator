#include "recipemodel.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>


RecipeModel::RecipeModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    loadRecipes();
}

int RecipeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return columnCountVal;
}

int RecipeModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }

    return recipeDB.size();
}

QVariant RecipeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role != Qt::DisplayRole)
    {
        return {};
    }

    const auto& recipeIndex = recipeList.at(index.row());
    const auto& recipeEntry = recipeDB.at(recipeIndex);

    switch(index.column())
    {
    case 0:
        return {recipeEntry.outputName};

    case 1:
        return {recipeEntry.outputQty};

    case 2:
        return {recipeEntry.productionTime};

    case 3:
        return {recipeEntry.buildingName};

    // Input #1
    case 4:
        return {recipeEntry.inputs[0].name};

    case 5:
        return {recipeEntry.inputs[0].qty};
    // Input #2
    case 6:
        if(recipeEntry.inputs.size() > 1)
        {
            return {recipeEntry.inputs[1].name};
        }
        else
        {
            return {};
        }

    case 7:
        if(recipeEntry.inputs.size() > 1)
        {
            return {recipeEntry.inputs[1].qty};
        }
        else
        {
            return {};
        }
    // Input #3
    case 8:
        if(recipeEntry.inputs.size() > 2)
        {
            return {recipeEntry.inputs[2].name};
        }
        else
        {
            return {};
        }

    case 9:
        if(recipeEntry.inputs.size() > 2)
        {
            return {recipeEntry.inputs[2].qty};
        }
        else
        {
            return {};
        }
    // Input #4
    case 10:
        if(recipeEntry.inputs.size() > 3)
        {
            return {recipeEntry.inputs[3].name};
        }
        else
        {
            return {};
        }

    case 11:
        if(recipeEntry.inputs.size() > 3)
        {
            return {recipeEntry.inputs[3].qty};
        }
        else
        {
            return {};
        }

    default:
        return {};
    }
}

QVariant RecipeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation != Qt::Horizontal || role != Qt::DisplayRole )
    {
        return QAbstractTableModel::headerData( section, orientation, role );
    }

    switch(section)
    {
    case 0:
        return {"Item Name"};

    case 1:
        return {"Quantity"};

    case 2:
        return {"Production Time (s)"};

    case 3:
        return {"Production Building"};

    case 4:
        return {"Input #1"};

    case 5:
        return {"Input #1 Qty"};

    case 6:
        return {"Input #3"};

    case 7:
        return {"Input #2 Qty"};

    case 8:
        return {"Input #4"};

    case 9:
        return {"Input #3 Qty"};

    case 10:
        return {"Input #4"};

    case 11:
        return {"Input #4 Qty"};

    }

    return {};
}

void RecipeModel::addRecipe(const RecipeModel::Recipe &newRecipe)
{
    int newRowCount = recipeList.size() + 1;

    beginInsertRows(QModelIndex(), newRowCount, newRowCount);
    recipeList.push_back(newRecipe.outputName);
    recipeDB[newRecipe.outputName]= newRecipe;
    endInsertRows();
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
            Recipe newRecipe;
            newRecipe.outputName = entryKey;

            const auto entryMap = entryValue.toObject().toVariantMap();

            if(!entryMap.contains("Qty") || !entryMap.contains("Time") || !entryMap.contains("Building") || !entryMap.contains("Inputs"))
            {
                qCritical("Recipe missing required fields for key %s", qUtf8Printable(entryKey));
                break;
            }

            newRecipe.outputQty = entryMap.value("Qty").toInt();
            newRecipe.productionTime = entryMap.value("Time").toInt();
            newRecipe.buildingName = entryMap.value("Building").toString();

            if(!entryMap.contains("Inputs"))
            {
                qCritical("Recipe is missing inputs for key %s", qUtf8Printable(entryKey));
                break;
            }

            auto inputsMap = entryMap.value("Inputs").toMap();

            for(auto inputEntry = inputsMap.cbegin(); inputEntry != inputsMap.cend(); ++inputEntry)
            {
                RecipeInput newInput;
                newInput.name = inputEntry.key();
                newInput.qty = inputEntry.value().toInt();

                newRecipe.inputs.push_back(newInput);
            }

            recipeList.push_back(entryKey);
            recipeDB[entryKey]=newRecipe;
        }
        else
        {
            qWarning("Unrecognized recipe entry found for key %s", qUtf8Printable(entryKey));
        }
    }

}
