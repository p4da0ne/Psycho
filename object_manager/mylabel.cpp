#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent) :
    QLabel(parent)
{


}

void MyLabel::mouseDoubleClickEvent(QMouseEvent *e)
{
   if(e->button() == Qt::LeftButton)
     {
      emit label_clicked();
     }
}
