#ifndef PRODQTYDELEGATE_H
#define PRODQTYDELEGATE_H

#include <QStyledItemDelegate>

class ProdQtyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ProdQtyDelegate(QObject* parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // PRODQTYDELEGATE_H
