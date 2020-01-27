#ifndef SELECTPRODUCTIONDIALOG_H
#define SELECTPRODUCTIONDIALOG_H

#include <QDialog>

class RecipeModel;
class QStandardItemModel;

namespace Ui {
class SelectProductionDialog;
}

class SelectProductionDialog : public QDialog
{
    Q_OBJECT

    RecipeModel* recipeModelPtr;
    QStandardItemModel* prodModelPtr;

public:
    explicit SelectProductionDialog(RecipeModel* recipeModelRef, QStandardItemModel* prodModelRef, QWidget *parent = nullptr);
    ~SelectProductionDialog();

private:
    Ui::SelectProductionDialog *ui;

private slots:
    void addSelectedOutput();
};

#endif // SELECTPRODUCTIONDIALOG_H
