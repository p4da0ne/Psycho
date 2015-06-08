#include "event_manager.h"

EventManager::EventManager(QWidget *parent, Coord *coord)
    : QWidget(parent){
    if(coord == 0){
        isDialog = true;
        this->coord = new Coord();
        this->whithCoord = false;
    }else{
        isDialog = false;
        this->coord = coord;
        this->whithCoord = true;
        return;
    }

    initUIX();
}

EventManager::~EventManager(){

}

void EventManager::initUIX(){
    current_event = new Event();
    QGridLayout * grid = new QGridLayout(this);
    addNewEventPB = new QPushButton(QIcon(":/icons/icons/add_but.png"),"",this);
    addNewEventPB->setToolTip("Добавить новое событие");
    upDateModelButton = new QPushButton(QIcon(":/icons/icons/sync.ico"),"",this);
    upDateModelButton->setToolTip("Обновить список событий");
    tableView = new QTableView(this);

    filterGroupBox = new QGroupBox("Фильтр событий", this);
    filterName = new QLineEdit(this);
    QHBoxLayout * groupBoxLayout = new QHBoxLayout();
    groupBoxLayout->addWidget(new QLabel("Имя:"));
    groupBoxLayout->addWidget(filterName);
    filterGroupBox->setLayout(groupBoxLayout);

    formLayoutE = new QFormLayout();
    formLayoutECoord = new QFormLayout();
    nameLEE = new QLineEdit();
    statusCBE = new QComboBox();
    statusCBE->addItem(QIcon(":/icons/icons/red.ico"),"Актуальное",1);
    statusCBE->addItem(QIcon(":/icons/icons/grey.ico"),"Прошедшее",2);
    statusCBE->addItem(QIcon(":/icons/icons/magenta.ico"),"Планируемое",3);
    statusCBE->addItem(QIcon(":/icons/icons/yellow.ico"),"Не состоявшееся",4);

    typeCBE = new QComboBox();
    QMap<int, QMap<QString,int > > types = current_event->getEventsTypes();
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
            typeCBE->addItem(QIcon(pixmap),it.key(),id_type);
        }
    }

    QCalendarWidget* cws = new QCalendarWidget();
    cws->setFirstDayOfWeek(Qt::Monday);
    QCalendarWidget* cwe = new QCalendarWidget();
    cwe->setFirstDayOfWeek(Qt::Monday);

    DTSE = new QDateTimeEdit();
    DTSE->setCalendarPopup(true);
    DTSE->setCalendarWidget(cws);

    DTEE = new QDateTimeEdit();
    DTEE->setCalendarPopup(true);
    DTEE->setCalendarWidget(cwe);

    descriptionTEE = new QTextEdit();
    resumeTEE = new QTextEdit();

    lagLEE = new QLineEdit();
    lagLEE->setValidator(new QIntValidator(0,360,lagLEE));
    lamLEE = new QLineEdit();
    lamLEE->setValidator(new QIntValidator(0,60,lamLEE));
    lasLEE = new QLineEdit();
    lasLEE->setValidator(new QDoubleValidator(0.0,60.0,2,lasLEE));
    logLEE = new QLineEdit();
    logLEE->setValidator(new QIntValidator(0,360,logLEE));
    lomLEE = new QLineEdit();
    lomLEE->setValidator(new QIntValidator(0,60,lomLEE));
    losLEE = new QLineEdit();
    losLEE->setValidator(new QDoubleValidator(0.00,60.00,2,losLEE));
    QPushButton * editCoordinates = new QPushButton("Изменить координаты");

    this->suorceTypeObjectCBNEE = new QComboBox();
    this->suorceObjectCBNEE = new QComboBox();
    this->getTypeObjectCBNEE = new QComboBox();
    this->getObjectCBNEE = new QComboBox();
    this->getTypeObjectCB(this->suorceTypeObjectCBNEE,this->getTypeObjectCBNEE);

    eventMedia = new QTableView(this);
    eventMedia->setContextMenuPolicy(Qt::CustomContextMenu);
    addEventMedia = new QPushButton(QIcon(":/icons/icons/add_but.png"),"",this);
    addEventMedia->setToolTip("Добавить контент");

    formLayoutE->addRow("Наименование:",nameLEE);
    formLayoutE->addRow("Статус события:",statusCBE);
    formLayoutE->addRow("Тип события:",typeCBE);
    formLayoutE->addRow("Время начала события:",DTSE);
    formLayoutE->addRow("Время окончания события:",DTEE);
    formLayoutE->addRow("Описание события:",descriptionTEE);
    formLayoutE->addRow("Выводы по событию:",resumeTEE);
    formLayoutECoord->addRow("Широта (градусы):",lagLEE);
    formLayoutECoord->addRow("Широта (минуты):",lamLEE);
    formLayoutECoord->addRow("Широта (секунды):",lasLEE);
    formLayoutECoord->addRow("Долгота (градусы):",logLEE);
    formLayoutECoord->addRow("Долгота (минуты):",lomLEE);
    formLayoutECoord->addRow("Долгота (секунды):",losLEE);
    formLayoutE->addRow("Тип объекта инициатора:",this->suorceTypeObjectCBNEE);
    formLayoutE->addRow("Объект инициатор события:",suorceObjectCBNEE);
    formLayoutE->addRow("Тип объекта события:",this->getTypeObjectCBNEE);
    formLayoutE->addRow("Объект события:",getObjectCBNEE);

    connect(nameLEE,SIGNAL(textEdited(QString)),this,SLOT(nameLEEChanged(QString)));
    connect(statusCBE,SIGNAL(currentIndexChanged(int)),this,SLOT(statusCBEChanged(int)));
    connect(typeCBE,SIGNAL(currentIndexChanged(int)),this,SLOT(typeCBEChanged(int)));
    connect(DTSE,SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(DTSEChanged(QDateTime)));
    connect(DTEE,SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(DTSEChanged(QDateTime)));
    connect(descriptionTEE,SIGNAL(textChanged()),this,SLOT(descriptionTEEChanged()));
    connect(resumeTEE,SIGNAL(textChanged()),this,SLOT(resumeTEEChanged()));
    connect(suorceObjectCBNEE,SIGNAL(activated(int)),this,SLOT(suorceObjectCBNEEChanged(int)));
    connect(getObjectCBNEE,SIGNAL(activated(int)),this,SLOT(getObjectCBNEEChanged(int)));
    connect(editCoordinates,SIGNAL(clicked()),this,SLOT(CoordChanged()));

    connect(this->suorceTypeObjectCBNEE, SIGNAL(currentIndexChanged(int)), this , SLOT(sourceTypeChangeE(int)));
    connect(this->getTypeObjectCBNEE, SIGNAL(currentIndexChanged(int)), this , SLOT(getTypeChangeE(int)));

    grid->addWidget(this->addNewEventPB,0,0);
    grid->addWidget(this->upDateModelButton,0,1);

    grid->addWidget(filterGroupBox,1,0,1,10);

    grid->addWidget(tableView,2,0,10,10);
    grid->addLayout(formLayoutE,1,11,10,2);
    grid->addLayout(formLayoutECoord,1,13,4,2);
    grid->addWidget(editCoordinates,4,13);
    grid->addWidget(addEventMedia,5,13);
    grid->addWidget(eventMedia,6,13,4,2);

    eventsModel = new EventsModel();
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(eventsModel);
    proxyModel->setDynamicSortFilter(true);

    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    tableView->setSortingEnabled(true);
    tableView->setModel(proxyModel);
    resizeTableView();
    this->setLayout(grid);

    setEventsPropertyEnabled(false);
    connect(tableView, SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(EventsTableCustomMenu(const QPoint &)));
    connect(eventMedia, SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(EventsMediaTableCustomMenu(const QPoint &)));

    connect(filterName,SIGNAL(textChanged(QString)),this,SLOT(filterNameTextChanged(QString)));
    connect(upDateModelButton,SIGNAL(clicked()),this,SLOT(updateModel()));
    connect(tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(eventClick(QModelIndex)));
    connect(eventMedia,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(mediaClick(QModelIndex)));
    connect(addEventMedia,SIGNAL(clicked()),this,SLOT(newEventMediaDialog()));
    connect(this->addNewEventPB,SIGNAL(clicked()),this,SLOT(openNewEventDialog()));
    connect(this,SIGNAL(eventDataChanged()),this,SLOT(updateModel()));
}

void EventManager::filterNameTextChanged(QString text){
    proxyModel->setFilterKeyColumn(0);
    QRegExp::PatternSyntax syntax =QRegExp::FixedString;
    Qt::CaseSensitivity caseSensitivity =Qt::CaseInsensitive;
    QRegExp regExp(text, caseSensitivity, syntax);
    proxyModel->setFilterRegExp(regExp);
}

void EventManager::updateModel(){
    eventsModel->UpdateModel();
    resizeTableView();
}

void EventManager::resizeTableView(){
    tableView->setColumnWidth(0,180);
    tableView->setColumnWidth(1,110);
    tableView->setColumnWidth(2,130);
    tableView->setColumnWidth(3,110);
    tableView->setColumnWidth(4,110);
}

void EventManager::newEventMediaDialog(){
    mediaDialog = new QDialog(this);
    QFormLayout * FML = new QFormLayout();
    QHBoxLayout * mediaL = new QHBoxLayout();
    mediaFilePath = new QLineEdit();
    QPushButton * OFD = new QPushButton("...");
    mediaDescription = new QTextEdit();
    mediaType = new QComboBox();
    QSqlQuery query;
    if(!query.exec("SELECT * FROM media_type")){
        qDebug() << query.lastError().text();
        return;
    }
    while(query.next()){
        mediaType->addItem(query.value(1).toString(),query.value(0));
    }
    QPushButton * saveMedia = new QPushButton("Сохранить");

    mediaL->addWidget(mediaFilePath);
    mediaL->addWidget(OFD);
    FML->addRow("Путь к файлу",mediaL);
    FML->addRow("Тип медиа",mediaType);
    FML->addRow("Описание файла",mediaDescription);
    FML->addWidget(saveMedia);
    mediaDialog->setLayout(FML);
    mediaDialog->show();

    connect(OFD,SIGNAL(clicked()),this,SLOT(openFileDialog()));
    connect(saveMedia,SIGNAL(clicked()),this,SLOT(addNewEventMedia()));
}

void EventManager::addNewEventMedia(){
    QString filePath = mediaFilePath->text();
    if(filePath.isEmpty())
        return;
    QFileInfo FI(filePath);
    QString fileName = "SaturnMediaContent" + FI.suffix() + "." + FI.suffix();
    int i=0;
    int type = mediaType->itemData(mediaType->currentIndex()).toInt();
    QString description;
    if(mediaDescription->toPlainText().isEmpty())
        description="";
    else
        description=mediaDescription->toPlainText();

    QDialog * l = new QDialog(this,Qt::Popup);
    l->setWindowTitle("Идет загрузка...");
    connect(current_event,SIGNAL(MediaContentInserted(int)),l,SLOT(close()));
    connect(current_event,SIGNAL(ErrorMediaContentInsert(QString)),l,SLOT(close()));
    current_event->InsertMediaItems(filePath,type,fileName,description);
    connect(current_event,SIGNAL(MediaContentInserted(int)),this,SLOT(MediaContentInserted(int)));
    connect(current_event,SIGNAL(ErrorMediaContentInsert(QString)),this,SLOT(ErrorDialog(QString)));
    mediaDialog->close();
    QHBoxLayout la;
    QLabel lab;
    l->setBackgroundRole(QPalette::Light);

    lab.setText("Подождите, идет загрузка данных в БД");
    la.addWidget(&lab);
    l->setModal(true);
    l->resize(300,100);
    l->setLayout(&la);
    l->exec();


}

void EventManager::MediaContentInserted(int)
{

    current_event->updateMediaEvents();
}

void EventManager::ErrorDialog(QString error)
{
    QMessageBox::warning(this,"Ошибка",error);
}

void EventManager::openFileDialog(){
    mediaFilePath->setText(QFileDialog::getOpenFileName());
}

void EventManager::viewMediaContentDialog(int id_event, QWidget *parent)
{
    tableMediaContent = new QTableView();
    Event * dialogEvent = new Event(id_event);
    dialogEvent->updateMediaEvents();
    QSortFilterProxyModel * proxy= new QSortFilterProxyModel();
    proxy->setSourceModel(dialogEvent->getMediaEvents());
    tableMediaContent->setModel(proxy);
    QDialog * mediaContentDialog = new QDialog(parent);
    QVBoxLayout * VBL = new QVBoxLayout();
    VBL->addWidget(tableMediaContent);
    mediaContentDialog->setLayout(VBL);
    mediaContentDialog->show();

    tableMediaContent->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(tableMediaContent, SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(EventsMediaTableCustomMenu(const QPoint &)));
    connect(tableMediaContent,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(mediaClick(QModelIndex)));

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
    this->suorceTypeObjectCBNE = new QComboBox();
    this->suorceObjectCBNE = new QComboBox();
    this->getTypeObjectCBNE = new QComboBox();
    this->getObjectCBNE = new QComboBox();
    this->getTypeObjectCB(this->suorceTypeObjectCBNE,this->getTypeObjectCBNE);

    QPushButton *saveEvent = new QPushButton("Сохранить событие");

    formLayout->addRow("Наименование:",nameLE);
    formLayout->addRow("Статус события:",statusCB);
    formLayout->addRow("Тип события:",typeCB);
    formLayout->addRow("Время начала события:",DTS);
    formLayout->addRow("Время окончания события:",DTE);
    formLayout->addRow("Описание события:",descriptionTE);
    formLayout->addRow("Выводы по событию:",resumeTE);
    formLayout->addRow("Широта (градусы):",lagLE);
    formLayout->addRow("Широта (минуты):",lamLE);
    formLayout->addRow("Широта (секунды):",lasLE);
    formLayout->addRow("Долгота (градусы):",logLE);
    formLayout->addRow("Долгота (минуты):",lomLE);
    formLayout->addRow("Долгота (секунды):",losLE);
    formLayout->addRow("Тип объекта инициатора:",this->suorceTypeObjectCBNE);
    formLayout->addRow("Объект инициатор события:",suorceObjectCBNE);
    formLayout->addRow("Тип объекта события:",this->getTypeObjectCBNE);
    formLayout->addRow("Объект события:",getObjectCBNE);
    formLayout->addWidget(saveEvent);
    qDebug()<<"Otkrita forma";
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

void EventManager::eventClick(QModelIndex index){
    setEventsPropertyEnabled(true);
    Event * temp_event = new Event(index.data(Qt::UserRole + 3).toInt());
    nameLEE->setText(temp_event->getName());

    int statusIndex = statusCBE->findData(temp_event->getIdStatus(),Qt::UserRole,Qt::MatchFixedString);
    statusCBE->setCurrentIndex(statusIndex);

    int typeIndex = typeCBE->findData(temp_event->getIdTypeEvent(),Qt::UserRole,Qt::MatchFixedString);
    typeCBE->setCurrentIndex(typeIndex);

    DTSE->setDateTime(*temp_event->getStartDate());
    DTEE->setDateTime(*temp_event->getEndDate());

    descriptionTEE->setPlainText(temp_event->getDescription());
    resumeTEE->setPlainText(temp_event->getResume());

    lagLEE->setText(QString::number(temp_event->getCoordinate()->getLatDegrees()));
    lamLEE->setText(QString::number(temp_event->getCoordinate()->getLatMinutes()));
    lasLEE->setText(QString::number(temp_event->getCoordinate()->getLatSeconds()));
    logLEE->setText(QString::number(temp_event->getCoordinate()->getLongDegrees()));
    lomLEE->setText(QString::number(temp_event->getCoordinate()->getLongMinutes()));
    losLEE->setText(QString::number(temp_event->getCoordinate()->getLongSeconds()));
    QList<EventObject * > eventObject = *temp_event->getObjects();
    for(int i=0; i < eventObject.size();i++){
        EventObject * object = eventObject.at(i);
        if(object->isSource()){
            int index = this->suorceTypeObjectCBNEE->findData(object->getIdTypeEventObject(),Qt::UserRole + 1,Qt::MatchFixedString);
            this->suorceTypeObjectCBNEE->setCurrentIndex(index);
            index = this->suorceObjectCBNEE->findData(object->getIdObject(),Qt::UserRole,Qt::MatchFixedString);
            this->suorceObjectCBNEE->setCurrentIndex(index);
        }else{
            int index = this->getTypeObjectCBNEE->findData(object->getIdTypeEventObject(),Qt::UserRole + 1,Qt::MatchFixedString);
            this->getTypeObjectCBNEE->setCurrentIndex(index);
            index = this->getObjectCBNEE->findData(object->getIdObject(),Qt::UserRole,Qt::MatchFixedString);
            this->getObjectCBNEE->setCurrentIndex(index);
        }
    }
    current_event = new Event(index.data(Qt::UserRole + 3).toInt());
    current_event->updateMediaEvents();
    proxyMediaModel = new QSortFilterProxyModel(this);
    proxyMediaModel->setSourceModel(current_event->getMediaEvents());
    proxyMediaModel->setDynamicSortFilter(true);
    eventMedia->setSortingEnabled(true);
    eventMedia->setModel(proxyMediaModel);
}

void EventManager::mediaClick(QModelIndex index)
{
    ProgressThread * PD = new ProgressThread();
    PD->id_event_media = index.data(34).toInt();
    PD->start();
    QDialog * l = new QDialog(this,Qt::Popup);
    l->setWindowTitle("Идет загрузка...");
    connect(PD,SIGNAL(finished()),l,SLOT(close()));
    QHBoxLayout la;
    QLabel lab;
    lab.setText("Подождите, идет загрузка данных с сервера");
    la.addWidget(&lab);
    l->setBackgroundRole(QPalette::Light);
    l->setModal(true);
    l->resize(300,100);
    l->setLayout(&la);
    l->exec();
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

void EventManager::sourceTypeChangeE(int index)
{
    QSqlQuery query;
    this->suorceObjectCBNEE->clear();
    this->suorceObjectCBNEE->addItem("Не выбран объект",0);
    QString table_name = this->suorceTypeObjectCBNEE->itemData(index).toString();
    if(table_name == "not"){
        return;
    }
    QString str = QString("SELECT name_%1, id_%1  FROM %1").arg(table_name);
    if(!query.exec(str)){
        qDebug() << query.lastError().text();
        return;
    }
    while(query.next()){
        this->suorceObjectCBNEE->addItem(query.value(0).toString(),query.value(1).toInt());
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

void EventManager::getTypeChangeE(int index)
{
    QSqlQuery query;
    this->getObjectCBNEE->clear();
    this->getObjectCBNEE->addItem("Не выбран объект",0);
    QString table_name = this->getTypeObjectCBNEE->itemData(index).toString();
    if(table_name == "not"){
        return;
    }
    if(!query.exec(QString("SELECT name_%1, id_%1  FROM %1").arg(table_name))){
        qDebug() << query.lastError().text();
        return;
    }
    while(query.next()){
        this->getObjectCBNEE->addItem(query.value(0).toString(),query.value(1).toInt());
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
    qDebug()<< DTS->dateTime().toString();
    qDebug()<< DTE->dateTime().toString();
    newEvent->setStartDate(DTS->dateTime());
    newEvent->setEndDate(DTE->dateTime());
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
        emit eventDataChanged();
        isEventDialogOpen = false;
    }
}

void EventManager::getTypeObjectCB(QComboBox * suorce,QComboBox * get){
    QSqlQuery query;
    suorce->clear();
    get->clear();
    suorce->addItem("Не выбран тип","not");
    get->addItem("Не выбран тип","not");
    if(!query.exec(QString("SELECT name_type_event_object,\"table_name\" ,id_type_event_object  FROM type_event_object"))){
        qDebug() << query.lastError().text();
        return;
    }
    int index=1;
    while(query.next()){
        int id_type_event_object = query.value(2).toInt();
        suorce->insertItem(index,query.value(0).toString(),query.value(1).toString());
        suorce->setItemData(index,id_type_event_object,Qt::UserRole + 1);
        get->insertItem(index,query.value(0).toString(),query.value(1).toString());
        get->setItemData(index,id_type_event_object,Qt::UserRole + 1);
        index++;
    }
}

void EventManager::getObjectsCB()
{

}

/*!
Слот контекстного меню для списка событий
void EventManager::EventsTableCustomMenu(const QPoint &pe)
*/
void EventManager::EventsTableCustomMenu(const QPoint &pe)
{
    if(eventsModel->rowCount() > 0)
    {
        QPushButton *popupButton = new QPushButton;
        QMenu *menu = new QMenu(this);
        QAction *removeOne = new QAction("Удалить событие",this);
        connect(removeOne,SIGNAL(triggered()),this,SLOT(slotRemoveEvent()));

        menu->addAction(removeOne);
        popupButton->setMenu(menu);
        menu->exec(QCursor::pos());
    }
}


/*!
Слот удаления событияы
void EventManager::slotRemoveEvent()
*/
void EventManager::slotRemoveEvent()
{
    QModelIndex index = tableView->currentIndex();
    if(index.isValid())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Предупреждение");
        msgBox.setText("Вы уверены что хотите удалить данное событие?");
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::Cancel);
        switch (msgBox.exec()) {
        case QMessageBox::Yes:
            Event * event;
            event->DeleteEvent(index.data(Qt::UserRole + 3).toInt());
            restEventsProperty();
            setEventsPropertyEnabled(false);
            current_event = new Event();
            updateModel();
            return;
            break;
        }

    }
}

/*!
Слот контекстного меню для списка медиа
void EventManager::EventsMediaTableCustomMenu(const QPoint &pe)
*/
void EventManager::EventsMediaTableCustomMenu(const QPoint &pe)
{
    if(eventsModel->rowCount() > 0)
    {
        QPushButton *popupButton = new QPushButton;
        QMenu *menu = new QMenu(this);
        QAction *removeOne = new QAction("Удалить",this);
        connect(removeOne,SIGNAL(triggered()),this,SLOT(slotRemoveEventMedia()));

        menu->addAction(removeOne);
        popupButton->setMenu(menu);
        menu->exec(QCursor::pos());
    }
}


/*!
Слот удаления медиа данного события
void EventManager::slotRemoveEventMedia()
*/
void EventManager::slotRemoveEventMedia()
{
    QModelIndex index;
    isDialog? index = tableMediaContent->currentIndex(): index = eventMedia->currentIndex();
    if(index.isValid())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Предупреждение");
        msgBox.setText("Вы уверены что хотите удалить данный контент?");
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::Cancel);
        switch (msgBox.exec()) {
        case QMessageBox::Yes:
            Event * event;
            event->DeleteEventMedia(index.data(34).toInt());
            MediaContentInserted(0);
            return;
            break;
        }

    }
}

void EventManager::statusCBEChanged(int index)
{
    current_event->setStatus(statusCBE->itemData(index).toInt());
}

void EventManager::typeCBEChanged(int index)
{
    current_event->setIdTypeEvent(typeCBE->itemData(index).toInt());
}

void EventManager::descriptionTEEChanged()
{
    current_event->setDescription(descriptionTEE->toPlainText());
}

void EventManager::resumeTEEChanged()
{
    current_event->setResume(resumeTEE->toPlainText());
}

void EventManager::CoordChanged()
{
    Coord * upCoord = new Coord(lagLEE->text().toInt(),lamLEE->text().toInt(),lasLEE->text().toDouble(),logLEE->text().toInt(),lomLEE->text().toInt(),losLEE->text().toDouble());
    current_event->setCoordinate(upCoord);
}

void EventManager::suorceObjectCBNEEChanged(int index)
{
    int id_sr_type = suorceTypeObjectCBNEE->itemData(suorceTypeObjectCBNEE->currentIndex()).toInt();
    EventObject * EO = new EventObject(suorceObjectCBNEE->itemData(index).toInt(),id_sr_type,true);
    current_event->addEventObject(EO);
}

void EventManager::getObjectCBNEEChanged(int index)
{
    int id_sr_type = getTypeObjectCBNEE->itemData(getTypeObjectCBNEE->currentIndex()).toInt();
    EventObject * EO = new EventObject(getObjectCBNEE->itemData(index).toInt(),id_sr_type,true);
    current_event->addEventObject(EO);
}

void EventManager::restEventsProperty(){
    nameLEE->clear();
    statusCBE->setCurrentIndex(0);
    typeCBE->setCurrentIndex(0);
    QDateTime DT;
    DTSE->setDateTime(DT.currentDateTime());
    DTEE->setDateTime(DT.currentDateTime());
    descriptionTEE->clear();
    resumeTEE->clear();
    lagLEE->clear();
    lamLEE->clear();
    lasLEE->clear();
    logLEE->clear();
    lomLEE->clear();
    losLEE->clear();
    this->suorceTypeObjectCBNEE->setCurrentIndex(0);
    suorceObjectCBNEE->clear();
    this->getTypeObjectCBNEE->setCurrentIndex(0);
    getObjectCBNEE->clear();
    eventMedia->setModel(new QStandardItemModel());
}

void EventManager::nameLEEChanged(QString text)
{
    current_event->setName(text);
}

void EventManager::DTSEChanged(QDateTime dateTime)
{
    current_event->setStartDate(dateTime);
}

void EventManager::DTEEChanged(QDateTime dateTime)
{
    current_event->setEndDate(dateTime);
}

void EventManager::setEventsPropertyEnabled(bool enabled){
    nameLEE->setEnabled(enabled);
    statusCBE->setEnabled(enabled);
    typeCBE->setEnabled(enabled);
    DTSE->setEnabled(enabled);
    DTEE->setEnabled(enabled);
    descriptionTEE->setEnabled(enabled);
    resumeTEE->setEnabled(enabled);
    lagLEE->setEnabled(enabled);
    lamLEE->setEnabled(enabled);
    lasLEE->setEnabled(enabled);
    logLEE->setEnabled(enabled);
    lomLEE->setEnabled(enabled);
    losLEE->setEnabled(enabled);
    this->suorceTypeObjectCBNEE->setEnabled(enabled);
    suorceObjectCBNEE->setEnabled(enabled);
    this->getTypeObjectCBNEE->setEnabled(enabled);
    getObjectCBNEE->setEnabled(enabled);
    addEventMedia->setEnabled(enabled);
    eventMedia->setEnabled(enabled);
}
