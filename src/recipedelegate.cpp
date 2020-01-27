#include "recipedelegate.h"

#include <QPainter>
#include <QIcon>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>

#include "recipemodel.h"

constexpr int RecipeDelegate::iconWidth;
constexpr int RecipeDelegate::iconHeight;
constexpr int RecipeDelegate::iconMargin;

RecipeDelegate::RecipeDelegate(std::unordered_map<QString, QIcon*>& iconDB, QObject* parent) :
    QItemDelegate(parent),
    iconDatabase(iconDB)
{
}

void RecipeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    //drawBackground(painter, option, index);
    // Using the style instead looks better (e.g. on Windows)
    option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, option.widget);

    const int vMargin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameVMargin);
    const int hMargin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameHMargin);

    const QRect rect = option.rect;

    if(index.internalId() == RecipeModel::KeyID)
    {
        // This is an output
        const QString outputName = index.data(Qt::DisplayRole).toString();

        const int lineWidth = rect.width() - hMargin * 2;
        const int lineHeight = (rect.height() - vMargin * 2);

        const QRect nameRect(rect.left() + hMargin + iconWidth + iconMargin,
                              rect.top() + vMargin,
                              lineWidth,
                              lineHeight);

        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        auto pixmap = icon.pixmap(iconWidth, iconHeight);

        painter->drawPixmap(rect.left() + hMargin, rect.top() + vMargin, pixmap);

        painter->setPen(option.palette.text().color());
        painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, outputName);
    }
    else
    {
        // This is a recipe
        int left = rect.left() + hMargin;
        int top = rect.top() + vMargin;

        const QFontMetrics metrics(option.font);
        const int textWidth = metrics.horizontalAdvance("=");
        const int qtyWidth = metrics.horizontalAdvance("88x");

        auto recipeVariant = index.data(RecipeRole);
        if(!recipeVariant.isNull() && recipeVariant.canConvert<Recipe*>())
        {
            auto theRecipe = recipeVariant.value<Recipe*>();

            // Draw Checkbox
            const auto checkRect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &option);
            const QRect drawCheckRect(left, top, checkRect.width(), iconHeight);
            drawCheck(painter, option, drawCheckRect, (theRecipe->active) ? Qt::Checked : Qt::Unchecked);

            left += checkRect.width() + iconMargin;

            painter->setPen(option.palette.text().color());

            // Draw Each Recipe Input
            for(const auto& recipeInput : theRecipe->inputs)
            {
                const QRect qtyRect(left, top, qtyWidth, iconHeight);
                painter->drawText(qtyRect,
                                  (Qt::AlignRight | Qt::AlignVCenter),
                                  (QString::number(recipeInput.qty) + QString("x")));
                left += qtyWidth;

                auto inputIcon = iconDatabase.at(recipeInput.name);
                auto inputPixmap = inputIcon->pixmap(iconWidth, iconHeight);
                painter->drawPixmap(left, top, inputPixmap);

                left += iconWidth + iconMargin;


            }

            // Draw Equals Sign
            const QRect eqsRect(left, top, textWidth, iconHeight);
            painter->drawText(eqsRect, Qt::AlignLeft | Qt::AlignVCenter, "=");

            // Draw Output
            left += textWidth + iconMargin;
            const QRect qtyRect(left, top, qtyWidth, iconHeight);
            painter->drawText(qtyRect,
                              (Qt::AlignRight | Qt::AlignVCenter),
                              (QString::number(theRecipe->outputQty) + QString("x")));

            left += qtyWidth;

            auto outputIcon = iconDatabase.at(theRecipe->outputName);
            auto outputPixmap = outputIcon->pixmap(iconWidth, iconHeight);
            painter->drawPixmap(left, top, outputPixmap);

            // Draw focus
            drawFocus(painter, option, option.rect);
        }
        else
        {
            qWarning("Unable to get recipe data for index row = %s, internalID = %s",
                     qUtf8Printable(QString::number(index.row())),
                     qUtf8Printable(QString::number(index.internalId())));
        }
    }

    painter->restore();
}

QSize RecipeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.internalId() == RecipeModel::KeyID)
    {
        // Size Hint for output
        const QString name = index.data(Qt::DisplayRole).toString();
        const QFontMetrics metrics(option.font);
        const int vMargin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameVMargin);
        const int hMargin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameHMargin);
        const int textWidth = metrics.horizontalAdvance(name);
        const int textHeight = metrics.height();

        const int contentsHeight = qMax(textHeight, iconHeight);

        const int totalWidth = hMargin + iconWidth + iconMargin + textWidth + hMargin;
        const int totalHeight = vMargin + contentsHeight + vMargin;

        return QSize(totalWidth, totalHeight);
    }
    else
    {
        // Size Hint for Recipe
        const QFontMetrics metrics(option.font);

        const auto checkRect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &option);

        const int eqsWidth = metrics.horizontalAdvance("=");
        const int qtyWidth = metrics.horizontalAdvance("88x");
        const int vMargin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameVMargin);
        const int hMargin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameHMargin);

        const int totalWidth = hMargin + checkRect.width() + iconMargin + (qtyWidth + iconWidth + iconMargin) * 5 + eqsWidth + hMargin;
        const int totalHeight = vMargin + iconHeight + vMargin;

        return QSize(totalWidth, totalHeight);
    }
}
