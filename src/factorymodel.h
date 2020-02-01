#ifndef FACTORYMODEL_H
#define FACTORYMODEL_H

#include "factorynode.h"

#include <unordered_map>
#include <QHash>

class RecipeModel;

class FactoryModel
{
    std::vector<FactoryNode*> OutputNodes;
    RecipeModel& modelRef;

    std::unordered_map<QString, FactoryNode*> buildingSet;

    void deleteTree(FactoryNode*);
    FactoryNode* addNode(const Recipe* newNode, double nodeRate, FactoryNode* parentNode);
    void consolidateNode(FactoryNode*);

public:
    FactoryModel(RecipeModel& recipeModelRef);
    FactoryModel() = delete;
    ~FactoryModel();
    FactoryModel(const FactoryModel&) = default;
    FactoryModel(FactoryModel&&) = default;

    const std::unordered_map<QString, FactoryNode*>& getBuildingSet() const
    {
        return buildingSet;
    }

    void addOutput(const QString& newOutput, double outputRate);
    void consolidate();

    void clear();
};

#endif // FACTORYMODEL_H
