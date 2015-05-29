#ifndef SIGNSTREE_H
#define SIGNSTREE_H

#include <QTreeView>

class SignsTree : public QTreeView
{
    Q_OBJECT
public:
    explicit SignsTree(QObject *parent = 0);
    
    void mousePressEvent(QMouseEvent *event);
signals:
    
public slots:
    
};

#endif // SIGNSTREE_H
