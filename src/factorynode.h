#ifndef FACTORYNODE_H
#define FACTORYNODE_H

#include <vector>
#include <QString>

class Recipe;

class FactoryNode
{
    FactoryNode* DestinationNode {nullptr};
    QString OutputName;
    double instanceCount;
    QString BuildingName;
    std::vector<FactoryNode*> SourceNodes;

    const Recipe* nodeRecipe {nullptr};

public:

    FactoryNode(const QString& nodeOutputName, double nodeCount, const QString& nodeBuildingName, const Recipe* nodeRecipePtr);
    FactoryNode() = delete;
    ~FactoryNode() = default;
    FactoryNode(const FactoryNode&) = default;
    FactoryNode(FactoryNode&&) = default;
    FactoryNode& operator=(const FactoryNode&) = default;
    FactoryNode& operator=(FactoryNode&&) = default;

    void addSourceNode(FactoryNode* newSourceNode);

    FactoryNode* getDestination() const
    {
        return DestinationNode;
    }

    std::vector<FactoryNode*>& getSources()
    {
        return SourceNodes;
    }

    const QString& getBuildingName() const
    {
        return BuildingName;
    }

    const QString& getOutputName() const
    {
        return OutputName;
    }

    double getInstanceCount() const
    {
        return instanceCount;
    }

    void setInstanceCount(double newCount)
    {
        instanceCount = newCount;
    }

    double getTotalOutputRate() const;

    const Recipe* getRecipe() const
    {
        return nodeRecipe;
    }

    size_t getNumSources() const
    {
        return SourceNodes.size();
    }

};

#endif // FACTORYNODE_H
