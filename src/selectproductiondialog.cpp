#include "selectproductiondialog.h"
#include "ui_selectproductiondialog.h"

#include "recipemodel.h"
#include <QStandardItemModel>

SelectProductionDialog::SelectProductionDialog(RecipeModel* newRecipeModelPtr, QStandardItemModel* newProdModelPtr, QWidget *parent) :
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
        auto prodItem = selectedItem.data(Qt::DisplayRole).toString();
        auto prodIcon = recipeModelPtr->iconDatabase.at(prodItem);

        auto foundItems = prodModelPtr->findItems(prodItem);

        // Don't add duplicates
        if(foundItems.count() == 0)
        {
            auto newItem = new QStandardItem(*prodIcon, prodItem);
            prodModelPtr->appendRow(newItem);
        }
    }
}
