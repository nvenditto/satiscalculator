#include "factorynode.h"
#include "Recipe.h"

FactoryNode::FactoryNode(const QString &nodeOutputName,
                         double nodeCount,
                         const QString &nodeBuildingName,
                         const Recipe* nodeRecipePtr) :
    OutputName(nodeOutputName),
    instanceCount(nodeCount),
    BuildingName(nodeBuildingName),
    nodeRecipe(nodeRecipePtr)
{

}

void FactoryNode::addSourceNode(FactoryNode *newSourceNode)
{
    if(newSourceNode != nullptr)
    {
        newSourceNode->setDestination(this);
        SourceNodes.push_back(newSourceNode);
    }
}

double FactoryNode::getTotalOutputRate() const
{
    double singleProdRate = nodeRecipe->outputQty * (60.0 / nodeRecipe->productionTime);

    return (singleProdRate * instanceCount);
}
