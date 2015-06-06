#ifndef PERSONESDATA_H
#define PERSONESDATA_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class PersonesData;
}

class PersonesData : public QDialog
{
    Q_OBJECT
    
public:
    explicit PersonesData(QString type_element, int id_object, QWidget *parent);
    ~PersonesData();
private slots:

    void    save_persones();
    void    open_file();
    
private:


    int     id_pers_group;
    int     id_pers_smi;
    int     id_pers_ls;
    int     id_object_pers;
    QString type_elem_pers;

    int     insert_in_table(QString table,QMap<QString,QString> map,QString id);
    void    fill_combobox_persones(QComboBox *box);

    Ui::PersonesData *UI;

};

#endif // PERSONESDATA_H
