#ifndef MYQMIDIAREA_H
#define MYQMIDIAREA_H

#include <QMdiArea>
#include <QtGui>
 
class myQMdiArea : public QMdiArea
{
  Q_OBJECT
  public:
    myQMdiArea(QPixmap image,QWidget *parent = 0);
	~myQMdiArea();
    QPixmap img;
  protected:
    void paintEvent(QPaintEvent *) override;


};
 
#endif // MYQMIDIAREA_H
