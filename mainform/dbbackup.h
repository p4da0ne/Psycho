#ifndef DBBACKUP_H
#define DBBACKUP_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QToolButton>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

class DbBackup : public QDialog
{
    Q_OBJECT
public:
    explicit DbBackup(QWidget *parent = 0);
    
signals:

private slots:
    void slotAccepted();
	void slotGetBackupDir();
	void slotCreateBackup();


public:
    QLabel backupDirLabel;
    QLineEdit backupDirLineEdit;
	QToolButton backupDirButton;
	QPushButton okButton, cancelButton, backupButton;
    QHBoxLayout backupDirLay,backupButtonLay,buttonLay;
    QVBoxLayout dlgLay;
private:
    QString backupDir;
	QString backupFileName;
	QSettings *settings;
    void messageToUser(QString message);
	void getSettings();
	void saveSettings();
};

#endif // DBBACKUP_H
