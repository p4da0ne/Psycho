#include <QLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDialog>
#include <QTextCodec>
#include <QPaintDevice>
#include <QPixmap>
#include <QPicture>
#include <QPaintEvent>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QtSql>
#include <QSqlError>
#include <QMessageBox>
#include <QComboBox>
#include <QMap>
#include <cmath>
#include <QFileInfo>
#include "mapview.h"
#include "view_manage.h"
#include "signdata.h"
#include "calculating_mps.h"
#include "Calculate_K_omkrf.h"
#include "People_Losses.h"
#include "regions_mpos.h"
#include "formationsPsiLooses.h"
#include "formationsMPS.h"
#include "move_model.h"
#include <reports.h>
#include <searchengine.h>
#include "events_adapter.h"


MapView::MapView(QWidget *parent, const char *name)
    : QWidget(parent)
{
	MainCodec = QTextCodec::codecForName("CP1251");
	
	rstModel = new QStandardItemModel;

	setWindowIcon(QIcon(":/Resources/mapwork.png"));

	settings = new QSettings(SETTINGS_ORGANIZATION,SETTINGS_APPLICATION);

	// === mapscroll ================================================================================================================================
	mapwin = new MapScroll();
	
	connect(mapwin,SIGNAL(cursorIsMoved(QPointF)),this,SLOT(showCoordinates(QPointF))); //отображение координат в строке состояния при движении курсора по карте
	connect(mapwin,SIGNAL(selectedPoint(double,double)),this,SLOT(changeObjectCoordInDB(double,double))); //изменение координат в БД после указания точки мышью
	connect(mapwin,SIGNAL(leftButtonClicked(QPoint, QList<QStringList>)),this,SLOT(slotMouseLeftButtonClicked(QPoint, QList<QStringList>)));
	connect(mapwin,SIGNAL(rightButtonClicked(QPoint, QList<QStringList>)),this,SLOT(slotMouseRightButtonClicked(QPoint, QList<QStringList>)));
	// ===================================================================================================================================
	vertLayout = new QVBoxLayout();  //==== основной лэйаут
	vertLayout->setMargin(1);
	
	//==== формирование меню на основе QFrame и QToolButton's ====
	
	initToolButtonsPanel();
	
	//===================================================================
    
	centralLayout = new QHBoxLayout(); //======= лэйаут для размещения левой панели и mapview  

	initSaturnLeftMenu();

	// === mapscroll ========================
    centralLayout->addWidget(mapwin);

	//========================================
	vertLayout->addLayout(centralLayout);

	QHBoxLayout *coord_layout = new QHBoxLayout();
	cursor_coord = new QLineEdit();
	cursor_coord->setReadOnly(true);
	QLineEdit *map_scale = new QLineEdit();
	map_scale->setReadOnly(true);
	scale_info = new QLineEdit();
	scale_info->setReadOnly(true);

	coord_layout->addWidget(cursor_coord);
	coord_layout->addWidget(map_scale);
	coord_layout->addWidget(scale_info);

	vertLayout->addLayout(coord_layout);

//=================================================
	setLayout(vertLayout);
//=========================================
	model = new ViewManage();
//////////////////////////////////////////////////
	openMapFromSettings();
/////////////////////////////////////////////////
}


//=========================================================
//====== Деструктор ========
//========================================================
MapView::~MapView()
{
	mapwin->~MapScroll();
}



//=========================================================
//== Метод создания панели кнопок управления картой =======
//=========================================================
void MapView::initToolButtonsPanel()
{
	QFrame *buttons_menu = new QFrame();
	buttons_menu->setFrameStyle(QFrame::Panel | QFrame::Raised);
	buttons_menu->setLineWidth(2);

//Открыть карту
	QToolButton *open_map_but = new QToolButton();
	open_map_but->setIcon(QIcon(":/Resources/map_open.png"));
	open_map_but->setIconSize(QSize(20,20));
	open_map_but->setToolTip("Открыть карту");
	connect(open_map_but, SIGNAL(clicked()), this, SLOT(openNewMap()));
//Закрыть карту и все данные
	QToolButton *close_map_but = new QToolButton();
	close_map_but->setIcon(QIcon(":/Resources/map_close.png"));
	close_map_but->setIconSize(QSize(20,20));
	close_map_but->setToolTip("Закрыть карту и все данные");
	connect(close_map_but, SIGNAL(clicked()), this, SLOT(closeMap()));
//Увеличить яркость
	QToolButton *set_map_bright1 = new QToolButton();
	set_map_bright1->setIcon(QIcon(":/Resources/up_bright.png"));
	set_map_bright1->setIconSize(QSize(20,20));
	set_map_bright1->setToolTip("Увеличить яркость карты");
	connect(set_map_bright1, SIGNAL(clicked()), this, SLOT(changeBrihgtUp()));
//Уменьшить яркость
	QToolButton *set_map_bright2 = new QToolButton();
	set_map_bright2->setIcon(QIcon(":/Resources//down_bright.png"));
	set_map_bright2->setIconSize(QSize(20,20));
	set_map_bright2->setToolTip("Уменьшить яркость карты");
	connect(set_map_bright2, SIGNAL(clicked()), this, SLOT(changeBrihgtDown()));
//Увеличить контрастность
	QToolButton *set_map_contrast1 = new QToolButton();
	set_map_contrast1->setIcon(QIcon(":/Resources/contrast-up.png"));
	set_map_contrast1->setIconSize(QSize(20,20));
	set_map_contrast1->setToolTip("Увеличить контрастность карты");
	connect(set_map_contrast1, SIGNAL(clicked()), this, SLOT(changeContrastUp()));
//Уменьшить контрастность
	QToolButton *set_map_contrast2 = new QToolButton();
	set_map_contrast2->setIcon(QIcon(":/Resources/contrast-down.png"));
	set_map_contrast2->setIconSize(QSize(20,20));
	set_map_contrast2->setToolTip("Уменьшить контрастность карты");
	connect(set_map_contrast2, SIGNAL(clicked()), this, SLOT(changeContrastDown()));
//печать всей карты
	QToolButton *print_map_but = new QToolButton();
	print_map_but->setIcon(QIcon(":/Resources/print.png"));
	print_map_but->setIconSize(QSize(20,20));
	print_map_but->setToolTip("Печать всей карты");
	connect(print_map_but, SIGNAL(clicked()), this, SLOT(PrintMapSlot()));
//печать видимой области карты
	QToolButton *print_screen_but = new QToolButton();
	print_screen_but->setIcon(QIcon(":/Resources/print_part.png"));
	print_screen_but->setIconSize(QSize(20,20));
	print_screen_but->setToolTip("Печать видимой области карты");
	connect(print_screen_but, SIGNAL(clicked()), this, SLOT(PrintScreenSlot()));
//уменьшить масштаб отображения карты
	QToolButton *less_scale_but = new QToolButton();
	less_scale_but->setIcon(QIcon(":/Resources/less_scale.jpg"));
	less_scale_but->setIconSize(QSize(20,20));
	less_scale_but->setToolTip("Уменьшить масштаб");
	connect(less_scale_but, SIGNAL(clicked()), this, SLOT(lessScale()));
//увеличить масштаб отображения карты
	QToolButton *greate_scale_but = new QToolButton();
	greate_scale_but->setIcon(QIcon(":/Resources/greate_scale.jpg"));
	greate_scale_but->setIconSize(QSize(20,20));
	greate_scale_but->setToolTip("Увеличить масштаб");
	connect(greate_scale_but, SIGNAL(clicked()), this, SLOT(greateScale()));

//РАСТР! ! ! Открыть растр
	QToolButton *open_rsc_but = new QToolButton();
	open_rsc_but->setIcon(QIcon(":/Resources/foto.png"));
	open_rsc_but->setIconSize(QSize(20,20));
	open_rsc_but->setToolTip("Открыть растр");
	connect(open_rsc_but, SIGNAL(clicked()), this, SLOT(openRST()));
//закрыть растр
	QToolButton *close_rsc_but = new QToolButton();
	close_rsc_but->setIcon(QIcon(":/Resources/no_photo1.png"));
	close_rsc_but->setIconSize(QSize(20,20));
	close_rsc_but->setToolTip("Закрыть растр");
	connect(close_rsc_but, SIGNAL(clicked()), this, SLOT(closeRST()));

	QLabel *v_lab = new QLabel();

	v_lab->setFrameStyle(QFrame::VLine | QFrame::Raised);
	v_lab->setLineWidth(2);

	QLabel *v_lab1 = new QLabel();

	v_lab1->setFrameStyle(QFrame::VLine | QFrame::Raised);
	v_lab1->setLineWidth(2);
	
	QLabel *v_lab2 = new QLabel();

	v_lab2->setFrameStyle(QFrame::VLine | QFrame::Raised);
	v_lab2->setLineWidth(2);

	QLabel *v_lab3 = new QLabel();

	v_lab3->setFrameStyle(QFrame::VLine | QFrame::Raised);
	v_lab3->setLineWidth(2);

	QLabel *v_lab4 = new QLabel();

	v_lab4->setFrameStyle(QFrame::VLine | QFrame::Raised);
	v_lab4->setLineWidth(2);

	QLabel *v_lab5 = new QLabel();
	v_lab5->setFrameStyle(QFrame::VLine | QFrame::Raised);
	v_lab5->setLineWidth(2);

	QLabel *v_lab6 = new QLabel();
	v_lab6->setFrameStyle(QFrame::VLine | QFrame::Raised);
	v_lab6->setLineWidth(2);

	QHBoxLayout *menuLayout = new QHBoxLayout();
	menuLayout->setAlignment(Qt::AlignLeft);
	menuLayout->setMargin(2);
	menuLayout->setSpacing(0);

	menuLayout->addWidget(open_map_but);
	menuLayout->addWidget(close_map_but);
	menuLayout->addWidget(v_lab);
	menuLayout->addWidget(set_map_bright1);
	menuLayout->addWidget(set_map_bright2);
	menuLayout->addWidget(v_lab1);
	menuLayout->addWidget(set_map_contrast1);
	menuLayout->addWidget(set_map_contrast2);
	menuLayout->addWidget(v_lab2);
	//menuLayout->addWidget(print_map_but);
	//menuLayout->addWidget(print_screen_but);
	//menuLayout->addWidget(v_lab4);
	//menuLayout->addWidget(print_screen_but);
	//menuLayout->addWidget(v_lab3);
	menuLayout->addWidget(less_scale_but);
	menuLayout->addWidget(greate_scale_but);
	menuLayout->addWidget(v_lab5);
	menuLayout->addWidget(open_rsc_but);
	menuLayout->addWidget(close_rsc_but);

	buttons_menu->setLayout(menuLayout);

	vertLayout->addWidget(buttons_menu);
}


//================================================================================
//============== Метод создания левой панели управления картой ===================
//================================================================================
void MapView::initSaturnLeftMenu()
{
	QFrame *fr = new QFrame();
	fr->setFrameStyle(QFrame::Box | QFrame::Raised);
	fr->setLineWidth(2);
	
	//--------------- Панель "Фильтр объектов" ----------------------------------------
	QWidget *mapWorkWidget = createObjectPanel();
	//----------------------------------------------------------------------------------
	
	//--------------- Панель "Расчетные задачи" ----------------------------------------

	QWidget *calcWidget = createCalculatePanel();
	
	//----------------------------------------------------------------
	
	//--------------- Панель "Фильтр событий" ----------------------------------------

	QWidget *eventWidget = createEventPanel();
	
	//----------------------------------------------------------------

	QToolBox *mapWorkToolBox = new QToolBox;
	mapWorkToolBox->addItem(mapWorkWidget,QIcon(":/Resources/map_search.png"),"Фильтр объектов");
	mapWorkToolBox->addItem(calcWidget,QIcon(":/Resources/edit_1.png"),"Расчетные задачи");
	mapWorkToolBox->addItem(eventWidget,QIcon(":/Resources/event-search.png"),"Фильтр событий");

	QVBoxLayout *left_panel_layout = new QVBoxLayout();
	left_panel_layout->setAlignment(Qt::AlignTop);
	left_panel_layout->setMargin(2);
	left_panel_layout->setContentsMargins(4,5,4,5);

	left_panel_layout->addWidget(mapWorkToolBox);

	fr->setLayout(left_panel_layout);
	fr->setMaximumWidth(215);
	centralLayout->addWidget(fr);
}


//====================================================================
//====== Метод формирует панель фильтра отображения объектов =========
//====================================================================
QWidget* MapView::createObjectPanel()
{
	mpo_regions_checkbox = new QCheckBox("МПОб регионов");
	mpo_regions_checkbox->setChecked(true);
	
	QLabel *means_label = new QLabel("Средства:");
	QFont font("Arial",8);
	font.setUnderline(true);
	means_label->setFont(font);

	smi_means_checkbox = new QCheckBox("СМИ");
	smi_means_checkbox->setChecked(true);

	formation_means_checkbox = new QCheckBox("Формирований");
	formation_means_checkbox->setChecked(true);

	organization_means_checkbox = new QCheckBox("Организаций");
	organization_means_checkbox->setChecked(true);

	//--------------------------------------------------------------
	
	formations_checkbox = new QCheckBox("Формирования");
	formations_checkbox->setChecked(true);

	conditions_checkbox = new QCheckBox("Особые условия");
	conditions_checkbox->setChecked(true);
	//-----------------------------------------------------

	persones_checkbox = new QCheckBox("Персоналии");
	persones_checkbox->setChecked(true);



	QPushButton * show_oper_obst_but = new QPushButton("Показать");
	connect(show_oper_obst_but, SIGNAL(clicked()), this, SLOT(showCheckedObjects()));
	
	
	QVBoxLayout *show_objects_layout = new QVBoxLayout;
	
	show_objects_layout->addWidget(mpo_regions_checkbox);
	QLabel *lineLabel = new QLabel();
	lineLabel->setFrameStyle(QFrame::HLine | QFrame::Raised);
	lineLabel->setLineWidth(2);
	show_objects_layout->addWidget(lineLabel);
	
	show_objects_layout->addWidget(means_label);
	show_objects_layout->addWidget(smi_means_checkbox);
	show_objects_layout->addWidget(formation_means_checkbox);
	show_objects_layout->addWidget(organization_means_checkbox);

	lineLabel = new QLabel();
	lineLabel->setFrameStyle(QFrame::HLine | QFrame::Raised);
	lineLabel->setLineWidth(2);

	show_objects_layout->addWidget(lineLabel);
	show_objects_layout->addWidget(formations_checkbox);
	show_objects_layout->addWidget(conditions_checkbox);
	show_objects_layout->addWidget(persones_checkbox);

	lineLabel = new QLabel();
	lineLabel->setFrameStyle(QFrame::HLine | QFrame::Raised);
	lineLabel->setLineWidth(2);
	show_objects_layout->addWidget(lineLabel);
	show_objects_layout->addWidget(show_oper_obst_but);
	show_objects_layout->addStretch();
	
	QWidget *mapWorkWidget = new QWidget;
	mapWorkWidget->setLayout(show_objects_layout);
	return mapWorkWidget;
}



//====================================================================
//====== Метод формирует панель расчетных задач ======================
//====================================================================
QWidget* MapView::createCalculatePanel()
{
	mps_our_Mil_checkbox = new QCheckBox("МПС своих войск");
	mps_our_Mil_checkbox->setChecked(true);

	mps_enemy_checkbox = new QCheckBox("МПС противника");
	mps_enemy_checkbox->setChecked(true);

	psi_looses_checkbox = new QCheckBox("Психогенные потери");
	psi_looses_checkbox->setChecked(true);

	//-----------------------------------------------------

	QPushButton * calc_button = new QPushButton("Рассчитать");
	connect(calc_button, SIGNAL(clicked()), this, SLOT(showCheckedCalcResults()));
	
	
	QVBoxLayout *calc_layout = new QVBoxLayout;
	
	
	calc_layout->addWidget(mps_our_Mil_checkbox);
	calc_layout->addWidget(mps_enemy_checkbox);
	calc_layout->addWidget(psi_looses_checkbox);

	QLabel *lineLabel = new QLabel();
	lineLabel->setFrameStyle(QFrame::HLine | QFrame::Raised);
	lineLabel->setLineWidth(2);

	calc_layout->addWidget(lineLabel);
	calc_layout->addWidget(calc_button);
	calc_layout->addStretch();
	
	QWidget *calcWidget = new QWidget;
	calcWidget->setLayout(calc_layout);
	return calcWidget;
}


//====================================================================
//====== Метод формирует панель отображения событий ==================
//====================================================================
QWidget* MapView::createEventPanel()
{
	//----------- Панель "Период:" --------------------
	QLabel *periodLabel = new QLabel("Период:");
	QFont font("Arial",8);
	font.setUnderline(true);
	font.setBold(true);
	periodLabel->setFont(font);

	QDate currDate;
	currDate = QDate::currentDate();
	QDate yesterday = currDate.addDays(-1);
	
	beginEventDate = new QDateEdit(yesterday);
	endEventDate = new QDateEdit(currDate);
	
	QCalendarWidget* cw = new QCalendarWidget();
	cw->setFirstDayOfWeek(Qt::Monday);
	beginEventDate->setCalendarWidget(cw);
	beginEventDate->setCalendarPopup(true);

	cw = new QCalendarWidget();
	cw->setFirstDayOfWeek(Qt::Monday);
	endEventDate->setCalendarWidget(cw);
	endEventDate->setCalendarPopup(true);
	
	QLabel *defLabel = new QLabel("-");
	
	QHBoxLayout *periodLay = new QHBoxLayout;
	periodLay->addWidget(beginEventDate);
	periodLay->addWidget(defLabel);
	periodLay->addWidget(endEventDate);

	QVBoxLayout *dateLay = new QVBoxLayout;
	dateLay->addWidget(periodLabel);
	dateLay->addLayout(periodLay);

	//------------------------------------------------------------

	//------- Панель "По объектам:" -------------------
	QLabel *lineLabel = new QLabel();
	lineLabel->setFrameStyle(QFrame::HLine | QFrame::Raised);
	lineLabel->setLineWidth(2);
	
	QLabel *objectsLabel = new QLabel("По объектам:");
	QFont font1("Arial",8);
	font1.setUnderline(true);
	font1.setBold(true);
	objectsLabel->setFont(font1);

	allObjectsButton = new QRadioButton("Все объекты");
	allObjectsButton->toggle();
	selectObjectsButton = new QRadioButton("Выбор объектов");

	connect(selectObjectsButton,SIGNAL(toggled(bool)),this,SLOT(slotSelectButtonToggled(bool)));

	searchObjectLineEdit = new QLineEdit;
	QToolButton *searchObjectButton = new QToolButton;
	searchObjectButton->setIcon(QIcon(":/Resources/search.png"));

	connect(searchObjectLineEdit,SIGNAL(returnPressed()),searchObjectButton,SIGNAL(clicked()));
	connect(searchObjectButton,SIGNAL(clicked()),this,SLOT(slotSearchObject()));

	QHBoxLayout *searchLay = new QHBoxLayout;
	searchLay->addWidget(searchObjectLineEdit); /// Поле ввода строки поиска
	searchLay->addWidget(searchObjectButton);	/// Кнопка поиска объектов

	QLabel *selectedObjLabel = new QLabel("Отобранные объекты:");
	selectedObjectsListView = new QListView;
	selectedObjectsModel = new QStandardItemModel;
	selectedObjectsListView->setModel(selectedObjectsModel);
	selectedObjectsListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	selectedObjectsListView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(selectedObjectsListView, SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(slotSelectedObjectsListViewCustomMenu(const QPoint &))); 

	selectObjectsWidget = new QWidget;
	QVBoxLayout *selLay = new QVBoxLayout;
	selLay->addLayout(searchLay);
	selLay->addWidget(selectedObjLabel);
	selLay->addWidget(selectedObjectsListView);
	selectObjectsWidget->setLayout(selLay);

	QVBoxLayout *objectsLay = new QVBoxLayout;
	objectsLay->addWidget(lineLabel);
	objectsLay->addWidget(objectsLabel);
	objectsLay->addWidget(allObjectsButton);
	objectsLay->addWidget(selectObjectsButton);
	objectsLay->addWidget(selectObjectsWidget);

	selectObjectsWidget->hide();
	
	//-----------------------------------------------------
	QLabel *stateLabel = new QLabel("По состоянию:");
	QFont font2("Arial",8);
	font2.setUnderline(true);
	font2.setBold(true);
	stateLabel->setFont(font2);

	eventStatesModel = new QStandardItemModel;
	
	EventsAdapter *adapter = new EventsAdapter;

	eventStatesModel = adapter->getEventStatusList();

	eventStatesView = new QListView;
	eventStatesView->setModel(eventStatesModel);
	eventStatesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	eventStatesView->setFixedHeight(65);

	QVBoxLayout *statesLay = new QVBoxLayout;
	
	lineLabel = new QLabel();
	lineLabel->setFrameStyle(QFrame::HLine | QFrame::Raised);
	lineLabel->setLineWidth(2);
	statesLay->addWidget(lineLabel);
	statesLay->addWidget(stateLabel);
	statesLay->addWidget(eventStatesView);

	lineLabel = new QLabel();
	lineLabel->setFrameStyle(QFrame::HLine | QFrame::Raised);
	lineLabel->setLineWidth(2);
	statesLay->addWidget(lineLabel);
	//-----------------------------------------------------

	//-----------------------------------------------------
	QLabel *eventTypeLabel = new QLabel("По типу:");
	QFont font3("Arial",8);
	font3.setUnderline(true);
	font3.setBold(true);
	eventTypeLabel->setFont(font3);

	eventTypesModel = new QStandardItemModel;
	
	eventTypesModel = adapter->getEventTypesList();

	eventTypesView = new QListView;
	eventTypesView->setModel(eventTypesModel);
	eventTypesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//eventTypesView->setFixedHeight(65);

	QVBoxLayout *typesLay = new QVBoxLayout;
	
	lineLabel = new QLabel();
	lineLabel->setFrameStyle(QFrame::HLine | QFrame::Raised);
	lineLabel->setLineWidth(2);
	typesLay->addWidget(lineLabel);
	typesLay->addWidget(eventTypeLabel);
	typesLay->addWidget(eventTypesView);

	lineLabel = new QLabel();
	lineLabel->setFrameStyle(QFrame::HLine | QFrame::Raised);
	lineLabel->setLineWidth(2);
	typesLay->addWidget(lineLabel);
	//-----------------------------------------------------

	QPushButton * event_button = new QPushButton("Показать события");
	connect(event_button, SIGNAL(clicked()), this, SLOT(showCheckedEvents()));
	
	typesLay->addWidget(event_button);
	typesLay->addStretch();


	QVBoxLayout *event_layout = new QVBoxLayout;
	event_layout->setMargin(2);
	event_layout->setContentsMargins(5,2,5,2);
	
	event_layout->addLayout(dateLay);
	event_layout->addLayout(objectsLay);
	event_layout->addLayout(statesLay);
	event_layout->addLayout(typesLay);
	
	event_layout->addStretch();

	QWidget *eventWidget = new QWidget;
	eventWidget->setLayout(event_layout);

	return eventWidget;
}

//===============================================================================
//== Слот контекстного меню для списка отобранных объектов в фильтре событий ====
//===============================================================================
void MapView::slotSelectedObjectsListViewCustomMenu(const QPoint &pe)
{
	if(selectedObjectsModel->rowCount() > 0)
	{
		QPushButton *popupButton = new QPushButton;
		QMenu *menu = new QMenu(this);
		QAction *removeOne = new QAction("Удалить объект из списка",this);
		connect(removeOne,SIGNAL(triggered()),this,SLOT(slotRemoveOneObject()));

		QAction *clearAct=new QAction("Очистить список",this);
		connect(clearAct,SIGNAL(triggered()),this,SLOT(slotClearSelectedList()));
		menu->addAction(removeOne);
		menu->addAction(clearAct);
		popupButton->setMenu(menu);
		menu->exec(QCursor::pos());
	}
}


//===============================================================================
//== Слот удаления объекта из списка отобранных объектов в фильтре событий ======
//===============================================================================
void MapView::slotRemoveOneObject()
{
	QModelIndex index = selectedObjectsListView->currentIndex();
	if(index.isValid())
	{
		int row = selectedObjectsModel->itemFromIndex(index)->row();
		selectedObjectsModel->removeRow(row);
	}
}


//===============================================================================
//== Слот очистки списка отобранных объектов в фильтре событий ==================
//===============================================================================
void MapView::slotClearSelectedList()
{
	selectedObjectsModel->clear();
}


//===============================================================================
//== Слот показа/сокрытия панели поиска объектов для отбора в фильтре событий ===
//===============================================================================
void MapView::slotSelectButtonToggled(bool checked)
{
	if(checked)
	{
		selectObjectsWidget->show();
	
	}
	else
	{
		selectObjectsWidget->hide();
	}
}


//==========================================================================
//====== Слот поиска объектов по введенной строке в поле ввода =============
//====== Результаты поиска отображаются в диалоговом окне ==================
//==========================================================================
void MapView::slotSearchObject()
{
	QString searchPattern = searchObjectLineEdit->text();
	SearchEngine *searchEngine = new SearchEngine;
	
	searchResultsModel = searchEngine->findObjects(searchPattern);
	searchResultsModel->sort(0,Qt::AscendingOrder);
	
//////////////////////////////////////////////
	searchResultsDialog = new QDialog(this);
	searchResultsDialog->resize(400,300);
	searchResultsDialog->setAttribute(Qt::WA_DeleteOnClose);
	searchResultsDialog->setWindowTitle("Результаты поиска объектов");

	//-------- построение таблицы-------
	searchResultListView = new QListView;
	searchResultListView->setModel(searchResultsModel);
	searchResultListView->setEditTriggers(QAbstractItemView::NoEditTriggers);


	QHBoxLayout *buttonLay = new QHBoxLayout();

	QPushButton *select_button = new QPushButton("Выбрать");
	connect(select_button, SIGNAL(clicked()), this, SLOT(chooseSelectedObjects()));
	buttonLay->addWidget(select_button);
	buttonLay->addStretch();

	QPushButton *select_all_button = new QPushButton("Выбрать все");
	connect(select_all_button, SIGNAL(clicked()), this, SLOT(chooseAllObjects()));
	buttonLay->addWidget(select_all_button);

	QVBoxLayout *dlgVLay = new QVBoxLayout(searchResultsDialog);
	dlgVLay->addWidget(searchResultListView);
	dlgVLay->addLayout(buttonLay);
					
	if(searchResultsDialog->exec() == QDialog::Rejected)
	{	
	}

/////////////////////////////////////////////
	
	
	
}


//============================================================================================================
//===== Слот отбора отмеченных среди найденных объектов для работы с событиями (с ними связанными) ===========
//============================================================================================================
void MapView::chooseSelectedObjects()
{
	bool alreadySelectedFlag = false;
	for(int row=searchResultsModel->rowCount()-1;row>=0;row--)
	{
		QModelIndex index = searchResultsModel->index(row,0);
	
		QStandardItem *item = new QStandardItem;

		if(searchResultsModel->item(row)->checkState() == Qt::Checked)
		{
			QString text = searchResultsModel->data(index,Qt::DisplayRole).toString();

			QList<QStandardItem*> items = selectedObjectsModel->findItems(text);
			if(items.isEmpty())
			{
				item->setData(searchResultsModel->data(index,Qt::DisplayRole),Qt::DisplayRole);
				item->setData(searchResultsModel->data(index,Qt::ToolTipRole),Qt::ToolTipRole);
				item->setData(searchResultsModel->data(index,Qt::UserRole),Qt::UserRole);
				item->setData(searchResultsModel->data(index,Qt::UserRole+1),Qt::UserRole+1);
			
				selectedObjectsModel->appendRow(item);
				searchResultsModel->removeRows(row,1);
			}
			else
			{
				alreadySelectedFlag = true;
			}
		}
	}
	if((searchResultsModel->rowCount() == 0) || (alreadySelectedFlag))  //если все объекты были отобраны
	{
		searchResultsDialog->reject();
	}

	selectedObjectsModel->sort(0,Qt::AscendingOrder);
}

//================================================================================================
//===== Слот отбора всех найденных объектов для работы с событиями (с ними связанными) ===========
//================================================================================================
void MapView::chooseAllObjects()
{
	bool alreadySelectedFlag = false;
	for(int row=searchResultsModel->rowCount()-1;row>=0;row--)
	{
		QModelIndex index = searchResultsModel->index(row,0);
	
		QStandardItem *item = new QStandardItem;

		QString text = searchResultsModel->data(index,Qt::DisplayRole).toString();

		QList<QStandardItem*> items = selectedObjectsModel->findItems(text);
		if(items.isEmpty())
		{
			item->setData(searchResultsModel->data(index,Qt::DisplayRole),Qt::DisplayRole);
			item->setData(searchResultsModel->data(index,Qt::ToolTipRole),Qt::ToolTipRole);
			item->setData(searchResultsModel->data(index,Qt::UserRole),Qt::UserRole);
			item->setData(searchResultsModel->data(index,Qt::UserRole+1),Qt::UserRole+1);
			
			selectedObjectsModel->appendRow(item);
			searchResultsModel->removeRows(row,1);
		}
		else
		{
			alreadySelectedFlag = true;
		}
	}
	if((searchResultsModel->rowCount() == 0) || (alreadySelectedFlag))  //если все объекты были отобраны
	{
		searchResultsDialog->reject();
	}
	selectedObjectsModel->sort(0,Qt::AscendingOrder);
}



//====================================================================
//======  Отклик на пункт меню открыть новую карту ===================
//====================================================================
void MapView::openNewMap()
{
	openMap();
}
//====================================================================
//==== Слот открытия карты, находящейся по пути в настройках =========
//====================================================================
void MapView::openMapFromSettings()
{	
	QString mapPath = settings->value("/mapSettings/mapPath","").toString();
	openMap(mapPath);
}


//====================================================================
//============= Метод открытия карты =================================
//====================================================================
bool MapView::openMap(QString mapFilepath)
{
	QString filePath = mapFilepath;
	if(filePath == "")
	{
		filePath = QFileDialog::getOpenFileName(this, QString::null, QString::null, 
                   "Maps (*.map)\n Sites (*.sit)\n Matrixes (*.mtw)\n Rasters (*.rsw)" );
		if (filePath.isEmpty()) return false;//если карта не выбрана
	}
	if(mapwin->mapOpen(filePath.toStdString().c_str()))
	{
		//показать середину карты при ее открытии

		mapwin->setMapCenter();

		showViewScale();

		QSettings *settings = new QSettings("vka","saturnMap");
		settings->setValue("/mapSettings/mapPath",filePath);
		return true;
	}
	else
	{
		return false;
	}
}

//====================================================================
//=== Метод закрытия пользовательской карты по имени =================
//====================================================================
void MapView::closeSitByName(QString sitFileName)
{
	mapwin->closeSitByName(mapwin->hMap,sitFileName.toStdString().c_str());
}


//======================================================================
//===== Слот открытия растра ===========================================
//======================================================================
void MapView::openRST()
{
	long int a = mapwin->IsActive(mapwin->hMap);
	if (a)
	{
		QString filePath = QFileDialog::getOpenFileName(this, QString::null, 
						QString::null, "Maps (*.rsw)" );

		if (filePath.isEmpty()) return;//если растр не выбран
		long int a1 = mapwin->openRstOnMap(filePath.toLocal8Bit().data());
		long int a2 = mapwin->setRstOnMap(a1);
		
		QStandardItem *item = new QStandardItem;
		item->setData(filePath,Qt::DisplayRole);
		item->setData(a1,Qt::UserRole);
		item->setCheckable(true);
		item->setCheckState(Qt::Unchecked);
		rstModel->appendRow(item);

		mapwin->updateScreen();
	}		
	else
	{
		showInformationDialog("Не открыта карта местности\n для открытия растра необходимо открыть карту");
	}
}

//=====================================================================
//== Слот закрытия растра(ов) с диалогом выбора растров для закрытия ==
//=====================================================================
void MapView::closeRST()
{
	if (mapwin->IsActive(mapwin->hMap))
	{
		if (rstModel->rowCount() == 1)
		{
			QModelIndex index = rstModel->index(0,0);
			mapwin->closeRstForMap(rstModel->data(index,Qt::UserRole).toInt());
			rstModel->removeRow(0);
		}

		if(rstModel->rowCount() > 1)
		{	
			closeRSTdialog = new QDialog(this);
			closeRSTdialog->setAttribute(Qt::WA_DeleteOnClose);
			closeRSTdialog->setWindowTitle("Выбор растров для закрытия");

			//-------- построение таблицы-------
			rstListView = new QListView;
			rstListView->setModel(rstModel);

			QHBoxLayout *buttonLay = new QHBoxLayout();
			
			QPushButton *close_button = new QPushButton("Закрыть выбранные");
			connect(close_button, SIGNAL(clicked()), this, SLOT(closeSelectedRST()));
			buttonLay->addWidget(close_button);
			buttonLay->addStretch();

			QPushButton *close_all_button = new QPushButton("Закрыть все");
			connect(close_all_button, SIGNAL(clicked()), this, SLOT(closeAllRST()));
			buttonLay->addWidget(close_all_button);

			QVBoxLayout *dlgVLay = new QVBoxLayout(closeRSTdialog);
			dlgVLay->addWidget(rstListView);
			dlgVLay->addLayout(buttonLay);
							
			if(closeRSTdialog->exec() == QDialog::Rejected)
			{	
			}
		}
	}
}

//======================================================================
//======= Слот закрытия выбранных растров ==========
//======================================================================
void MapView::closeSelectedRST()
{
	for (int i=rstModel->rowCount()-1;i>=0; i--)
	{
		QModelIndex index = rstModel->index(i,0);
		if(rstModel->item(i)->checkState() == Qt::Checked)
		{
			mapwin->closeRstForMap(rstModel->data(index,Qt::UserRole).toInt());
			rstModel->removeRow(i);
		}
	}
	if(rstModel->rowCount() == 0)
	{
		closeRSTdialog->close();
	}
}

//======================================================================
//==================== Слот закрытия всех растров ======================
//======================================================================
void MapView::closeAllRST()
{
	for (int i=rstModel->rowCount()-1;i>=0; i--)
	{
		QModelIndex index = rstModel->index(i,0);
		mapwin->closeRstForMap(rstModel->data(index,Qt::UserRole).toInt());
		rstModel->removeRow(i);
		
	}
	if(rstModel->rowCount() == 0)
	{
		closeRSTdialog->close();
	}
}

//===============================================================
// ==== Слот - отклик на нажатие кнопки/пункта меню "Закрыть" ===
//===============================================================
void MapView::closeMap()
{
   mapwin->closeMap();
}


//===============================================================
//===== Слот - отклик на кнопку "<" =============================
//===============================================================
void MapView::lessScale()
{
    mapwin->changeScale(0.5);	
	showViewScale();
}

//===============================================================
//===== Слот - отклик на кнопку ">" =============================
//===============================================================
void MapView::greateScale()
{
    mapwin->changeScale(2.0);
	showViewScale();
}

//===============================================================
//===== Слот - отклик на кнопку "увеличить яркость" =============
//===============================================================
void MapView::changeBrihgtUp()
{
	long int bright = mapwin->getMapBright();
	if (bright<16)
	{
		bright++;
		mapwin->setMapBright(bright);
		mapwin->updateScreen();
	}
	else
	showInformationDialog("Яркость максимальная");
}

//===============================================================
//===== Слот - отклик на кнопку "уменьшить яркость" =============
//===============================================================
void MapView::changeBrihgtDown()
{
	long int bright = mapwin->getMapBright();
	if (bright>-16)
	{
		bright--;
		mapwin->setMapBright(bright);
		mapwin->updateScreen();
	}
	else
	showInformationDialog("Яркость минимальная");
}

//===============================================================
//===== Слот - отклик на кнопку "увеличить контрастность" =======
//===============================================================
void MapView::changeContrastUp()
{
	long int contrast = mapwin->getMapContrast();
	if (contrast<16)
	{
		contrast++;
		mapwin->setMapContrast(contrast);
		mapwin->updateScreen();
	}
	else
	showInformationDialog("Контрастность максимальная");
}

//===============================================================
//===== Слот - отклик на кнопку "уменьшить контрастность" =======
//===============================================================
void MapView::changeContrastDown()
{
	long int contrast = mapwin->getMapContrast();
	if (contrast>-16)
	{
		contrast--;
		mapwin->setMapContrast(contrast);
		mapwin->updateScreen();
	}
	else
	showInformationDialog("Контрастность минимальная");
}

//============================================================================
//===== Метод отображения текущего масштаба в правом нижнем углу карты =======
//============================================================================
//
void MapView::showViewScale()
{
	QString info;
	long int scale = mapwin->getScale();
	info = "Масштаб отображения карты: 1:";
	info +=QString::number(scale);

	scale_info->setText(info);
}

//======================================================================
//======= Метод обработки нажатия клавиш вверх-вниз ====================
//======================================================================
void MapView::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Less || e->key() == Qt::Key_Comma)
  {
    lessScale();
	showViewScale();
	return;
  }
  if (e->key() == Qt::Key_Greater || e->key() == Qt::Key_Period)
  {
    greateScale();
	showViewScale();
	return;
  }
  switch(e->key())
  {
     case Qt::Key_Left:
       mapwin->changePos(-16,0);
       return;
     case Qt::Key_Up:
       mapwin->changePos(0,-16);
       return;
     case Qt::Key_Right:
       mapwin->changePos(16,0);
       return;
     case Qt::Key_Down:
       mapwin->changePos(0,16);
       return;
  }
  QWidget::keyPressEvent(e);
}

//======================================================================
//======= Метод обработки движения мыши по карте =======================
//======= Заносит координаты в строку состояния =======================
//======================================================================
void MapView::showCoordinates(QPointF xyCoord)
{
	QString s = "x: ";
	s += QString::number(xyCoord.x(),'f',0);
	s += "   y: ";
	s += QString::number(xyCoord.y(),'f',0);
	
	Coord *coord = new Coord(xyCoord.x(),xyCoord.y());
	coord = planeToWGS(mapwin->hMap,coord);
	
	s += "          B: ";
	s += coord->latitudeToString();
	s += "     L: ";
	s += coord->longitudeToString();

	cursor_coord->setText(s);

}

//======================================================================================
//========= Метод перевода прямоугольных координат в геодезические (WGS-84) ============
//======================================================================================
Coord* MapView::planeToWGS(long int hMap,Coord *coord)
{
	Coord *tempCoord = coord;
   
	if(hMap == 0) return 0;

	MyMapAccess *map = new MyMapAccess;
	int nD,nM,eD,eM;
	double nS,eS;
	
	if(map->mapIsGeoSupported(hMap))
    {
        GEODEGREE N, E;
		double N_rad, E_rad, H;
		N_rad = tempCoord->getX();
		E_rad = tempCoord->getY();
		map->mapPlaneToGeoWGS843D(hMap,&N_rad,&E_rad,&H);
		map->mapRadianToDegree(&N_rad,&N);
		map->mapRadianToDegree(&E_rad,&E);
	    nD = N.Degree;	 
		nM = N.Minute;
		nS = N.Second;
		eD = E.Degree;
		eM = E.Minute;
		eS = E.Second;
		
		tempCoord->setLatitude(nD,nM,nS);
		tempCoord->setLongitude(eD,eM,eS);
    }
	return tempCoord;
}


//============================================================================
//=== Метод открытия пользовательской карты ==================================
//============================================================================
HSITE MapView::openMapSit(QString sitFileName, QString rscFilePath)
{				
	HSITE this_hsite=0;
	long int a = mapwin->IsActive(mapwin->hMap);
	if(!a) return 0;

	this_hsite = mapwin->openSit(mapwin->hMap,sitFileName.toLocal8Bit().data(),rscFilePath.toLocal8Bit().data());
	mapwin->createsite.Length = sizeof(mapwin->createsite);
	return this_hsite;
}


//======================================================================================
//====== Метод формирует меню увеличения/уменьшения масштаба отображения карты =========
//======================================================================================
QMenu* MapView::createGreateLessScaleMenu()
{
	mouse_menu = new QMenu; 
	QAction *great_scale_act = new QAction("Увеличить масштаб карты  \">\"", this);
	great_scale_act->setIcon(QIcon(":/Resources/greate_scale.jpg"));

	QAction *less_scale_act = new QAction("Уменьшить масштаб карты  \"<\"", this);
	less_scale_act->setIcon(QIcon(":/Resources/less_scale.jpg"));


	mouse_menu->addAction(great_scale_act); 
	connect(great_scale_act, SIGNAL(triggered()), this, SLOT(greateScale()));

	mouse_menu->addAction(less_scale_act); 
	connect(less_scale_act, SIGNAL(triggered()), this, SLOT(lessScale()));
	
	mouse_menu->addSeparator();

	return mouse_menu;
}


//=================================================================================================
//========= Меню по клику правой клавишей мыши в любом месте карты ================================
//=================================================================================================
void MapView::mouseRightSimpleMenu(QPoint pe)
{
	mouse_menu = createGreateLessScaleMenu(); 
	mouse_menu->exec(pe);
}


//============================================================================
//== Метод отображения на карте объектов с помощью условных знаков ===========
//== Первый параметр - номер пользовательской карты (*.sit), второй ==========
//== параметр - список объектов SignData с информацией о знаке ===============
//============================================================================
void MapView::createSitObjects(HSITE hSite, QList<SignData*> signsList)
{
	for(int i=0;i<signsList.count();i++)
	{
		mapwin->createObject(hSite,&signsList.at(i)->getMetricList(),
							 signsList.at(i)->getSignCode().toStdString().c_str(),
							 &signsList.at(i)->getSemanticList());
	}
	mapwin->updateScreen();
}




//////////////////////////////////////////////////////////////////////////////////
////// Методы, специфичные для ПК "Сатурн" /////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////


//================================================================
//== Слот отображения на карте выбранных пользователем объектов ==
//== в соответствии с фильтром отображения объектов ==============
//================================================================
void MapView::showCheckedObjects()
{
	if(mapwin->hMap==0)
	{
		openMapFromSettings();
	}
	//------ Получение координат углов карты ---------
	double x1 = mapwin->getMapX1(mapwin->hMap);
	double y1 = mapwin->getMapY1(mapwin->hMap);
	double x2 = mapwin->getMapX2(mapwin->hMap);
	double y2 = mapwin->getMapY2(mapwin->hMap);
	//-------------------------------------------------

	QString rscPath = settings->value("/mapSettings/rscPath","").toString();
	QFileInfo *info = new QFileInfo(rscPath);
	QString sitPath = info->absolutePath();
	sitPath.append("/");
	//-------------------------------------------------------------------------
	QString mpoRegionsSitName = sitPath + "mpoRegions.sit";
	QString smiMeansSitName = sitPath + "smiMeans.sit";
	QString formationMeansSitName = sitPath + "formationMeans.sit";
	QString organizationMeansSitName = sitPath + "organizationMeans.sit";
	QString formationsSitName = sitPath + "formations.sit";
	QString conditionsSitName = sitPath +"conditions.sit";
	QString personesSitName = sitPath +"persones.sit";
	//-------------------------------------------------------------------------


	if (mpo_regions_checkbox->checkState())
	{
		//показать результаты расчета МПО регионов
		closeSitByName(mpoRegionsSitName);
		HSITE mpoRegionsSite = openMapSit(mpoRegionsSitName,rscPath);
		RegionsMpos * regionsModel = new RegionsMpos;

		QList<SignData*> mpoRegionsSigns = regionsModel->getRegions(mapwin->hMap,x1,y1,x2,y2);
		createSitObjects(mpoRegionsSite, mpoRegionsSigns);
		/////// Временно - для обновления пользовательской карты, чтобы отображались знаки регионов ///////////////
		greateScale();
		lessScale();
		///////////////////////////////////////////////
	}
	else
	{
		closeSitByName(mpoRegionsSitName);
	}

	//-------------------------------------------------------------------------
	if (smi_means_checkbox->checkState())
	{
		//показать средства СМИ
		closeSitByName(smiMeansSitName);
		HSITE smiMeansSite = openMapSit(smiMeansSitName,rscPath);
		QList<SignData*> smiMeansSigns = model->getSmiMeans(mapwin->hMap,x1,y1,x2,y2);
		createSitObjects(smiMeansSite, smiMeansSigns);
	}
	else
	{
		closeSitByName(smiMeansSitName);
	}

	//-------------------------------------------------------------------------
	if (formation_means_checkbox->checkState())
	{
		//показать средства формирований
		closeSitByName(formationMeansSitName);
		HSITE formationMeansSite = openMapSit(formationMeansSitName,rscPath);
		QList<SignData*> formationsMeansSigns = model->getFormationsMeans(mapwin->hMap,x1,y1,x2,y2);
		createSitObjects(formationMeansSite, formationsMeansSigns);
	}
	else
	{
		closeSitByName(formationMeansSitName);
	}

	//-------------------------------------------------------------------------
	if (organization_means_checkbox->checkState())
	{
		//показать средства организаций
		closeSitByName(organizationMeansSitName);
		HSITE organizationMeansSite = openMapSit(organizationMeansSitName,rscPath);
		QList<SignData*> groupsMeansSigns = model->getGroupsMeans(mapwin->hMap,x1,y1,x2,y2);
		createSitObjects(organizationMeansSite, groupsMeansSigns);
	}
	else
	{
		closeSitByName(organizationMeansSitName);
	}

	//-------------------------------------------------------------------------
	if (formations_checkbox->checkState())
	{
		//показать формирования
		closeSitByName(formationsSitName);
		HSITE formationsSite = openMapSit(formationsSitName,rscPath);
		QList<SignData*> formationsSigns = model->getFormations(mapwin->hMap,x1,y1,x2,y2);
		createSitObjects(formationsSite, formationsSigns);
	}
	else
	{
		closeSitByName(formationsSitName);
	}

	//-------------------------------------------------------------------------
	if (conditions_checkbox->checkState())
	{
		//показать особые условия
		closeSitByName(conditionsSitName);
		HSITE conditionsSite = openMapSit(conditionsSitName,rscPath);
		QList<SignData*> specialConditionsSigns = model->getSpecialConditions(mapwin->hMap,x1,y1,x2,y2);
		createSitObjects(conditionsSite, specialConditionsSigns);
	}
	else
	{
		closeSitByName(conditionsSitName);
	}
	//-------------------------------------------------------------------------
	if (persones_checkbox->checkState())
	{
		//показать персоналии
		closeSitByName(personesSitName);
		HSITE personesSite = openMapSit(personesSitName,rscPath);
		QList<SignData*> personesSigns = model->getPersones(mapwin->hMap,x1,y1,x2,y2);
		createSitObjects(personesSite, personesSigns);
	}
	else
	{
		closeSitByName(personesSitName);
	}
	//-------------------------------------------------------------------------

	if (mapwin->hMap) mapwin->updateScreen();
}




///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////  Расчетные задачи  ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

//================================================================
//== Слот отображения на карте результатов расчетных задач, ======
//== отобранных оператором для решения ===========================
//================================================================
void MapView::showCheckedCalcResults()
{
	if(mapwin->hMap==0)
	{
		openMapFromSettings();
	}
	
	//------ Получение координат углов карты ---------
	double x1 = mapwin->getMapX1(mapwin->hMap);
	double y1 = mapwin->getMapY1(mapwin->hMap);
	double x2 = mapwin->getMapX2(mapwin->hMap);
	double y2 = mapwin->getMapY2(mapwin->hMap);
	//-------------------------------------------------

	QString rscPath = settings->value("/mapSettings/rscPath","").toString();
	QFileInfo *info = new QFileInfo(rscPath);
	QString sitPath = info->absolutePath();
	sitPath.append("/");
	//-------------------------------------------------------------------------
	
	QString mpsOursSitName = sitPath + "mpsOurs.sit";
	QString mpsEnemiesSitName = sitPath + "mpsEnemies.sit";
	QString psiLoosesSitName = sitPath + "psiLooses.sit";
	

	//-------------------------------------------------------------------------
	if (mps_our_Mil_checkbox->checkState())
	{
		//показать результаты расчета МПС наших войск
		closeSitByName(mpsOursSitName);
		HSITE mpsOursSite = openMapSit(mpsOursSitName,rscPath);
		FormationsMPS *mpsCalc = new FormationsMPS;
		QList<SignData*> mpsOursSigns = mpsCalc->getFormationsMPS(mapwin->hMap,x1,y1,x2,y2,false);
		createSitObjects(mpsOursSite, mpsOursSigns);
	}
	else
	{
		closeSitByName(mpsOursSitName);
	}

	//-------------------------------------------------------------------------
	if (mps_enemy_checkbox->checkState())
	{
		//показать результаты расчета МПС противника
		closeSitByName(mpsEnemiesSitName);
		HSITE mpsEnemiesSite = openMapSit(mpsEnemiesSitName,rscPath);
		FormationsMPS *mpsCalc = new FormationsMPS;
		QList<SignData*> mpsEnemiesSigns = mpsCalc->getFormationsMPS(mapwin->hMap,x1,y1,x2,y2,true);
		createSitObjects(mpsEnemiesSite, mpsEnemiesSigns);
	
	}
	else
	{
		closeSitByName(mpsEnemiesSitName);
	}

	//-------------------------------------------------------------------------
	if (psi_looses_checkbox->checkState())
	{
		//показать результаты расчета психогенных потерь
		closeSitByName(psiLoosesSitName);
		HSITE psiLoosesSite = openMapSit(psiLoosesSitName,rscPath);
		
		FormationsPsiLooses * psiLooses = new FormationsPsiLooses;

	
		QList<SignData*> psiLoosesSigns = psiLooses->getPsiFormationsLooses(mapwin->hMap,x1,y1,x2,y2);
		createSitObjects(psiLoosesSite, psiLoosesSigns);
	
	}
	else
	{
		closeSitByName(psiLoosesSitName);
	}

	//-------------------------------------------------------------------------

	if (mapwin->hMap) mapwin->updateScreen();
}




//==========================================================================
//== Слот отображения на карте событий в соответствии с фильтром событий ===
//==========================================================================
void MapView::showCheckedEvents()
{

	QDate *startDate = new QDate(beginEventDate->date());
	QDate *endDate = new QDate(endEventDate->date());

	//------ Получение координат углов карты ---------
	double x1 = mapwin->getMapX1(mapwin->hMap);
	double y1 = mapwin->getMapY1(mapwin->hMap);
	double x2 = mapwin->getMapX2(mapwin->hMap);
	double y2 = mapwin->getMapY2(mapwin->hMap);
	//-------------------------------------------------
	
	
	EventsAdapter *adapter = new EventsAdapter(startDate,endDate,selectedObjectsModel,eventStatesModel);
	adapter->getEvents(mapwin->hMap,x1,y1,x2,y2);
	
}

//============================================================================
//==== Слот обработки нажатия левой кнопки мыши ==============================
//============================================================================
void MapView::slotMouseLeftButtonClicked(QPoint pe, QList<QStringList> objectsList)
{
	if(objectsList.isEmpty()) return;
	
	mouse_menu = createObjectsListMenu(objectsList);
	mouse_menu->exec(pe);
}



//============================================================================
//============ Слот обработки нажатия правой кнопки мыши =====================
//============================================================================
void MapView::slotMouseRightButtonClicked(QPoint pe, QList<QStringList> objectsList)
{
	if(objectsList.isEmpty())
	{
		mouseRightSimpleMenu(pe);
	}
	else
	{
		mouse_menu = createObjectsListComplexMenu(objectsList);
		mouse_menu->exec(pe);
	}
}


//======================================================================================
//====== Метод формирует меню в виде списка найденных в точке поиска объектов ==========
//======================================================================================
QMenu* MapView::createObjectsListMenu(QList<QStringList> objectsList)
{
	QMenu *mouse_menu = new QMenu; 
	
	for(int i=0;i<objectsList.count();i++)
	{
		QString text = model->getObjectTypeAndName(objectsList.at(i).at(0).toInt(), objectsList.at(i).at(1).toInt());
		QString idAndType = objectsList.at(i).at(0) + "_" + objectsList.at(i).at(1);
		QAction *obj_act = new QAction(text,this);
		obj_act->setData(idAndType);
	
		mouse_menu->addAction(obj_act); 
		connect(obj_act, SIGNAL(triggered()), this, SLOT(slotObjectInfo()));
	}
	
	return mouse_menu;
}


//=========================================================================================
//== Метод формирует сложное многоуровневое меню в зависимости от типов объектов. =========
//== Для обработки нажатия правой кнопки мыши. ============================================
//=========================================================================================
QMenu* MapView::createObjectsListComplexMenu(QList<QStringList> objectsList)
{
	QMenu *mouse_menu = createGreateLessScaleMenu();//new QMenu; 
	
	for(int i=0;i<objectsList.count();i++)
	{
		int objType = objectsList.at(i).at(1).toInt();
		switch(objType)
		{
			case FORMATIONS:
				mouse_menu->addMenu(createFormationsMenu(objectsList.at(i)));
				break;
			
			case SPECIAL_CONDITIONS:
				mouse_menu->addMenu(createSpecialConditionsMenu(objectsList.at(i)));
				break;
			
			case SMI_MEANS:
				mouse_menu->addMenu(createSmiMeansMenu(objectsList.at(i)));
				break;
			
			case FORMATIONS_MEANS:
				mouse_menu->addMenu(createFormationsMeansMenu(objectsList.at(i)));
				break;
							
			case GROUPS_MEANS:
				mouse_menu->addMenu(createGroupsMeansMenu(objectsList.at(i)));
				break;

			case REGIONS:
				mouse_menu->addMenu(createRegionsMenu(objectsList.at(i)));
				break;

			case PERSONNEL:
				mouse_menu->addMenu(createPersonnelMenu(objectsList.at(i)));
				break;
		}	
	}
	return mouse_menu;
}




//=============================================================================
//==== Слот вывода информации об объекте по нажатию левой кнопки мыши =========
//=============================================================================
void MapView::slotObjectInfo()
{
	QAction *action = qobject_cast<QAction*>(sender());
	QString str;
	if(action)
	{
		QStringList objInfo = action->data().toString().split("_");

		str = model->getObjectInfo(objInfo.at(0).toInt(),objInfo.at(1).toInt());
	
		showInformationDialog(str);

	}

}


//===========================================================================================
//===== Слот расчета психогенных потерь формирования (для конткстного меню) =================
//===========================================================================================
void MapView::slotFormationPsiLooses() 
{
	QAction *action = qobject_cast<QAction*>(sender());
	QString str;
	if(action)
	{
		QStringList objInfo = action->data().toString().split("_");

		FormationsPsiLooses *psiCalc = new FormationsPsiLooses;

		str = psiCalc->getPsiLosses(objInfo.at(0).toInt()); 
			
		showInformationDialog(str);

	}

}

//===========================================================================================
//===== Слот вывода уровня МПОб региона (для контекстного меню) =================
//===========================================================================================
void MapView::slotRegionMpos() 
{
	QAction *action = qobject_cast<QAction*>(sender());
	QString str;
	if(action)
	{
		QStringList objInfo = action->data().toString().split("_");

		RegionsMpos *mpos = new RegionsMpos;

		str = mpos->getRegionMpos(objInfo.at(0).toInt()); 
			
		showInformationDialog(str);

	}

}


//===============================================================================
//====== Слот перемещения объекта на карте с обновлением координат в БД =========
//===============================================================================
void MapView::slotMoveObject()
{
	QAction *action = qobject_cast<QAction*>(sender());
	QString str;
	if(action)
	{
		QStringList objInfo = action->data().toString().split("_");

		MoveModel *model = new MoveModel;

		QList<Coord*> coord = model->getObjectCoordinates(mapwin->hMap,objInfo.at(0).toInt(),objInfo.at(1).toInt());

		dlg = new ChangeCoordDialog(coord.at(0),objInfo.at(0).toInt(),objInfo.at(1).toInt());

		
		
		if(dlg->exec() == QDialog::Accepted)
		{
			mapwin->moveFlag = dlg->mouseFlag;
			if(!dlg->mouseFlag)
			{
				changeObjectCoordInDB(); //изменение координат в БД после ввода их в диалоговом окне
			}
		}


	}

}

//==================================================================================
//=== Слот изменения координат объекта в БД и обновления соответствующего слоя ====
//==================================================================================
void MapView::changeObjectCoordInDB(double x, double y)
{

	int idObject = dlg->idObject;
	int objectType = dlg->objectType;

	MoveModel *moveModel = new MoveModel;
	
	Coord *coord = new Coord(x,y);
	coord = moveModel->planeToWGS(mapwin->hMap,coord);

	moveModel->updateObjectCoordinates(idObject,objectType,coord);
	

	updateSite(objectType);

	mapwin->moveFlag = false;
}

//==================================================================================
//=== Метод изменения координат объекта в БД и обновления соответствующего слоя ====
//==================================================================================
void MapView::changeObjectCoordInDB()
{
	int idObject = dlg->idObject;
	int objectType = dlg->objectType;

	int latD = dlg->latDegreeLineEdit.text().toInt();
	int latM = dlg->latMinutesLineEdit.text().toInt();
	double latS = dlg->latSecondsLineEdit.text().toDouble();
	int longD = dlg->longDegreeLineEdit.text().toInt();
	int longM = dlg->longMinutesLineEdit.text().toInt();
	double longS = dlg->longSecondsLineEdit.text().toDouble();

	Coord *coord = new Coord(latD,latM,latS,longD,longM,longS);

	MoveModel *moveModel = new MoveModel;
	moveModel->updateObjectCoordinates(idObject,objectType,coord);
	

	updateSite(objectType);

}



//=============================================================================================
//======= Метод обновления пользовательской карты для указанного типа объектов ================
//=============================================================================================
void MapView::updateSite(int objectType)
{
	
	//------ Получение координат углов карты ---------
	double x1 = mapwin->getMapX1(mapwin->hMap);
	double y1 = mapwin->getMapY1(mapwin->hMap);
	double x2 = mapwin->getMapX2(mapwin->hMap);
	double y2 = mapwin->getMapY2(mapwin->hMap);
	//-------------------------------------------------
	
	QString rscPath = settings->value("/mapSettings/rscPath","").toString();
	QFileInfo *info = new QFileInfo(rscPath);
	QString sitPath = info->absolutePath();
	sitPath.append("/");
	//-------------------------------------------------------------------------
	QString mpoRegionsSitName = sitPath + "mpoRegions.sit";
	QString smiMeansSitName = sitPath + "smiMeans.sit";
	QString formationMeansSitName = sitPath + "formationMeans.sit";
	QString organizationMeansSitName = sitPath + "organizationMeans.sit";
	QString formationsSitName = sitPath + "formations.sit";
	QString conditionsSitName = sitPath +"conditions.sit";
	QString personesSitName = sitPath +"persones.sit";
	//-------------------------------------------------------------------------

	QList<SignData*> signs;
	HSITE site;

	switch(objectType)
		{
			case FORMATIONS:
				closeSitByName(formationsSitName);
				site = openMapSit(formationsSitName,rscPath);
				signs = model->getFormations(mapwin->hMap,x1,y1,x2,y2);
				break;
			
			case SPECIAL_CONDITIONS:
				closeSitByName(conditionsSitName);
				site = openMapSit(conditionsSitName,rscPath);
				signs = model->getSpecialConditions(mapwin->hMap,x1,y1,x2,y2);
				break;
			
			case SMI_MEANS:
				closeSitByName(smiMeansSitName);
				site = openMapSit(smiMeansSitName,rscPath);
				signs = model->getSmiMeans(mapwin->hMap,x1,y1,x2,y2);
				break;
			
			case FORMATIONS_MEANS:
				closeSitByName(formationMeansSitName);
				site = openMapSit(formationMeansSitName,rscPath);
				signs = model->getFormationsMeans(mapwin->hMap,x1,y1,x2,y2);
				break;
							
			case GROUPS_MEANS:
				closeSitByName(organizationMeansSitName);
				site = openMapSit(organizationMeansSitName,rscPath);
				signs = model->getGroupsMeans(mapwin->hMap,x1,y1,x2,y2);
				break;
			
			case PERSONNEL:
				closeSitByName(personesSitName);
				site = openMapSit(personesSitName,rscPath);
				signs = model->getPersones(mapwin->hMap,x1,y1,x2,y2);
				break;
		}
	createSitObjects(site, signs);

	if (mapwin->hMap) mapwin->updateScreen();
}


//=======================================================================================
//====== Слот формирования отчета об объекте =========
//=======================================================================================
void MapView::slotObjectReport()
{
	QAction *action = qobject_cast<QAction*>(sender());
	QString report;
	Reports *r = new Reports;

	if(action)
	{
		QStringList objInfo = action->data().toString().split("_");
		int idObj = objInfo.at(0).toInt();
		int objType = objInfo.at(1).toInt();

		switch(objType)
		{
			case FORMATIONS:
				report = r->create_object_formular_ls(idObj);
				break;
						
			case SMI_MEANS:
				report = r->create_object_formular_smi(idObj);
				break;
			
			case FORMATIONS_MEANS:
				report = r->create_object_formular_smi(idObj);
				break;
							
			case GROUPS_MEANS:
				report = r->create_object_formular(idObj);
				break;

			case REGIONS:
				
				break;

			case PERSONNEL:
				report = r->create_object_formular_pers(idObj);
				break;
		}	

		r->show_preview_dialog(report);

	}

}


/////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


//===================================================================================
//===== Метод создания контекстного меню для формирования =============
//===================================================================================
QMenu* MapView::createFormationsMenu(QStringList objInfo)
{
	//------------------------------------------------------------------------------
	QString text = model->getObjectTypeAndName(objInfo.at(0).toInt(), objInfo.at(1).toInt());
	
	QString idAndType = objInfo.at(0) + "_" + objInfo.at(1);

	QMenu *mouse_menu = new QMenu(text); 

	QAction *report_act = new QAction("Отчет",this);
	report_act->setData(idAndType);
	mouse_menu->addAction(report_act); 
	connect(report_act, SIGNAL(triggered()), this, SLOT(slotObjectReport()));

	QAction *psiLooses_act = new QAction("Психогенные потери",this);
	psiLooses_act->setData(idAndType);
	mouse_menu->addAction(psiLooses_act); 
	connect(psiLooses_act, SIGNAL(triggered()), this, SLOT(slotFormationPsiLooses()));
	
	QAction *move_act = new QAction("Переместить объект",this);
	move_act->setData(idAndType);
	mouse_menu->addAction(move_act); 
	connect(move_act, SIGNAL(triggered()), this, SLOT(slotMoveObject()));

	return mouse_menu;

	

}


//===================================================================================
//===== Метод создания и отображения контекстного меню для особых условий =============
//===================================================================================
QMenu* MapView::createSpecialConditionsMenu(QStringList objInfo)
{
	QString text = model->getObjectTypeAndName(objInfo.at(0).toInt(), objInfo.at(1).toInt());
	QString idAndType = objInfo.at(0) + "_" + objInfo.at(1);

	QMenu *mouse_menu = new QMenu(text); 

	QAction *report_act = new QAction("Отчет",this);
	report_act->setData(idAndType);
	mouse_menu->addAction(report_act); 
	connect(report_act, SIGNAL(triggered()), this, SLOT(slotObjectReport()));
	
	QAction *move_act = new QAction("Переместить объект",this);
	move_act->setData(idAndType);
	mouse_menu->addAction(move_act); 
	connect(move_act, SIGNAL(triggered()), this, SLOT(slotMoveObject()));
	
	return mouse_menu;

}


//===================================================================================
//===== Метод создания и отображения контекстного меню для средств СМИ ==============
//===================================================================================
QMenu* MapView::createSmiMeansMenu(QStringList objInfo)
{
	QString text = model->getObjectTypeAndName(objInfo.at(0).toInt(), objInfo.at(1).toInt());
	QString idAndType = objInfo.at(0) + "_" + objInfo.at(1);

	QMenu *mouse_menu = new QMenu(text); 

	QAction *report_act = new QAction("Отчет",this);
	report_act->setData(idAndType);
	mouse_menu->addAction(report_act); 
	connect(report_act, SIGNAL(triggered()), this, SLOT(slotObjectReport()));
	
	QAction *move_act = new QAction("Переместить объект",this);
	move_act->setData(idAndType);
	mouse_menu->addAction(move_act); 
	connect(move_act, SIGNAL(triggered()), this, SLOT(slotMoveObject()));

	return mouse_menu;

}


//===================================================================================
//===== Метод создания и отображения контекстного меню для средств формирований =====
//===================================================================================
QMenu* MapView::createFormationsMeansMenu(QStringList objInfo)
{
	QString text = model->getObjectTypeAndName(objInfo.at(0).toInt(), objInfo.at(1).toInt());
	QString idAndType = objInfo.at(0) + "_" + objInfo.at(1);

	QMenu *mouse_menu = new QMenu(text); 

	QAction *report_act = new QAction("Отчет",this);
	report_act->setData(idAndType);
	mouse_menu->addAction(report_act); 
	connect(report_act, SIGNAL(triggered()), this, SLOT(slotObjectReport()));
	
	QAction *move_act = new QAction("Переместить объект",this);
	move_act->setData(idAndType);
	mouse_menu->addAction(move_act); 
	connect(move_act, SIGNAL(triggered()), this, SLOT(slotMoveObject()));

	return mouse_menu;
}


//===================================================================================
//===== Метод создания и отображения контекстного меню для средств организаций ======
//===================================================================================
QMenu* MapView::createGroupsMeansMenu(QStringList objInfo)
{
	QString text = model->getObjectTypeAndName(objInfo.at(0).toInt(), objInfo.at(1).toInt());
	QString idAndType = objInfo.at(0) + "_" + objInfo.at(1);

	QMenu *mouse_menu = new QMenu(text); 

	QAction *report_act = new QAction("Отчет",this);
	report_act->setData(idAndType);
	mouse_menu->addAction(report_act); 
	connect(report_act, SIGNAL(triggered()), this, SLOT(slotObjectReport()));
	
	QAction *move_act = new QAction("Переместить объект",this);
	move_act->setData(idAndType);
	mouse_menu->addAction(move_act); 
	connect(move_act, SIGNAL(triggered()), this, SLOT(slotMoveObject()));

	return mouse_menu;

}


//===================================================================================
//===== Метод создания и отображения контекстного меню для регионов =================
//===================================================================================
QMenu* MapView::createRegionsMenu(QStringList objInfo)
{
	QString text = model->getObjectTypeAndName(objInfo.at(0).toInt(), objInfo.at(1).toInt());
	QString idAndType = objInfo.at(0) + "_" + objInfo.at(1);

	QMenu *mouse_menu = new QMenu(text); 

	//--- Добавление в меню специфичных действий для региона ---------

	QAction *report_act = new QAction("Отчет",this);
	report_act->setData(idAndType);
	mouse_menu->addAction(report_act); 
	connect(report_act, SIGNAL(triggered()), this, SLOT(slotObjectReport()));
	

	QAction *mpos_act = new QAction("МПОб региона",this);
	mpos_act->setData(idAndType);
	mouse_menu->addAction(mpos_act); 
	connect(mpos_act, SIGNAL(triggered()), this, SLOT(slotRegionMpos()));

	return mouse_menu;
}


//===================================================================================
//===== Метод создания и отображения контекстного меню для персоналий =================
//===================================================================================
QMenu* MapView::createPersonnelMenu(QStringList objInfo)
{
	QString text = model->getObjectTypeAndName(objInfo.at(0).toInt(), objInfo.at(1).toInt());
	QString idAndType = objInfo.at(0) + "_" + objInfo.at(1);

	QMenu *mouse_menu = new QMenu(text); 

	//--- Добавление в меню специфичных действий для персоналий ---------

	QAction *report_act = new QAction("Отчет",this);
	report_act->setData(idAndType);
	mouse_menu->addAction(report_act); 
	connect(report_act, SIGNAL(triggered()), this, SLOT(slotObjectReport()));
	
	QAction *move_act = new QAction("Переместить объект",this);
	move_act->setData(idAndType);
	mouse_menu->addAction(move_act); 
	connect(move_act, SIGNAL(triggered()), this, SLOT(slotMoveObject()));

	return mouse_menu;

}

//===============================================================================
//============== Диалоговое окно с информацией об объекте =======================
//===============================================================================
void MapView::showInformationDialog(QString information)
{
	info_dialog = new QDialog;
	info_dialog->setWindowTitle("Информация об объекте");
	info_dialog->resize(400,400);

	QTextBrowser *infoBrowser = new QTextBrowser;
	infoBrowser->setHtml(information);
	QVBoxLayout *vbox_layout = new QVBoxLayout;  
	vbox_layout->addWidget(infoBrowser);
	info_dialog->setLayout(vbox_layout);		
	info_dialog->exec();
}






/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////





//печать всей карты
void	 MapView::PrintMapSlot()
{
	showInformationDialog("\n			ВНИМАНИЕ! \n Перед печатью лучше увеличить яркость и контрастность карты\n");
	
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog *dialog = new QPrintDialog(&printer, this);
	if (dialog->exec() == QDialog::Accepted)
	 {
		QPainter painter(&printer);
		QRect rect = painter.viewport();
		hdc = painter.paintEngine()->getDC();
		RECT r;
		int k=1;
		r.bottom = rect.bottom()*k;
		r.left = rect.left()*k;
		r.right = rect.right()*k;
		r.top = rect.top()*k;
		mapwin->paintInDevice(hdc, &r);
    }
}
//печать видимой области карты
void	 MapView::PrintScreenSlot()
{
	QPrinter printer(QPrinter::HighResolution);
	printer.setOrientation(QPrinter::Landscape);
	QPrintDialog *dialog = new QPrintDialog(&printer, this);
	if (dialog->exec() == QDialog::Accepted)
	 {
		 QPainter painter(&printer);
		hdc = painter.paintEngine()->getDC();
		RECT r;
		int k1=6; int k2 =4;
		r.left = (mapwin->horizontalScrollBar()->value())*k1;//rect.left()*k;
		r.right = (r.left + mapwin->horizontalScrollBar()->pageStep())*k1;//rect.right()*k;
		r.top = (mapwin->verticalScrollBar()->value())*k2;//rect.top()*k;
		r.bottom = (r.top+mapwin->verticalScrollBar()->pageStep())*k2;//rect.bottom()*k;
		mapwin->paintInDevice(hdc, &r);
    }
}

