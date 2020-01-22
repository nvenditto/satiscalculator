#include "selectrecipedialog.h"
#include "ui_selectrecipedialog.h"

#include "recipedelegate.h"

SelectRecipeDialog::SelectRecipeDialog(RecipeModel* modelRef, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectRecipeDialog)
{
    ui->setupUi(this);

    auto recipeItemDelegate = new RecipeDelegate(this);

    ui->recipeTreeView->setModel(modelRef);
    ui->recipeTreeView->setItemDelegate(recipeItemDelegate);
}

SelectRecipeDialog::~SelectRecipeDialog()
{
    delete ui;
}
