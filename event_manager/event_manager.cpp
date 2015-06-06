#include "event_manager.h"

EventManager::EventManager(QWidget *parent, Coord *coord)
    : QWidget(parent)
{

    if(coord == 0){
        this->coord = new Coord();
        this->whithCoord = false;
    }else{
        this->coord = coord;
        this->whithCoord = true;
        return;
    }

    QGridLayout * grid = new QGridLayout(this);
    addNewEventPB = new QPushButton("Добавить новое событие");
    tableView = new QTableView(this);
    grid->addWidget(tableView,0,0);
    grid->addWidget(this->addNewEventPB,0,1);

    eventsModel = new EventsModel();
    tableView->setModel(eventsModel);

    this->setLayout(grid);

    connect(this->addNewEventPB,SIGNAL(clicked()),this,SLOT(openNewEventDialog()));
    connect(this,SIGNAL(eventInsert()),eventsModel,SLOT(UpdateModel()));
}

EventManager::~EventManager(){

}

void EventManager::addNewEventDialog(QWidget *parent){
    newEvent = new Event();

    newEventDialog = new QDialog(parent);
    newEventDialog->setWindowTitle("Добавление нового события");

    QGridLayout * grLayout = new QGridLayout();
    QFormLayout * formLayout = new QFormLayout();

    nameLE = new QLineEdit();

    statusCB = new QComboBox();
    statusCB->addItem(QIcon(":/icons/icons/red.ico"),"Актуальное",1);
    statusCB->addItem(QIcon(":/icons/icons/grey.ico"),"Прошедшее",2);
    statusCB->addItem(QIcon(":/icons/icons/magenta.ico"),"Планируемое",3);
    statusCB->addItem(QIcon(":/icons/icons/yellow.ico"),"Не состоявшееся",4);

    typeCB = new QComboBox();
    QMap<int, QMap<QString,int > > types = newEvent->getEventsTypes();
    QMapIterator<int, QMap<QString,int > > i(types);
    while (i.hasNext()) {
        i.next();
        int id_type = i.key();
        QMapIterator<QString, int> it(i.value());
        while (it.hasNext()) {
            it.next();
            QSqlQuery query;
            if(!query.exec(QString("select sign_picture from signs where id_sign = %1").arg(it.value()))){
                qDebug() << query.lastError().text();
                return;
            }
            QPixmap pixmap;
            if(query.size() > 0){
                while(query.next()){
                    pixmap.loadFromData(query.value(0).toByteArray());
                }
            }else{
                pixmap.load(":/icons/icons/no_photo.png");
            }
            typeCB->addItem(QIcon(pixmap),it.key(),id_type);
        }
    }

    QDateTime DT;
    QCalendarWidget* cws = new QCalendarWidget();
    cws->setFirstDayOfWeek(Qt::Monday);
    QCalendarWidget* cwe = new QCalendarWidget();
    cwe->setFirstDayOfWeek(Qt::Monday);

    DTS = new QDateTimeEdit(DT.currentDateTime());
    DTS->setCalendarPopup(true);
    DTS->setCalendarWidget(cws);

    DTE = new QDateTimeEdit(DT.currentDateTime());
    DTE->setCalendarPopup(true);
    DTE->setCalendarWidget(cwe);

    descriptionTE = new QTextEdit();
    resumeTE = new QTextEdit();

    this->whithCoord?lagLE = new QLineEdit(QString::number(coord->getLatDegrees())):lagLE = new QLineEdit();
    lagLE->setValidator(new QIntValidator(0,360,lagLE));
    this->whithCoord?lamLE = new QLineEdit(QString::number(coord->getLatMinutes())):lamLE = new QLineEdit();
    lamLE->setValidator(new QIntValidator(0,60,lamLE));
    this->whithCoord?lasLE = new QLineEdit(QString::number(coord->getLatSeconds())):lasLE = new QLineEdit();
    lasLE->setValidator(new QDoubleValidator(0.0,60.0,2,lasLE));
    this->whithCoord?logLE = new QLineEdit(QString::number(coord->getLongDegrees())):logLE = new QLineEdit();
    logLE->setValidator(new QIntValidator(0,360,logLE));
    this->whithCoord?lomLE = new QLineEdit(QString::number(coord->getLongMinutes())):lomLE = new QLineEdit();
    lomLE->setValidator(new QIntValidator(0,60,lomLE));
    this->whithCoord?losLE = new QLineEdit(QString::number(coord->getLongSeconds())):losLE = new QLineEdit();
    losLE->setValidator(new QDoubleValidator(0.00,60.00,2,losLE));

    QLabel * line = new QLabel();
    line->setFrameStyle(QFrame::HLine | QFrame::Raised);
    line->setLineWidth(2);
    QLabel * line2 = new QLabel();
    line2->setFrameStyle(QFrame::HLine | QFrame::Raised);
    line2->setLineWidth(2);

    this->suorceTypeObjectCBNE = new QComboBox();
    this->suorceObjectCBNE = new QComboBox();
    this->getTypeObjectCBNE = new QComboBox();
    this->getObjectCBNE = new QComboBox();
    this->getTypeObjectCB();

    QPushButton *saveEvent = new QPushButton("Сохранить событие");

    formLayout->addRow("Наименование:",nameLE);
    formLayout->addRow("Статус события:",statusCB);
    formLayout->addRow("Тип события:",typeCB);
    formLayout->addRow("Время начала события:",DTS);
    formLayout->addRow("Время окончания события:",DTE);
    formLayout->addRow("Описание события:",descriptionTE);
    formLayout->addRow("Выводы по событию:",resumeTE);
    formLayout->addWidget(line);
    formLayout->addRow("Широта (градусы):",lagLE);
    formLayout->addRow("Широта (минуты):",lamLE);
    formLayout->addRow("Широта (секунды):",lasLE);
    formLayout->addRow("Долгота (градусы):",logLE);
    formLayout->addRow("Долгота (минуты):",lomLE);
    formLayout->addRow("Долгота (секунды):",losLE);
    formLayout->addWidget(line2);
    formLayout->addRow("Тип объекта инициатора:",this->suorceTypeObjectCBNE);
    formLayout->addRow("Объект инициатор события:",suorceObjectCBNE);
    formLayout->addRow("Тип объекта события:",this->getTypeObjectCBNE);
    formLayout->addRow("Объект события:",getObjectCBNE);
    formLayout->addWidget(saveEvent);

    grLayout->addLayout(formLayout,0,0,0,2);
    newEventDialog->setLayout(grLayout);
    newEventDialog->show();
    newEventDialog->setModal(true);
    isEventDialogOpen = true;

    connect(this->suorceTypeObjectCBNE, SIGNAL(currentIndexChanged(int)), this , SLOT(sourceTypeChange(int)));
    connect(this->getTypeObjectCBNE, SIGNAL(currentIndexChanged(int)), this , SLOT(getTypeChange(int)));
    connect(saveEvent,SIGNAL(clicked()),this,SLOT(saveNewEvent()));
}

void EventManager::openNewEventDialog()
{

    this->addNewEventDialog(this);
}

void EventManager::sourceTypeChange(int index)
{
    QSqlQuery query;
    this->suorceObjectCBNE->clear();
    this->suorceObjectCBNE->addItem("Не выбран объект",0);
    QString table_name = this->suorceTypeObjectCBNE->itemData(index).toString();
    if(table_name == "not"){
        return;
    }
    if(!query.exec(QString("SELECT name_%1, id_%1  FROM %1").arg(table_name))){
        qDebug() << query.lastError().text();
        return;
    }
    while(query.next()){
        this->suorceObjectCBNE->addItem(query.value(0).toString(),query.value(1).toInt());
    }
}

void EventManager::getTypeChange(int index)
{
    QSqlQuery query;
    this->getObjectCBNE->clear();
    this->getObjectCBNE->addItem("Не выбран объект",0);
    QString table_name = this->getTypeObjectCBNE->itemData(index).toString();
    if(table_name == "not"){
        return;
    }
    if(!query.exec(QString("SELECT name_%1, id_%1  FROM %1").arg(table_name))){
        qDebug() << query.lastError().text();
        return;
    }
    while(query.next()){
        this->getObjectCBNE->addItem(query.value(0).toString(),query.value(1).toInt());
    }
}

void EventManager::saveNewEvent()
{
    if(nameLE->text().isEmpty()){
        return;
    }else{
        newEvent->setName(nameLE->text());
    }
    newEvent->setStatus(statusCB->itemData(statusCB->currentIndex()).toInt());
    newEvent->setIdTypeEvent(typeCB->itemData(typeCB->currentIndex()).toInt());
    newEvent->setStartDate(&DTS->dateTime());
    newEvent->setEndDate(&DTE->dateTime());
    newEvent->setDescription(descriptionTE->toPlainText());
    newEvent->setResume(resumeTE->toPlainText());
    if((lagLE->text().isEmpty()) || (lamLE->text().isEmpty()) ||(lasLE->text().isEmpty()) ||(logLE->text().isEmpty()) ||(lomLE->text().isEmpty()) ||(losLE->text().isEmpty())){
        return;
    }
    Coord * coordinate = new Coord(lagLE->text().toInt(),lamLE->text().toInt(),lasLE->text().toDouble(),logLE->text().toInt(),lomLE->text().toInt(),losLE->text().toDouble());
    newEvent->setCoordinate(coordinate);
    QList<EventObject *> eventObjects;
    EventObject * SEO = new EventObject();
    int id_object;
    int id_type_object = suorceTypeObjectCBNE->itemData(suorceTypeObjectCBNE->currentIndex(),Qt::UserRole + 1).toInt();
    if(id_type_object > 0){
        SEO->setIdTypeEventObject(id_type_object);
        SEO->setIsSource(true);
        id_object = suorceObjectCBNE->itemData(suorceObjectCBNE->currentIndex()).toInt();
        if(id_object>0){
            SEO->setIdObject(id_object);
            eventObjects.append(SEO);
        }
    }

    EventObject * GEO = new EventObject();;
    id_type_object = getTypeObjectCBNE->itemData(getTypeObjectCBNE->currentIndex(),Qt::UserRole + 1).toInt();
    if(id_type_object > 0){
        GEO->setIdTypeEventObject(id_type_object);
        GEO->setIsSource(false);
        id_object = getObjectCBNE->itemData(getObjectCBNE->currentIndex()).toInt();
        if(id_object>0){
            GEO->setIdObject(id_object);
            eventObjects.append(GEO);
        }
    }
    if(eventObjects.size() > 0){
        newEvent->setEventObjects(&eventObjects);
    }else{
        return;
    }
    if(newEvent->insertEventToDB()){
        newEventDialog->close();
        emit eventInsert();
        isEventDialogOpen = false;
    }
}

void EventManager::getTypeObjectCB()
{
    QSqlQuery query;
    this->suorceTypeObjectCBNE->clear();
    this->getTypeObjectCBNE->clear();
    this->suorceTypeObjectCBNE->addItem("Не выбран тип","not");
    this->getTypeObjectCBNE->addItem("Не выбран тип","not");
    if(!query.exec(QString("SELECT name_type_event_object,\"table_name\" ,id_type_event_object  FROM type_event_object"))){
        qDebug() << query.lastError().text();
        return;
    }
    int index=1;
    while(query.next()){
        int id_type_event_object = query.value(2).toInt();
        this->suorceTypeObjectCBNE->insertItem(index,query.value(0).toString(),query.value(1).toString());
        this->suorceTypeObjectCBNE->setItemData(index,id_type_event_object,Qt::UserRole + 1);
        this->getTypeObjectCBNE->insertItem(index,query.value(0).toString(),query.value(1).toString());
        this->getTypeObjectCBNE->setItemData(index,id_type_event_object,Qt::UserRole + 1);
        index++;
    }
}

void EventManager::getObjectsCB()
{

}
