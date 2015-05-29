#include "signstree.h"

SignsTree::SignsTree(QObject *parent) :
    QTreeView(parent)
{
}
void SignsTree::mousePressEvent(QMouseEvent * event){
    if(event->button() == Qt::RightButton){
        QMenu * menu = new QMenu("Test");
        menu->addMenu("test2");
        menu->show();
    }
}
