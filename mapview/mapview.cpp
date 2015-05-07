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


MapView::MapView(QWidget *parent, const char *name)
    : QWidget(parent)
{
	MainCodec = QTextCodec::codecForName("CP1251");
	setMouseTracking(true);  //включает режим отлавливания событий движения мыши без нажатой клавиши
	rst_name_and_number.clear();
	setWindowIcon(QIcon("./icons/mapwork.png"));

	settings = new QSettings(SETTINGS_ORGANIZATION,SETTINGS_APPLICATION);

	// === mapscroll ================================================================================================================================
	mapwin = new MapScroll();
	connect(mapwin,SIGNAL(signal_for_info(long int, long int)),this,SLOT(showShortInformationObject(long int, long int)));//mysignal - движение мыши
	connect(mapwin,SIGNAL(signal_for_right_button(long int, QPoint, int, long int)),this,SLOT(mouseRightMenu(long int, QPoint, int, long int)));
	connect(mapwin,SIGNAL(signalFor1Action(double, double)),this,SLOT(showPositionWGSMouseSlot(double, double)));
	connect(mapwin,SIGNAL(signalFor2Action(double, double)),this,SLOT(showPositionHallMouseSlot(double, double)));
	connect(mapwin,SIGNAL(signalFor3Action(long int)),this,SLOT(showAppointMouseSlot(long int)));
	connect(mapwin,SIGNAL(signalFor4Action(double, double)),this,SLOT(redrawWithNewAngle(double, double)));
	connect(mapwin,SIGNAL(signal_for_change_scale(QPoint)),this,SLOT(mouseRightSimpleMenu(QPoint)));
	
	connect(mapwin,SIGNAL(leftButtonClicked(QPoint, int, int)),this,SLOT(slotMouseLeftButtonClicked(QPoint, int, int)));
	connect(mapwin,SIGNAL(rightButtonClicked(QPoint, int, int)),this,SLOT(slotMouseRightButtonClicked(QPoint, int, int)));
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

   //==============================================
	status_bar = new QLineEdit();
	status_bar->setReadOnly(true);
	vertLayout->addWidget(status_bar);

  //=================================================

	QHBoxLayout *coord_layout = new QHBoxLayout();
	cursor_coord = new QLineEdit();
	cursor_coord->setReadOnly(true);
	QLineEdit *map_scale = new QLineEdit();
	map_scale->setReadOnly(true);
	additional_info = new QLineEdit();
	additional_info->setReadOnly(true);

	coord_layout->addWidget(cursor_coord);
	coord_layout->addWidget(map_scale);
	coord_layout->addWidget(additional_info);

	vertLayout->addLayout(coord_layout);

//=================================================
	setLayout(vertLayout);
//=========================================
	model = new ViewManage();
	koef_mah1=1;
	koef_mah2=1;
	Id_coordinates =0;


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
	open_map_but->setIcon(QIcon("./icons/map_open.png"));
	open_map_but->setIconSize(QSize(20,20));
	open_map_but->setToolTip("Открыть карту");
	connect(open_map_but, SIGNAL(clicked()), this, SLOT(openNewMap()));
//Закрыть карту и все данные
	QToolButton *close_map_but = new QToolButton();
	close_map_but->setIcon(QIcon("./icons/map_close.png"));
	close_map_but->setIconSize(QSize(20,20));
	close_map_but->setToolTip("Закрыть карту и все данные");
	connect(close_map_but, SIGNAL(clicked()), this, SLOT(closeMap()));
//Увеличить яркость
	QToolButton *set_map_bright1 = new QToolButton();
	set_map_bright1->setIcon(QIcon("./icons/up_bright.png"));
	set_map_bright1->setIconSize(QSize(20,20));
	set_map_bright1->setToolTip("Увеличить яркость карты");
	connect(set_map_bright1, SIGNAL(clicked()), this, SLOT(changeBrihgtUp()));
//Уменьшить яркость
	QToolButton *set_map_bright2 = new QToolButton();
	set_map_bright2->setIcon(QIcon("./icons/down_bright.png"));
	set_map_bright2->setIconSize(QSize(20,20));
	set_map_bright2->setToolTip("Уменьшить яркость карты");
	connect(set_map_bright2, SIGNAL(clicked()), this, SLOT(changeBrihgtDown()));
//Увеличить контрастность
	QToolButton *set_map_contrast1 = new QToolButton();
	set_map_contrast1->setIcon(QIcon("./icons/contrast-up.png"));
	set_map_contrast1->setIconSize(QSize(20,20));
	set_map_contrast1->setToolTip("Увеличить контрастность карты");
	connect(set_map_contrast1, SIGNAL(clicked()), this, SLOT(changeContrastUp()));
//Уменьшить контрастность
	QToolButton *set_map_contrast2 = new QToolButton();
	set_map_contrast2->setIcon(QIcon("./icons/contrast-down.png"));
	set_map_contrast2->setIconSize(QSize(20,20));
	set_map_contrast2->setToolTip("Уменьшить контрастность карты");
	connect(set_map_contrast2, SIGNAL(clicked()), this, SLOT(changeContrastDown()));
//печать всей карты
	QToolButton *print_map_but = new QToolButton();
	print_map_but->setIcon(QIcon("./icons/print.png"));
	print_map_but->setIconSize(QSize(20,20));
	print_map_but->setToolTip("Печать всей карты");
	connect(print_map_but, SIGNAL(clicked()), this, SLOT(PrintMapSlot()));
//печать видимой области карты
	QToolButton *print_screen_but = new QToolButton();
	print_screen_but->setIcon(QIcon("./icons/print_part.png"));
	print_screen_but->setIconSize(QSize(20,20));
	print_screen_but->setToolTip("Печать видимой области карты");
	connect(print_screen_but, SIGNAL(clicked()), this, SLOT(PrintScreenSlot()));
//уменьшить масштаб отображения карты
	QToolButton *less_scale_but = new QToolButton();
	less_scale_but->setIcon(QIcon("./icons/less_scale.jpg"));
	less_scale_but->setIconSize(QSize(20,20));
	less_scale_but->setToolTip("Уменьшить масштаб");
	connect(less_scale_but, SIGNAL(clicked()), this, SLOT(lessScale()));
//увеличить масштаб отображения карты
	QToolButton *greate_scale_but = new QToolButton();
	greate_scale_but->setIcon(QIcon("./icons/greate_scale.jpg"));
	greate_scale_but->setIconSize(QSize(20,20));
	greate_scale_but->setToolTip("Увеличить масштаб");
	connect(greate_scale_but, SIGNAL(clicked()), this, SLOT(greateScale()));

//РАСТР! ! ! Открыть растр
	QToolButton *open_rsc_but = new QToolButton();
	open_rsc_but->setIcon(QIcon("./icons/foto.png"));
	open_rsc_but->setIconSize(QSize(20,20));
	open_rsc_but->setToolTip("Открыть растр");
	connect(open_rsc_but, SIGNAL(clicked()), this, SLOT(openRST()));
//закрыть растр
	QToolButton *close_rsc_but = new QToolButton();
	close_rsc_but->setIcon(QIcon("./icons/no_photo1.png"));
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
	menuLayout->addWidget(print_map_but);
	menuLayout->addWidget(print_screen_but);
	menuLayout->addWidget(v_lab4);
	menuLayout->addWidget(print_screen_but);
	menuLayout->addWidget(v_lab3);
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

	QPushButton * show_oper_obst_but = new QPushButton("Показать");
	connect(show_oper_obst_but, SIGNAL(clicked()), this, SLOT(showCheckedObjects()));
	
	
	QVBoxLayout *show_objects_layout = new QVBoxLayout;
	
	show_objects_layout->addWidget(means_label);
	show_objects_layout->addWidget(smi_means_checkbox);
	show_objects_layout->addWidget(formation_means_checkbox);
	show_objects_layout->addWidget(organization_means_checkbox);

	QLabel *lineLabel = new QLabel();
	lineLabel->setFrameStyle(QFrame::HLine | QFrame::Raised);
	lineLabel->setLineWidth(2);

	show_objects_layout->addWidget(lineLabel);
	show_objects_layout->addWidget(formations_checkbox);
	show_objects_layout->addWidget(conditions_checkbox);

	lineLabel = new QLabel();
	lineLabel->setFrameStyle(QFrame::HLine | QFrame::Raised);
	lineLabel->setLineWidth(2);
	show_objects_layout->addWidget(lineLabel);
	show_objects_layout->addWidget(show_oper_obst_but);
	show_objects_layout->addStretch();
	
	QWidget *mapWorkWidget = new QWidget;
	mapWorkWidget->setLayout(show_objects_layout);
	//----------------------------------------------------------------------------------
	
	//--------------- Панель "Расчетные задачи" ----------------------------------------

	mpo_regions_checkbox = new QCheckBox("МПО регионов");
	mpo_regions_checkbox->setChecked(true);

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
	
	calc_layout->addWidget(mpo_regions_checkbox);
	calc_layout->addWidget(mps_our_Mil_checkbox);
	calc_layout->addWidget(mps_enemy_checkbox);
	calc_layout->addWidget(psi_looses_checkbox);

	lineLabel = new QLabel();
	lineLabel->setFrameStyle(QFrame::HLine | QFrame::Raised);
	lineLabel->setLineWidth(2);

	calc_layout->addWidget(lineLabel);
	calc_layout->addWidget(calc_button);
	calc_layout->addStretch();
	
	QWidget *calcWidget = new QWidget;
	calcWidget->setLayout(calc_layout);
	
	//----------------------------------------------------------------
	
	QToolBox *mapWorkToolBox = new QToolBox;
	mapWorkToolBox->addItem(mapWorkWidget,QIcon("./icons/map_search.png"),"Фильтр отображения");
	mapWorkToolBox->addItem(calcWidget,QIcon("./icons/edit_1.png"),"Расчетные задачи");

	QVBoxLayout *left_panel_layout = new QVBoxLayout();
	left_panel_layout->setAlignment(Qt::AlignTop);
	left_panel_layout->setMargin(2);
	left_panel_layout->setContentsMargins(10,5,10,5);

	left_panel_layout->addWidget(mapWorkToolBox);
	left_panel_layout->addStretch();

	fr->setLayout(left_panel_layout);
	fr->setMaximumWidth(200);
	centralLayout->addWidget(fr);
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
	if(mapwin->mapOpen(filePath.toLocal8Bit().data()))
	{
		//показать середину карты при ее открытии
		long int b, sb;
		b=mapwin->verticalScrollBar()->maximum();
		sb=mapwin->verticalScrollBar()->value();
		sb=(b-sb)/2;
		mapwin->horizontalScrollBar()->setValue(0);
		mapwin->verticalScrollBar()->setValue(sb);
		mapwin->updateScreen();
		setAdditionalInfo();

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
		QString File = QFileDialog::getOpenFileName(this, QString::null, QString::null, 
					   "Maps (*.rsw)" );

		if (File.isEmpty()) return;//если карта не выбрана
		long int a1 = mapwin->openRstOnMap(File.toLocal8Bit().data());
		long int a2 = mapwin->setRstOnMap(a1);
		File+="+"+QString::number(a1);
		rst_name_and_number.append(File);
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
		QString name;
		QStringList number_list;
		number_int.clear();
		if (rst_name_and_number.size()==1)
		{
			name = rst_name_and_number.at(0);
			QStringList number_list;
			number_list = name.split("+");
			if (number_list.size()>0) 
				mapwin->closeRstForMap(number_list.at(1).toInt());
			rst_name_and_number.removeAt(0);
		}
		if (rst_name_and_number.size()>1)
		{
			if (mapwin->IsActive(mapwin->hMap))
			{	
				QString name;
				QStringList number_list;
				int rsc_number;
				closeRSTdialog = new QDialog(this);
				closeRSTdialog->setAttribute(Qt::WA_DeleteOnClose);
				closeRSTdialog->setWindowTitle("Выбор растров ");

				//-------- построение таблицы-------
				QHBoxLayout *horizontalLayout = new QHBoxLayout(closeRSTdialog);
				QVBoxLayout *verticalLayout_1 = new QVBoxLayout();

				horizontalLayout->setSpacing(3);
				horizontalLayout->setMargin(11);

				verticalLayout_1->setSpacing(6);
				QLabel    *label = new QLabel(closeRSTdialog);

				verticalLayout_1->addWidget(label);  
				tableWidget = new 	QTableWidget(closeRSTdialog);
				if (tableWidget->columnCount() < 3)
					tableWidget->setColumnCount(3);
				QTableWidgetItem * colItem = new QTableWidgetItem();
				tableWidget->setHorizontalHeaderItem(0, colItem);
				QTableWidgetItem * colItem1 = new QTableWidgetItem();
				tableWidget->setHorizontalHeaderItem(1, colItem1);
				QTableWidgetItem * colItem2 = new QTableWidgetItem();
				tableWidget->setHorizontalHeaderItem(2, colItem2);
			   
				tableWidget->setMinimumSize(QSize(410,0));
				tableWidget->setColumnCount(2);

				verticalLayout_1->addWidget(tableWidget);
				horizontalLayout->addLayout(verticalLayout_1);

				tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
				tableWidget->horizontalHeader()->setResizeMode(0,QHeaderView::Stretch);
				tableWidget->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
				tableWidget->setColumnWidth(0,600);
				tableWidget->setColumnWidth(1,50);

				 tableWidget->horizontalHeaderItem(0)->setText("Название растра");
				 tableWidget->hideColumn(1);
				 //заполняем
				  int row = 0;		
				  
				  for (row=0;row<rst_name_and_number.size(); row++) 
				  {						
				  tableWidget->insertRow(row);
				  tableWidget->setRowHeight(row,30);
				  name = rst_name_and_number.at(row);
				  number_list = name.split("+");
				  rsc_number = number_list.at(1).toInt();

					
					QTableWidgetItem  *item1 = new QTableWidgetItem();
					QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(rsc_number));

					item1->setText(number_list.at(0));

					tableWidget->setItem(row,1,item2);
					item1->setData(Qt::CheckStateRole, Qt::Unchecked);
					tableWidget->setItem(row,0,item1);
				  }

				QHBoxLayout *horizontal_layout_2 = new QHBoxLayout();
				QVBoxLayout *v_button = new QVBoxLayout();
				QPushButton *all_button = new QPushButton("Закрыть");
				connect(all_button, SIGNAL(clicked()), this, SLOT(closeRST1()));
				v_button->addWidget(all_button);
				
				QPushButton *close_all_button = new QPushButton("Закрыть все");
				connect(close_all_button, SIGNAL(clicked()), this, SLOT(closeAllRST()));
				v_button->addWidget(close_all_button);

				horizontal_layout_2->addLayout(v_button);

				horizontalLayout->addLayout(horizontal_layout_2);
						
				if(closeRSTdialog->exec() == QDialog::Rejected)
				{	
				}
			}
		}
	}
}

//======================================================================
//======= Слот закрытия выбранных растров ==========
//======================================================================
void MapView::closeRST1()
{
	for (int i=0; i<rst_name_and_number.size(); i++)
	{
		if (tableWidget->item(i,0)->data(Qt::CheckStateRole).toBool()==true)
		{
			number_int.append(tableWidget->item(i,1)->data(Qt::DisplayRole).toInt());
			rst_name_and_number.removeAt(i);
		}
	}
	for (int i=0; i<number_int.size(); i++)
	{
		mapwin->closeRstForMap(number_int.at(i));
	}
}

//======================================================================
//==================== Слот закрытия всех растров ======================
//======================================================================
void MapView::closeAllRST()
{
	if (mapwin->IsActive(mapwin->hMap))
	{
		QString name;
		QStringList number_list;
		if (rst_name_and_number.size()>1)
		{
			//закрыть все
			for (int i=0; i<rst_name_and_number.count(); i++)
			{
				name = rst_name_and_number.at(i);
				number_list = name.split("+");
				long int a = mapwin->closeRstForMap(number_list.at(1).toInt());//???
			}
			for (int i=0; i<rst_name_and_number.count(); i++)
			{
				rst_name_and_number.removeAt(i);
			}
		}
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
}

//===============================================================
//===== Слот - отклик на кнопку ">" =============================
//===============================================================
void MapView::greateScale()
{
    mapwin->changeScale(2.0);		
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
void MapView::setAdditionalInfo()
{
	QString info;
	long int scale = mapwin->getScale();
	info = "						  Масштаб отображения карты: 1:";
	info +=QString::number(scale);

	additional_info->setText(info);
}

//======================================================================
//======= Метод обработки нажатия клавиш вверх-вниз ====================
//======================================================================
void MapView::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Less || e->key() == Qt::Key_Comma)
  {
    lessScale();
	setAdditionalInfo();
	setStatusInfo("info");
    return;
  }
  if (e->key() == Qt::Key_Greater || e->key() == Qt::Key_Period)
  {
    greateScale();
	setAdditionalInfo();
	setStatusInfo("info");
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
void MapView::mouseMoveEvent(QMouseEvent * event)
{
	if (event->Move)
	{
		QPoint a;
		double x,y;
		if (mapwin->hMap)
		{
			mouse_pos = event->pos();
			x= mouse_pos.x();
			y = mouse_pos.y();
			a=mapwin->getXY(x, y);
		}
		QString s = "x: ";
		s += QString::number(a.x());
		s += " y: ";
		s += QString::number(a.y());
		cursor_coord->setText(s);
	}
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
	mapwin->flag2=TRUE;
	mapwin->createsite.Length = sizeof(mapwin->createsite);
	mapwin->hSite=this_hsite;
	
	return this_hsite;
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
		mapwin->createObjectTest(hSite,&signsList.at(i)->getMetricList(),
								 signsList.at(i)->getSignCode().toStdString().c_str(),
								 &signsList.at(i)->getSemanticList());
	}
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
	/*double x1 = mapwin->getMapX1(mapwin->hMap);
	double y1 = mapwin->getMapY1(mapwin->hMap);
	double x2 = mapwin->getMapX2(mapwin->hMap);
	double y2 = mapwin->getMapY2(mapwin->hMap);*/
	//-------------------------------------------------

	QString rscPath = settings->value("/mapSettings/rscPath","").toString();
	QFileInfo *info = new QFileInfo(rscPath);
	QString sitPath = info->absolutePath();
	sitPath.append("/");
	//-------------------------------------------------------------------------
	QString smiMeansSitName = sitPath + "smiMeans.sit";
	QString formationMeansSitName = sitPath + "formationMeans.sit";
	QString organizationMeansSitName = sitPath + "organizationMeans.sit";
	QString formationsSitName = sitPath + "formations.sit";
	QString conditionsSitName = sitPath +"conditions.sit";
	//-------------------------------------------------------------------------

	if (smi_means_checkbox->checkState())
	{
		//показать средства СМИ
		closeSitByName(smiMeansSitName);
		HSITE smiMeansSite = openMapSit(smiMeansSitName,rscPath);
		QList<SignData*> smiMeansSigns = model->getSmiMeans();
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
		QList<SignData*> formationsMeansSigns = model->getFormationsMeans();
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
		QList<SignData*> groupsMeansSigns = model->getGroupsMeans();
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
		QList<SignData*> formationsSigns = model->getFormations();
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
		QList<SignData*> specialConditionsSigns = model->getSpecialConditions();
		createSitObjects(conditionsSite, specialConditionsSigns);
	}
	else
	{
		closeSitByName(conditionsSitName);
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
	
	QString rscPath = settings->value("/mapSettings/rscPath","").toString();
	QFileInfo *info = new QFileInfo(rscPath);
	QString sitPath = info->absolutePath();
	sitPath.append("/");
	//-------------------------------------------------------------------------
	QString mpoRegionsSitName = sitPath + "mpoRegions.sit";
	QString mpsOursSitName = sitPath + "mpsOurs.sit";
	QString mpsEnemiesSitName = sitPath + "mpsEnemies.sit";
	QString psiLoosesSitName = sitPath + "psiLooses.sit";
	//-------------------------------------------------------------------------

	if (mpo_regions_checkbox->checkState())
	{
		//показать результаты расчета МПО регионов
		closeSitByName(mpoRegionsSitName);
		HSITE mpoRegionsSite = openMapSit(mpoRegionsSitName,rscPath);
		
		//QList<SignData*> mpoRegionsSigns = model->getMpoRegions();		//раскомментировать после реализации функции в модели
		//createSitObjects(mpoRegionsSite, mpoRegionsSigns);
	}
	else
	{
		closeSitByName(mpoRegionsSitName);
	}

	//-------------------------------------------------------------------------
	if (mps_our_Mil_checkbox->checkState())
	{
		//показать результаты расчета МПС наших войск
		closeSitByName(mpsOursSitName);
		HSITE mpsOursSite = openMapSit(mpsOursSitName,rscPath);
		//QList<SignData*> mpsOursSigns = model->getMpsOurs();			//раскомментировать после реализации функции в модели
		//createSitObjects(mpsOursSite, mpsOursSigns);
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
		//QList<SignData*> mpsEnemiesSigns = model->getMpsEnemies();			//раскомментировать после реализации функции в модели
		//createSitObjects(mpsEnemiesSite, mpsEnemiesSigns);
	
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
		//QList<SignData*> psiLoosesSigns = model->getPsiLooses();			//раскомментировать после реализации функции в модели
		//createSitObjects(psiLoosesSite, psiLoosesSigns);
	
	}
	else
	{
		closeSitByName(psiLoosesSitName);
	}

	//-------------------------------------------------------------------------

	if (mapwin->hMap) mapwin->updateScreen();
}









//============================================================================
//==== Слот обработки нажатия левой кнопки мыши ==============================
//============================================================================
void MapView::slotMouseLeftButtonClicked(QPoint pe, int idObject, int objectType)
{
	QString str;
	str = "Идентификатор объекта: " + QString::number(idObject) + "\n"; 
	str = str + "Тип объекта: " + QString::number(objectType); 
	
	errors_message(str);

}



//============================================================================
//==== Слот обработки нажатия правой кнопки мыши ==============================
//============================================================================
void MapView::slotMouseRightButtonClicked(QPoint pe, int idObject, int objectType)
{
	if(!idObject || !objectType)
	{
		mouseRightSimpleMenu(pe);
	}
	else
	{
		switch(objectType)
		{
			case ViewManage::FORMATIONS:
				//
				break;
			
			case ViewManage::SPECIAL_CONDITIONS:
				///
				break;
			
			case ViewManage::SMI_MEANS:
				////
				break;
			
			case ViewManage::FORMATIONS_MEANS:
				///
				break;
			
			case ViewManage::GROUPS_MEANS:
				///
				break;
		}
	
	}
		
		
		

}

















/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////




//void	 MapView::showGroupBox()
//{
//	group_box2->show();
//}
//====== Фильтр отображения результатов целераспределения для средств поражения =======
//void			MapView::showWeaponList()
//{	
//	if (mapwin->hMap)
//	{
//	   weapon_listdialog = new QDialog(this);	
//	   weapon_listdialog->setWindowTitle("Выбор средств для отображения");
//
//		  QList <QString> list;			
//		  QString str_version2;			
//		  list = model->versionForComboboxWeapon();			
//		  int k2 = list.size(), id_w;	
//
//		//-------- построение таблицы-------
//		QHBoxLayout *horizontalLayout = new QHBoxLayout(weapon_listdialog);
//		QVBoxLayout *verticalLayout_3 = new QVBoxLayout();
//
//		horizontalLayout->setSpacing(3);
//		horizontalLayout->setMargin(11);
//
//		verticalLayout_3->setSpacing(6);
//		QLabel    *label = new QLabel(weapon_listdialog);
//
//		verticalLayout_3->addWidget(label);  
//		tableWidget = new 	QTableWidget(weapon_listdialog);
//		if (tableWidget->columnCount() < 3)
//			tableWidget->setColumnCount(3);
//		QTableWidgetItem * colItem = new QTableWidgetItem();
//		tableWidget->setHorizontalHeaderItem(0, colItem);
//		QTableWidgetItem * colItem1 = new QTableWidgetItem();
//		tableWidget->setHorizontalHeaderItem(1, colItem1);
//		QTableWidgetItem * colItem2 = new QTableWidgetItem();
//		tableWidget->setHorizontalHeaderItem(2, colItem2);
//	   
//		tableWidget->setMinimumSize(QSize(410,0));
//		tableWidget->setColumnCount(2);
//
//		verticalLayout_3->addWidget(tableWidget);
//
//		horizontalLayout->addLayout(verticalLayout_3);
//
//		tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
//		tableWidget->horizontalHeader()->setResizeMode(0,QHeaderView::Stretch);
//		tableWidget->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
//		tableWidget->setColumnWidth(0,600);
//		tableWidget->setColumnWidth(1,50);
//
//		 tableWidget->horizontalHeaderItem(0)->setText("Наименование средства поражения");
//
//		 tableWidget->hideColumn(1);
//		 //заполняем
//		  int row = 0;			
//		  
//		  for (row=0;row<k2; row++) 
//		  {						
//		  tableWidget->insertRow(row);
//		  tableWidget->setRowHeight(row,30);
//		  QString w_string = list.at(row);
//		  QStringList kkk = w_string.split("_");
//		  id_w = kkk.at(0).toInt();
//
//			
//			QTableWidgetItem  *item1 = new QTableWidgetItem();
//			QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(id_w));
//
//			item1->setText(kkk.at(1));
//
//			tableWidget->setItem(row,1,item2);
//			item1->setData(Qt::CheckStateRole, Qt::Unchecked);
//			tableWidget->setItem(row,0,item1);
//			//row++;
//		  }
//
//		QHBoxLayout *horizontalLayout_2 = new QHBoxLayout();
//		QVBoxLayout *v_button_lay = new QVBoxLayout();
//		QPushButton *vse_button = new QPushButton("Показать");
//		connect(vse_button, SIGNAL(clicked()), weapon_listdialog, SLOT(accept()));
//		v_button_lay->addWidget(vse_button);
//		
//		QPushButton *all_select_button = new QPushButton("Выбрать все");
//		connect(all_select_button, SIGNAL(clicked()), this, SLOT(select_all_weapons()));
//		v_button_lay->addWidget(all_select_button);
//		/*QHBoxLayout *horizontalLayout_2 = new QHBoxLayout();
//		QPushButton *vse_button = new QPushButton("Показать");
//		connect(vse_button, SIGNAL(clicked()), weapon_listdialog, SLOT(accept()));*/
//
//		horizontalLayout_2->addLayout(v_button_lay);
//
//		horizontalLayout->addLayout(horizontalLayout_2);
//		if(weapon_listdialog->exec()==QDialog::Accepted)
//		{
//			group_weapon_list.clear();
//			if (mapwin->hSite_p) mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);
//			if (mapwin->hSite_s) mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);
////			bool check1 = get_znaki_checkbox(), check2 = get_line_checkbox();
//			if ((check1==false)&&(check2==false))
//			{
//				if (mapwin->hSite_p) mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);
//				if (mapwin->hSite_s) mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);
//				return;
//			}
//			for (int i=0; i<row; i++)
//			{
//				if (tableWidget->item(i,0)->data(Qt::CheckStateRole).toBool()==true)
//				{
//					group_weapon_list.append(tableWidget->item(i,1)->data(Qt::DisplayRole).toInt());
//				}
//			}
//			if (check1)	
//			{	
//				paintPlaner();
//				if (!check2) {if (mapwin->hSite_s) mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);}	
//			}
//			if (check2)	
//			{				
//				joinObjectWithWeapon(); 
//				if (!check1) {if (mapwin->hSite_p) mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);}}
//		}
//	}
//	else
//	{
//		showInformationDialog("Сначала необходимо открыть карту и нанести объекты!");
//		showOpenMapDialog();
//		return;
//	}
//}
//
////======== Выделение всех средств поражения в диалоге-фильтре ======
//void			MapView::select_all_weapons()
//{
//	int row_count = tableWidget->rowCount();
//	
//	for(int row=0;row<row_count;row++)
//	{
//		tableWidget->item(row,0)->setData(Qt::CheckStateRole,true);
//	}
//
//}
//
////выбор версии целераспределения combobox 1
//int				MapView::get_targeting_version()
//{
//	int index = version_combobox->currentIndex();
//	int id_targeting_version = version_combobox->itemData(index,Qt::UserRole).toInt();
//	return id_targeting_version;
//}
////выбор версии operation combobox 3
//int				MapView::get_operation_combo()
//{
//	int index = operation_combobox->currentIndex();
//	int id_operation = operation_combobox->itemData(index,Qt::UserRole).toInt();
//	return id_operation;
//}
//выбор версии direction combobox
//int				MapView::getDirectionCombo()
//{
//	int index = direction_combobox->currentIndex();
//	int id_direction = direction_combobox->itemData(index,Qt::UserRole).toInt();
//	return id_direction;
//}
////запрос выбранного параметра в комбобокс
//inline bool		MapView::get_object_checkbox()
//{
//	return object_checkbox->checkState();	 
//}
////запрос выбранного параметра в комбобокс
//inline bool		MapView::get_weapon_checkbox()
//{
//	return weabon_checkbox->checkState();
//}
////запрос выбранного параметра в комбобокс
//inline bool		MapView::get_lbs_checkbox()
//{
//	return lbs_checkbox->checkState();
//}
//inline bool		MapView::get_corr_checkbox()
//{
//	return corridors_checkbox->checkState();
//}
////запрос выбранного параметра в комбобокс
//inline bool		MapView::get_znaki_checkbox()
//{
//	return znaki_checkbox->checkState();
//}
////запрос выбранного параметра в комбобокс
//inline bool		MapView::get_line_checkbox()
//{
//	return line_checkbox->checkState();
//}
////запрос выбранного параметра в комбобокс
//inline bool		MapView::getMarkDirCheckbox()
//{
//	return mark_checkbox_dir->checkState();
//}
////запрос выбранного параметра в комбобокс
//inline bool		MapView::getLineDirCheckbox()
//{
//	return line_checkbox_dir->checkState();
//}
////при нажатии на "дополнительно"
//void	MapView::showGroupBox1(int id_version)
//{
//	int id_targeting_version = get_targeting_version();
//	if (id_targeting_version==0) 
//	{
//		group_box2->hide();
//		group_box3->hide();
//		return;
//	}
//	group_box2->show();
//	group_box3->show();
//}
////
//void	MapView::hideOperation(int id_operation)
//{
//	if (id_operation!=0)
//	{
//		group_box2->hide();
//		group_box3->show();
//	}
//	else
//		group_box2->show();
//}
////
//void	MapView::hideDirection(int id_direction)
//{
//	if (id_direction!=0)
//	{
//		group_box2->show();
//		group_box3->hide();
//	}
//	else
//		group_box3->show();
//}






//для возможности отображения на карте средств поражения
HSITE	 MapView::openMapSitWeapon()
{				
		HSITE this_hsite=0;
		long int a = mapwin->IsActive(mapwin->hMap);
		if (mapwin->hSite_w==0)
		{
			QString File = "C:/projects/new_sit/Rigel.rsc";
			QString curFile = "C:/projects/new_sit/weapon.sit";
			QString str_mapname = "weapon";
			if (File.isEmpty()) return 0;//если карта не выбрана
			this_hsite = mapwin->openSit(mapwin->hMap,curFile.toLocal8Bit().data(),File.toLocal8Bit().data());
			mapwin->flag2=TRUE;
			mapwin->createsite.Length = sizeof(mapwin->createsite);
			mapwin->hSite_w=this_hsite;
		}
		return this_hsite;
}

//пользовательский слой для стрелок
HSITE	 MapView::openMapSitFor()
{
	HSITE this_hsite=0;
	long int a = mapwin->IsActive(mapwin->hMap);
	if (a)
	{
		if (mapwin->hSite_s==0)
		{
			QString File = "C:/projects/new_sit/Rigel.rsc";
			QString curFile = "C:/projects/new_sit/iSt.sit";
			QString str_mapname = "cel";

			if (File.isEmpty()) return 0;//если карта не выбрана
		
			this_hsite = mapwin->openSit(mapwin->hMap,curFile.toLocal8Bit().data(),File.toLocal8Bit().data());
			mapwin->createsite.Length = sizeof(mapwin->createsite);
			mapwin->hSite_s=this_hsite;
		}
		return this_hsite;
	}
	return 0;
}

//пользовательский слой для целераспределения картинками
HSITE	 MapView::openMapSitForPicture()
{
	HSITE this_hsite=0;
	long int a = mapwin->IsActive(mapwin->hMap);
	if (a)
	{
		if (mapwin->hSite_p==0)
		{
			QString File = "C:/projects/new_sit/Rigel.rsc";
			QString curFile = "C:/projects/new_sit/iPicture.sit";
			QString str_mapname = "Picture";

			if (File.isEmpty()) return 0;//если карта не выбрана
			
			this_hsite = mapwin->openSit(mapwin->hMap,curFile.toLocal8Bit().data(),File.toLocal8Bit().data());
			mapwin->createsite.Length = sizeof(mapwin->createsite);
			mapwin->hSite_p=this_hsite;
		}
		return this_hsite;
	}
	return 0;
}
//слой для "галочек"
HSITE	 MapView::openMapSitForOkWeapon()
{
	HSITE this_hsite=0;
	long int a = mapwin->IsActive(mapwin->hMap);
	if (a)
	{
		if (mapwin->hSite_ok_weapon==0)
		{
			QString File = "C:/projects/new_sit/Rigel.rsc";
			QString curFile = "C:/projects/new_sit/forOkWeapon.sit";
			QString str_mapname = "forOkWeapon";

			if (File.isEmpty()) return 0;//если карта не выбрана
			
			this_hsite = mapwin->openSit(mapwin->hMap,curFile.toLocal8Bit().data(),File.toLocal8Bit().data());
			mapwin->createsite.Length = sizeof(mapwin->createsite);
			mapwin->hSite_ok_weapon=this_hsite;
		}
		return this_hsite;
	}
	return 0;
}
//пользовательский слой для границ разграничения
HSITE	 MapView::openMapSitForLine()
{
	HSITE this_hsite=0;
	long int a = mapwin->IsActive(mapwin->hMap);
	if (a)
	{
		if (mapwin->hSite_line==0)
		{
			QString File = "C:/projects/new_sit/Rigel.rsc";//currentDirectory QDir
			QString curFile = "C:/projects/new_sit/iline.sit";
			QString str_mapname = "line";

			if (File.isEmpty()) return 0;//если карта не выбрана

			this_hsite = mapwin->openSit(mapwin->hMap,curFile.toLocal8Bit().data(),File.toLocal8Bit().data());
			mapwin->createsite.Length = sizeof(mapwin->createsite);
			mapwin->hSite_line=this_hsite;
		}
		return this_hsite;
	}
	return 0;
}
//пользовательский слой для коридоров пролета
HSITE MapView::openMapSitForCorridors()
{
	HSITE this_hsite=0;
	long int a = mapwin->IsActive(mapwin->hMap);
	if (a)
	{
			QString File = "c:/projects/new_sit/Rigel.rsc";//currentDirectory QDir
			QString curFile = "c:/projects/new_sit/icorridors.sit";
			QString str_mapname = "corridors";

			if (File.isEmpty()) return 0;//если карта не выбрана

			this_hsite = mapwin->openSit(mapwin->hMap,curFile.toLocal8Bit().data(),File.toLocal8Bit().data());
			mapwin->createsite.Length = sizeof(mapwin->createsite);
			mapwin->hSite_corr=this_hsite;
		return this_hsite;
	}
	return 0;
}

//отмена целераспределения
//void	 MapView::dissolutionPlan()
//{
//	int id_taget_version = get_targeting_version();
//	model->deleteForDissolutionPlan(Id_obj, id_taget_version);
//	bool p=0, p1=0;
//	if (mapwin->hSite_s)
//	{
//		p=1;
//		mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);
//	}
//	if (mapwin->hSite_p) 
//	{
//		p1=1;
//		mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);
//	}
//	if (p)
//	{ joinObjectWithWeapon();
//	}
//	if (p1)
//	{ paintPlaner();
//	}
//	mapwin->updateScreen();
//}

 //получаем информацию из базы для отображения информации
void	 MapView::showInfoAboutObject()
{
	//Reports *r = new Reports;
	//QString report = r->create_object_formular(Id_obj);
	//r->show_preview_dialog(report);
}

//отобразить все объекты поражения (противник) на карте
void	 MapView::paintAllObject()
{
	//if (mapwin->hSite) mapwin->closeSit(mapwin->hMap, mapwin->hSite);
	//openMapSit("","");		
	//if (mapwin->hSite)
	//{
	//	int id_targeting_version = get_targeting_version();
	//	QSqlQuery query;
	//	QMap< QString,QMap< QString,QString > > objects_inmap;
	//	double x1 = mapwin->getMapX1(mapwin->hMap);//получаем координаты карты
	//	double y1 = mapwin->getMapY1(mapwin->hMap);
	//	double x2 = mapwin->getMapX2(mapwin->hMap);
	//	double y2 = mapwin->getMapY2(mapwin->hMap);
	//	objects_inmap = model->get_all_objects_info(id_targeting_version, x1, y1, x2, y2);

	//	QString  code, code1;
	//	long int id_object, x, y;
	//	long int a2=0, a1=0;
	//	int len=0;
	//	QMap <QString, QMap <QString,QString>>::iterator it = objects_inmap.begin(); 
	//		for (;it != objects_inmap.end(); ++it) 
	//		{	
	//			code = it.key();
	//			id_object = code.toLong();
	//			code = it.value()["x"];
	//			x = code.toDouble();
	//			code = it.value()["y"];
	//			y = code.toDouble();
	//			code = it.value()["key"];
	//			len=code.length();
	//			code1=code;
	//			if (code[len-1]==' ')//убираем пробел - ошибку при вводе
	//			{
	//				for (int i=0; i<len; i++)
	//				{
	//					if (code[i]!=' ')
	//					{
	//						code1[i]=code[i];
	//					}
	//				}
	//			}
	//			QString sem = it.value()["sem"];;
	//			bool if_shtab = ifShtab(code1);
	//			a2 = mapwin->createObject(mapwin->hSite, x, y , code1.toLocal8Bit().data(),id_object, 2.00,if_shtab,sem);	//
	//		}	
	//}
	//else
	//{
	//	
	//}
}

//отобразить средства поражения
void	 MapView::paintWeapon()
{
	//openMapSitWeapon();
	//if (mapwin->hSite_w)
	//{
	//	QList <QString> objects_inmap;
	//	double x1 = mapwin->getMapX1(mapwin->hMap);
	//	double y1 = mapwin->getMapY1(mapwin->hMap);
	//	double x2 = mapwin->getMapX2(mapwin->hMap);
	//	double y2 = mapwin->getMapY2(mapwin->hMap);
	//	objects_inmap = model->get_all_objects_weapon(x1,y1,x2,y2);
	//	QString  str_id_weapon, str_id_weapon_coordinates, sign_key, takt_number, str_x, str_y;
	//	long int id_object, id_weapon_coordinates, id_weapon;
	//	double x, y;
	//	long int a2=0, count1, count2;
	//	int i=0;
	//	float angle=0;
	//	while (i<objects_inmap.size())
	//	{	
	//			str_id_weapon = objects_inmap.at(i);
	//			id_weapon = str_id_weapon.toLong();i++;
	//			str_id_weapon_coordinates = objects_inmap.at(i);
	//			id_weapon_coordinates = str_id_weapon_coordinates.toLong();
	//			i++;
	//			sign_key = objects_inmap.at(i);	i++;
	//			takt_number = objects_inmap.at(i);	i++;
	//			str_x = objects_inmap.at(i); i++;
	//			str_y = objects_inmap.at(i); i++;
	//			x = str_x.toDouble();
	//			y = str_y.toDouble();
	//			str_y = objects_inmap.at(i); i++;
	//			angle = str_y.toDouble();
	//			a2 = mapwin->createObject(mapwin->hSite_w, x, y , sign_key.toLocal8Bit().data(), id_weapon, 1.00, 0, takt_number, id_weapon_coordinates, angle);
	//	}
	//}
}


//граница !!
void	 MapView::paintLine()
{
	/*openMapSitForLine();
	mapwin->war_line_coord = model->getWarLine();
	if (mapwin->hSite_line)
	{
		long int  id_obj = 1;
		QString name_ff = "L0000500099";
		long int a1 = mapwin->createLine(mapwin->hSite_line, name_ff.toLocal8Bit().data(), id_obj);
	}*/
}
//------ Отображение коридоров пролета ---------
void MapView::paintCorridors()
{
//	this->openMapSitForCorridors();
//	if (mapwin->hSite_corr)
//	{
//		long int  id_obj = 5, id_coord;
//		QString name_ff = "V0001304060";		
////===========================
//		QList <QString> corridors_coord;		
//		corridors_coord = model->get_corridors_coordinates();
//
//		double x01, y01, x02, y02;
//		
//		int i=0;
//		while (i<corridors_coord.size())//2 линии
//		{	
//			QStringList coord_list = corridors_coord.at(i).split("_");
//			
//			x01 = coord_list.at(0).toDouble();
//			y01 = coord_list.at(1).toDouble();
//			x02 = coord_list.at(2).toDouble();
//			y02 = coord_list.at(3).toDouble();			
//			id_coord = coord_list.at(4).toInt();
//	    	long int a1 = mapwin->createV0(mapwin->hSite_corr,x01,y01,x02,y02,name_ff.toLocal8Bit().data(),id_obj, 3,id_coord);
//			i++;	
//		}	
	//}
//=============================
}
//значками!
void	 MapView::paintPlaner()
{
	//if (mapwin->hSite)
	//{
	//	openMapSitForPicture();
	//	if (mapwin->hSite_p)
	//	{
	//		QList <QStringList> result;
	//		QStringList srt_list;
	//		QString str_x, str_y, key, semantic, takt_number, id_object;
	//		int id_weapon;
	//		long int  X1_M=mapwin->getMapX1(mapwin->hMap);
	//		long int  X2_M=mapwin->getMapX2(mapwin->hMap);
	//		long int  Y1_M=mapwin->getMapY1(mapwin->hMap);
	//		long int  Y2_M=mapwin->getMapY2(mapwin->hMap);
	//		long int x,y;
	//		//нужно выбрать в зависимости от combobox
	//		int id_targeting_version = get_targeting_version();
	//		int id_operation = get_operation_combo();
	//		int size = group_weapon_list.size();
	//		if (size==0)
	//		{
	//			return;
	//		}
	//		for (int j=0; j<size; j++)
	//		{
	//			id_weapon = group_weapon_list.at(j);
	//			result = model->get_destroy_signs(id_targeting_version, id_weapon, id_operation);//vvv();
	//				for (int i=0; i<result.size(); i++)
	//				{
	//					 srt_list = result.at(i);
	//					 str_x = srt_list.at(0);
	//					 str_y = srt_list.at(1);
	//					 key = srt_list.at(2);
	//					 semantic = srt_list.at(5); //takt_number
	//					 semantic.append("_");						
	//					 semantic.append(srt_list.at(4)); //id_object
	//					 semantic.append("_");						
	//					 semantic.append(srt_list.at(6)); //transfer_name
	//					// takt_number = srt_list.at(4);
	//					 //id_object = srt_list.at(5);
	//					 x = str_x.toDouble();
	//					 y = str_y.toDouble();
	//				 if ((x>X1_M)&&(x<X2_M)&&(y<Y2_M)&&(y>Y1_M))
	//				 {
	//					 mapwin->createObject(mapwin->hSite_p,x,y,key.toLocal8Bit().data(),0,0,0,semantic);
	//				 }
	//				}
	//		}
	//	}
	//}
	//else
	//{
	//		showInformationDialog("Сначала нанесите объекты!");
	//		return;
	//}
}

//значками по направлениям
void	 MapView::paintPlanerDirection()
{
	//if (mapwin->hSite)
	//{
	//	openMapSitForPicture();
	//	if (mapwin->hSite_p)
	//	{
	//		QList <QStringList> result;
	//		QStringList srt_list;
	//		QString str_x, str_y, key, semantic, takt_number, id_object;
	//		int id_weapon;
	//		long int  X1_M=mapwin->getMapX1(mapwin->hMap);
	//		long int  X2_M=mapwin->getMapX2(mapwin->hMap);
	//		long int  Y1_M=mapwin->getMapY1(mapwin->hMap);
	//		long int  Y2_M=mapwin->getMapY2(mapwin->hMap);
	//		long int x,y;
	//		//нужно выбрать в зависимости от combobox
	//		int id_targeting_version = get_targeting_version();
	//		int id_operation = get_operation_combo();
	//		int id_direction = getDirectionCombo();
	//		result = model->get_destroy_signs_direction(id_targeting_version, id_operation, id_direction);//vvv();
	//		for (int i=0; i<result.size(); i++)
	//		{
	//			 srt_list = result.at(i);
	//			 str_x = srt_list.at(0);
	//			 str_y = srt_list.at(1);
	//			 key = srt_list.at(2);
	//			 semantic = srt_list.at(5); //takt_number
	//			 semantic.append("_");						
	//			 semantic.append(srt_list.at(4)); //id_object
	//			 semantic.append("_");						
	//			 semantic.append(srt_list.at(6)); //transfer_name
	//			 x = str_x.toDouble();
	//			 y = str_y.toDouble();
	//		 if ((x>X1_M)&&(x<X2_M)&&(y<Y2_M)&&(y>Y1_M))
	//		 {
	//			 mapwin->createObject(mapwin->hSite_p,x,y,key.toLocal8Bit().data(),0,0,0,semantic);
	//		 }
	//		}
	//	}
	//}
	//else
	//{
	//		showInformationDialog("Сначала нанесите объекты!");
	//		return;
	//}
}
//показать целераспределение линией от средства к объекту
void	 MapView::joinObjectWithWeapon()
{
	//if (mapwin->hSite)
	//{
	//	openMapSitFor();
	//	if (mapwin->hSite_s)
	//	{			
	//		long int  X1_M=mapwin->getMapX1(mapwin->hMap);//габариты карты
	//		long int  X2_M=mapwin->getMapX2(mapwin->hMap);//габариты карты
	//		long int  Y1_M=mapwin->getMapY1(mapwin->hMap);//габариты карты
	//		long int  Y2_M=mapwin->getMapY2(mapwin->hMap);//габариты карты
	//		//нужно выбрать в зависимости от combobox
	//		int id_targeting_version = get_targeting_version();
	//		int id_operation = get_operation_combo();
	//		QList<QList<Coord*>> result;
	//		QStringList lines_key;
	//		QString str_x, str_y, key, semantic;
	//		long int x1, y1, x2, y2;
	//		long int  id_obj = 505;
	//		lines_key.append("L0000200058");
	//		lines_key.append("L0000200060");
	//		lines_key.append("L0000200061");
	//		lines_key.append("L0000200062");
	//		lines_key.append("L0000200063");
	//		lines_key.append("L0000200059");
	//		int id_weapon, size = group_weapon_list.size();
	//		if (size==0)
	//		{
	//			return;
	//		}
	//		if (id_operation==0) 
	//		{
	//			for (int j=1; j<7; j++)
	//			{
	//				for (int sc=0; sc<size; sc++)
	//				{
	//					id_weapon = group_weapon_list.at(sc);
	//					result = model->getLinesCoordDirect(id_targeting_version, id_weapon, j);	
	//					for(int i =0;i<result.count();i++)
	//					{//центр формируется в view_manage
	//						 QList<Coord*> temp;
	//						 temp=result.at(i);
	//						 Coord *t_point1= new Coord;
	//						 t_point1 = temp.at(0);
	//						 Coord *t_point2= new Coord;
	//						 Coord *t_point3= new Coord;
	//						 Coord *t_point4= new Coord;
	//						 //t_point2 = temp.at(0);
	//						 str_x = t_point1->get_X();
	//						 str_y = t_point1->get_Y();
	//						 x1 = str_x.toDouble();
	//						 y1 = str_y.toDouble();
	//						 if ((x1>X1_M)&&(x1<X2_M)&&(y1<Y2_M)&&(y1>Y1_M))
	//						 {
	//							 long int a1;
	//							 t_point2 = temp.at(1);
	//							 str_x = t_point2->get_X();
	//							 str_y = t_point2->get_Y();
	//							 x2 = str_x.toDouble();
	//							 y2 = str_y.toDouble();	
	//							if ((x2>X1_M)&&(x2<X2_M)&&(y2<Y2_M)&&(y2>Y1_M))
	//							a1 = mapwin->createV0(mapwin->hSite_s, x2, y2, x1, y1, lines_key.at(j-1).toLocal8Bit().data(), id_obj);
	//							if (temp.size()>2)
	//							{
	//								t_point3 = temp.at(2);
	//								 str_x = t_point3->get_X();
	//								 str_y = t_point3->get_Y();
	//								 x1 = str_x.toDouble();
	//								 y1 = str_y.toDouble();	
	//								if ((x1>X1_M)&&(x1<X2_M)&&(y1<Y2_M)&&(y1>Y1_M))
	//								a1 = mapwin->createV0(mapwin->hSite_s, x2, y2, x1, y1, lines_key.at(j-1).toLocal8Bit().data(), id_obj);
	//								t_point4 = temp.at(3);
	//								 str_x = t_point4->get_X();
	//								 str_y = t_point4->get_Y();
	//								 x2 = str_x.toDouble();
	//								 y2 = str_y.toDouble();	
	//								if ((x2>X1_M)&&(x2<X2_M)&&(y2<Y2_M)&&(y2>Y1_M))
	//								a1 = mapwin->createV0(mapwin->hSite_s, x2, y2, x1, y1, lines_key.at(j-1).toLocal8Bit().data(), id_obj);
	//							}
	//						}
	//					}
	//				}
	//			}
	//			return;
	//		}
	//		for (int sc=0; sc<size; sc++)
	//		{
	//			id_weapon = group_weapon_list.at(sc);
	//			result = model->getLinesCoordDirect(id_targeting_version, id_weapon, id_operation);	//vvv()			

	//			id_obj = 5;
	//			QString key1 = "L0000200058";
	//			for(int i =0;i<result.count();i++)
	//			{
	//				 QList <Coord*> temp;
	//				 temp=result.at(i);
	//				 Coord *t_point1= new Coord;
	//				 Coord *t_point2= new Coord;
	//				 Coord *t_point3= new Coord;
	//				 Coord *t_point4= new Coord;
	//				 t_point1 = temp.at(0);
	//				 t_point2 = temp.at(1);
	//				 str_x = t_point1->get_X();
	//				 str_y = t_point1->get_Y();
	//				 x1 = str_x.toDouble();
	//				 y1 = str_y.toDouble();
	//				if ((x1>X1_M)&&(x1<X2_M)&&(y1<Y2_M)&&(y1>Y1_M))
	//				 {
	//				 str_x = t_point2->get_X();
	//				 str_y = t_point2->get_Y();
	//				 x2 = str_x.toDouble();
	//				 y2 = str_y.toDouble();	
	//				long int a1;
	//				if ((x2>X1_M)&&(x2<X2_M)&&(y2<Y2_M)&&(y2>Y1_M))
	//				a1 = mapwin->createV0(mapwin->hSite_s, x2, y2, x1, y1, key1.toLocal8Bit().data(), id_obj);		
	//					if (temp.size()>2)
	//					{
	//						t_point3 = temp.at(2);
	//						 str_x = t_point3->get_X();
	//						 str_y = t_point3->get_Y();
	//						 x1 = str_x.toDouble();
	//						 y1 = str_y.toDouble();	
	//						if ((x1>X1_M)&&(x1<X2_M)&&(y1<Y2_M)&&(y1>Y1_M))
	//						a1 = mapwin->createV0(mapwin->hSite_s, x2, y2, x1, y1, key1.toLocal8Bit().data(), id_obj);
	//						t_point4 = temp.at(3);
	//						 str_x = t_point4->get_X();
	//						 str_y = t_point4->get_Y();
	//						 x2 = str_x.toDouble();
	//						 y2 = str_y.toDouble();	
	//						if ((x2>X1_M)&&(x2<X2_M)&&(y2<Y2_M)&&(y2>Y1_M))
	//						a1 = mapwin->createV0(mapwin->hSite_s, x2, y2, x1, y1, key1.toLocal8Bit().data(), id_obj);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//		showInformationDialog("Сначала нанесите объекты!");
	//}
}
//
void	 MapView::joinObjectWithWeaponDirection()
{
	//if (mapwin->hSite)
	//{
	//	openMapSitFor();
	//	if (mapwin->hSite_s)
	//	{			
	//		long int  X1_M=mapwin->getMapX1(mapwin->hMap);//габариты карты
	//		long int  X2_M=mapwin->getMapX2(mapwin->hMap);//габариты карты
	//		long int  Y1_M=mapwin->getMapY1(mapwin->hMap);//габариты карты
	//		long int  Y2_M=mapwin->getMapY2(mapwin->hMap);//габариты карты
	//		//нужно выбрать в зависимости от combobox
	//		int id_targeting_version = get_targeting_version();
	//		int id_operation = get_operation_combo();
	//		int id_direction = getDirectionCombo();
	//		QList<QList<Coord*>> result;
	//		QStringList lines_key;
	//		QString str_x, str_y, key, semantic;
	//		long int x1, y1, x2, y2;
	//		long int  id_obj = 505;
	//		lines_key.append("L0000200058");
	//		lines_key.append("L0000200060");
	//		lines_key.append("L0000200061");
	//		lines_key.append("L0000200062");
	//		lines_key.append("L0000200063");
	//		lines_key.append("L0000200059");
	//			result = model->getLinesCoordDirect(id_targeting_version, 0, id_operation, id_direction);	//vvv()
	//			id_obj = 5;
	//			QString key1 = "L0000200058";
	//			for(int i =0;i<result.count();i++)
	//			{
	//				 QList <Coord*> temp;
	//				 temp=result.at(i);
	//				 Coord *t_point1= new Coord;
	//				 Coord *t_point2= new Coord;
	//				 Coord *t_point3= new Coord;
	//				 Coord *t_point4= new Coord;
	//				 t_point1 = temp.at(0);
	//				 t_point2 = temp.at(1);
	//				 str_x = t_point1->get_X();
	//				 str_y = t_point1->get_Y();
	//				 x1 = str_x.toDouble();
	//				 y1 = str_y.toDouble();
	//				if ((x1>X1_M)&&(x1<X2_M)&&(y1<Y2_M)&&(y1>Y1_M))
	//				 {
	//				 str_x = t_point2->get_X();
	//				 str_y = t_point2->get_Y();
	//				 x2 = str_x.toDouble();
	//				 y2 = str_y.toDouble();	
	//				long int a1;
	//				if ((x2>X1_M)&&(x2<X2_M)&&(y2<Y2_M)&&(y2>Y1_M))
	//				a1 = mapwin->createV0(mapwin->hSite_s, x2, y2, x1, y1, key1.toLocal8Bit().data(), id_obj);		
	//					if (temp.size()>2)
	//					{
	//						t_point3 = temp.at(2);
	//						 str_x = t_point3->get_X();
	//						 str_y = t_point3->get_Y();
	//						 x1 = str_x.toDouble();
	//						 y1 = str_y.toDouble();	
	//						if ((x1>X1_M)&&(x1<X2_M)&&(y1<Y2_M)&&(y1>Y1_M))
	//						a1 = mapwin->createV0(mapwin->hSite_s, x2, y2, x1, y1, key1.toLocal8Bit().data(), id_obj);
	//						t_point4 = temp.at(3);
	//						 str_x = t_point4->get_X();
	//						 str_y = t_point4->get_Y();
	//						 x2 = str_x.toDouble();
	//						 y2 = str_y.toDouble();	
	//						if ((x2>X1_M)&&(x2<X2_M)&&(y2<Y2_M)&&(y2>Y1_M))
	//						a1 = mapwin->createV0(mapwin->hSite_s, x2, y2, x1, y1, key1.toLocal8Bit().data(), id_obj);
	//					}
	//				}
	//			}
	//		}
	//}
	//else
	//{
	//		showInformationDialog("Сначала нанесите объекты!");
	//}
}
//проверяем, если это флаг, штаб, министерство обороны - чтобы менять направление флажка
bool	 MapView::ifShtab(QString code)
{
	if ((code=="V0000000335")||(code=="L0100000546")||(code=="V0000001004")
		||(code=="V3154100001")||(code=="V0000000076")||(code=="V0000000075")
		||(code=="V0000000077")||(code=="V20000072")||(code=="V0000000072")
		||(code=="V0000000076")||(code=="V0000000075")||(code=="V0000000077")
		||(code=="V20000077")||(code=="V3154100001")||(code=="L0100007755")
		||(code=="V20000077")||(code=="V0000001004")||(code=="V20000074")
		||(code=="V20000074")||(code=="V20000075")||(code=="V20000077")
		||(code=="L0100007755")||(code=="V0000001008")||(code=="V20000072")
		||(code=="V20000072")||(code=="V20000074")||(code=="V20000075"))
	{
		return TRUE;
	}
	return FALSE;
}
void	 MapView::errors_message(QString str)
{
	QMessageBox error;
	QMessageBox::warning(this, tr("Warning!!!"),str,
		QMessageBox::Ok);
}


//меню по клику правой клавишей мыши
void	 MapView::mouseRightMenu(long int id_obj, QPoint pe, int semantic_flag, long int id_coordinates)
{
		Id_obj=id_obj;//
		Id_coordinates = id_coordinates;//?
		mouse_menu = new QMenu(this); 
		showAction = new QAction("Формуляр объекта", this);
		
		changeCoordObjectAction = new QAction("Переместить объект", this);
		change_angle_action = new QAction("Повернуть объект", this);
		appointWeapon = new QAction("Назначить средство поражения", this);
		dissolutionPlanAction = new QAction("Отменить целераспределение", this);
		QAction *great_scale_act = new QAction("Увеличить масштаб карты  \">\"", this);
		QAction *less_scale_act = new QAction("Уменьшить масштаб карты  \"<\"", this);

		if (semantic_flag==49)
		{
			mouse_menu->addAction(great_scale_act); 
			connect(great_scale_act, SIGNAL(triggered()), this, SLOT(greateScale()));

			mouse_menu->addAction(less_scale_act); 
			connect(less_scale_act, SIGNAL(triggered()), this, SLOT(lessScale()));

			mouse_menu->addAction(changeCoordObjectAction);
			connect(changeCoordObjectAction, SIGNAL(triggered()), this, SLOT(changeObjectCoord()));

			mouse_menu->addAction(change_angle_action);
			connect(change_angle_action, SIGNAL(triggered()), this, SLOT(changeAngleWithMouse()));
		}
		if (semantic_flag==51)
		{
			mouse_menu->addAction(great_scale_act); 
			connect(great_scale_act, SIGNAL(triggered()), this, SLOT(greateScale()));

			mouse_menu->addAction(less_scale_act); 
			connect(less_scale_act, SIGNAL(triggered()), this, SLOT(lessScale()));

			mouse_menu->addAction(changeCoordObjectAction);
			connect(changeCoordObjectAction, SIGNAL(triggered()), this, SLOT(changeHallCoord()));
		}
		if (semantic_flag==50) 
		{	
			mouse_menu->addAction(great_scale_act); 
			connect(great_scale_act, SIGNAL(triggered()), this, SLOT(greateScale()));

			mouse_menu->addAction(less_scale_act); 
			connect(less_scale_act, SIGNAL(triggered()), this, SLOT(lessScale()));

			mouse_menu->addAction(showAction); 
			connect(showAction, SIGNAL(triggered()), this, SLOT(showInfoAboutObject()));

			mouse_menu->addAction(dissolutionPlanAction);//отменить целераспределение
			connect(dissolutionPlanAction, SIGNAL(triggered()), this, SLOT(dissolutionPlan()));

			mouse_menu->addAction(appointWeapon);
			connect(appointWeapon, SIGNAL(triggered()), this, SLOT(appointWeaponSlot()));
		}	
		mouse_menu->exec(pe);
}
//меню по клику правой клавишей мыши в любом месте
void	 MapView::mouseRightSimpleMenu(QPoint pe)
{
		mouse_menu = new QMenu(this); 
		QAction *great_scale_act = new QAction("Увеличить масштаб карты  \">\"", this);
		QAction *less_scale_act = new QAction("Уменьшить масштаб карты  \"<\"", this);

			mouse_menu->addAction(great_scale_act); 
			connect(great_scale_act, SIGNAL(triggered()), this, SLOT(greateScale()));

			mouse_menu->addAction(less_scale_act); 
			connect(less_scale_act, SIGNAL(triggered()), this, SLOT(lessScale()));
		//mouse_menu->addAction("&DeleteObject");  
		mouse_menu->exec(pe);
}

//диалоговое окно для перемещения объекта c настоящими координатами
void	 MapView::changeObjectCoord()
{
		/*mapwin->number_action=1;
		change_coord_dialog = new QDialog;
		change_coord_dialog->setWindowTitle("Перемещение объекта");

		GEODEGREEXY G_XY= model->getObjectCoordinatesWGS(Id_coordinates);
		if ((G_XY.Degree_x==0)&&(G_XY.Degree_y==0)&&(G_XY.x!=0)) 
		{
			double x = G_XY.x, y = G_XY.y, h = G_XY.h;
			double *xx, *yy, *hh;
			xx=&x; yy=&y; hh=&h;
			G_XY = doubleToGeodegree(xx, yy, hh);
		}
		QLabel * infolabel1 = new QLabel("Введите координаты для средства\n координаты вводятся в WGS84: градусы, минуты, секунды");
		QLabel * x_label = new QLabel("  X ");
		QLabel * y_label = new QLabel("  Y ");
		QLabel * g_label = new QLabel("Градусы");
		QLabel * m_label = new QLabel("Минуты");
		QLabel * s_label = new QLabel("Секунды");
		QLabel * h_label = new QLabel("Высота");
		x_edit_g = new QLineEdit(QString::number(G_XY.Degree_x));
		x_edit_m = new QLineEdit(QString::number(G_XY.Minute_x));
		x_edit_s = new QLineEdit(QString::number(G_XY.Second_x));
		y_edit_g = new QLineEdit(QString::number(G_XY.Degree_y));
		y_edit_m = new QLineEdit(QString::number(G_XY.Minute_y));
		y_edit_s = new QLineEdit(QString::number(G_XY.Second_y));
		h_edit_h = new QLineEdit(QString::number(G_XY.h_wgs));
		QPushButton * new_cel_button = new QPushButton("Указать мышью");
		 connect(new_cel_button, SIGNAL(clicked()), this, SLOT(closeDhangeDoordDialog()));
		QPushButton * paint_button = new QPushButton("Нанести");
		 connect(paint_button, SIGNAL(clicked()), change_coord_dialog, SLOT(accept()));

		QVBoxLayout *layout1_1 = new QVBoxLayout;
		layout1_1->addStretch();
		layout1_1->addWidget(g_label);
		layout1_1->addStretch();
		layout1_1->addWidget(m_label);
		layout1_1->addStretch();
		layout1_1->addWidget(s_label);

		QVBoxLayout *layout1_2 = new QVBoxLayout;
		layout1_2->addWidget(x_label);
		layout1_2->addWidget(x_edit_g);
		layout1_2->addWidget(x_edit_m);
		layout1_2->addWidget(x_edit_s);

		QVBoxLayout *layout1_3 = new QVBoxLayout;
		layout1_3->addWidget(y_label);
		layout1_3->addWidget(y_edit_g);
		layout1_3->addWidget(y_edit_m);
		layout1_3->addWidget(y_edit_s);		

		QHBoxLayout *layout1 = new QHBoxLayout;
		layout1->addLayout(layout1_1);
		layout1->addLayout(layout1_2);
		layout1->addLayout(layout1_3);

		QHBoxLayout *layout2 = new QHBoxLayout;
		layout2->addWidget(h_label);
		layout2->addStretch();
		layout2->addWidget(h_edit_h);

		QHBoxLayout *layout3 = new QHBoxLayout;
		layout3->addWidget(new_cel_button);
		layout3->addWidget(paint_button);

		QVBoxLayout * main_layout = new QVBoxLayout;
		main_layout->addWidget(infolabel1);
		main_layout->addLayout(layout1);
		main_layout->addLayout(layout2);
		main_layout->addLayout(layout3);

		change_coord_dialog->setLayout(main_layout);
		if(change_coord_dialog->exec() == QDialog::Accepted)
		{
			apdateInDatabase();
		}*/
}
//диалоговое окно для перемещения коридора пролета
void	MapView::changeHallCoord()
{
		mapwin->number_action=2;
		change_hall_dialog = new QDialog;
		change_hall_dialog->setWindowTitle("Перемещение объекта");

		QPushButton * button1 = new QPushButton("Указать мышью");
		 connect(button1, SIGNAL(clicked()), this, SLOT(closeChangeHollCoordDialog()));
		QPushButton * button2 = new QPushButton("Указать вручную");
		 connect(button2, SIGNAL(clicked()), change_hall_dialog, SLOT(slot()));

		 QVBoxLayout * main_layout = new QVBoxLayout;
		main_layout->addWidget(button1);
		//main_layout->addWidget(button2);

		change_hall_dialog->setLayout(main_layout);
		change_hall_dialog->exec();
}
//слот для изменения координат объекта в базе
void	 MapView::apdateInDatabase()
{
	//пересчет во все координаты
	WGS_to_other();
}
//по левой кнопке мыши, если противник (2) или если наш (1)//показать информацию по щелчку левой клавишей мыши на объекте/средстве поражении
void	 MapView::showShortInformationObject(long int id_obj, long int flag)
{
	//QString country, takt_number, name, str_info50, str_but, destroy_information;
	//int itog_num=0;
	//QMap<QString, QList<Coord*>> mas;
	//mas=model->get_obj_info4 (id_obj, flag);
	//if (id_obj==5)
	//{
	//	showInformationDialog("Коридор пролета	бомбардировочной авиации");
	//	return;
	//}
	//if (id_obj==505)
	//{
	//	//showInformationDialog("Траектория полета");
	//	return;
	//}
	//if (flag==2)//если противник
	//{
	//	str_info50 = "   Число точек прицеливания:  ";
	//	str_but = "Информация о поражении";
	//	QString code, destroy_object;
	//	QMap<QString, QList<Coord*>>::iterator it = mas.begin();

	//	int count = mas.size();//class_name - country_name - description - destroy_recomendation - - id_object - latitude_sk_42 - element_name 
	//	code = it.key();//latitude_wgs_84 - longitude_sk_42 - longitude_wgs_84 - object_name - property_value - size_x - size_y - type_name
	//	QList<Coord*> temp;
	//	Coord *t_point= new Coord;
	//	temp = mas["type_name"];
	//	t_point = temp.at(0);
	//	 name = t_point->obj_val;
	//	temp = mas["country_name"];
	//	t_point = temp.at(0);
	//	 country = t_point->obj_val;
	//	temp = mas["element_name"];
	//	t_point = temp.at(0);
	//	 destroy_object = t_point->obj_val + ",";//число уязвимых элементов string
	//	  temp = mas["object_name"];
	//	  t_point = temp.at(0);
	//	  name += " ";
	//	  name += t_point->obj_val;		   
	//	int s_size = destroy_object.length();
	//	int kol=0;
	//	for (int i=0; i<s_size; i++)
	//	{
	//		if (destroy_object[i]==',')	kol++;
	//	}
	//	/*QString element;
	//	int k=0, k1=0, it_num=0;
	//	for (int i=0; i<kol-1; i++)
	//	{
	//		while (destroy_object[k]!=',')
	//		{
	//			element[k1]=destroy_object[k];
	//			k1++;
	//			k++;
	//		}
	//		k++;
	//		temp = mas[element];
	//		it_num = temp.count();//координаты уязвимых элементов нужны?
	//		itog_num+=it_num;
	//		
	//	}*/
	//	itog_num=kol;
	//	int id_targeting_version = get_targeting_version();
	//	if (id_targeting_version!=0)
	//	{
	//		destroy_information = model->get_destroy_obj_info(id_targeting_version, id_obj);
	//	}
	//	else
	//	{
	//			destroy_information = " Нет информации о поражении объекта.\n";
	//			destroy_information += " ";
	//	}
	//}
	//if (flag==1)//если наш
	//{
	//	country = "____________________";
	//	QString code, destroy_object, srt_id_obj;
	//	srt_id_obj = QString::number(id_obj);
	//	QMap<QString, QList<Coord*>>::iterator it = mas.begin();
	//	
	//	int count = mas.size(), n_point;
	//	code = it.key();
	//	QList<Coord*> temp;
	//	Coord *t_point= new Coord;

	//	temp = mas["takt_number"];
	//	t_point = temp.at(0);
	//	 takt_number = t_point->obj_val;
	//		 if (takt_number=="")
	//		 {
	//			temp = mas["short_name_type"];
	//			t_point = temp.at(0);
	//			takt_number = t_point->obj_val;
	//		 }
	//	temp = mas["weapon_name"];//смерч, искандер
	//	t_point = temp.at(0);
	//	 name = t_point->obj_val;//str_info50
	//	temp = mas[srt_id_obj];
	//	int t_size=temp.size();
	//	str_info50 = "Носители:\n";
	//	if (t_size>1)
	//	{
	//		for (int i=0; i<t_size; i++)
	//		{
	//			t_point = temp.at(i);
	//			str_info50 += t_point->obj_val;
	//			//str_info50 += " - ";
	//			//n_point = t_point->get_n_point();
	//			//str_info50 += QString::number(n_point);
	//			if (i<t_size-1) str_info50 += "\n";
	//		}
	//	}
	//	else 
	//	{
	//		temp = mas["type_name"];//смерч, искандер
	//		t_point = temp.at(0);
	//		 str_info50 = t_point->obj_val;//str_info50
	//	}
	//	str_but = "не отображается)";
	//	itog_num = 0;
	//	destroy_information = "Не отображается";
	//}
	//	//---------------------------------------------------------------------------
	//			short_info_dialog = new QDialog;
	//			short_info_dialog ->setWindowTitle("Информация об объекте");
	//			//short_info_dialog->setFixedSize(200,300);
	//			QLabel * info_label1 = new QLabel(takt_number + " \"" + name + "\"");
	//			QLabel * info_label2 = new QLabel(country);
	//			/*QLabel * info_label30 = new QLabel("X = ");
	//			QLabel * info_label31 = new QLabel("435433443");
	//			QLabel * info_label40 = new QLabel("Y = ");
	//			QLabel * info_label41 = new QLabel("535433443");*/
	//			QLabel * info_label50 = new QLabel(str_info50);
	//			QLabel * info_label51 = new QLabel(QString::number(itog_num));

	//		  //==========размещение объектов=================
	//			QHBoxLayout *hbox1_layout = new QHBoxLayout;   //
	//			hbox1_layout->addStretch();
	//			hbox1_layout->addWidget(info_label1);			//
	//			hbox1_layout->addStretch();
	//			//-----
	//			/*QHBoxLayout *hbox2_layout = new QHBoxLayout;   //
	//			hbox2_layout->addWidget(info_label30);			//
	//			hbox2_layout->addWidget(info_label31);			
	//			//-----
	//			QHBoxLayout *hbox3_layout = new QHBoxLayout;   //
	//			hbox3_layout->addWidget(info_label40);			//
	//			hbox3_layout->addWidget(info_label41);			*/
	//			//-----
	//			QHBoxLayout *hbox4_layout = new QHBoxLayout; 
	//			hbox4_layout->addStretch();
	//			hbox4_layout->addWidget(info_label2);
	//			hbox4_layout->addStretch();
	//			//-----
	//			QHBoxLayout *hbox5_layout = new QHBoxLayout;   //
	//			hbox5_layout->addStretch();
	//			hbox5_layout->addWidget(info_label50);			//
	//			if (flag==2) hbox5_layout->addWidget(info_label51);	
	//			hbox5_layout->addStretch();
 //				//-----
	//			more_but = new QPushButton(str_but);
	//			connect(more_but,SIGNAL(clicked()),this,SLOT(moreButtonClicked()));
 //				//-----
	//			QVBoxLayout *vbox1_layout = new QVBoxLayout;   //координаты?
	//			vbox1_layout->addLayout(hbox1_layout);
	//			/*vbox_layout->addLayout(hbox2_layout);
	//			vbox_layout->addLayout(hbox3_layout);*/
	//			vbox1_layout->addLayout(hbox4_layout);
	//			vbox1_layout->addLayout(hbox5_layout);
	//			if (flag==2) vbox1_layout->addWidget(more_but);
 //				//-----==============||||||||||||||||||||||||||||
	//			QLabel * info_label0_1 = new QLabel(destroy_information);
 //				//-----							
	//			QHBoxLayout *hbox0_1_layout = new QHBoxLayout;   //
	//			hbox0_1_layout->addStretch();					 //
	//			hbox0_1_layout->addWidget(info_label0_1);		 //
	//			hbox0_1_layout->addStretch();
 //				//-----		

	//			QGroupBox * primary_group_box = new QGroupBox;
	//			primary_group_box->setLayout(vbox1_layout);
	//			//===
	//			secondary_group_box = new QGroupBox;
	//			if (flag==2) secondary_group_box->setLayout(hbox0_1_layout);		
	//			secondary_group_box->hide();
	//			//======================
	//			QVBoxLayout * vbox_layout = new QVBoxLayout;   //координаты?
	//			vbox_layout->addWidget(primary_group_box);
	//			vbox_layout->addWidget(secondary_group_box);
	//		  //==============================================
	//			short_info_dialog->setLayout(vbox_layout);
	//			short_info_dialog->exec();
}
//открытие дополнительной информации о поражении объекта
void	 MapView::moreButtonClicked()
{
		secondary_group_box->show();
		more_but->hide();
}
//
void	 MapView::showInformationDialog(QString information)
{
	create_object_dialog = new QDialog;
	create_object_dialog ->setWindowTitle("Информация об объекте");
	QLabel * info_label1 = new QLabel(information);
	info_label1->setAlignment(Qt::AlignCenter);
	QHBoxLayout *hbox1_layout = new QHBoxLayout;  
	hbox1_layout->addStretch();
	hbox1_layout->addWidget(info_label1);
	hbox1_layout->addStretch();
	create_object_dialog->setLayout(hbox1_layout);		
	create_object_dialog->exec();
}

//отображать внизу карты в стаусе
void	 MapView::setStatusInfo(QString status)
{
	/*double a,b;
	status = "гориз: ";
	if (mapwin->horizontalScrollBar()->isVisible()==true) 
	{
		//a=mapwin->horizontalScrollBar()->maximum()-mapwin->horizontalScrollBar()->minimum()+mapwin->horizontalScrollBar()->pageStep();
		if (mapwin->horizontalScrollBar()->value() != 0)
		{
			a = mapwin->horizontalScrollBar()->maximum()- mapwin->horizontalScrollBar()->minimum()+ mapwin->horizontalScrollBar()->pageStep();
			status +=QString::number(a);
			a /=mapwin->horizontalScrollBar()->value();
		}
		else a =0;
	}
	else a=0;//
	status +=" Вертик: ";
	if (mapwin->verticalScrollBar()->isVisible()==true)
	{
		//b=mapwin->verticalScrollBar()->maximum()-mapwin->verticalScrollBar()->minimum()+mapwin->verticalScrollBar()->pageStep();
		if (mapwin->verticalScrollBar()->value() != 0)
		{
			b = mapwin->verticalScrollBar()->maximum()- mapwin->verticalScrollBar()->minimum()+ mapwin->verticalScrollBar()->pageStep();
			status +=QString::number(b);
			b /=mapwin->verticalScrollBar()->value();
		}
		else b =0;
	}
	else b=0;//
	status +=" - G: "+QString::number(a)+" V: "+QString::number(b);*/
	status_bar->setText(status);
}
//=====нажатие на тулбаттон test ==================================
void	 MapView::test()
{
		/*Qt::HANDLE hwnd;
		long int a=8294400;
		HWND	hWnd;
		hWnd->unused;
		long int width=1600;
		long int height=1200;
		QSqlQuery query;
		HIMAGE hImage = mapwin->map->mapCreateImage(hWnd);
		QString str = "SELECT sign_foto FROM object_signs WHERE id_sign=7";
		query.prepare(str);

		if(!query.exec())
		{
			QString sss = query.lastError().text();
		}

		QSqlRecord rec = query.record();
		query.next();*/
		setStatusInfo("test");
}
//// Запросить округленный масштаб отображения карты
void	 MapView::appendSit_Test()
{
	//mapwin->emitSignalForPlanner(1,0);
	setStatusInfo("q");
}


//
void	 MapView::showOnlyUnAllocation()
{
	/*int id_targeting_version = get_targeting_version();
	QList <int> id_unlocation = model->get_undestroing_objects(id_targeting_version);
	int id_unlocation_object;
	int size_list = id_unlocation.size();
	int data;
	for (int i=0; i<size_list; i++)
	{
		data = id_unlocation.at(i);
	}
	int size = id_unlocation.size();
	if (size==0) 
	{
		showInformationDialog("Нет нераспределенных объектов");
		return;
	}
	if (mapwin->hSite) mapwin->closeSit(mapwin->hMap, mapwin->hSite);
	openMapSit("","");
	QStringList info_unlocation;
	for (int i=0; i<size; i++)
	{
		id_unlocation_object = id_unlocation.at(i);
		info_unlocation = model->infoUnlocationObject(id_unlocation_object);
		QString sign_key = info_unlocation.at(0);
		long int x = info_unlocation.at(1).toLong();
		long int y = info_unlocation.at(2).toLong();
		int id_coord = info_unlocation.at(3).toInt();
		bool if_shtab = ifShtab(sign_key);
		mapwin->createObject(mapwin->hSite, x, y, sign_key.toLocal8Bit().data(), id_unlocation_object, 2, if_shtab, "", id_coord);
	}
	mapwin->updateScreen();*/
}
//
void	 MapView::checkPaintCel()
{
	if (mapwin->hMap==0)
	{
		showOpenMapDialog();
	}
//	bool znaki_bool = get_znaki_checkbox();
//	bool line_bool = get_line_checkbox();
	/*if (znaki_bool)
	{
		if (mapwin->hSite_p) mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);
		paintPlaner();
	}
	else	
		mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);
	if (line_bool)
	{
		if (mapwin->hSite_s) mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);
		joinObjectWithWeapon();
	}
	else	
		mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);*/

	mapwin->updateScreen();

}
//отобразить средства по направлениям
void	 MapView::checkPaintDirection()
{
//	bool mark_bool = getMarkDirCheckbox();
//	bool line_bool = getLineDirCheckbox();
	//bool mark_bool = get_znaki_checkbox();
	//bool line_bool = get_line_checkbox();

	/*if (mark_bool)
	{
		if (mapwin->hSite_p) mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);
		paintPlanerDirection();
	}
	else	
		mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);
	if (line_bool)
	{
		if (mapwin->hSite_s) mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);
		joinObjectWithWeaponDirection();
	}
	else	
		mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);*/
	mapwin->updateScreen();
}
//открытие карты при неправильном порядке работы (Для начала необходимо открыть карту)
void	 MapView::showOpenMapDialog()
{
	openMapDialog = new QDialog;
	openMapDialog ->setWindowTitle(tr("Information"));
	QLabel * info_label1 = new QLabel("Для начала работы необходимо открыть карту. \n Открыть карту? ");
	QPushButton * if_open_map = new QPushButton("Да");
	connect(if_open_map, SIGNAL(clicked()), openMapDialog, SLOT(accept()));
	QPushButton * if_no_open_map = new QPushButton("Нет");
	connect(if_no_open_map, SIGNAL(clicked()), openMapDialog, SLOT(close()));
	
	QHBoxLayout *layout1 = new QHBoxLayout;
	layout1->addWidget(if_open_map);
	layout1->addWidget(if_no_open_map);

	QVBoxLayout * layout2 = new QVBoxLayout;
	layout2->addWidget(info_label1);
	layout2->addLayout(layout1);

	openMapDialog->setLayout(layout2);
	if(openMapDialog->exec() == QDialog::Accepted)
	{
		openNewMap();
	}
	
}

//перевод координат при перемещении средства поражения - в базе
void	 MapView::WGS_to_other()
{
//	if (mapwin->hSite_s) mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);
//	if (mapwin->hSite_p) mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);
//
//	mapwin->number_action=0;
//	if(mapwin->isGeoSupported())
//	{
//		GEODEGREE N, E, W, Wy;
//		double N_rad, E_rad, H;
//		double N_rad42, E_rad42;		
//		N.Degree = x_edit_g->text().toLong();
//		N.Minute = x_edit_m->text().toLong();
//		N.Second = x_edit_s->text().toFloat();
//		W.Degree = N.Degree;
//		W.Minute = N.Minute;
//		W.Second = N.Second;
//		mapwin->degreeToRadian(&N, &N_rad); //из градусов в радианы
//
//		E.Degree = y_edit_g->text().toLong();
//		E.Minute = y_edit_m->text().toLong();
//		E.Second = y_edit_s->text().toFloat();
//
//		Wy.Degree = E.Degree;
//		Wy.Minute = E.Minute;
//		Wy.Second = E.Second;
//
//		mapwin->degreeToRadian(&E, &E_rad); //из градусов в радианы
//
//		H = h_edit_h->text().toDouble();
//		//-----------------
//		mapwin->geoWGS84ToPlane3D(mapwin->hMap, &N_rad, &E_rad, &H); //из геодезических координат в радианах WGS84 в метры на местности в проекции карты
//		N_rad42 = N_rad;
//		E_rad42 = E_rad;
//		mapwin->planeToGeo423D(&N_rad42, &E_rad42, &H); // Преобразование из метров на местности (проекция карты) в геодезические координаты в радианах (эллипсоид Красовского)
//		mapwin->radianToDegree(&N_rad42, &N);
//		mapwin->radianToDegree(&E_rad42, &E);
//
//		model->add_new_coord (W, Wy, N_rad, E_rad, H, N, E, Id_obj, Id_coordinates);
////		int id_targeting_version = get_targeting_version();
//		//allocation->begin_targeting_for_mapview(id_targeting_version);
//		if (mapwin->hSite_w) mapwin->closeSit(mapwin->hMap, mapwin->hSite_w);
//		paintWeapon();
//		if (mapwin->hMap) mapwin->updateScreen();
//	}		
}


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

//перевод координат DOUBLEPOINT GEODEGREE
//на входе x,y в метрах
//на выходе x,y в радианах
GEODEGREEXY	 MapView::doubleToGeodegree(double *x, double *y, double *h)
{
	GEODEGREEXY G_XY;
	//GEODEGREE * G_X;
	//GEODEGREE * G_Y;
	//long int a1 = mapwin->planeToGeoWGS843D(x, y, h);
	//if (a1) 
	//{
	//	mapwin->radianToDegree(x, G_X);
	//	mapwin->radianToDegree(y, G_Y);
	//}
	//G_XY.Degree_x = G_X->Degree;
	//G_XY.Degree_y = G_Y->Degree;
	//G_XY.Minute_x = G_X->Minute;
	//G_XY.Minute_y = G_Y->Minute;
	//G_XY.Second_x = G_X->Second;
	//G_XY.Second_y = G_Y->Second;
	//model->furtherCoordInBase(Id_coordinates, G_XY);
	return G_XY;
}

//выбор места для средства по клику мыши
void	 MapView::showPositionWGSMouseSlot(double X, double Y, double H)
{
	double *x, *y, *h;
	double xx=X, yy=Y, hh=H;
	x = &xx;//-
	y = &yy;//-- чтобы не менять X Y H
	h = &hh;//-
	GEODEGREEXY G_XY = mapwin->pictureToWGS(x, y, h);
	x_edit_g->setText(QString::number(G_XY.Degree_x));
	x_edit_m->setText(QString::number(G_XY.Minute_x));
	x_edit_s->setText(QString::number(G_XY.Second_x));
	y_edit_g->setText(QString::number(G_XY.Degree_y));
	y_edit_m->setText(QString::number(G_XY.Minute_y));
	y_edit_s->setText(QString::number(G_XY.Second_y));
	WGS_to_other();
}
//пересчет координат
void	 MapView::showPositionHallMouseSlot(double X, double Y, double H)
{
	//mapwin->number_action=0;
	//QStringList G_XY= model->getHallCoordinatesWGS(Id_coordinates);
	//long int d1,d2,x1,x2,x3,y1,y2,y3;//коридор:
	//x1 = G_XY.at(0).toLong();					//x2, y2							x3, y3
	//y1 = G_XY.at(1).toLong();					//		\\\						///
	//x2 = G_XY.at(2).toLong();					//			\\\				///
	//y2 = G_XY.at(3).toLong();					//				------------
	//x3 = G_XY.at(4).toLong();					//				____________
	//y3 = G_XY.at(5).toLong();					//			///				\\\					/**/
	//d1 = (x3-x1)*0.5;						//		///						\\\				/**/
	//d2 = (y3-y1)*0.5;						//x1, y1							x4, y4
	//x1 = X-d1;
	//y1 = Y-d2;
	//x3 = X+d1;
	//y3 = Y+d2;
	//model->apdateHollCoord(x1, y1, x3, y3, Id_coordinates);
	//if (mapwin->hSite_corr) mapwin->closeSit(mapwin->hMap, mapwin->hSite_corr);
	//paintCorridors();
	//if (mapwin->hMap) mapwin->updateScreen();
}
void	 MapView::showAppointMouseSlot(long int id_object)
{
	mapwin->number_action=0;//дальше только обычный поиск
	if (mapwin->hSite_ok_weapon) mapwin->closeSit(mapwin->hMap, mapwin->hSite_ok_weapon);
	if (mapwin->hSite_p) mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);
	if (mapwin->hSite_s) mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);
	//запрос в базу
	/*country = "____________________";
		QString code, destroy_object, srt_id_obj;
		srt_id_obj = QString::number(id_obj);
		QMap<QString, QList<Coord*>>::iterator it = mas.begin();
		
		int count = mas.size(), n_point;
		code = it.key();
		QList<Coord*> temp;
		Coord *t_point= new Coord;

		temp = mas["takt_number"];
		t_point = temp.at(0);
		 takt_number = t_point->obj_val;
			 if (takt_number=="")
			 {
				temp = mas["short_name_type"];
				t_point = temp.at(0);
				takt_number = t_point->obj_val;
			 }
		temp = mas["weapon_name"];//смерч, искандер
		t_point = temp.at(0);
		 name = t_point->obj_val;//str_info50
		temp = mas[srt_id_obj];
		int t_size=temp.size();
		str_info50 = "Носители:\n";
		if (t_size>1)
		{
			for (int i=0; i<t_size; i++)
			{
				t_point = temp.at(i);
				str_info50 += t_point->obj_val;
				//str_info50 += " - ";
				//n_point = t_point->get_n_point();
				//str_info50 += QString::number(n_point);
				if (i<t_size-1) str_info50 += "\n";
			}
		}*/
	shortOfGunDialog();
	//paintCorridors();
	//if (mapwin->hMap) mapwin->updateScreen();
}
void				MapView::shortOfGunDialog()
{
	//рассчеты можно ли применить данное средство
	short_gun_dialog = new QDialog;
	short_gun_dialog->setWindowTitle("Добавить боеприпасы");
	QHBoxLayout *main_layout = new QHBoxLayout;
	QLabel *label0 = new QLabel("Добавить боеприпасы?");
	QPushButton *new_gun_button = new QPushButton("Да");
	connect(new_gun_button, SIGNAL(clicked()), short_gun_dialog, SLOT(close()));

	QPushButton *cancel_button = new QPushButton("Нет");
	connect(cancel_button, SIGNAL(clicked()), short_gun_dialog, SLOT(close()));
	main_layout->addWidget(label0);
	main_layout->addWidget(new_gun_button);
	main_layout->addStretch();
	main_layout->addWidget(cancel_button);

	short_gun_dialog->setLayout(main_layout);
	short_gun_dialog->exec();
}
///указать мышью средство
void				MapView::closeDhangeDoordDialog()
{
	if (mapwin->hSite_s) mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);
	if (mapwin->hSite_p) mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);
	change_coord_dialog->close();
}
//
void				MapView::closeAppointDialog()
{
	if (mapwin->hSite_s) mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);
	if (mapwin->hSite_p) mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);
	appointWeaponDialog->close();
}
//указать мышью коридор
void				MapView::closeChangeHollCoordDialog()
{
	if (mapwin->hSite_s) mapwin->closeSit(mapwin->hMap, mapwin->hSite_s);
	if (mapwin->hSite_p) mapwin->closeSit(mapwin->hMap, mapwin->hSite_p);
	change_hall_dialog->close();
}
//повернуть средство поражения
void				MapView::changeAngleWithMouse()
{
	//данные, необходимые для рассчетов угла!
	//G_XY_c= model->getObjectCoordinatesWGS(Id_coordinates);//получение координат объекта=центр
	//
	//mapwin->x_for_change_angle; //
	//mapwin->y_for_change_angle; 
	//mapwin->number_action=4;
}
void				MapView::redrawWithNewAngle(double X, double Y)
{
	//mapwin->number_action=0;
	////рассчеты!
	//double x0=G_XY_c.x+10000, y0=G_XY_c.y;
	//double a = X - G_XY_c.x;
	//double b = Y - G_XY_c.y;
	//double angle=atan(abs(a)/abs(b));//в угле!
	//angle = 180*angle/M_PI;//перевод из радиан в угол
	//if (a>0&&b>0)//1я четверть
	//{ angle = 90-angle;
	//}
	//if (a>0&&b<0)
	//{ angle+= 270;
	//}
	//if (a<0&&b>0)
	//{ angle+= 90;
	//}
	//if (a<0&&b<0)
	//{ angle = 270-angle;
	//}
	////status_bar->setText(QString::number(angle));
	//model->apdateAngle(Id_coordinates, angle);
	//if (mapwin->hSite_w) mapwin->closeSit(mapwin->hMap, mapwin->hSite_w);
	//paintWeapon();
	//mapwin->updateScreen();
}


