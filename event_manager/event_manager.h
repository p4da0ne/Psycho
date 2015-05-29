#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <QtGui>
#include <QObject>
#include "config_event_manager.h"

class __EXPORT_EVENT_MANAGER EventManager : public QWidget
{
    Q_OBJECT

public:
	EventManager(QWidget *parent = 0);
    ~EventManager();
   
public slots:


private:

};

#endif // EVENT_MANAGER_H
