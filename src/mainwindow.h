#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "recipemodel.h"

#include <unordered_map>

class QStandardItemModel;
class SelectProductionDialog;
class SelectRecipeDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow* ui {nullptr};
    std::unordered_map<QString, QIcon*> iconDB;
    RecipeModel* recipeModel {nullptr};
    QStandardItemModel* productionModel {nullptr};

    SelectProductionDialog* prodDlg {nullptr};
    SelectRecipeDialog* recipeDlg {nullptr};


    void loadIcons();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showAbout();
    void showRecipes();
    void clearProdList();
    void showSelectProduction();
};

#endif // MAINWINDOW_H
