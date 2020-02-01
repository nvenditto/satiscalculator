#include "buildinglistmodel.h"

#include <QIcon>

#include "factorymodel.h"

BuildingListModel::BuildingListModel(std::unordered_map<QString, QIcon*>& iconDB, QObject *parent) :
    QAbstractTableModel(parent),
    iconDatabase(iconDB)
{

}

int BuildingListModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
    {
        return static_cast<int>(buildingList.size());
    }
    else
    {
        return 0;
    }
}

int BuildingListModel::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
    {
        return numColumns;
    }
    else
    {
        return 0;
    }
}

QVariant BuildingListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= rowCount())
    {
        return {};
    }

    try {
        auto buildingEntry = buildingList[static_cast<size_t>(index.row())];
        if(index.column() == 0)
        {
            if(role == Qt::DisplayRole)
            {
                return QVariant{buildingEntry->getOutputName()};
            }
            else if(role == Qt::DecorationRole)
            {
                auto outputIcon = iconDatabase.at(buildingEntry->getOutputName());
                return QVariant{*outputIcon};
            }
        }
        else if (index.column() == 1)
        {
            if(role == Qt::DisplayRole)
            {
                return QVariant{buildingEntry->getBuildingName()};
            }
        }
        else if (index.column() == 2)
        {
            if(role == Qt::DisplayRole)
            {
                return QVariant{buildingEntry->getInstanceCount()};
            }
        }
        else if (index.column() == 3)
        {
            if(role == Qt::DisplayRole)
            {
                return QVariant{buildingEntry->getTotalOutputRate()};
            }
        }


    } catch (std::exception e) {
        qWarning("Unable to access data for index row = %s, col = %s",
                 qUtf8Printable(QString::number(index.row())),
                 qUtf8Printable(QString::number(index.column())));
    }

    return {};
}

QVariant BuildingListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0)
        {
            return {"Item"};
        }
        else if(section == 1)
        {
            return {"Building"};
        }
        else if(section == 2)
        {
            return {"Building Count"};
        }
        else if(section == 3)
        {
            return {"Output/min"};
        }
    }

    return {};
}

void BuildingListModel::updateModel(FactoryModel *modelPtr)
{
    if(modelPtr != nullptr)
    {
    beginResetModel();

        buildingList.clear();

        auto buildingSet = modelPtr->getBuildingSet();

        for(const auto& buildingEntry : buildingSet)
        {
            buildingList.push_back(buildingEntry.second);
        }

    endResetModel();
    }
}
