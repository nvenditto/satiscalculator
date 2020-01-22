#include "recipedelegate.h"

#include <QPainter>
#include <QIcon>

#include "recipemodel.h"

constexpr int RecipeDelegate::iconWidth;
constexpr int RecipeDelegate::iconHeight;
constexpr int RecipeDelegate::iconMargin;

RecipeDelegate::RecipeDelegate(QObject* parent)
: QItemDelegate(parent)
{
}

void RecipeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    //drawBackground(painter, option, index);
    // Using the style instead looks better (e.g. on Windows)
    option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, option.widget);

    drawFocus(painter, option, option.rect);


    const int vMargin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameVMargin);
    const int hMargin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameHMargin);

    const QRect rect = option.rect;

    if(index.data(OutputQtyRole).isNull())
    {
        const QString outputName = index.data(Qt::DisplayRole).toString();

        const int lineWidth = rect.width() - hMargin * 2;
        const int lineHeight = (rect.height() - vMargin * 2) / 2;

        const QRect nameRect(rect.left() + hMargin + iconWidth + iconMargin,
                              rect.top() + vMargin,
                              lineWidth,
                              lineHeight);

        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        auto pixmap = icon.pixmap(iconWidth, iconHeight);

        painter->drawPixmap(rect.left() + hMargin, rect.top() + vMargin, pixmap);

        painter->setPen(option.state & QStyle::State_Selected ? option.palette.highlightedText().color() : QColor(Qt::black));
        painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, outputName);
    }
    else
    {
        int left = rect.left() + hMargin;
        int top = rect.top() + vMargin;

        const QFontMetrics metrics(option.font);
        const int textWidth = metrics.width("=");
        const int qtyWidth = metrics.width("88");

        auto input1Var = index.data(Input1IconRole);
        if(!input1Var.isNull())
        {
            const QRect qtyRect(left, top, qtyWidth, iconHeight);
            auto input1Qty = index.data(Input1QtyRole).toUInt();
            painter->drawText(qtyRect,  Qt::AlignRight | Qt::AlignVCenter, QString::number(input1Qty));
            left += qtyWidth;

            QIcon iconInput1 = qvariant_cast<QIcon>(input1Var);
            auto pixmap = iconInput1.pixmap(iconWidth, iconHeight);
            painter->drawPixmap(left, top, pixmap);

            left += iconWidth + iconMargin;
        }

        auto input2Var = index.data(Input2IconRole);
        if(!input2Var.isNull())
        {
            const QRect qtyRect(left, top, qtyWidth, iconHeight);
            auto input2Qty = index.data(Input2QtyRole).toUInt();
            painter->drawText(qtyRect,  Qt::AlignRight | Qt::AlignVCenter, QString::number(input2Qty));
            left += qtyWidth;

            QIcon iconInput2 = qvariant_cast<QIcon>(input2Var);
            auto pixmap = iconInput2.pixmap(iconWidth, iconHeight);
            painter->drawPixmap(left, top, pixmap);

            left += iconWidth + iconMargin;
        }

        auto input3Var = index.data(Input3IconRole);
        if(!input3Var.isNull())
        {
            const QRect qtyRect(left, top, qtyWidth, iconHeight);
            auto input3Qty = index.data(Input3QtyRole).toUInt();
            painter->drawText(qtyRect,  Qt::AlignRight | Qt::AlignVCenter, QString::number(input3Qty));
            left += qtyWidth;

            QIcon iconInput3 = qvariant_cast<QIcon>(input3Var);
            auto pixmap = iconInput3.pixmap(iconWidth, iconHeight);
            painter->drawPixmap(left, top, pixmap);

            left += iconWidth + iconMargin;
        }

        auto input4Var = index.data(Input4IconRole);
        if(!input4Var.isNull())
        {
            const QRect qtyRect(left, top, qtyWidth, iconHeight);
            auto input4Qty = index.data(Input4QtyRole).toUInt();
            painter->drawText(qtyRect,  Qt::AlignRight | Qt::AlignVCenter, QString::number(input4Qty));
            left += qtyWidth;

            QIcon iconInput4 = qvariant_cast<QIcon>(input4Var);
            auto pixmap = iconInput4.pixmap(iconWidth, iconHeight);
            painter->drawPixmap(left, top, pixmap);

            left += iconWidth + iconMargin;
        }



        const QRect eqsRect(left, top, textWidth, iconHeight);

        painter->setPen(option.state & QStyle::State_Selected ? option.palette.highlightedText().color() : QColor(Qt::black));
        painter->drawText(eqsRect, Qt::AlignLeft | Qt::AlignVCenter, "=");

        left += textWidth + iconMargin;

        const QRect qtyRect(left, top, qtyWidth, iconHeight);
        auto outQty = index.data(OutputQtyRole).toUInt();
        painter->drawText(qtyRect,  Qt::AlignRight | Qt::AlignVCenter, QString::number(outQty));
        left += qtyWidth;

        QIcon outIcon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        auto outPixmap = outIcon.pixmap(iconWidth, iconHeight);

        painter->drawPixmap(left, top, outPixmap);
    }

    painter->restore();
}

QSize RecipeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data(OutputQtyRole).isNull())
    {
        const QString name = index.data(Qt::DisplayRole).toString();
        const QFontMetrics metrics(option.font);
        const int vMargin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameVMargin);
        const int hMargin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameHMargin);
        const int textWidth = metrics.width(name);
        const int textHeight = metrics.height();

        const int contentsHeight = qMax(textHeight, iconHeight);

        const int totalWidth = hMargin + iconWidth + iconMargin + textWidth + hMargin;
        const int totalHeight = vMargin + contentsHeight + vMargin;

        return QSize(totalWidth, totalHeight);
    }
    else
    {
        const QFontMetrics metrics(option.font);
        const int eqsWidth = metrics.width("=");
        const int qtyWidth = metrics.width("88");
        const int vMargin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameVMargin);
        const int hMargin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameHMargin);

        const int totalWidth = hMargin + (qtyWidth + iconWidth + iconMargin) * 5 + eqsWidth + hMargin;
        const int totalHeight = vMargin + iconHeight + vMargin;

        return QSize(totalWidth, totalHeight);
    }
}
