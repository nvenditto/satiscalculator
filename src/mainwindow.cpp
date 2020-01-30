#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aboutdialog.h"

#include <QPointer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QStandardItemModel>

#include "selectrecipedialog.h"
#include "selectproductiondialog.h"
#include "prodqtydelegate.h"

void MainWindow::showAbout()
{
    QPointer<AboutDialog> aboutDlg = new AboutDialog(this);

    aboutDlg->exec();

    if(aboutDlg)
    {
        delete aboutDlg;
    }
}

void MainWindow::showRecipes()
{
    if(!recipeDlg)
    {
        recipeDlg = new SelectRecipeDialog(recipeModel, this);
    }

    recipeDlg->show();
}

void MainWindow::removeCurrentItem()
{
    auto SelectedItemList = this->ui->productTableView->selectionModel()->selectedRows();

    const int firstSelectedRow = SelectedItemList.first().row();
    const int lastSelectedRow = SelectedItemList.last().row();

    const int rowCount = lastSelectedRow - firstSelectedRow + 1;

    productionModel->removeRows(firstSelectedRow, rowCount);
}

void MainWindow::clearProdList()
{
    productionModel->clear();
}

void MainWindow::showSelectProduction()
{
    if(!prodDlg)
    {
        prodDlg = new SelectProductionDialog(recipeModel, productionModel, this);
    }

    prodDlg->show();
}

void MainWindow::loadIcons()
{
    QFile materialsFile(":/builtin/data/materials.json");

    if(!materialsFile.open(QIODevice::ReadOnly))
    {
        qCritical("Unable to load materials listing!");
        return;
    }

    QByteArray materialsData = materialsFile.readAll();

    QJsonDocument materialsDoc(QJsonDocument::fromJson(materialsData));
    QJsonObject jsonRoot = materialsDoc.object();

    for(auto jsonIt = jsonRoot.constBegin(); jsonIt != jsonRoot.constEnd(); ++jsonIt)
    {
        auto entryKey = jsonIt.key();
        auto entryValue = jsonIt.value();

        if(entryValue.isString())
        {
            auto newIcon = new QIcon(":/builtin/data/images/" + entryValue.toString());

            iconDB.emplace(entryKey,newIcon);
        }
        else
        {
            qWarning("Unrecognized entry found for key %s", qUtf8Printable(entryKey));
        }
    }

}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);

     connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
     connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);

     connect(ui->selectRecipesButton, &QPushButton::clicked, this, &MainWindow::showRecipes);

     connect(ui->addProdButton, &QPushButton::clicked, this, &MainWindow::showSelectProduction);
     connect(ui->removeProdButton, &QPushButton::clicked, this, &MainWindow::removeCurrentItem);
     connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearProdList);


     loadIcons();

     recipeModel = new RecipeModel(iconDB, this);
     productionModel = new ProductionModel(iconDB, this);

     productionModel->setHeaderData(0, Qt::Horizontal, "Item");
     productionModel->setHeaderData(1, Qt::Horizontal, "Rate");
     ui->productTableView->setModel(productionModel);

     auto qtyDelegate = new ProdQtyDelegate(this);
     ui->productTableView->setItemDelegateForColumn(1, qtyDelegate);

}

MainWindow::~MainWindow()
{
}
