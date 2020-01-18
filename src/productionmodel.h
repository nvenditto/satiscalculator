#ifndef PRODUCTIONMODEL_H
#define PRODUCTIONMODEL_H

#include <QAbstractListModel>
#include <vector>

class ProductionModel : public QAbstractListModel
{
public:
    explicit ProductionModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:

    struct OutputItem
    {
        QString outputName;
        unsigned int qty;
    };

    std::vector<OutputItem> outputList;
};

#endif // PRODUCTIONMODEL_H
