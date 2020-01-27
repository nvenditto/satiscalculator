#ifndef SELECTPRODUCTIONDIALOG_H
#define SELECTPRODUCTIONDIALOG_H

#include <QDialog>

namespace Ui {
class SelectProductionDialog;
}

class SelectProductionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectProductionDialog(QWidget *parent = nullptr);
    ~SelectProductionDialog();

private:
    Ui::SelectProductionDialog *ui;
};

#endif // SELECTPRODUCTIONDIALOG_H
