#ifndef PRODUCTIONMODEL_H
#define PRODUCTIONMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include <unordered_map>

class QIcon;

class ProductionModel : public QAbstractTableModel
{
    std::unordered_map<QString, QIcon*>& iconDatabase;

public:
    explicit ProductionModel(std::unordered_map<QString, QIcon*>& iconDB, QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    std::vector<QModelIndex> findItems(const QString& searchText, Qt::CaseSensitivity sensitivity = Qt::CaseSensitive) const;
    void appendRow(QString outputName, double outputQty);
    void clear();

private:

    struct OutputItem
    {
        QString outputName;
        double qty;
    };

    std::vector<OutputItem> outputList;
};

#endif // PRODUCTIONMODEL_H
