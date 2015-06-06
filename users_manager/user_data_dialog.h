#ifndef USER_DATA_DIALOG_H
#define USER_DATA_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QStandardItemModel>

class UserDataDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UserDataDialog(int idUser = 0, QWidget *parent = 0);
    
signals:

private slots:
    void slotAccepted();

public:
    QLineEdit nameLineEdit, surnameLineEdit, patronLineEdit, loginLineEdit, passwordLineEdit;
    QComboBox groupCombo, rankCombo;
    QPushButton okButton, cancelButton;
    QFormLayout formLay;
	QVBoxLayout dlgLay;
	QHBoxLayout buttonLay;
	int idUser;
	QString str_to_md5(QString str);
private:
	QStandardItemModel* getRankList();
	QStandardItemModel* getGroupList();
	QString getUserRank(int idUser);
	QString getUserGroupName(int idUser);
    void messageToUser(QString message);
	void fillUserData(int idUser);
	bool isUniqueLogin(QString login);

	QStandardItemModel *rankModel;
	QStandardItemModel *groupModel;

};

#endif // USER_DATA_DIALOG_H
