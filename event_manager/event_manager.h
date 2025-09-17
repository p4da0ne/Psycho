#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <QtGui>
#include <QObject>
#include <QTableView>
#include <QLayout>
#include <QList>
#include <QSortFilterProxyModel>
#include <QtWidgets>
#include <QRegExp>
#include "config_event_manager.h"
#include "progressthread.h"
#include "../eventsmodel/eventsmodel.h"

class __EXPORT_EVENT_MANAGER EventManager : public QWidget
{
    Q_OBJECT

public:
    EventManager(QWidget *parent = 0, Coord * coord = 0 );
    EventManager(QWidget *parent, bool isDialog);
    ~EventManager();




public slots:
    void addNewEventDialog(QWidget *parent);
    void openNewEventDialog();
    void eventClick(QModelIndex index);
    void mediaClick(QModelIndex);
    void openFileDialog();
    void viewMediaContentDialog(int id_event, QWidget *parent = 0);
    void setEventsPropertyEnabled(bool enabled);
    void restEventsProperty();

private slots:
    void nameLEEChanged(QString text);
    void DTSEChanged(QDateTime dateTime);
    void DTEEChanged(QDateTime dateTime);
    void sourceTypeChange(int index);
    void sourceTypeChangeE(int index);
    void getTypeChange(int index);
    void getTypeChangeE(int index);
    void saveNewEvent();
    void filterNameTextChanged(QString text);
    void updateModel();
    void resizeTableView();
    void newEventMediaDialog();
    void addNewEventMedia();
    void MediaContentInserted(int);
    void ErrorDialog(QString error);
    void EventsTableCustomMenu(const QPoint &);
    void slotRemoveEvent();
    void EventsMediaTableCustomMenu(const QPoint &pe);
    void slotRemoveEventMedia();

    void statusCBEChanged(int index);
    void typeCBEChanged(int index);

    void descriptionTEEChanged();
    void resumeTEEChanged();
    void CoordChanged();
    void suorceObjectCBNEEChanged(int index);
    void getObjectCBNEEChanged(int index);

private:
    void initUIX();
    QTableView * tableView;
    QSortFilterProxyModel *proxyModel;
    EventsModel * eventsModel;
    Event * current_event;
    QPushButton * addNewEventPB;
    void getTypeObjectCB(QComboBox *source, QComboBox *get);
    void getObjectsCB();
    QComboBox *suorceTypeObjectCBNE;
    QComboBox *suorceObjectCBNE;
    QComboBox *getTypeObjectCBNE;
    QComboBox *getObjectCBNE;
    QPushButton *saveEvent;
    QFormLayout * formLayout;

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

    QPushButton * upDateModelButton;
    QGroupBox * filterGroupBox;
    QLineEdit * filterName;
    Coord * coord;

    QFormLayout * formLayoutE;
    QFormLayout * formLayoutECoord;
    QComboBox *suorceTypeObjectCBNEE;
    QComboBox *suorceObjectCBNEE;
    QComboBox *getTypeObjectCBNEE;
    QComboBox *getObjectCBNEE;
    QLineEdit * nameLEE;
    QComboBox * statusCBE;
    QComboBox * typeCBE;
    QDateTimeEdit * DTSE;
    QDateTimeEdit * DTEE;
    QTextEdit * descriptionTEE;
    QTextEdit * resumeTEE;
    QLineEdit * lagLEE;
    QLineEdit * lamLEE;
    QLineEdit * lasLEE;
    QLineEdit * logLEE;
    QLineEdit * lomLEE;
    QLineEdit * losLEE;

    QTableView * eventMedia;
    QPushButton *addEventMedia;
    QLineEdit * mediaFilePath;
    QTextEdit * mediaDescription;
    QComboBox * mediaType;
    QDialog * mediaDialog;
    QSortFilterProxyModel * proxyMediaModel;

    QTableView * tableMediaContent;

    bool whithCoord;
    bool isEventDialogOpen;
    bool isDialog;

signals:
    void eventDataChanged();
};


#endif // EVENT_MANAGER_H
