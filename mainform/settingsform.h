#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>
#include <QSettings>

class SettingsForm : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsForm(QDialog *parent = 0);


signals:
    
public slots:
    void selectRSC();
    void selectIMG();

private:
    QPushButton *pb_rsc;
    QPushButton *pb_img;
    QPushButton *pb_ok;
    QLineEdit *line_rsc;
    QLineEdit *line_img;
    QLabel *label_rsc;
    QLabel *label_img;

    
};

#endif // SETTINGSFORM_H
