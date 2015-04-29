#include "mapsettingsdialog.h"

MapSettingsDialog::MapSettingsDialog(QWidget *parent) :
   QDialog(parent)
{
    setWindowTitle(tr("Map settings"));

	getMapSettings();

    mapPathLabel.setText(tr("Map filepath (*.map):"));
    
	mapPathButton.setText(tr("..."));
	mapPathButton.setToolTip(tr("Getting map filepath..."));
	mapPathLineEdit.setText(mapPath);
	mapPathLay.addWidget(&mapPathLineEdit);
	mapPathLay.addWidget(&mapPathButton);
	
	rscPathLabel.setText(tr("Classifier filepath (*.rsc):"));
    
	rscPathButton.setText(tr("..."));
	rscPathButton.setToolTip(tr("Getting classifier filepath..."));
	rscPathLineEdit.setText(rscPath);
	rscPathLay.addWidget(&rscPathLineEdit);
	rscPathLay.addWidget(&rscPathButton);
	
    okButton.setText(tr("Ok"));
    cancelButton.setText(tr("Cancel"));
    buttonLay.addStretch();
    buttonLay.addWidget(&okButton);
    buttonLay.addWidget(&cancelButton);

    dlgLay.addWidget(&mapPathLabel);
	dlgLay.addLayout(&mapPathLay);
    dlgLay.addWidget(&rscPathLabel);
	dlgLay.addLayout(&rscPathLay);
	dlgLay.addStretch();
    dlgLay.addLayout(&buttonLay);
   
    setLayout(&dlgLay);

    
	connect(&mapPathButton,SIGNAL(clicked()),this,SLOT(slotGetMapFilePath()));
	connect(&rscPathButton,SIGNAL(clicked()),this,SLOT(slotGetRscFilePath()));
	connect(&okButton,SIGNAL(clicked()),this,SLOT(slotAccepted()));
    connect(&cancelButton,SIGNAL(clicked()),this,SLOT(reject()));

    okButton.setFocus();
	resize(400,250);
}


//== Слот валидации заполненных данных =======
void MapSettingsDialog::slotAccepted()
{
    if(mapPathLineEdit.text() == "")
    {
        messageToUser(tr("You need to set the map filepath."));
        mapPathLineEdit.setFocus();
        return;
    }
    if(rscPathLineEdit.text() == "")
    {
        messageToUser(tr("You need to set the classifier filepath."));
        rscPathLineEdit.setFocus();
        return;
    }
	saveSettings();
    accept();
}

//==============================================================
//=============== Слот получения пути к файлу карты  ===========
//==============================================================
void MapSettingsDialog::slotGetMapFilePath()
{
	QString mapFilePath = QFileDialog::getOpenFileName(this, tr("Open map file"),
                                                    "/", "*.map");
	mapPathLineEdit.setText(mapFilePath);
}

//==================================================================
//=== Слот получения пути к файлу классификатора условных знаков ===
//==================================================================
void MapSettingsDialog::slotGetRscFilePath()
{
	QString rscFilePath = QFileDialog::getOpenFileName(this, tr("Open map file"),
                                                    "/", "*.rsc");
	rscPathLineEdit.setText(rscFilePath);
}


//==============================================================
//========= Сообщение пользователю в диалоговом окне ===========
//==============================================================
void MapSettingsDialog::messageToUser(QString message)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Message"));
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

//================================================
//==== Метод получения сохраненных настроек ======
//==== путей к файлам карты и классификатора  ====
//================================================
void MapSettingsDialog::getMapSettings()
{
	settings = new QSettings("vka","saturnMap");
	mapPath = settings->value("/mapSettings/mapPath","").toString();
	rscPath = settings->value("/mapSettings/rscPath","").toString();
}

//================================================
//==== Метод сохранения настроек путей ===========
//==== к файлам карты и классификатора ===========
//================================================
void MapSettingsDialog::saveSettings()
{
	mapPath = mapPathLineEdit.text();	
	rscPath = rscPathLineEdit.text();	
	
	settings = new QSettings("vka","saturnMap");
	settings->setValue("/mapSettings/mapPath",mapPath);
	settings->setValue("/mapSettings/rscPath",rscPath);
}