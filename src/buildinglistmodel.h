#ifndef BUILDINGLISTMODEL_H
#define BUILDINGLISTMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include <unordered_map>

class QIcon;

class FactoryNode;
class FactoryModel;

class BuildingListModel : public QAbstractTableModel
{
    std::vector<FactoryNode*> buildingList;
    std::unordered_map<QString, QIcon*>& iconDatabase;
    static const int numColumns = 4;

public:
    BuildingListModel(std::unordered_map<QString, QIcon*>& iconDB, QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;


    void updateModel(FactoryModel* modelPtr);

};

#endif // BUILDINGLISTMODEL_H
