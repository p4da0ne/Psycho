#include "mymdiarea.h"
 
myQMdiArea::myQMdiArea(QPixmap image,QWidget *parent)
  : QMdiArea(parent)
{
    img = image;
    viewport()->setAutoFillBackground(false);
}

myQMdiArea::~myQMdiArea()
{

}

void myQMdiArea::paintEvent(QPaintEvent *paintEvent)
{
    QPainter painter(viewport());

    if (!img.isNull()) {
        QPixmap scaled = img.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        int x = (width() - scaled.width()) / 2;
        int y = (height() - scaled.height()) / 2;
        painter.drawPixmap(x, y, scaled);
    }

    QMdiArea::paintEvent(paintEvent);
}
