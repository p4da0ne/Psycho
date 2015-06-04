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
    explicit PersonesData(QWidget *parent = 0);
    ~PersonesData();
    
private:

    void fill_combobox_persones(QComboBox *box);

    Ui::PersonesData *ui;
};

#endif // PERSONESDATA_H
