#ifndef PASSWORD_DIALOG_H
#define PASSWORD_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QVariant>


class PasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PasswordDialog(int idUser, QWidget *parent = 0);
    
signals:

private slots:
    void slotAccepted();

public:
    QLineEdit oldPasswordLineEdit, newPasswordLineEdit, confirmPasswordLineEdit;
    QPushButton okButton, cancelButton;
    QFormLayout formLay;
	QVBoxLayout dlgLay;
	QHBoxLayout buttonLay;
	int idUser;
	QString str_to_md5(QString str);
private:
	QString getUserPassword(int idUser);
	void messageToUser(QString message);

};

#endif // PASSWORD_DIALOG_H
