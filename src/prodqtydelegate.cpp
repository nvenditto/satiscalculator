#include "prodqtydelegate.h"

#include <QDoubleSpinBox>

ProdQtyDelegate::ProdQtyDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

QWidget *ProdQtyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    auto spinEdit = new QDoubleSpinBox(parent);
    spinEdit->setFrame(false);
    spinEdit->setRange(0.0, 10000.0);
    spinEdit->setDecimals(3);

    return spinEdit;
}

void ProdQtyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto qtyValue = index.model()->data(index, Qt::EditRole).toDouble();

    auto spinEdit = static_cast<QDoubleSpinBox*>(editor);
    spinEdit->setValue(qtyValue);
}

void ProdQtyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto spinEdit = static_cast<QDoubleSpinBox*>(editor);
    spinEdit->interpretText();
    auto qtyValue = spinEdit->value();

    model->setData(index, qtyValue, Qt::EditRole);
}

void ProdQtyDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

