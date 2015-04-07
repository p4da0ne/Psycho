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

private:
    QPushButton *pb_rsc;
    QLineEdit *line_rsc;
    QLabel *label_rsc;

    
};

#endif // SETTINGSFORM_H
