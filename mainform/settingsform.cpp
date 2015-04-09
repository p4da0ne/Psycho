#include "settingsform.h"


//menage by settings of programm
SettingsForm::SettingsForm(QDialog *parent) :
    QDialog(parent)
{
    QSettings settings("Saturn");
    this->setWindowTitle(tr("Settings dialog"));
    QGridLayout * gridLayout = new QGridLayout(this);
    this->setLayout(gridLayout);

    label_rsc=new QLabel(tr("RSC file:"), this);
    label_img = new QLabel(tr("Path to images:"), this);

    line_rsc=new QLineEdit(this);
    line_rsc->setMinimumWidth(200);
    line_rsc->setText(settings.value("last_rsc").toString());

    line_img = new QLineEdit(line_rsc);
    line_img->setMinimumWidth(200);
    line_img->setText(settings.value("last_img").toString());

    pb_rsc=new QPushButton(this);
    pb_rsc->setMaximumWidth(40);
    pb_rsc->setText("...");
    pb_img=new QPushButton(this);
    pb_img->setMaximumWidth(40);
	pb_img->setText("...");

    pb_ok = new QPushButton(this);
    pb_ok->setText("Save");
    gridLayout->addWidget(label_rsc,0,0);
    gridLayout->addWidget(line_rsc,0,1);
    gridLayout->addWidget(pb_rsc,0,2);
    gridLayout->addWidget(label_img,1,0);
    gridLayout->addWidget(line_img,1,1);
    gridLayout->addWidget(pb_img,1,2);
    gridLayout->addWidget(pb_ok,2,2);
    connect(pb_rsc,SIGNAL(clicked()),this, SLOT(selectRSC()));
    connect(pb_img,SIGNAL(clicked()),this, SLOT(selectIMG()));
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

void SettingsForm::selectIMG()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open directory whith images"),
                                                     QString::null,
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty()){
        return;
    }else{
        QSettings settings("Saturn");
        settings.setValue("last_img",dir);
        line_img->setText(dir);
    }
}
