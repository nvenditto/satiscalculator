#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "recipemodel.h"
#include "productionmodel.h"
#include "selectrecipedialog.h"

#include <unordered_map>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow* ui {nullptr};
    std::unordered_map<QString, QIcon*> iconDB;
    RecipeModel* recipeModel {nullptr};
    ProductionModel* prodModel {nullptr};

    SelectRecipeDialog* recipeDlg {nullptr};

    void loadIcons();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showAbout();
    void showRecipes();
};

#endif // MAINWINDOW_H
