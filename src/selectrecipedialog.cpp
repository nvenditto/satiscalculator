#include "selectrecipedialog.h"
#include "ui_selectrecipedialog.h"

SelectRecipeDialog::SelectRecipeDialog(RecipeModel* modelRef, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectRecipeDialog)
{
    ui->setupUi(this);

    ui->recipeTableView->setModel(modelRef);
}

SelectRecipeDialog::~SelectRecipeDialog()
{
    delete ui;
}
