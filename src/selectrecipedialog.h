#ifndef SELECTRECIPEDIALOG_H
#define SELECTRECIPEDIALOG_H

#include <QDialog>
#include "recipemodel.h"

namespace Ui {
class SelectRecipeDialog;
}

class SelectRecipeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectRecipeDialog(RecipeModel* modelRef, QWidget *parent = 0);
    ~SelectRecipeDialog();

private:
    Ui::SelectRecipeDialog *ui;
};

#endif // SELECTRECIPEDIALOG_H
