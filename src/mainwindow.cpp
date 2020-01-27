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

     connect(ui->addProdButton, &QPushButton::clicked, this, &MainWindow::showSelectProduction);
     connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearProdList);
     connect(ui->selectRecipesButton, &QPushButton::clicked, this, &MainWindow::showRecipes);

     loadIcons();

     recipeModel = new RecipeModel(iconDB, this);
     productionModel = new QStandardItemModel(this);

     ui->productionListView->setModel(productionModel);
}

MainWindow::~MainWindow()
{
}
