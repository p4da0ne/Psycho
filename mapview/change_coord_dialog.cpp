#include "change_coord_dialog.h"


ChangeCoordDialog::ChangeCoordDialog(Coord *coord, int idObj,int objType,QWidget *parent) :
   QDialog(parent), coordinates(coord), idObject(idObj), objectType(objType)
{
    mouseFlag = false;
	setWindowTitle("Перемещение объекта");

	infoLabel.setText("Введите новые координаты (WGS-84):");
	degreeLabel.setText("Градусы:");
	minutesLabel.setText("Минуты:");
	secondsLabel.setText("Секунды:");
	latLabel.setText("Широта");
	longLabel.setText("Долгота");
	
	latDegreeLineEdit.setText(QString::number(coordinates->getLatDegrees()));
	latMinutesLineEdit.setText(QString::number(coordinates->getLatMinutes()));
	latSecondsLineEdit.setText(QString::number(coordinates->getLatSeconds()));
	longDegreeLineEdit.setText(QString::number(coordinates->getLongDegrees()));
	longMinutesLineEdit.setText(QString::number(coordinates->getLongMinutes()));
	longSecondsLineEdit.setText(QString::number(coordinates->getLongSeconds()));

	coordLay.addWidget(&latLabel,0,1);
	coordLay.addWidget(&longLabel,0,2);
	coordLay.addWidget(&degreeLabel,1,0);
	coordLay.addWidget(&minutesLabel,2,0);
	coordLay.addWidget(&secondsLabel,3,0);
	coordLay.addWidget(&latDegreeLineEdit,1,1);
	coordLay.addWidget(&latMinutesLineEdit,2,1);
	coordLay.addWidget(&latSecondsLineEdit,3,1);	
	coordLay.addWidget(&longDegreeLineEdit,1,2);
	coordLay.addWidget(&longMinutesLineEdit,2,2);
	coordLay.addWidget(&longSecondsLineEdit,3,2);

    okButton.setText("Ok");
    mouseSelectButton.setText("Указать мышью");
    
	buttonLay.addWidget(&mouseSelectButton);
	buttonLay.addStretch();
    buttonLay.addWidget(&okButton);
	
    dlgLay.addWidget(&infoLabel);
    dlgLay.addLayout(&coordLay);
    dlgLay.addLayout(&buttonLay);

    setLayout(&dlgLay);

    connect(&okButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(&mouseSelectButton,SIGNAL(clicked()),this,SLOT(slotSelectMouse()));

	mouseSelectButton.setFocus();
}



void ChangeCoordDialog::messageToUser(QString message)
{
    QMessageBox msgBox;

    msgBox.setWindowTitle(tr("Message"));
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void ChangeCoordDialog::slotSelectMouse()
{
	mouseFlag = true;
	accept();

}