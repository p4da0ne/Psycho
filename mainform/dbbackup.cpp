#include "dbbackup.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QDateTime>

#include "../mainform/settings.h"

DbBackup::DbBackup(QWidget *parent) :
   QDialog(parent)
{
    settings = new QSettings("vka","saturnBackupDb");
	
	setWindowTitle(tr("Backup database dialog"));

	getSettings();

    backupDirLabel.setText(tr("Backup dir:"));
    
	backupDirButton.setText(tr("..."));
	backupDirButton.setToolTip(tr("Getting backup dir path..."));
	backupDirLineEdit.setText(backupDir);
	backupDirLay.addWidget(&backupDirLabel);
	backupDirLay.addWidget(&backupDirLineEdit);
	backupDirLay.addWidget(&backupDirButton);
	
	backupButton.setText(tr("&Create backup"));
	backupButtonLay.addStretch();
	backupButtonLay.addWidget(&backupButton);

    okButton.setText(tr("Ok"));
    cancelButton.setText(tr("Cancel"));
    buttonLay.addStretch();
    buttonLay.addWidget(&okButton);
    buttonLay.addWidget(&cancelButton);

	dlgLay.addLayout(&backupDirLay);
    dlgLay.addLayout(&backupButtonLay);
	dlgLay.addStretch();
    dlgLay.addLayout(&buttonLay);
   
    setLayout(&dlgLay);

    
	connect(&backupDirButton,SIGNAL(clicked()),this,SLOT(slotGetBackupDir()));
	connect(&backupButton,SIGNAL(clicked()),this,SLOT(slotCreateBackup()));
	connect(&okButton,SIGNAL(clicked()),this,SLOT(slotAccepted()));
    connect(&cancelButton,SIGNAL(clicked()),this,SLOT(reject()));

    okButton.setFocus();
	resize(400,200);
}


//== Слот валидации заполненных данных =======
void DbBackup::slotAccepted()
{
    if(backupDirLineEdit.text() == "")
    {
        messageToUser(tr("You need to set the backup directory."));
        backupDirLineEdit.setFocus();
        return;
    }
	saveSettings();
    accept();
}

//==============================================================
//=============== Слот получения пути к файлу карты  ===========
//==============================================================
void DbBackup::slotGetBackupDir()
{
	QString backDir = QFileDialog::getExistingDirectory(this,
                       tr("Open backup directory"), backupDir);

	backupDirLineEdit.setText(backDir);
}

//==============================================================
//========= Сообщение пользователю в диалоговом окне ===========
//==============================================================
void DbBackup::messageToUser(QString message)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Message"));
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

//================================================================
//==== Метод получения сохраненных настроек ======================
//==== директории сохранения файлов резервных копий БД ===========
//================================================================
void DbBackup::getSettings()
{
	backupDir = settings->value("/dbBackupSettings/dbBackupDir","").toString();
}

//=====================================================
//==== Метод сохранения настроек директории ===========
//==== сохранения файлов резервных копий БД ===========
//=====================================================
void DbBackup::saveSettings()
{
	backupDir = backupDirLineEdit.text();	
	settings->setValue("/dbBackupSettings/dbBackupDir",backupDir);

}

//===================================================
//======== Слот создания резервной копии БД =========
//===================================================
void DbBackup::slotCreateBackup()
{
	
//===========================================================================
//===== Выбор настроек для формирования скрипта резервного копирования БД ===
//===========================================================================
	Settings *dbSettings = new Settings(this);

	QString host = dbSettings->host();
	QString user = dbSettings->user_name();
	QString dbName = dbSettings->db_name();
	QString password = dbSettings->user_password();

	QString backupQuery;
//=========================================================
//===== Формирование скрипта резервного копирования БД ====
//=========================================================

	QDir dir;
	QString currentPath = dir.currentPath();

	QString pathStr = currentPath + "/recovery/pg_dump.exe";

	backupQuery.append(pathStr);
	backupQuery.append(" -Fc -v");
	backupQuery.append(" --host=");
	backupQuery.append(host);
	backupQuery.append(" --username=");
	backupQuery.append(user);
	backupQuery.append(" ");
	backupQuery.append(dbName);
	backupQuery.append(" > ");
	backupQuery.append(backupDir);
	backupQuery.append("/");
	backupQuery.append("saturn");

	QDateTime dt;
	QString dateTime = dt.currentDateTime().toString("dd" "MM" "yyyy" "hh" "mm" "ss");

	backupQuery.append(dateTime);
	backupQuery.append(".backup");
//=========================================================
//===== Запись сформированной строки в bat-файл ===========
//=========================================================
	QString tempFilePath = currentPath + "/recovery/backup.bat";
	QFile tempFile(tempFilePath);
	
	if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return;
	}
	tempFile.write(backupQuery.toStdString().c_str());
	tempFile.close();

//=========================================================
//===== Выполнение bat-файла ==============================
//=========================================================
	QDesktopServices::openUrl(QUrl::fromLocalFile(QString(tempFilePath)));

//==============================================================
//========= Сообщение пользователю в диалоговом окне ===========
//==============================================================
	//messageToUser(QString("Создан файл резервной копии БД с именем %1").arg("saturn"+dateTime+".backup"));

}