#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <QtGui>
#include <QObject>
#include <QTableView>
#include <QLayout>
#include "config_event_manager.h"
#include "../eventsmodel/eventsmodel.h"

class __EXPORT_EVENT_MANAGER EventManager : public QWidget
{
    Q_OBJECT

public:
	EventManager(QWidget *parent = 0);
    ~EventManager();
   
public slots:


private:
    QTableView * tableView;
    EventsModel * eventsModel;


};

#endif // EVENT_MANAGER_H
