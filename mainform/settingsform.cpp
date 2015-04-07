#include "settingsform.h"


//menage by settings of programm
SettingsForm::SettingsForm(QDialog *parent) :
    QDialog(parent)
{
    QSettings settings("Saturn");
    this->setWindowTitle(tr("Settings dialog"));
    QGridLayout * gridLayout = new QGridLayout(this);
    this->setLayout(gridLayout);
    label_rsc=new QLabel(tr("RSC file:"));
    line_rsc=new QLineEdit();
    line_rsc->setText(settings.value(last_rsc).toString());
    pb_rsc=new QPushButton();
    pb_rsc->setText("...");
    gridLayout->addWidget(label_rsc,0,0);
    gridLayout->addWidget(line_rsc,0,1);
    gridLayout->addWidget(pb_rsc,0,2);
    connect(pb_rsc,SIGNAL(clicked()),this, SLOT(selectRSC()));

}

void SettingsForm::selectRSC()
{
    QString fileRSC = QFileDialog::getOpenFileName(this, tr("Select RSC file"), QString::null,
                   "RSC (*.rsc)" );
    if (File.isEmpty()){
        return;
    }else{
        QSettings settings("Saturn");
        settings.setValue("last_rsc",fileRSC);
        line_rsc->setText(fileRSC);
    }
}
