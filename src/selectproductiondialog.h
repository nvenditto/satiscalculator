#ifndef SELECTPRODUCTIONDIALOG_H
#define SELECTPRODUCTIONDIALOG_H

#include <QDialog>

class RecipeModel;
class ProductionModel;
class QStandardItemModel;

namespace Ui {
class SelectProductionDialog;
}

class SelectProductionDialog : public QDialog
{
    Q_OBJECT

    RecipeModel* recipeModelPtr;
    ProductionModel* prodModelPtr;

public:
    explicit SelectProductionDialog(RecipeModel* recipeModelRef, ProductionModel* prodModelRef, QWidget *parent = nullptr);
    ~SelectProductionDialog();

private:
    Ui::SelectProductionDialog *ui;

private slots:
    void addSelectedOutput();
};

#endif // SELECTPRODUCTIONDIALOG_H
