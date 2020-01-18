#ifndef RECIPEMODEL_H
#define RECIPEMODEL_H

#include <QAbstractTableModel>
#include <unordered_map>
#include <vector>
#include <QHash>

namespace std {
  template<> struct hash<QString> {
    std::size_t operator()(const QString& s) const noexcept {
      return (size_t) qHash(s);
    }
  };
}

class RecipeModel : public QAbstractTableModel
{
public:
    struct RecipeInput
    {
        QString name;
        unsigned int qty {0U};
    };

    struct Recipe
    {
        QString outputName;
        unsigned int outputQty {0U};
        unsigned int productionTime {0U};
        QString buildingName;
        std::vector<RecipeInput> inputs;

        Recipe() = default;

        Recipe(const Recipe& other)
        {
            outputName = other.outputName;
            outputQty = other.outputQty;
            productionTime = other.productionTime;
            buildingName = other.buildingName;
            inputs = other.inputs;
        }
    };

    explicit RecipeModel(QObject* parent = nullptr);

    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;


    void addRecipe(const Recipe& newRecipe);

protected:

    void loadRecipes();

    static constexpr int columnCountVal = 12U; // accounts for up to 4 inputs
    std::unordered_map<QString, Recipe> recipeDB;
    std::vector<QString> recipeList;
};

#endif // RECIPEMODEL_H
