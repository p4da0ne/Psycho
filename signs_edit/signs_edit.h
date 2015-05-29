#ifndef SIGNS_EDIT_H
#define SIGNS_EDIT_H

#include <QtGui/QDialog>
#include "config_signs_edit.h"
#include <QLabel>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QSqlRecord>
#include <QSqlError>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QToolButton>
#include <QMoveEvent>
#include <QMenu>



namespace Ui
{
class signs_edit_form;
}


class __EXPORT_SIGNS_EDIT SignsEdit : public QDialog
{
    Q_OBJECT

public:
    SignsEdit(QDialog *parent = 0, Qt::WFlags flags = 0);
    ~SignsEdit();
    void delete_sign();

public slots:
    void show_object_types();
    void show_signs_table(const QModelIndex &index);
    void create_signs_table(const QModelIndex &index);
    void clear_tableWidget(QTableWidget *table);
    void get_path();
    void add_new_sign();
    void delete_sign(int row, int column);
    void save_changes();


private:
    void add_type_to_model(QString str_query, QStandardItem *parent);
    QTableWidgetItem *item;
    QStandardItemModel *model;
    QStandardItem *parentItem;
    QDialog *add_sign;
    QLabel *path_lab;
    QLabel *sign_code_lab;
    QLabel *sign_name_lab;
    QLineEdit *sign_filepath_edit;
    QLineEdit *sign_code_edit;
    QLineEdit *sign_name_edit;
    QToolButton *path_button;
    QPushButton *ok_button;
    QPushButton *cancel_button;

    Ui::signs_edit_form *UI;
};

#endif // SIGNS_EDIT_H
