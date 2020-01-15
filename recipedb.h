#ifndef RECIPEDB_H
#define RECIPEDB_H

#include <vector>
#include <unordered_map>
#include <QHash>

namespace std {
  template<> struct hash<QString> {
    std::size_t operator()(const QString& s) const noexcept {
      return (size_t) qHash(s);
    }
  };
}

struct RecipeInput
{
    QString name;
    unsigned int qty {0U};
};

struct Recipe
{
    QString output;
    std::vector<RecipeInput> inputs;
    unsigned int outputQty {0U};
    unsigned int productionTime {0U};
};


#endif // RECIPEDB_H
