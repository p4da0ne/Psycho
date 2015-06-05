#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <QtGui>
#include <QObject>
#include <QTableView>
#include <QLayout>
#include <QList>
#include "config_event_manager.h"
#include "../eventsmodel/eventsmodel.h"

class __EXPORT_EVENT_MANAGER EventManager : public QWidget
{
    Q_OBJECT

public:
    EventManager(QWidget *parent = 0, Coord * coord = 0 );
    ~EventManager();

public slots:
    void addNewEventDialog(QWidget *parent);
    void openNewEventDialog();

private slots:
    void sourceTypeChange(int index);
    void getTypeChange(int index);
    void saveNewEvent();

private:
    QTableView * tableView;
    EventsModel * eventsModel;
    Event * current_event;
    QPushButton * addNewEventPB;
    void getTypeObjectCB();
    void getObjectsCB();
    QComboBox *suorceTypeObjectCBNE;
    QComboBox *suorceObjectCBNE;
    QComboBox *getTypeObjectCBNE;
    QComboBox *getObjectCBNE;

    QDialog * newEventDialog;
    Event * newEvent;
    QLineEdit * nameLE;
    QComboBox * statusCB;
    QComboBox * typeCB;
    QDateTimeEdit * DTS;
    QDateTimeEdit * DTE;
    QTextEdit * descriptionTE;
    QTextEdit * resumeTE;
    QLineEdit * lagLE;
    QLineEdit * lamLE;
    QLineEdit * lasLE;
    QLineEdit * logLE;
    QLineEdit * lomLE;
    QLineEdit * losLE;

    Coord * coord;

    bool whithCoord;
    bool isEventDialogOpen;

signals:
    void eventInsert();

};

#endif // EVENT_MANAGER_H
