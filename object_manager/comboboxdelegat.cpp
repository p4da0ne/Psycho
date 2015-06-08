#include "comboboxdelegat.h"
#include <QComboBox>
#include <QSqlQuery>

ComboboxDelegat::ComboboxDelegat(QWidget *parent) :
    QItemDelegate(parent)
{
}

QWidget *ComboboxDelegat::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    QSqlQuery query;
    QString combodata;
    int id_type;
    query.exec(QString("SELECT id_family_types,type_name FROM family_types"));
    while(query.next()){
        combodata = query.value(1).toString();
        id_type = query.value(0).toInt();
        editor->addItem(combodata,id_type);

    }

    return editor;
}

void ComboboxDelegat::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *combobox = static_cast<QComboBox*>(editor);
    model->setData(index,combobox->currentText(),Qt::EditRole);
    int c_index = combobox->currentIndex();
    int id_type = combobox->itemData(c_index,Qt::UserRole).toInt();
    model->setData(index,id_type,Qt::UserRole);
}

void ComboboxDelegat::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *combobox = static_cast<QComboBox*>(editor);
}

void ComboboxDelegat::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
