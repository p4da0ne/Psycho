#ifndef SIGNSTREE_H
#define SIGNSTREE_H

#include <QTreeView>
#include <QMouseEvent>
#include <QMenu>

class SignsTree : public QTreeView
{
    Q_OBJECT
public:
    explicit SignsTree(QWidget *parent = 0);
    
    void mousePressEvent(QMouseEvent * event);
signals:
    
public slots:
    
};

#endif // SIGNSTREE_H
