#ifndef RECIPE_H
#define RECIPE_H

#include <QMetaType>
#include <QString>

#include <vector>

class Recipe
{
public:
    struct RecipeInput
    {
        QString name;
        unsigned int qty {0U};
    };

    QString outputName;
    unsigned int outputQty {0U};
    unsigned int productionTime {0U};
    QString buildingName;
    std::vector<RecipeInput> inputs;
    bool active {false};
};

Q_DECLARE_METATYPE(Recipe*)

#endif // RECIPE_H
