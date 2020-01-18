#include "productionmodel.h"

ProductionModel::ProductionModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

int ProductionModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }

    return outputList.size();
}

QVariant ProductionModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= rowCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        auto outputEntry = outputList.at(index.row());
        return QVariant{outputEntry.outputName};
    }

    return QVariant();
}
