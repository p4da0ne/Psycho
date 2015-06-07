#include "password_dialog.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QCryptographicHash>


PasswordDialog::PasswordDialog(int idUser, QWidget *parent) :
   QDialog(parent),idUser(idUser)
{
  	resize(300,250);

	setWindowTitle("Смена пароля пользователя");
	
	formLay.addRow("Текущий пароль:",&oldPasswordLineEdit);
	formLay.addRow("Новый пароль:",&newPasswordLineEdit);
	formLay.addRow("Подтверждение пароля:",&confirmPasswordLineEdit);
		
	oldPasswordLineEdit.setEchoMode(QLineEdit::Password);
	newPasswordLineEdit.setEchoMode(QLineEdit::Password);
	confirmPasswordLineEdit.setEchoMode(QLineEdit::Password);
	
	formLay.setVerticalSpacing(10);

	okButton.setText("Сменить пароль");
	cancelButton.setText("Отмена");
	buttonLay.addWidget(&okButton);
	buttonLay.addStretch();
	buttonLay.addWidget(&cancelButton);

	dlgLay.addLayout(&formLay);
	dlgLay.addStretch();
	dlgLay.addLayout(&buttonLay);

	setLayout(&dlgLay);
	
	connect(&okButton,SIGNAL(clicked()),this,SLOT(slotAccepted()));
	connect(&cancelButton,SIGNAL(clicked()),this,SLOT(reject()));

}


//== Слот валидации заполненных данных =======
void PasswordDialog::slotAccepted()
{
    QString mess;

	QString userPassword = getUserPassword(idUser);

	if(str_to_md5(oldPasswordLineEdit.text()) != userPassword)
    {
		mess.append("Неверный текущий пароль пользователя.\n");
    }
	if(newPasswordLineEdit.text() != confirmPasswordLineEdit.text())
    {
		mess.append("Новый пароль и пароль для подтверждения не совпадают.\n");
    }
	
	if(mess != "")
	{
		messageToUser(mess);
		return;
	}
    accept();
}


void PasswordDialog::messageToUser(QString message)
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("Сообщение");
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}



//============================================================================
//======= Метод возвращает воинское звание пользователя из БД ================
//============================================================================
QString PasswordDialog::getUserPassword(int idUser)
{
	QString password;
	QSqlQuery query;
	QString str = QString("SELECT passwd FROM users_passwd WHERE id_user = %1").arg(idUser);
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{		
			password = query.value(rec.indexOf("passwd")).toString();
		}
	}
	return password;
}

//====================================================================
//===== Метод шифрования строки по методу MD5 ========================
//====================================================================
QString PasswordDialog::str_to_md5(QString str)
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(str.toAscii()); 
	QString md5_str(hash.result().toHex());
	return md5_str;
}