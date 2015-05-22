#ifndef CHANGE_COORD_DIALOG_H
#define CHANGE_COORD_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include "coord.h"

class ChangeCoordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChangeCoordDialog(Coord *coord = 0,int idObj = 0,int objType = 0, QWidget *parent = 0);
    
signals:

private slots:

	void slotSelectMouse();
public:
    QLabel infoLabel, degreeLabel, minutesLabel, secondsLabel, latLabel, longLabel;
    QLineEdit latDegreeLineEdit, latMinutesLineEdit, latSecondsLineEdit, longDegreeLineEdit, longMinutesLineEdit, longSecondsLineEdit;
    QPushButton okButton, mouseSelectButton;
	QGridLayout coordLay;
	QHBoxLayout buttonLay;
    QVBoxLayout dlgLay;

	int idObject;
	int objectType;
	bool mouseFlag;
private:
    Coord *coordinates;

    void messageToUser(QString message);
};

#endif // CHANGE_COORD_DIALOG_H
