#include "settingsform.h"


//menage by settings of programm
SettingsForm::SettingsForm(QDialog *parent) :
    QDialog(parent)
{
    QSettings settings("Saturn");
    this->setWindowTitle(tr("Settings dialog"));
    QGridLayout * gridLayout = new QGridLayout(this);
    this->setLayout(gridLayout);

    label_rsc=new QLabel(tr("RSC file:"),this);

    line_rsc=new QLineEdit(this);
    line_rsc->setMinimumWidth(200);
    line_rsc->setText(settings.value("last_rsc").toString());

    pb_rsc=new QPushButton(this);
    pb_rsc->setMaximumWidth(40);
    pb_rsc->setText("...");

    pb_ok = new QPushButton(this);
    pb_ok->setText("Save");
    gridLayout->addWidget(label_rsc,0,0);
    gridLayout->addWidget(line_rsc,0,1);
    gridLayout->addWidget(pb_rsc,0,2);
    gridLayout->addWidget(pb_ok,1,2);
    connect(pb_rsc,SIGNAL(clicked()),this, SLOT(selectRSC()));
    connect(pb_ok,SIGNAL(clicked()),this,SLOT(close()));

}

void SettingsForm::selectRSC()
{
    QString fileRSC = QFileDialog::getOpenFileName(this, tr("Select RSC file"), QString::null, "RSC (*.rsc)" );
    if (fileRSC.isEmpty()){
        return;
    }else{
        QSettings settings("Saturn");
        settings.setValue("last_rsc",fileRSC);
        line_rsc->setText(fileRSC);
    }
}
