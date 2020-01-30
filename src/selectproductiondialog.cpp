#include "selectproductiondialog.h"
#include "ui_selectproductiondialog.h"

#include "recipemodel.h"
#include "productionmodel.h"
#include <QStandardItemModel>

SelectProductionDialog::SelectProductionDialog(RecipeModel* newRecipeModelPtr, ProductionModel* newProdModelPtr, QWidget *parent) :
    QDialog(parent),
    recipeModelPtr(newRecipeModelPtr),
    prodModelPtr(newProdModelPtr),
    ui(new Ui::SelectProductionDialog)
{
    ui->setupUi(this);

    ui->availProdListView->setModel(newRecipeModelPtr);

    connect(this->ui->addButton, &QPushButton::clicked, this, &SelectProductionDialog::addSelectedOutput);
}

SelectProductionDialog::~SelectProductionDialog()
{
    delete ui;
}

void SelectProductionDialog::addSelectedOutput()
{
    auto SelectedItemList = this->ui->availProdListView->selectionModel()->selectedIndexes();

    for(const auto& selectedItem : SelectedItemList)
    {
        auto prodItemVar = selectedItem.data(Qt::DisplayRole);

        if(!prodItemVar.isNull() && prodItemVar.canConvert<QString>())
        {
            auto prodItem = prodItemVar.toString();
            auto prodRecipe = recipeModelPtr->LookupRecipe(prodItem);

            if(prodRecipe)
            {
                auto foundItems = prodModelPtr->findItems(prodItem);

                // Don't add duplicates
                if(foundItems.empty())
                {
                    double prodRate = (60.0 / prodRecipe->productionTime);

                    prodModelPtr->appendRow(prodItem, prodRate);

                    return;
                }
                else
                {
                    // do nothing, it's already in the list
                    return;
                }
            }
        }

        qWarning("Null Recipe found!");
    }
}
