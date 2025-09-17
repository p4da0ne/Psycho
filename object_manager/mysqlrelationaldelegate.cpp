#include "mysqlrelationaldelegate.h"
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QSqlRecord>
#include <QItemDelegate>

mySqlRelationalDelegate::mySqlRelationalDelegate(QObject *parent) :
    QSqlRelationalDelegate (parent)
{
}
QWidget *mySqlRelationalDelegate::createEditor(QWidget *aParent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    const QSqlRelationalTableModel *sqlModel = qobject_cast<const QSqlRelationalTableModel *>(index.model());
    QSqlTableModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : 0;
    if (!childModel )
    {
        const QAbstractProxyModel* proxyModel = qobject_cast<const QAbstractProxyModel *>(index.model());
        if (proxyModel)
        {
            sqlModel = qobject_cast<const QSqlRelationalTableModel *>(proxyModel->sourceModel());
            childModel = sqlModel ? sqlModel->relationModel(index.column()) : 0;
        }
    }
    if (!childModel)
    {
        return QSqlRelationalDelegate::createEditor(aParent, option, index);
    }
    QComboBox *combo = new QComboBox(aParent);
combo->setModel(childModel);
combo->setModelColumn(childModel->fieldIndex(sqlModel->relation(index.column()).displayColumn()));
combo->installEventFilter(const_cast<mySqlRelationalDelegate *>(this));
return combo;
}
void mySqlRelationalDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString strVal = "";
    const QSqlRelationalTableModel *sqlModel = qobject_cast<const QSqlRelationalTableModel *>(index.model());
    if (!sqlModel )
    {
        const QAbstractProxyModel* proxyModel = qobject_cast<const QAbstractProxyModel *>(index.model());
        if (proxyModel) {
            strVal = proxyModel->data(index).toString();
        }
    } else {
        strVal = sqlModel->data(index).toString();
    }
    QComboBox *combo = qobject_cast<QComboBox *>(editor);
    if (strVal.isEmpty() || !combo) {
        QSqlRelationalDelegate::setEditorData(editor, index);
        return;
    }
    combo->setCurrentIndex(combo->findText(strVal));
}
void mySqlRelationalDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (!index.isValid())
        return;
    QSqlRelationalTableModel *sqlModel = qobject_cast<QSqlRelationalTableModel *>(model);
    QAbstractProxyModel* proxyModel = NULL;
    if (!sqlModel )
    {
        proxyModel = qobject_cast<QAbstractProxyModel *>(model);
        if (proxyModel)
            sqlModel = qobject_cast<QSqlRelationalTableModel *>(proxyModel->sourceModel());
    }
    QSqlTableModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : 0;
    QComboBox *combo = qobject_cast<QComboBox *>(editor);
    if (!sqlModel || !childModel || !combo) {
        QSqlRelationalDelegate::setModelData(editor, model, index);
        return;
    }
    int currentItem = combo->currentIndex();
    int childColIndex = childModel->fieldIndex(sqlModel->relation(index.column()).displayColumn());
    int childEditIndex = childModel->fieldIndex(sqlModel->relation(index.column()).indexColumn());
    if (proxyModel) {
        proxyModel->setData(index, childModel->data(childModel->index(currentItem, childColIndex), Qt::DisplayRole), Qt::DisplayRole);
        proxyModel->setData(index, childModel->data(childModel->index(currentItem, childEditIndex), Qt::EditRole), Qt::EditRole);
    } else {
        sqlModel->setData(index, childModel->data(childModel->index(currentItem, childColIndex), Qt::DisplayRole), Qt::DisplayRole);
        sqlModel->setData(index, childModel->data(childModel->index(currentItem, childEditIndex), Qt::EditRole), Qt::EditRole);
    }
}
