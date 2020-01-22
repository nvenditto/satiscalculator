#ifndef RECIPEMODEL_H
#define RECIPEMODEL_H

#include <QAbstractItemModel>
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


enum Roles
{
    TitleRole = Qt::UserRole + 1,
    OutputQtyRole,
    ProductionTimeRole,
    BuildingNameRole,
    Input1NameRole,
    Input1IconRole,
    Input1QtyRole,
    Input2NameRole,
    Input2IconRole,
    Input2QtyRole,
    Input3NameRole,
    Input3IconRole,
    Input3QtyRole,
    Input4NameRole,
    Input4IconRole,
    Input4QtyRole
};

class RecipeModel : public QAbstractItemModel
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

    std::unordered_map<QString, QIcon*>& iconDatabase;

    explicit RecipeModel(std::unordered_map<QString, QIcon*>& iconDB, QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    void addRecipe(const Recipe& newRecipe);

protected:

    void loadRecipes();

    void addRecipe(QString outputName, QJsonObject recipeJsonObj);

    static constexpr int columnCountVal = 12U; // accounts for up to 4 inputs
    std::unordered_multimap<QString, Recipe*> recipeDB;
    std::vector<QString> recipeKeys;
};

#endif // RECIPEMODEL_H
