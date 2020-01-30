#include "productionmodel.h"
#include <QIcon>

ProductionModel::ProductionModel(std::unordered_map<QString, QIcon*>& iconDB, QObject *parent) :
    QAbstractTableModel(parent),
    iconDatabase(iconDB)
{

}

int ProductionModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }

    return static_cast<int>(outputList.size());
}

int ProductionModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }

    return 2;
}

QVariant ProductionModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= rowCount())
    {
        return {};
    }

    try {
        if(index.column() == 0)
        {
            if(role == Qt::DisplayRole)
            {
                auto outputEntry = outputList.at(static_cast<size_t>(index.row()));
                return QVariant{outputEntry.outputName};
            }
            else if(role == Qt::DecorationRole)
            {
                auto outputEntry = outputList.at(static_cast<size_t>(index.row()));
                auto outputIcon = iconDatabase.at(outputEntry.outputName);
                return QVariant{*outputIcon};
            }
        } else if (index.column() == 1)
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                auto outputEntry = outputList.at(static_cast<size_t>(index.row()));
                return QVariant{outputEntry.qty};
            }
        }


    } catch (std::exception e) {
        qWarning("Unable to access data for index row = %s, col = %s",
                 qUtf8Printable(QString::number(index.row())),
                 qUtf8Printable(QString::number(index.column())));
    }

    return {};
}

QVariant ProductionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0)
        {
            return {"Item"};
        }
        else if(section == 1)
        {
            return {"Rate"};
        }
    }

    return {};
}

bool ProductionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // The only thing that can be set is the checkbox for a recipe
    if(index.isValid() && index.column() == 1 && role == Qt::EditRole && value.canConvert<double>())
    {
        try {
            OutputItem& outputEntry = outputList.at(static_cast<size_t>(index.row()));

            outputEntry.qty = value.toDouble();

            emit dataChanged(index, index);
            return true;
        }
        catch(std::exception e)
        {
            qWarning("Unable to set data for index with row = %s, column = %s",
                     qUtf8Printable(QString::number(index.row())),
                     qUtf8Printable(QString::number(index.column())));
        }
    }

    return false;
}

Qt::ItemFlags ProductionModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        // This is the root node, you get NOTHING
        return Qt::NoItemFlags;
    }
    else if(index.column() == 0)
    {
        // this is an output name
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    else if(index.column() == 1)
    {
        // this is a recipe entry
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    }
    else
    {
        return Qt::NoItemFlags;
    }
}

bool ProductionModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(row >= 0 && static_cast<size_t>(row) <= outputList.size())
    {
        beginInsertRows(parent, row, (row + count - 1));

            auto iterator = outputList.begin();
            outputList.insert(iterator + row, static_cast<size_t>(count), OutputItem{});

        endInsertRows();

        return true;
    }
    else
    {
        return false;
    }
}

bool ProductionModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(row >= 0 && count >= 0 && static_cast<size_t>(row + count) <= outputList.size())
    {
        beginRemoveRows(parent, row, (row + count - 1));

            auto firstItem = outputList.begin() + row;
            auto end = outputList.begin() + (row + count);

            outputList.erase(firstItem, end);

        endRemoveRows();

        return true;
    }

    return false;
}


std::vector<QModelIndex> ProductionModel::findItems(const QString &searchText, Qt::CaseSensitivity sensitivity) const
{
    std::vector<QModelIndex> foundItems;

    int rowIndex = 0;
    for(const auto& outEntry : outputList)
    {
        if(outEntry.outputName.compare(searchText, sensitivity) == 0)
        {
            foundItems.push_back(createIndex(rowIndex, 0));
        }

        ++rowIndex;
    }

    return foundItems;
}

void ProductionModel::appendRow(QString outputName, double outputQty)
{
    auto newRowNumber = static_cast<int>(outputList.size());

    beginInsertRows(QModelIndex(), newRowNumber, newRowNumber);

    outputList.push_back({outputName, outputQty});

    endInsertRows();
}

void ProductionModel::clear()
{
    beginResetModel();

        outputList.clear();

        endResetModel();
}

