#include "selectproductiondialog.h"
#include "ui_selectproductiondialog.h"

SelectProductionDialog::SelectProductionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectProductionDialog)
{
    ui->setupUi(this);
}

SelectProductionDialog::~SelectProductionDialog()
{
    delete ui;
}
