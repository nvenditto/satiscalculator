#ifndef RECIPEDELEAGATE_H
#define RECIPEDELEAGATE_H

#include <QItemDelegate>

class RecipeDelegate : public QItemDelegate
{
    Q_OBJECT

    static constexpr int iconWidth = 20;
    static constexpr int iconHeight = 20;
    static constexpr int iconMargin = 10;

public:
    explicit RecipeDelegate(QObject* parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // RECIPEDELEAGATE_H
