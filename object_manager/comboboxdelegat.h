#ifndef COMBOBOXDELEGAT_H
#define COMBOBOXDELEGAT_H

#include <QAbstractItemDelegate>
#include <QItemDelegate>

class ComboboxDelegat : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ComboboxDelegat(QWidget *parent = 0);
    
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    
public slots:
    
};

#endif // COMBOBOXDELEGAT_H
