#include "user_data_dialog.h"

UserDataDialog::UserDataDialog(QStandardItemModel *rankList, QWidget *parent) :
   QDialog(parent)
{
  	resize(300,350);
	rankCombo.setModel(rankList);


	formLay.addRow("Фамилия:",&nameLineEdit);
	formLay.addRow("Имя:",&surnameLineEdit);
	formLay.addRow("Отчество:",&patronLineEdit);
	formLay.addRow("Воинское звание:",&rankCombo);
	formLay.addRow("Группа:",&groupCombo);
	formLay.addRow("Логин:",&loginLineEdit);
	formLay.addRow("Пароль:",&passwordLineEdit);
	formLay.setVerticalSpacing(10);

	okButton.setText("Сохранить");
	cancelButton.setText("Отмена");
	buttonLay.addWidget(&okButton);
	buttonLay.addStretch();
	buttonLay.addWidget(&cancelButton);

	dlgLay.addLayout(&formLay);
	dlgLay.addStretch();
	dlgLay.addLayout(&buttonLay);

	setLayout(&dlgLay);
	
	

}


//== Слот валидации заполненных данных =======
void UserDataDialog::slotAccepted()
{
    /*if(varFileLineEdit.text() == "")
    {
        messageToUser(tr("You need to set the variant filename."));
        varFileLineEdit.setFocus();
        return;
    }*/
    accept();
}


void UserDataDialog::messageToUser(QString message)
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("Сообщение");
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}
