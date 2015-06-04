#ifndef PERSONESDATA_H
#define PERSONESDATA_H

#include <QDialog>

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
    Ui::PersonesData *ui;
};

#endif // PERSONESDATA_H
