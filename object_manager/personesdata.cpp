#include "personesdata.h"
#include "ui_personesdata_form.h"

PersonesData::PersonesData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonesData)
{
    ui->setupUi(this);
}

PersonesData::~PersonesData()
{
    delete ui;
}
