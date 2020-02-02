#ifndef FACTORYMODEL_H
#define FACTORYMODEL_H

#include "factorynode.h"

#include <unordered_map>
#include <QHash>
#include <QAbstractItemModel>

#include <unordered_map>

class QIcon;
class RecipeModel;

class FactoryModel : public QAbstractItemModel
{
    std::vector<FactoryNode*> OutputNodes;
    std::unordered_map<QString, QIcon*>& iconDatabase;
    RecipeModel& modelRef;

    std::unordered_map<QString, FactoryNode*> buildingSet;

    void deleteTree(FactoryNode*);
    FactoryNode* addNode(const Recipe* newNode, double nodeRate, FactoryNode* parentNode);
    void consolidateNode(FactoryNode*);

public:
    FactoryModel(RecipeModel& recipeModelRef, std::unordered_map<QString, QIcon*>& iconDB, QObject* parent = nullptr);
    FactoryModel() = delete;
    FactoryModel(const FactoryModel&) = delete;
    FactoryModel(FactoryModel&&) = delete;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    const std::unordered_map<QString, FactoryNode*>& getBuildingSet() const
    {
        return buildingSet;
    }

    void addOutput(const QString& newOutput, double outputRate);
    void consolidate();

    void clear();
};

#endif // FACTORYMODEL_H
