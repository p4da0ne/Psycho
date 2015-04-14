#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QMouseEvent>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);
    
signals:
    void label_clicked();

protected:
        void mouseDoubleClickEvent(QMouseEvent *e);
public slots:
    
};

#endif // MYLABEL_H
