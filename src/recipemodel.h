#ifndef RECIPEMODEL_H
#define RECIPEMODEL_H

#include <QAbstractItemModel>
#include <unordered_map>
#include <vector>
#include <QHash>

#include "Recipe.h"

enum Roles
{
    TitleRole = Qt::UserRole + 1,
    RecipeRole,
};

class RecipeModel : public QAbstractItemModel
{

public:
    static constexpr quintptr KeyID = 0U;

    std::unordered_map<QString, QIcon*>& iconDatabase;

    explicit RecipeModel(std::unordered_map<QString, QIcon*>& iconDB, QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;




protected:

    void loadRecipes();

    void addRecipe(QString outputName, QJsonObject recipeJsonObj);

    static constexpr int columnCountVal = 12U; // accounts for up to 4 inputs
    std::unordered_map<QString, std::vector<Recipe*>> recipeDB;
    std::vector<QString> recipeKeys;
};

#endif // RECIPEMODEL_H
