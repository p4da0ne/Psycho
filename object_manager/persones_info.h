#ifndef PERSONES_INFO_H
#define PERSONES_INFO_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>

namespace Ui {
class Persones_info;
}

class Persones_info : public QDialog
{
    Q_OBJECT
    
public:
    explicit Persones_info(QString type_element, int id_object, QWidget *parent);
    ~Persones_info();

    QPushButton *exit_but;

public slots:


private:
    int id_object_pers;
    void fill_combobox_persones_(QComboBox *Box,int current_index);
    void show_info_pers();



    Ui::Persones_info *UI;
};

#endif // PERSONES_INFO_H
