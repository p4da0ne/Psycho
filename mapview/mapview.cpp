#include "mapview.h"
#include "view_manage.h"


MapView::MapView(QWidget * parent)
    : QWidget(parent)
{
       
       MainCodec = QTextCodec::codecForName("CP1251");
       setMouseTracking(true);

       setWindowIcon(QIcon("./icons/mapwork.png"));

    QVBoxLayout *vertLayout = new QVBoxLayout();  //==== основной лэйаут
    vertLayout->setMargin(1);
	
    //==== формирование меню на основе QFrame и QToolButton's ====
    QFrame *buttons_menu = new QFrame();
    buttons_menu->setFrameStyle(QFrame::Panel | QFrame::Raised);
    buttons_menu->setLineWidth(2);

    QToolButton *open_map_but = new QToolButton();
    open_map_but->setIcon(QIcon("./icons/map_open.png"));
    open_map_but->setIconSize(QSize(20,20));
    open_map_but->setToolTip("Открыть карту");
    connect(open_map_but, SIGNAL(clicked()), this, SLOT(open()));

//добавление пользовательского слоя
    QToolButton *tool_button_test = new QToolButton();
    tool_button_test->setIcon(QIcon("./icons/map_open.png"));
    tool_button_test->setIconSize(QSize(20,20));
    tool_button_test->setToolTip("Добавить слой");
    connect(tool_button_test, SIGNAL(clicked()), this, SLOT(appendSit()));


    QToolButton *close_map_but = new QToolButton();
    close_map_but->setIcon(QIcon("./icons/map_close.png"));
    close_map_but->setIconSize(QSize(20,20));
    close_map_but->setToolTip("Закрыть карту и все данные");
    connect(close_map_but, SIGNAL(clicked()), this, SLOT(closeMap()));
//яркость
    QToolButton *set_map_bright1 = new QToolButton();
    set_map_bright1->setIcon(QIcon("./icons/up.png"));
    set_map_bright1->setIconSize(QSize(20,20));
    set_map_bright1->setToolTip("Увеличить яркость карты");
    connect(set_map_bright1, SIGNAL(clicked()), this, SLOT(changeBrihgtUp()));
//яркость
    QToolButton *set_map_bright2 = new QToolButton();
    set_map_bright2->setIcon(QIcon("./icons/down.png"));
    set_map_bright2->setIconSize(QSize(20,20));
    set_map_bright2->setToolTip("Уменьшить яркость карты");
    connect(set_map_bright2, SIGNAL(clicked()), this, SLOT(changeBrihgtDown()));
    QLabel *v_lab = new QLabel();


//уменьшить масштаб отображения карты
	QToolButton *less_scale_but = new QToolButton();
	less_scale_but->setIcon(QIcon("./icons/less_scale.jpg"));
	less_scale_but->setIconSize(QSize(20,20));
	less_scale_but->setToolTip("Уменьшить масштаб");
	connect(less_scale_but, SIGNAL(clicked()), this, SLOT(LessScale()));
//увеличить масштаб отображения карты
	QToolButton *greate_scale_but = new QToolButton();
	greate_scale_but->setIcon(QIcon("./icons/greate_scale.jpg"));
	greate_scale_but->setIconSize(QSize(20,20));
	greate_scale_but->setToolTip("Увеличить масштаб");
	connect(greate_scale_but, SIGNAL(clicked()), this, SLOT(GreateScale()));




    v_lab->setFrameStyle(QFrame::VLine | QFrame::Raised);
    v_lab->setLineWidth(2);


    QHBoxLayout *menuLayout = new QHBoxLayout();
    menuLayout->setAlignment(Qt::AlignLeft);
    menuLayout->setMargin(2);
    menuLayout->setSpacing(0);

    menuLayout->addWidget(open_map_but);
    menuLayout->addWidget(tool_button_test);
    menuLayout->addWidget(close_map_but);
    menuLayout->addWidget(set_map_bright1);
    menuLayout->addWidget(set_map_bright2);
    menuLayout->addWidget(v_lab);

	
	menuLayout->addWidget(less_scale_but);
    menuLayout->addWidget(greate_scale_but);


    buttons_menu->setLayout(menuLayout);

    vertLayout->addWidget(buttons_menu);

    //===================================================================
    
    QHBoxLayout *centralLayout = new QHBoxLayout();

    QFrame *fr = new QFrame();
    fr->setFrameStyle(QFrame::Box | QFrame::Raised);
    fr->setLineWidth(2);
	
    QLabel *left_panel_name = new QLabel("Работа с объектами");
    left_panel_name->setAlignment(Qt::AlignCenter);
    left_panel_name->setFont(QFont("Arial",8,QFont::Bold,false));
	
//------------------тест класса ViewManage------------------------------------
	

    QPushButton *add_obj = new QPushButton("Показать средства");
    connect(add_obj, SIGNAL(clicked()), this, SLOT(openMapSit1()));

    QPushButton *show_obj_but = new QPushButton("Показать формирования");
    connect(show_obj_but, SIGNAL(clicked()), this, SLOT(openMapSit()));

    QPushButton *close_obj_but = new QPushButton("Скрыть все объекты");
    connect(close_obj_but, SIGNAL(clicked()), this, SLOT(showCloseSitInfo()));

    /*QPushButton *but1 = new QPushButton("1");
    connect(but1, SIGNAL(clicked()), this, SLOT(test_view_manage()));

    QPushButton *but2 = new QPushButton("2");
    connect(but2, SIGNAL(clicked()), this, SLOT(test_view_manage2()));

    QPushButton *show_line_but = new QPushButton("Show line");
    connect(show_line_but, SIGNAL(clicked()), this, SLOT(selectLineObject()));*/

    QPushButton *but_PL = new QPushButton("Психогенные потери");
    connect(but_PL, SIGNAL(clicked()), this, SLOT(People_Losse()));

    QPushButton *mps = new QPushButton("МПС своих войск");
    connect(mps, SIGNAL(clicked()), this, SLOT(calc_mps()));

        QPushButton *mps2 = new QPushButton("МПС войск противника");
    connect(mps2, SIGNAL(clicked()), this, SLOT(calc_mps2()));


    QVBoxLayout *left_layout = new QVBoxLayout();
    left_layout->setAlignment(Qt::AlignTop);
    left_layout->setMargin(2);
    //left_layout->setHorizontalSpacing(0);
    left_layout->addWidget(left_panel_name);
    left_layout->addWidget(add_obj);
    left_layout->addWidget(show_obj_but);
		
    /*left_layout->addWidget(show_line_but);
    left_layout->addWidget(but1);
    left_layout->addWidget(but2);*/
    left_layout->addWidget(mps);
    left_layout->addWidget(mps2);
    left_layout->addWidget(but_PL);
    left_layout->addWidget(close_obj_but);
    fr->setLayout(left_layout);

    centralLayout->addWidget(fr);

    // === mapscroll ========================

    mapwin = new MapScroll();

    connect(mapwin,SIGNAL(signal_for_info_2_arg(QString, long int)),this,SLOT(showShortInformationObject_2arg(QString, long int)));
    //connect(mapwin,SIGNAL(signal_for_info(long int)),this,SLOT(showShortInformationObject(long int)));//mysignal - движение мыши
    
	
	
	connect(mapwin,SIGNAL(signal_for_right_button(HOBJ, long int, long int, QPoint, bool)),this,SLOT(mouseRightMenu(HOBJ, long int, long int, QPoint, bool)));
    
	
	
	
	connect(mapwin,SIGNAL(signal_for_change_scale(QPoint)),this,SLOT(mouseRightSimpleMenu(QPoint)));
	
	centralLayout->addWidget(mapwin);

    //========================================

    //QFrame *right_fr = new QFrame();
    //right_fr->setFrameStyle(QFrame::Box | QFrame::Raised);
    //right_fr->setLineWidth(2);

    //QToolButton *but3 = new QToolButton();
    //but3->setIcon(QIcon("./icons/open.png"));
    //but3->setIconSize(QSize(20,20));
    //
    //QToolButton *but4 = new QToolButton();
    //but4->setIcon(QIcon("./icons/exit.png"));
    //but4->setIconSize(QSize(20,20));

    //QGridLayout *right_grid = new QGridLayout();
    //right_grid->setAlignment(Qt::AlignTop);
    //right_grid->setMargin(2);
    //right_grid->setHorizontalSpacing(0);

    //right_grid->addWidget(but3,0,0);
    //right_grid->addWidget(but4,0,1);
    //
    //right_fr->setLayout(right_grid);

    //centralLayout->addWidget(right_fr);

    //========================================
    vertLayout->addLayout(centralLayout);

   //==============================================
    QLineEdit *status_bar = new QLineEdit();
    status_bar->setReadOnly(true);
    vertLayout->addWidget(status_bar);

  //=================================================

    QHBoxLayout *coord_layout = new QHBoxLayout();
    cursor_coord = new QLineEdit();
    cursor_coord->setReadOnly(true);
    QLineEdit *map_scale = new QLineEdit();
    map_scale->setReadOnly(true);
    QLineEdit *additional_info = new QLineEdit();
    additional_info->setReadOnly(true);

    coord_layout->addWidget(cursor_coord);
    coord_layout->addWidget(map_scale);
    coord_layout->addWidget(additional_info);

    vertLayout->addLayout(coord_layout);

//=================================================
    setLayout(vertLayout);
//=========================================
    flag1=0;
    flag2=FALSE;
    flag=FALSE;
    model = new ViewManage();
//	lbl = new QLabel();

//    mapwin->mapOpen("C:/projects/Saturn_500m/Saturn_topo.MAP");
//    mapwin->appendData("C:/projects/Saturn_500m/Saturn/Saturn.sit");
//    mapwin->ChangeScale(0.5);
//    mapwin->ChangeScale(0.5);
//    mapwin->ChangeScale(0.5);
//    mapwin->ChangeScale(0.5);
}
//
//MapView::~MapView()
//{
//	delete lbl;
//}

void MapView::showCloseSitInfo()
{
			info_close_dialog = new QDialog;
			info_close_dialog ->setWindowTitle("Информация");
			QLabel * info_label01 = new QLabel("Все объекты удалены.");
		  //==========размещение объектов=================
			QHBoxLayout *layout = new QHBoxLayout;		 //
			layout->addStretch();				    	 //
			layout->addWidget(info_label01);
			layout->addStretch();				
			info_close_dialog->setLayout(layout);		 
		  //==============================================//закрываем данные на всех открытых пользовательских слоях
			mapwin->dataClose(mapwin->hMap,hSite);
			info_close_dialog->exec();
}
//отклик на кнопку "-"
void MapView::LessScale()
{
    mapwin->ChangeScale(0.5);			
}

//отклик на кнопку "+"
void MapView::GreateScale()
{
    mapwin->ChangeScale(2);			
}

//отклик на пункт меню открыть
void MapView::open()
{
    QString File = QFileDialog::getOpenFileName(this, QString::null, QString::null, 
                   "Maps (*.map)\n Sites (*.sit)" );

    if (File.isEmpty()) return;//если карта не выбрана
	//mapwin->mapOpen(File.toLatin1().data());
	mapwin->mapOpen(File.toLocal8Bit().data());
}

// отклик на нажатие кнопки/пункта меню "Закрыть"
void MapView::closeMap()
{
   mapwin->closeMap();
   flag2=FALSE;
}
// затенение карты
void MapView::opacityMap()
{
	
}


void MapView::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Less || e->key() == Qt::Key_Comma)
  {
    LessScale();
    return;
  }
  if (e->key() == Qt::Key_Greater || e->key() == Qt::Key_Period)
  {
    GreateScale();
    return;
  }

  switch(e->key())
  {
     case Qt::Key_Left:
       mapwin->ChangePos(-16,0);
       return;
     case Qt::Key_Up:
       mapwin->ChangePos(0,-16);
       return;
     case Qt::Key_Right:
       mapwin->ChangePos(16,0);
       return;
     case Qt::Key_Down:
       mapwin->ChangePos(0,16);
       return;
  }

  QWidget::keyPressEvent(e);
}

//отклик на пункт меню нанести объекты (СРЕДСТВА!!!)
//===================================================
HSITE MapView::openMapSit1()
{	
	long int a = mapwin->IsActive(mapwin->hMap);
	if (a)
	{
		QDir dir;
		curFile= dir.rootPath() + "saturn";
		dir.setPath(curFile);
		if (!dir.exists())
		{
			dir.mkdir(curFile);
		}
		curFile= curFile + "/sites";
		dir.setPath(curFile);
		if (!dir.exists())
		{
			dir.mkdir(curFile);
			QString File = QFileDialog::getOpenFileName(this, QString::null, QString::null, 
                   "Классификатор (*.rsc)" );
			if (File.isEmpty()) 
			{
				int ret = QMessageBox::critical(this, "Сатурн",
                                "Не выбран классификатор! \n");
			}//если карта не выбрана
		}
		curFile=curFile + "/sit_lear.sit";
		QString File = "C:/Saturn/sites/Saturn.rsc";

		QString str_mapname = "OBJECTS";
			
		HSITE this_hsite;
		
		memset((void*)&createsite,0,sizeof(createsite));
		createsite.Length = sizeof(createsite);
		strcpy(createsite.MapName, str_mapname.toLocal8Bit().data());
		createsite.MapType=2;
			
		this_hsite = mapwin->dataOpen(mapwin->hMap,curFile.toLocal8Bit().data(),File.toLocal8Bit().data(),&createsite);
		mapwin->flag2=TRUE;
		hSite=this_hsite;
			
		createsite.Length = sizeof(createsite);
		hSite=this_hsite;
		
		selectAllObject1();
selectAllObject2();
		showInformationDialog("Объекты нанесены на карту");
		return this_hsite;
	}
	mapwin->UpdateScreen();
	return 0;
}
//отклик на пункт меню нанести объекты (ФОРМИРОВАНИЯ, ФЛАЖКИ)
//===============================================================================
HSITE MapView::openMapSit()
{	
	long int a = mapwin->IsActive(mapwin->hMap);
	if (a)
	{
		QDir dir;
		curFile= dir.rootPath() + "saturn";
		dir.setPath(curFile);
		if (!dir.exists())
		{
			dir.mkdir(curFile);
		}
		curFile= curFile + "/sites";
		dir.setPath(curFile);
		if (!dir.exists())
		{
			dir.mkdir(curFile);
			QString File = QFileDialog::getOpenFileName(this, QString::null, QString::null, 
                   "Классификатор (*.rsc)" );
			if (File.isEmpty()) 
			{
				int ret = QMessageBox::critical(this, "Сатурн",
                                "Не выбран классификатор! \n");
			}//если карта не выбрана
		}
		curFile=curFile + "/sit_lear.sit";
		QString File = "C:/Saturn/sites/Saturn.rsc";
		QString str_mapname = "OBJECTS";
		
		HSITE this_hsite;
		
		memset((void*)&createsite,0,sizeof(createsite));
		createsite.Length = sizeof(createsite);
		strcpy(createsite.MapName, str_mapname.toLocal8Bit().data());
		createsite.MapType=2;
		
		this_hsite = mapwin->dataOpen(mapwin->hMap,curFile.toLocal8Bit().data(),File.toLocal8Bit().data(),&createsite);
		mapwin->flag2=TRUE;
		hSite=this_hsite;
		
		createsite.Length = sizeof(createsite);
		hSite=this_hsite;
		
		selectAllObject();
		showInformationDialog("Объекты нанесены на карту");
		return this_hsite;
	}
	mapwin->UpdateScreen();
	return 0;
}
//вычисление координат на катре с движением мыши
void  MapView::mouseMoveEvent(QMouseEvent * event)
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


 //получаем информацию из базы для отображения информации
void MapView::showInfoAboutObject(long int id_obj)
{
	if (flag2)
	{
		if (flag1)
		{
			QString id_object, object_name, class_name, unique_number, country_name;
			QString size_x, description, property_value, size_y;
			QString destroy_recomendation, latitude_wgs_84;
			QString longitude_wgs_84, latitude_sk_42, longitude_sk_42;
			//===========================================================
			info_dialog = new QDialog;
			info_dialog ->setWindowTitle(tr("Information about object"));
			QMap<QString, QString> obj_info;
			obj_info = model->get_obj_info(id_obj);
			//======================================
			id_object = obj_info["id_object"];
			object_name = obj_info["object_name"];
			class_name = obj_info["class_name"];
			unique_number = obj_info["unique_number"];
			country_name = obj_info["country_name"];
			description = obj_info["description"];
			property_value = obj_info["property_value"];
			size_x = obj_info["size_x"];
			size_y = obj_info["size_y"];
			destroy_recomendation = obj_info["destroy_recomendation"];
			latitude_wgs_84 = obj_info["latitude_wgs_84"];
			longitude_wgs_84 = obj_info["longitude_wgs_84"];
			latitude_sk_42 = obj_info["latitude_sk_42"];
			longitude_sk_42 = obj_info["longitude_sk_42"];
			//----------------------------------------------------------------------------------
			QLabel * info_label11 = new QLabel(tr("id_object"));
			QLabel * info_label12 = new QLabel(id_object);
			QLabel * info_label21 = new QLabel(tr("object_name"));
			QLabel * info_label22 = new QLabel(object_name);
			QLabel * info_label31 = new QLabel(tr("class_name "));
			QLabel * info_label32 = new QLabel(class_name);
			QLabel * info_label41 = new QLabel(tr("unique_number "));
			QLabel * info_label42 = new QLabel(unique_number);
			QLabel * info_label51 = new QLabel(tr("country_name "));
			QLabel * info_label52 = new QLabel(country_name);
			QLabel * info_label61 = new QLabel(tr("description "));
			QLabel * info_label62 = new QLabel(description);
			QLabel * info_label71 = new QLabel(tr("property_value "));
			QLabel * info_label72 = new QLabel(property_value);
			QLabel * info_label81 = new QLabel(tr("size_x "));
			QLabel * info_label82 = new QLabel(size_x);
			QLabel * info_label91 = new QLabel(tr("size_y "));
			QLabel * info_label92 = new QLabel(size_y);
			QLabel * info_label101 = new QLabel(tr("destroy_recomendation "));
			QLabel * info_label102 = new QLabel(destroy_recomendation);
			QLabel * info_label111 = new QLabel(tr("latitude_wgs_84 "));
			QLabel * info_label112 = new QLabel(latitude_wgs_84);
			QLabel * info_label121 = new QLabel(tr("longitude_wgs_84 "));
			QLabel * info_label122 = new QLabel(longitude_wgs_84);
			QLabel * info_label131 = new QLabel(tr("latitude_sk_42 "));
			QLabel * info_label132 = new QLabel(latitude_sk_42);
			QLabel * info_label141 = new QLabel(tr("longitude_sk_42 "));
			QLabel * info_label142 = new QLabel(longitude_sk_42);
		  //==========размещение объектов=================
			QGridLayout *gridLayout = new QGridLayout;   //
			gridLayout->addWidget(info_label11,0,0);     //
			gridLayout->addWidget(info_label12,0,1);     //
			gridLayout->addWidget(info_label21,1,0);      //
			gridLayout->addWidget(info_label22,1,1);     //
			gridLayout->addWidget(info_label31,2,0);     //
			gridLayout->addWidget(info_label32,2,1);     //
			gridLayout->addWidget(info_label41,3,0);     //
			gridLayout->addWidget(info_label42,3,1);     //
			gridLayout->addWidget(info_label51,4,0);     //
			gridLayout->addWidget(info_label52,4,1);     //
			gridLayout->addWidget(info_label61,5,0);     //
			gridLayout->addWidget(info_label62,5,1);     //
			gridLayout->addWidget(info_label71,6,0);     //
			gridLayout->addWidget(info_label72,6,1);     //
			gridLayout->addWidget(info_label81,7,0);     //
			gridLayout->addWidget(info_label82,7,1);     //
			gridLayout->addWidget(info_label91,8,0);     //
			gridLayout->addWidget(info_label92,8,1);     //
			gridLayout->addWidget(info_label101,9,0);     //
			gridLayout->addWidget(info_label102,9,1);     //
			gridLayout->addWidget(info_label111,10,0);     //
			gridLayout->addWidget(info_label112,10,1);     //
			gridLayout->addWidget(info_label121,11,0);     //
			gridLayout->addWidget(info_label122,11,1);     //
			gridLayout->addWidget(info_label131,12,0);     //
			gridLayout->addWidget(info_label132,12,1);     //
			gridLayout->addWidget(info_label141,13,0);     //
			gridLayout->addWidget(info_label142,13,1);     //
			info_dialog->setLayout(gridLayout);			   //
		  //==============================================
			info_dialog->exec();
		}
	}

}
//отобразить все объекты на карте
void MapView::selectAllObject()
{
	QSqlQuery query;
	long int a11111=0;
	QString str=QString("SELECT name_ls, coordinates.x_coordinates, coordinates.y_coordinates, type_ls.excode_type_ls, ls.short_name_ls, id_ls \
						FROM ls, coordinates, type_ls WHERE ls.id_coordinates=coordinates.id_coordinates AND ls.id_type_ls=type_ls.id_type_ls\
						AND type_ls.excode_type_ls <> '' AND coordinates.x_coordinates<>0 AND coordinates.y_coordinates<>0 AND ls.short_name_ls <> ''");
	if(query.exec(str))
	{
		while (query.next())
		{
			QString Name_ls = query.value(0).toString();
			long int X_coord=query.value(1).toInt();
			long int Y_coord=query.value(2).toInt();
			QString kodeX = query.value(3).toString();
			QString ShotName_ls = query.value(4).toString();
			QString ID_ls = query.value(5).toString();


			if( query.value(1).isNull() || query.value(2).isNull() || query.value(3).isNull() || query.value(4).isNull())
			{
				int ret = QMessageBox::warning (this, tr("My Application"),  Name_ls,
                           QMessageBox::Save | QMessageBox::Discard
                           | QMessageBox::Cancel,
                           QMessageBox::Save);
			}
			else
			{	
     			
	
			QMap<int,QString> semantic_map;
			semantic_map[105]=ShotName_ls;
			semantic_map[19]=ShotName_ls;
			
			semantic_map[32812]="ls";
			semantic_map[32822]=ID_ls;
			

			//	a11111 = mapwin->createObject(hSite,X_coord,Y_coord,kodeX.toLocal8Bit().data(),1, ShotName_ls.toLocal8Bit().data());
				a11111 = mapwin->createObject(hSite,X_coord,Y_coord,kodeX.toLocal8Bit().data(),1, semantic_map);
			//	a11111 = mapwin->createObject(hSite,X_coord,Y_coord,kodeX.toLocal8Bit().data(),1);

			}
		}
		query.clear();
	}
	else
	{
		int ret = QMessageBox::warning(this, tr("My Application"),
                           tr("Bla-Bla-Bla!!!.\n"
                              "Do you want to save your changes?"),
                           QMessageBox::Save | QMessageBox::Discard
                           | QMessageBox::Cancel,
                           QMessageBox::Save);
	}


 //   long int	qqq = mapwin->createObject(hSite,X_coord,Y_coord,kodeX.toLocal8Bit().data(),1, semantic_map);//
//	                                      (long int hSit, double x, double y, const char * name_ff, long int id_obj, QMap<int,QString> semantic_map)


	
}
//========================================================================================================================================================
void MapView::selectAllObject1()
{
QSqlQuery query;
	long int a11111=0;
	QString str=QString("SELECT name_type_mpo_pso, coordinates.x_coordinates, coordinates.y_coordinates, type_mpo_pso.excode_type_mpo_pso, id_mpo_pso, mpo_pso.semantika_digit1, mpo_pso.semantika_digit2, semantika_1 FROM mpo_pso, coordinates, type_mpo_pso\
						WHERE mpo_pso.id_coordinates=coordinates.id_coordinates AND mpo_pso.id_type_mpo_pso=type_mpo_pso.id_type_mpo_pso \
						AND type_mpo_pso.excode_type_mpo_pso <> '' AND coordinates.x_coordinates<>0 AND coordinates.y_coordinates<>0 AND excode_type_mpo_pso  IN ('V0000169007' , 'V0000169029')");
	if(query.exec(str))
	{
		while (query.next())
		{
			QString Name_mpo_pso = query.value(0).toString();
			long int X_coord=query.value(1).toInt();
			long int Y_coord=query.value(2).toInt();
			QString kodeX = query.value(3).toString();
	
			QString ID_mpo_pso = query.value(4).toString();
			// дальность (радиус) действия, километры
			QString semantika_digit1_mpo_pso = query.value(5).toString();
			//float sem1=query.value(5).toDouble();
			// угол (направление) относительно горизонта против часовой стрелки, градусы
			QString semantika_digit2_mpo_pso = query.value(6).toString();

			QString semantika_1_mpo_pso = query.value(7).toString();


			if( query.value(1).isNull() || query.value(2).isNull() || query.value(3).isNull())
			{
				int ret = QMessageBox::warning (this, tr("My Application"),  Name_mpo_pso,
                           QMessageBox::Save | QMessageBox::Discard
                           | QMessageBox::Cancel,
                           QMessageBox::Save);
			}
			else
			{	
     			
	
			QMap<int,QString> semantic_map;
			semantic_map[32812]="mpo_pso";
			semantic_map[32822]=ID_mpo_pso;

			semantic_map[18]=semantika_digit1_mpo_pso;	// иногда это наполнение значка (в тех случаях, когда не "дальность")
			semantic_map[19]=semantika_1_mpo_pso;	// подпись значка

			semantic_map[32811]=semantika_digit1_mpo_pso;	//дальность действия средства
			semantic_map[32852]=semantika_digit2_mpo_pso;	//направление (угол) действия средства

				a11111 = mapwin->createObject(hSite,X_coord,Y_coord,kodeX.toLocal8Bit().data(),1, semantic_map);
			//	a11111 = mapwin->createObject(hSite,X_coord,Y_coord,kodeX.toLocal8Bit().data(),1);

			}
		}
		query.clear();
	}
	else
	{
		int ret = QMessageBox::warning(this, tr("My Application"),
                           tr("Bla-Bla-Bla!!!.\n"
                              "Do you want to save your changes?"),
                           QMessageBox::Save | QMessageBox::Discard
                           | QMessageBox::Cancel,
                           QMessageBox::Save);
	}
	





	QSqlQuery query1;
	
	QString str1=QString("SELECT name_type_mpo_pso, coordinates.x_coordinates, coordinates.y_coordinates, type_mpo_pso.excode_type_mpo_pso, id_mpo_pso, mpo_pso.semantika_digit1, mpo_pso.semantika_digit2, semantika_1 FROM mpo_pso, coordinates, type_mpo_pso\
						WHERE mpo_pso.id_coordinates=coordinates.id_coordinates AND mpo_pso.id_type_mpo_pso=type_mpo_pso.id_type_mpo_pso \
						AND type_mpo_pso.excode_type_mpo_pso <> '' AND coordinates.x_coordinates<>0 AND coordinates.y_coordinates<>0 AND excode_type_mpo_pso NOT IN ('V0000169007', 'V0000169029')");
	if(query1.exec(str1))
	{
		while (query1.next())
		{
			QString Name_mpo_pso = query1.value(0).toString();
			long int X_coord=query1.value(1).toInt();
			long int Y_coord=query1.value(2).toInt();
			QString kodeX = query1.value(3).toString();
	
			QString ID_mpo_pso = query1.value(4).toString();
			// дальность (радиус) действия, километры
			QString semantika_digit1_mpo_pso = query1.value(5).toString();
			//float sem1=query.value(5).toDouble();
			// угол (направление) относительно горизонта против часовой стрелки, градусы
			QString semantika_digit2_mpo_pso = query1.value(6).toString();

			QString semantika_1_mpo_pso = query1.value(7).toString();


			if( query1.value(1).isNull() || query1.value(2).isNull() || query1.value(3).isNull())
			{
				int ret = QMessageBox::warning (this, tr("My Application"),  Name_mpo_pso,
                           QMessageBox::Save | QMessageBox::Discard
                           | QMessageBox::Cancel,
                           QMessageBox::Save);
			}
			else
			{	
     			
	
			QMap<int,QString> semantic_map;
			semantic_map[32812]="mpo_pso";
			semantic_map[32822]=ID_mpo_pso;

			semantic_map[18]=semantika_digit1_mpo_pso;	// иногда это наполнение значка (в тех случаях, когда не "дальность")
			semantic_map[19]=semantika_1_mpo_pso;	// подпись значка

			semantic_map[32811]=semantika_digit1_mpo_pso;	//дальность действия средства
			semantic_map[32852]=semantika_digit2_mpo_pso;	//направление (угол) действия средства

				a11111 = mapwin->createObject(hSite,X_coord,Y_coord,kodeX.toLocal8Bit().data(),1, semantic_map);
			//	a11111 = mapwin->createObject(hSite,X_coord,Y_coord,kodeX.toLocal8Bit().data(),1);

			}
		}
		query1.clear();
	}
	else
	{
		int ret = QMessageBox::warning(this, tr("My Application"),
                           tr("Bla-Bla-Bla!!!.\n"
                              "Do you want to save your changes?"),
                           QMessageBox::Save | QMessageBox::Discard
                           | QMessageBox::Cancel,
                           QMessageBox::Save);
	}

}

void MapView::selectAllObject2()
{	
	QSqlQuery query;
	long int a11111=0;
	QString str=QString("SELECT special_conditions.name_special_conditions, special_conditions.semantika_1, \
						special_conditions.semantika_2 , type_special_conditions.excode_type_sc, \
						coordinates.x_coordinates, coordinates.y_coordinates, special_conditions.id_special_conditions FROM special_conditions, \
						region, type_special_conditions, coordinates, coord_spec_cond \
						where special_conditions.id_region=region.id_region AND \
						special_conditions.id_type_special_conditions=type_special_conditions.id_type_special_conditions \
						AND coord_spec_cond.id_special_conditions=special_conditions.id_special_conditions \
						AND coord_spec_cond.id_coordinates=coordinates.id_coordinates");
	if(query.exec(str))
	{
		while (query.next())
		{
			QString Name_spec_cond = query.value(0).toString();
			long int X_coord=query.value(4).toInt();
			long int Y_coord=query.value(5).toInt();
			QString kodeX = query.value(3).toString();
	
			QString Sem_1_spec_cond = query.value(1).toString();
			QString Sem_2_spec_cond = query.value(2).toString();
			QString ID_special_conditions = query.value(6).toString();
			
			if( query.value(3).isNull() || query.value(4).isNull() || query.value(5).isNull())
			{
				int ret = QMessageBox::warning (this, tr("My Application"),  Name_spec_cond,
                           QMessageBox::Save | QMessageBox::Discard
                           | QMessageBox::Cancel,
                           QMessageBox::Save);
			}
			else
			{
	
				QMap<int,QString> semantic_map;
				semantic_map[17]=Sem_1_spec_cond;
				semantic_map[19]=Sem_2_spec_cond;

				semantic_map[32812]="special_conditions";
				semantic_map[32822]=ID_special_conditions;
			

				a11111 = mapwin->createObject(hSite,X_coord,Y_coord,kodeX.toLocal8Bit().data(),1, semantic_map);
			//	a11111 = mapwin->createObject(hSite,X_coord,Y_coord,kodeX.toLocal8Bit().data(),1);

			}
		}
		query.clear();
	}
	else
	{
		int ret = QMessageBox::warning(this, tr("My Application"),
                           tr("Bla-Bla-Bla!!!.\n"
                              "Do you want to save your changes?"),
                           QMessageBox::Save | QMessageBox::Discard
                           | QMessageBox::Cancel,
                           QMessageBox::Save);
	}

}
void MapView::selectLineObject()
{
	long int x = 6470096, y = 4807576, id_obj = 1;
	QString name_ff = "L1000000002";
	long int a1 = mapwin->createLineObject(hSite, x, y, name_ff.toLocal8Bit().data(), id_obj);
}
void MapView::errors_message(QString str)
{
	QMessageBox error;
	QMessageBox::warning(this, tr("Warning!!!"),str,
		QMessageBox::Ok);
}

void MapView::test_view_manage()
{
	QMap<QString,QString> obj_data;
	QMap<QString,QString>::iterator i;
	obj_data = model->get_obj_info(9);

	QTableWidget *table = new QTableWidget();
	
	int count = obj_data.size();
	table->setColumnCount(2);
	int row = 0;
	
	for (i = obj_data.begin(); i != obj_data.end(); ++i)
	{
		table->insertRow(row);
		
		QTableWidgetItem *item = new QTableWidgetItem(i.key());
		table->setItem(row,0,item);
            
		item = new QTableWidgetItem(i.value());
		table->setItem(row,1,item);
	}
	QDialog *dlg = new QDialog;
	QVBoxLayout *lay = new QVBoxLayout;
	lay->addWidget(table);
	dlg->setLayout(lay);
	dlg->exec();
}
void MapView::test_view_manage2()
{
    QMap<QString,QMap<QString,QString> > obj_data2;
    QMap<QString,QMap<QString,QString> >::iterator i;
	obj_data2 = model->get_all_objects_info(0,0,7000000,7000000);
	QMap<QString,QString> data_k;
	QMap<QString,QString>::iterator k;
	
	QTableWidget *table = new QTableWidget();
	
	int count = obj_data2.size();
	table->setColumnCount(3);
	int row = 0;
	
	for (i = obj_data2.begin(); i != obj_data2.end(); ++i)
	{
		data_k = i.value();
		k = data_k.begin();
		table->insertRow(row);
		
		QTableWidgetItem *item = new QTableWidgetItem(i.key());
		table->setItem(row,0,item);
       
		int j = 0;
		for ( k = data_k.begin(); j != 2, k != data_k.end(); ++j, ++k)
			{
				item = new QTableWidgetItem(k.key());
				table->setItem(row,1,item);
				item = new QTableWidgetItem(k.value());
				table->setItem(row,2,item);
				row++;
				table->insertRow(row);

			}
		row++;//можно убрать
	}
	QDialog *dlg = new QDialog;
	QVBoxLayout *lay = new QVBoxLayout;
	lay->addWidget(table);
	dlg->setLayout(lay);
	dlg->exec();
}



//меню по клику правой клавишей мыши

void MapView::mouseRightMenu(HOBJ hobj, long int num_obj, long int id_obj, QPoint pe, bool reg)
{    
	this->hobj = hobj;
	this->num_obj = num_obj;
	this->id_obj = id_obj;
	this->pe_menu = pe;
	this->region = reg;
		
	mouse_menu = new QMenu(this); 
	QAction *great_scale_act = new QAction("Увеличить масштаб карты  \">\"", this);
	QAction *less_scale_act = new QAction("Уменьшить масштаб карты  \"<\"", this);

	mouse_menu->addAction(great_scale_act); 
	connect(great_scale_act, SIGNAL(triggered()), this, SLOT(GreateScale()));
	mouse_menu->addAction(less_scale_act); 
	connect(less_scale_act, SIGNAL(triggered()), this, SLOT(LessScale()));
	//mouse_menu->addAction("&DeleteObject");  
	
	 if (region)
	 {
		QAction *regionMPOact = new QAction("Рассчитать уровень МПО", this);
		connect(regionMPOact, SIGNAL(triggered()), this, SLOT(regionMPOLevel()));
		mouse_menu->addAction(regionMPOact); 
	 }
	 else
	 {
		QAction *formationDamageAct = new QAction("Рассчитать психогенные потери", this);
		connect(formationDamageAct, SIGNAL(triggered()), this, SLOT(formatonDamage()));
		mouse_menu->addAction(formationDamageAct); 
	 }

	 mouse_menu->exec(pe_menu);
}



//-- Расчетная задача "МПО региона" ---
void MapView::regionMPOLevel()
{
	int id_region;
	QString name_region;
	QSqlQuery query;	
		query.exec(QString("SELECT name_region, id_region from region where excode_region = '%1'").arg(num_obj));
		while (query.next())
			{ 
				name_region = query.value(0).toString();
				id_region = query.value(1).toInt();
			}

		float rez = calc1.get_Rez_on_id_region(id_region);
			
		QString str_id_obj;
			if(rez > 0 && rez < 0.3)
				str_id_obj="1";
			if(rez >= 0.3 && rez < 0.5)
				str_id_obj="2";
			if(rez >=0.5 && rez <= 0.8)
				str_id_obj="3";
			if(rez == 0)
				str_id_obj="4";

	mapwin->RegionAppendSemantic(hobj, str_id_obj);


	QMessageBox *wgt = new QMessageBox;
	wgt->setWindowTitle("Уровень МПОб региона");
	QString text = name_region + ": ";
	text.append(QString::number(calc1.get_Rez_on_id_region(id_region)));
	wgt->setText(text);
	wgt->show();

}


//-- Расчетная задача "Психогенные потери формирования" ---
void MapView::formatonDamage()
{
	QSqlQuery query;
	QString Name_ls;
	QString str=QString("SELECT name_ls, enimy_ls, counte_ls FROM ls WHERE id_ls=%1").arg(id_obj);
		if(query.exec(str))
		{
				while (query.next())
				{
					Name_ls = query.value(0).toString();
									
				}
				query.clear();
			
		}
	People_Losses pl;
	pl.get_losses(id_obj);
	QMessageBox *wgt = new QMessageBox;
	wgt->setWindowTitle("Психогенные потери");
	
	wgt->setText(QString("Потери л/с  %9 - средние %1 - %2; \n%3 - %4\n%5 - %6\n%7 - %8").arg(pl.min[1][0]).arg(pl.max[1][0]).arg(pl.min[1][1]).arg(pl.max[1][1]).arg(pl.min[1][2]).arg(pl.max[1][2]).arg(pl.min[1][3]).arg(pl.max[1][3]).arg(Name_ls));
	
	//leb->setText(QString("Потери л/с  %9 - средние %1 - %2; \n%3 - %4\n%5 - %6\n%7 - %8").arg(pl.min[1][0]).arg(pl.max[1][0]).arg(pl.min[1][1]).arg(pl.max[1][1]).arg(pl.min[1][2]).arg(pl.max[1][2]).arg(pl.min[1][3]).arg(pl.max[1][3]).arg(Name_ls));
	//QHBoxLayout *lo = new QHBoxLayout();
	
	//lo->addWidget(leb);
	//wgt->setLayout(lo);
	
	wgt->show();
}

//меню по клику правой клавишей мыши в любом месте
void	 MapView::mouseRightSimpleMenu(QPoint pe)
{
		mouse_menu = new QMenu(this); 
		QAction *great_scale_act = new QAction("Увеличить масштаб карты  \">\"", this);
		QAction *less_scale_act = new QAction("Уменьшить масштаб карты  \"<\"", this);

			mouse_menu->addAction(great_scale_act); 
			connect(great_scale_act, SIGNAL(triggered()), this, SLOT(GreateScale()));

			mouse_menu->addAction(less_scale_act); 
			connect(less_scale_act, SIGNAL(triggered()), this, SLOT(LessScale()));
		//mouse_menu->addAction("&DeleteObject");  
		mouse_menu->exec(pe);
}		

void	MapView::showShortInformationObject(long int id_obj)
{
			short_info_dialog = new QDialog;
			short_info_dialog ->setWindowTitle("Информация о регионе");
			QLabel * info_label1 = new QLabel("АЭС");
			QLabel * info_label2 = new QLabel("Германия");
			QLabel * info_label30 = new QLabel("X = ");
			QLabel * info_label31 = new QLabel("435433443");
			QLabel * info_label40 = new QLabel("Y = ");
			QLabel * info_label41 = new QLabel("535433443");
			QLabel * info_label50 = new QLabel("3");
			QLabel * info_label51 = new QLabel(" точки прицеливания");

		  //==========размещение объектов=================
			QHBoxLayout *hbox1_layout = new QHBoxLayout;   //
			hbox1_layout->addStretch();
			hbox1_layout->addWidget(info_label1);			//
			hbox1_layout->addStretch();
			//-----
			QHBoxLayout *hbox2_layout = new QHBoxLayout;   //
			hbox2_layout->addWidget(info_label30);			//
			hbox2_layout->addWidget(info_label31);			
			//-----
			QHBoxLayout *hbox3_layout = new QHBoxLayout;   //
			hbox3_layout->addWidget(info_label40);			//
			hbox3_layout->addWidget(info_label41);			
 			//-----
			QHBoxLayout *hbox4_layout = new QHBoxLayout;   //
			hbox4_layout->addWidget(info_label50);			//
			hbox4_layout->addWidget(info_label51);			
 			//-----
			QHBoxLayout *hbox5_layout = new QHBoxLayout;   
			//-----
			QVBoxLayout *vbox_layout = new QVBoxLayout;   //
			vbox_layout->addLayout(hbox1_layout);
			vbox_layout->addWidget(info_label2);
			vbox_layout->addLayout(hbox2_layout);
			vbox_layout->addLayout(hbox3_layout);
			vbox_layout->addLayout(hbox4_layout);
			//vbox_layout->addLayout(hbox5_layout);
			short_info_dialog->setLayout(vbox_layout);			   //
		  //==============================================
			short_info_dialog->exec();
}


void	MapView::showInformationDialog(QString information)
{
	create_object_dialog = new QDialog;
	create_object_dialog ->setWindowTitle("Информация");
	QLabel * info_label1 = new QLabel(information);
	QHBoxLayout *hbox1_layout = new QHBoxLayout;  
	hbox1_layout->addStretch();
	hbox1_layout->addWidget(info_label1);
	hbox1_layout->addStretch();
	create_object_dialog->setLayout(hbox1_layout);		
	create_object_dialog->exec();
}
void	MapView::changeBrihgtUp()
{
	long int bright = mapwin->getmapBright();
	if (bright<16)
	{
		bright++;
		mapwin->setmapBright(bright);
		
	}
	else
	showInformationDialog("Яркость максимальная");
}
void	MapView::changeBrihgtDown()
{
	long int bright = mapwin->getmapBright();
	if (bright>-16)
	{
		bright--;
		mapwin->setmapBright(bright);
		
	}
	else
	showInformationDialog("Яркость минимальная");
}

void	 MapView::appendSit()
{
	QString name = QFileDialog::getOpenFileName(this, QString::null, QString::null, "Sites (*.sit)" );
	long int a = mapwin->appendData(name.toLocal8Bit().data());
	hSite1 = a;
	
}

void MapView::showShortInformationObject_2arg(QString a,long int id_obj)
{
//	calculating_mps calc;
//	calc.calculating(

	short_info_dialog = new QDialog;
	short_info_dialog->setWindowTitle("Информация об объекте");
	
	
	
	QSqlQuery query;
	QString Name_ls;
	QString Enimy;
	QString Name_mpo_pso;
	QString Name_region;
	QString Pict;
	QLabel * info_label1;

	if (a=="region")
	{
// Запрос информации о регионе из базы

		//QString str=QString("SELECT name_region FROM region WHERE excode_region=%1").arg(id_obj);
		//if(query.exec(str))
		//	{
		//		while (query.next())
		//		{
		//			Name_region = query.value(0).toString();
		//			/*long int X_coord=query.value(1).toInt();
		//			long int Y_coord=query.value(2).toInt();
		//			QString kodeX = query.value(3).toString();*/
		//		}
		//		query.clear();
		//	}


	/*name_region_edit = new QLineEdit();
	type_region_edit = new QLineEdit();
	description_region_edit = new QLineEdit();
	counte_population_edit = new QLineEdit();
	density_population_edit = new QLineEdit();
	emmigration_population_edit = new QLineEdit();
	immigration_population_edit = new QLineEdit();
	birth_population_edit = new QLineEdit();
	dead_population_edit = new QLineEdit();
	name_nations_edit = new QLineEdit();*/

		int id_region;
		QString Name_region;
		QString type_region_string;
		QString description_region_string;
		QString counte_population_string;
		QString density_population_string;
		QString emmigration_population_string;
		QString immigration_population_string;
		QString birth_population_string;
		QString dead_population_string;


		
	QSqlQuery query;
	QString str = QString("SELECT id_region, name_region, type_region, description_region, counte_population, density_population, emmigration_population, immigration_population, birth_population, dead_population FROM region WHERE excode_region = %1").arg(id_obj);
	query.exec(str);

	QSqlRecord data = query.record();

	while(query.next())
	{
		id_region =  query.value(data.indexOf("id_region")).toInt();
		Name_region = query.value(data.indexOf("name_region")).toString();
		type_region_string = query.value(data.indexOf("type_region")).toString();
		counte_population_string = query.value(data.indexOf("counte_population")).toString();
		density_population_string = query.value(data.indexOf("density_population")).toString();
		emmigration_population_string = query.value(data.indexOf("emmigration_population")).toString();
		immigration_population_string = query.value(data.indexOf("immigration_population")).toString();
		birth_population_string = query.value(data.indexOf("birth_population")).toString();
		dead_population_string = query.value(data.indexOf("dead_population")).toString();
	}

		

	/*		Печатные издания*/

	QString	print;
	QString	radio;
	QString	tv;
	QString	internet;
	QString	information_agency;
	QString	oposition_smi;
	QString	extrimism_smi;
	QString	penicitar_group;
	QString	flag_country;

	int id_type_smi=1;
		int b;

	QSqlQuery query1;
	QString str1 = QString("SELECT count(smi_region.id_smi) FROM smi_region, smi WHERE smi_region.id_region= %1  AND smi.id_smi=smi_region.id_smi AND smi.id_type_smi= %2").arg(id_region).arg(id_type_smi);
	query1.exec(str1);
	while (query1.next())
	{
		print = query1.value(0).toString();
	}
	
			/*Радиовещание*/

	id_type_smi=2;
	
	str1 = QString("SELECT count(smi_region.id_smi) FROM smi_region, smi WHERE smi_region.id_region= %1  AND smi.id_smi=smi_region.id_smi AND smi.id_type_smi= %2").arg(id_region).arg(id_type_smi);
	query1.exec(str1);
	while (query1.next())
	{
		radio = query1.value(0).toString();
	}

	/*		Телевидение	*/

	id_type_smi=3;
	
	str1 = QString("SELECT count(smi_region.id_smi) FROM smi_region, smi WHERE smi_region.id_region= %1  AND smi.id_smi=smi_region.id_smi AND smi.id_type_smi= %2").arg(id_region).arg(id_type_smi);
	query1.exec(str1);
	while (query1.next())
	{
		tv = query1.value(0).toString();
	}

	/*		Интернет издания		*/



	id_type_smi=4;
	
	str1 = QString("SELECT count(smi_region.id_smi) FROM smi_region, smi WHERE smi_region.id_region= %1  AND smi.id_smi=smi_region.id_smi AND smi.id_type_smi= %2").arg(id_region).arg(id_type_smi);
	query1.exec(str1);
	while (query1.next())
	{
		internet = query1.value(0).toString();
	}

			/*	Информационные агенства		*/


	id_type_smi=5;
	
	str1 = QString("SELECT count(smi_region.id_smi) FROM smi_region, smi WHERE smi_region.id_region= %1  AND smi.id_smi=smi_region.id_smi AND smi.id_type_smi= %2").arg(id_region).arg(id_type_smi);
	query1.exec(str1);
	while (query1.next())
	{
		information_agency = query1.value(0).toString();
	}

	/*		Оппозиционные СМИ	*/

	int id_position_smi=3;
		
		str1 = QString("SELECT count(smi.id_smi)FROM smi_region,smi WHERE smi_region.id_region = %1 AND smi_region.id_smi = smi.id_smi AND smi.id_position_smi = %2").arg(id_region).arg(id_position_smi);
		query1.exec(str1);
		while (query1.next())
		{
			oposition_smi = query1.value(0).toString();
		}

	/*		Экстримистские СМИ  */

		id_position_smi=5;
		
		str1 = QString("SELECT count(smi.id_smi)FROM smi_region,smi WHERE smi_region.id_region = %1 AND smi_region.id_smi = smi.id_smi AND smi.id_position_smi = %2").arg(id_region).arg(id_position_smi);
		query1.exec(str1);
		while (query1.next())
		{
			extrimism_smi = query1.value(0).toString();
		}

			/*	Пенецитарные учреждения		*/

		int id_sphere_group=6;
		
		str1 = QString("SELECT count(groups.id_groups) FROM groups WHERE groups.id_region = %1 AND groups.id_sphere_groups = %2").arg(id_region).arg(id_sphere_group);
		query1.exec(str1);
		while (query1.next())
		{
			penicitar_group = query1.value(0).toString();
		}


		//str1 = QString("SELECT count(groups.id_groups) FROM groups WHERE groups.id_region = %1 AND groups.id_sphere_groups = %2").arg(id_region).arg(id_sphere_group);
		//query1.exec(str1);
		//while (query1.next())
		//{
		//	penicitar_group = query1.value(0).toString();
		//}



//if (!(id_region == 0))
//	{
//


		/*	Нахождение корневого региона   */

		int parent_region = id_region;

		while (!(parent_region == 0))
		{
			str1 = QString("SELECT id_region, parent_region, id_country FROM region WHERE id_region = %1").arg(parent_region);
			query1.exec(str1);
			while (query1.next())
			{
				id_region = query1.value(0).toInt();
				parent_region = query1.value(1).toInt();
			}
			
		}
		

	//}
		/*	Выбор названия файла флага   */

		str1 = QString("SELECT country.flag FROM region, country WHERE region.id_country =country.id_country AND region.id_region = %1 ").arg(id_region);
			query1.exec(str1);
			while (query1.next())
			{
				flag_country = query1.value(0).toString();
			}


		Pict="C:\\projects\\Saturn_500m\\Saturn\\icons2\\" +flag_country ;


		QLabel *bred = new QLabel(
						"<H2><CENTER>"+Name_region+"</CENTER></H2>"
						"<CENTER><IMG BORDER=\"0\" SRC=\""+Pict+"\"></CENTER>"
						"<OL><LI><H3><B>Тип региона:    "+type_region_string+"</B></H3></LI>"
						"<OL><LI><H3><B>Население</B></H3></LI>"
						"<H4>Численность – "+counte_population_string+"  чел.</H4>"
						"<H4>Плотность – "+density_population_string+"  чел. на км2</H4>"
						"<H4>Уровень рождаемости – "+birth_population_string+"  чел.</H4>"
						"<H4>Уровень смертности – "+dead_population_string+"  чел.</H4>"
						"<H4>Уровень эммиграции – "+emmigration_population_string+"  чел.</H4>"
						"<H4>Уровень иммиграции – "+immigration_population_string+"  чел.</H4>"
						"<LI><H3><B>СМИ</B></H3></LI>"
						"<H3><B>Всего:</B></H3>"
						"<H4>Печатные издания (   "+print+"   )</H4>"
						"<H4>Радиовещание (   "+radio+"   )</H4>"
						"<H4>Телевидение (   "+tv+"   )</H4>"
						"<H4>Интернет издания (   "+internet+"   )</H4>"
						"<H4>Информационные агенства (   "+information_agency+"   )</H4>"
						"<H3><B>Из них:</B></H3>"
						"<H4>Оппозиционные -  (   "+oposition_smi+"   )</H4>"
						"<H4>Экстремистские - (   "+extrimism_smi+"   )</H4>"
						"<LI><H3>Организации</H3></LI>"
						"<H3><B>Всего:</B></H3>"
						"<H4>Пеницитарные -  (   "+penicitar_group+"   )</H4>"
						"</OL>"
						);
						
		bred->setWordWrap(true);				
		QHBoxLayout *lo = new QHBoxLayout();
		lo->addWidget(bred);
		short_info_dialog->setLayout(lo);
		short_info_dialog->exec();

	}

		/*		Воинские формирования    */

	if (a=="ls")
	{
	
			int id_ls1 = id_obj;
// Запрос информации об объекте из базы
		QSqlQuery query1;
		QString counte_ls;
		QString flag_country;
		QString name_country;
		QString name_blok;
		QString emblem_blok;
		QString id_root_ls;
		QString str1;
		int id_region;
		int id_ls;

		int id_country;
		
		QString str=QString("SELECT name_ls, enimy_ls, counte_ls FROM ls WHERE id_ls=%1").arg(id_obj);
		if(query.exec(str))
			{
				while (query.next())
				{
					Name_ls = query.value(0).toString();
					Enimy = query.value(1).toString();
					counte_ls = query.value(2).toString();
					/*long int X_coord=query.value(1).toInt();
					long int Y_coord=query.value(2).toInt();
					QString kodeX = query.value(3).toString();*/
				}
				query.clear();
			}
		
			

		/*	Нахождение корневого формирования   */

		int parent_ls = id_obj;

		 

		while (!parent_ls == 0)
		{
			str1 = QString("SELECT id_ls, parent_ls, id_region, name_ls FROM ls WHERE id_ls = %1").arg(parent_ls);
			query1.exec(str1);
			while (query1.next())
			{
				id_ls = query1.value(0).toInt();
				parent_ls = query1.value(1).toInt();
				id_region = query1.value(2).toInt();
				id_root_ls = query1.value(3).toString();
			}
			
		}
		

		/*	Нахождение корневого региона для формиования   */
	


		
		int parent_region = id_region;

		while (!(parent_region == 0))
		{
			str1 = QString("SELECT id_region, parent_region, id_country FROM region WHERE id_region = %1").arg(parent_region);
			query1.exec(str1);
			while (query1.next())
			{
				id_region = query1.value(0).toInt();
				parent_region = query1.value(1).toInt();

			}
			
		}

			 


		
			/*	Выбор названия файла флага   */

		str1 = QString("SELECT country.flag,  country.name_country, country.id_country FROM region, country WHERE region.id_country =country.id_country AND region.id_region = %1 ").arg(id_region);
			query1.exec(str1);
			while (query1.next())
			{
				flag_country = query1.value(0).toString();
				name_country = query1.value(1).toString();
				id_country = query1.value(2).toInt();


			}

		/*	Выбор названия блока и  файла эблемы блока*/

		str1 = QString("SELECT blok.name_blok, blok.emblem_blok FROM blok , blok_country WHERE blok.id_blok = blok_country.id_blok AND blok_country.id_country = %1 ").arg(id_country);
			query1.exec(str1);
			while (query1.next())
			{
				name_blok = query1.value(0).toString();
				emblem_blok = query1.value(1).toString();
				
			}



/*	Выбор средств формирования(Вооружение)*/
			QString list_mpo_pso = "";

		str1 = QString("SELECT name_mpo_pso FROM mpo_pso WHERE mpo_pso.id_ls = %1 ").arg(id_ls1);
			query1.exec(str1);
			while (query1.next())
			{
				list_mpo_pso = list_mpo_pso + query1.value(0).toString()+"; ";
				
				
			}


	//	list_mpo_pso = list_mpo_pso;


				 Pict="C:\\projects\\Saturn_500m\\Saturn\\icons2\\"+flag_country;

		QString  Pict1 = "C:\\projects\\Saturn_500m\\Saturn\\icons2\\"+emblem_blok;

		/*if (Enimy == "true") Pict="C:\\projects\\Saturn_500m\\Saturn\\icons2\\NATO.jpg";
		else Pict="C:\\projects\\Saturn_500m\\Saturn\\icons2\\всрф.jpg";*/
		QLabel *bred = new QLabel(
						"<H2><CENTER>"+Name_ls+"</CENTER></H2>"
						"<H3><B>Страна:       "+name_country+"</B></H3>"
						"<CENTER><IMG BORDER=\"0\" SRC=\""+Pict+"\"></CENTER>"
						"<H3><B>Блок:        "+name_blok+"</B></H3>"
						"<CENTER><IMG BORDER=\"0\" SRC=\""+Pict1+"\"></CENTER>"

						"<H3><B>Численность - "+counte_ls+"</B></H3>"
						
						"<H3><B>Средства ПсО: "+list_mpo_pso+"</B></H3>"
					
						"<H3>Подчиненность:  "+id_root_ls+"</H3></LI>"
					
					//	"</OL>"
						);
		bred->setWordWrap(true);
		QHBoxLayout *lo = new QHBoxLayout();
		lo->addWidget(bred);
		short_info_dialog->setLayout(lo);
		short_info_dialog->exec();
	//	info_label1 = new QLabel(Name_ls);
	}
	
	
	/*		Средства     */
	
	if (a=="mpo_pso")
	{

		QSqlQuery		query1;			
		QString			counte_mpo_pso;
		QString			description_mpo_pso;
		QString			image_mpo_pso;
		QString			name_org;
		QString			name_type_mpo_pso;
		QString			str1;
		int			id_ls;
		int			id_smi;
		int			id_groups;



// Запрос информации об объекте из базы
		QString str=QString("SELECT mpo_pso.name_mpo_pso, mpo_pso.counte_mpo_pso, mpo_pso.description_mpo_pso, mpo_pso.image_mpo_pso, mpo_pso.id_ls, mpo_pso.id_smi, mpo_pso.id_groups, type_mpo_pso.name_type_mpo_pso FROM mpo_pso , type_mpo_pso WHERE mpo_pso.id_type_mpo_pso = type_mpo_pso.id_type_mpo_pso AND mpo_pso.id_mpo_pso = %1").arg(id_obj);
		if(query.exec(str))
			{
				while (query.next())
				{
					Name_mpo_pso = query.value(0).toString();
					counte_mpo_pso = query.value(1).toString();
					description_mpo_pso = query.value(2).toString();
					image_mpo_pso = query.value(3).toString();
					id_ls = query.value(4).toInt();
					id_smi = query.value(5).toInt();
					id_groups = query.value(6).toInt();
					name_type_mpo_pso = query.value(7).toString();

					/*long int X_coord=query.value(1).toInt();
					long int Y_coord=query.value(2).toInt();
					QString kodeX = query.value(3).toString();*/
				}
				query.clear();
			}

		/*	Определение подчиненности средства			*/


		
		if (!(id_ls == 0))
		{
		str1 = QString("SELECT name_ls FROM ls WHERE id_ls = %1 ").arg(id_ls);
			query1.exec(str1);
			while (query1.next())
			{
				name_org = query1.value(0).toString();
							
			}
		}

		if (!(id_smi == 0))
		{
		str1 = QString("SELECT name_smi FROM smi WHERE id_smi = %1 ").arg(id_smi);
			query1.exec(str1);
			while (query1.next())
			{
				name_org = query1.value(0).toString();
							
			}
		}

		if (!(id_groups == 0))
		{
		str1 = QString("SELECT name_groups FROM groups WHERE id_groups = %1 ").arg(id_groups);
			query1.exec(str1);
			while (query1.next())
			{
				name_org = query1.value(0).toString();
							
			}
		}



		
		QString  Pict = "C:\\projects\\Saturn_500m\\Saturn\\icons2\\"+image_mpo_pso;

		
		QLabel *bred = new QLabel(
						"<H2><CENTER>"+Name_mpo_pso+"</CENTER></H2>"
						"<CENTER><IMG BORDER=\"0\" SRC=\""+Pict+"\"></CENTER>"
						"<H3>Тип объекта:  "+name_type_mpo_pso+"</H3></LI>"
						"<H3>Количество:  "+counte_mpo_pso+"</H3></LI>"
						"<H3>Подчиненность:  "+name_org+"</H3></LI>"
						"<H3><B>Описание:</B></H3>"
						"<H3><B>"+description_mpo_pso+"</B></H3>"

						) ;

	
			bred->setWordWrap(true);
			QHBoxLayout *lo = new QHBoxLayout();
					
			lo->addWidget(bred);
		//	lo->setGeometry(QRect(100,100,100,100));
	
			short_info_dialog->setLayout(lo);

			
			short_info_dialog->exec();


	//	info_label1 = new QLabel(Name_ls);

		/*info_label1 = new QLabel(Name_mpo_pso);*/
		//showInformationDialog(Name_mpo_pso);
	}
  //==========размещение объектов=================
	//QHBoxLayout *hbox1_layout = new QHBoxLayout;   //
	//hbox1_layout->addStretch();
	//hbox1_layout->addWidget(info_label1);			//
	//hbox1_layout->addStretch();
	//		//-----
			//QHBoxLayout *hbox2_layout = new QHBoxLayout;   //
			//hbox2_layout->addWidget(info_label30);			//
			//hbox2_layout->addWidget(info_label31);			
			////-----
			//QHBoxLayout *hbox3_layout = new QHBoxLayout;   //
			//hbox3_layout->addWidget(info_label40);			//
			//hbox3_layout->addWidget(info_label41);			
 		//	//-----
			//QHBoxLayout *hbox4_layout = new QHBoxLayout;   //
			//hbox4_layout->addWidget(info_label50);			//
			//hbox4_layout->addWidget(info_label51);			
 		//	//-----
			//QHBoxLayout *hbox5_layout = new QHBoxLayout;   
			////-----
	//QVBoxLayout *vbox_layout = new QVBoxLayout;   //
	//vbox_layout->addLayout(hbox1_layout);
			//vbox_layout->addWidget(info_label2);
			//vbox_layout->addLayout(hbox2_layout);
			//vbox_layout->addLayout(hbox3_layout);
			//vbox_layout->addLayout(hbox4_layout);
			////vbox_layout->addLayout(hbox5_layout);
//	short_info_dialog->setLayout(vbox_layout);			   //
		  //==============================================
//	short_info_dialog->exec();	

			//	info_label1 = new QLabel(Name_region);
			//	QLabel lbl ("<H1><CENTER>"+Name_ls+"</CENTER></H1>" 
			//			"<H2><CENTER>тип объекта - \"РЕГИОН\"</CENTER></H2>"
			//			"<CENTER><IMG BORDER=\"0\" SRC=\"C:\\projects\\3\\SATURN_10_07\\J0305493.WMF\"></CENTER>"
			//			"<OL><LI><B>Население</B></LI>"
			//			"<H2>Численность – количество чел.</H2>"
			//			"<H2>Национальный состав: % (количество) чел.</H2>"
			//			"<H2>Религиозный состав: % (количество) чел.</H2>"
			//			"<H2>Трудоспособное население:  % (количество) чел.</H2>"
			//			"<LI><B>СМИ</B></LI>"
			//			"<H2><B>Всего:</B></H2>"
			//			"<H2>Печатные издания (количество)</H2>"
			//			"<H2>Радиовещание (__________)</H2>"
			//			"<H2>Телевидение (__________)</H2>"
			//			"<H2>Интернет издания (__________)</H2>"
			//			"<H2><B>Из них:</B></H2>"
			//			"<H2>Оппозиционные -  % количество</H2>"
			//			"<H2>Экстремистские -  % количество</H2>"
			//			"<LI>Организации</LI>"
			//			"<H2><B>Всего:</B></H2>"
			//			"<H2>Пеницитарные -  % количество</H2>"
			//			"</LO>"
			//			);
			//	lbl.show();
			//short_info_dialog->exec();



if (a=="special_conditions")
	{
		QString	Name_special_conditions;
		QString description_special_conditions;
		QString image_special_conditions;


// Запрос информации об объекте из базы
		QString str=QString("SELECT name_special_conditions, description_special_conditions, image_special_conditions FROM special_conditions WHERE id_special_conditions=%1").arg(id_obj);
		if(query.exec(str))
			{
				while (query.next())
				{
				Name_special_conditions = query.value(0).toString();
				description_special_conditions = query.value(1).toString();
				image_special_conditions = query.value(2).toString();

					/*long int X_coord=query.value(1).toInt();
					long int Y_coord=query.value(2).toInt();
					QString kodeX = query.value(3).toString();*/
				}
				query.clear();
			}
		/*info_label1 = new QLabel(Name_mpo_pso);*/
		// showInformationDialog(Name_special_conditions);

	
QString  Pict = "C:\\projects\\Saturn_500m\\Saturn\\icons2\\"+image_special_conditions;

		
		QLabel *bred = new QLabel(
						"<H2><CENTER>"+Name_special_conditions+"</CENTER></H2>"
						"<CENTER><IMG BORDER=\"0\" SRC=\""+Pict+"\"></CENTER>"
						"<H3><B>Описание:</B></H3>"
						"<H3><B>"+description_special_conditions+"</B></H3>"
						);

				bred->setWordWrap(true);
						
				QHBoxLayout *lo = new QHBoxLayout();
				
				
				lo->addWidget(bred);
				
				short_info_dialog->setLayout(lo);
				
				short_info_dialog->exec();

	}




}

void	MapView::calc_mps()
{
	calculating_mps calc;
	QSqlQuery query;
	QString str=QString("SELECT name_ls, coordinates.x_coordinates, coordinates.y_coordinates, type_ls.excode_type_ls, \
							ls.short_name_ls, id_ls \
							FROM ls, coordinates, type_ls WHERE ls.id_coordinates=coordinates.id_coordinates AND \
							ls.id_type_ls=type_ls.id_type_ls	AND type_ls.excode_type_ls <> '' AND \
							coordinates.x_coordinates<>0 AND coordinates.y_coordinates<>0 AND \
							ls.short_name_ls <> '' and enimy_ls=FALSE");

//	QString str=QString("SELECT excode_region, name_region, id_region  FROM region WHERE excode_region<>0"); 
	long int a = mapwin->IsActive(mapwin->hMap);
	if (a)
	{
		if(query.exec(str))
		{
			while (query.next())
			{
				int id_ls = query.value(5).toInt();
				long int X_coord=query.value(1).toInt();
				long int Y_coord=query.value(2).toInt();
				QString excode_type_ls = query.value(3).toString();

				float MPS=calc.calculating(id_ls);
				float MPS_kont=calc.get_mps_kont();
				float MPS_ofec=calc.get_mps_ofec();
				float MPS_priz=calc.get_mps_priz();

				if (MPS!=-1)
				{
					QMap<int,QString> semantic_map;
					// показатель подразделения
					semantic_map[60004]=QString::number(MPS);						
					// принадлежность
					semantic_map[60028]="6";	
					semantic_map[60001]=QString::number(MPS_ofec);				
					semantic_map[60002]=QString::number(MPS_kont);	
					semantic_map[60003]=QString::number(MPS_priz);	

						
					// вид стрелки
					if (MPS < 0.3)					semantic_map[60012]="4";
					if ((MPS >= 0.3)&&(MPS <= 0.7))	semantic_map[60012]="5";
					if ((MPS > 0.7)&&(MPS <= 1.0))	semantic_map[60012]="3";
						
					QString kodeX = "V00000000312";	

					// если значкт формирований - авиа или военно-морские базы, то стрелка знака МПС рисуется левее и ниже
					if ((excode_type_ls=="1311701001")||(excode_type_ls=="0006701004")||(excode_type_ls=="0006701003")||(excode_type_ls=="V0000001118")) mapwin->createObject(hSite,X_coord-3000,Y_coord-6000,kodeX.toLocal8Bit().data(),1, semantic_map);
					// иначе просто левее флажка формирования
					else mapwin->createObject(hSite,X_coord,Y_coord-3000,kodeX.toLocal8Bit().data(),1, semantic_map);
				}
			}
			query.clear();
		}
	}
	showInformationDialog("Информация нанесена на карту");
	return;
}

void	MapView::calc_mps2()
{
	calculating_mps calc;
	QSqlQuery query;
	QString str=QString("SELECT name_ls, coordinates.x_coordinates, coordinates.y_coordinates, type_ls.excode_type_ls, \
							ls.short_name_ls, id_ls \
							FROM ls, coordinates, type_ls WHERE ls.id_coordinates=coordinates.id_coordinates AND \
							ls.id_type_ls=type_ls.id_type_ls	AND type_ls.excode_type_ls <> '' AND \
							coordinates.x_coordinates<>0 AND coordinates.y_coordinates<>0 AND \
							ls.short_name_ls <> '' and enimy_ls=TRUE");

//	QString str=QString("SELECT excode_region, name_region, id_region  FROM region WHERE excode_region<>0"); 
	long int a = mapwin->IsActive(mapwin->hMap);
	if (a)
	{
		if(query.exec(str))
		{
			while (query.next())
			{
				int id_ls = query.value(5).toInt();
				long int X_coord=query.value(1).toInt();
				long int Y_coord=query.value(2).toInt();
				QString excode_type_ls = query.value(3).toString();

				float MPS=calc.calculating(id_ls);

				float MPS_kont=calc.get_mps_kont();
				float MPS_ofec=calc.get_mps_ofec();
				float MPS_priz=calc.get_mps_priz();
				if (MPS!=-1)
				{
					QMap<int,QString> semantic_map;
					// показатель подразделения
					semantic_map[60004]=QString::number(MPS);	
					// принадлежность
					semantic_map[60028]="7";	

					semantic_map[60001]=QString::number(MPS_ofec);				
					semantic_map[60002]=QString::number(MPS_kont);	
					semantic_map[60003]=QString::number(MPS_priz);
					// вид стрелки
					if (MPS < 0.3)					semantic_map[60012]="4";
					if ((MPS >= 0.3)&&(MPS <= 0.7))	semantic_map[60012]="5";
					if ((MPS > 0.7)&&(MPS <= 1.0))	semantic_map[60012]="3";
						
					QString kodeX = "V00000000313";	

					// если значкт формирований - авиа или военно-морские базы, то стрелка знака МПС рисуется правее и ниже
					if ((excode_type_ls=="L1900000115")||(excode_type_ls=="V1234500048")||(excode_type_ls=="V1900000111")||(excode_type_ls=="L1900000052")) mapwin->createObject(hSite,X_coord-3000,Y_coord+6000,kodeX.toLocal8Bit().data(),1, semantic_map);
					// иначе просто правее флажка формирования
					else mapwin->createObject(hSite,X_coord,Y_coord+3000,kodeX.toLocal8Bit().data(),1, semantic_map);
				}
			}
			query.clear();
		}
	}
	showInformationDialog("Информация нанесена на карту");
	return;
}

void	MapView::People_Losse()
{
	/*People_Losses pl;
	pl.get_losses(20);
	QWidget *wgt = new QWidget();
	QLabel *leb = new QLabel();
	leb->setText(QString("Потери л/с БФ- средние %1 - %2;\n%3 - %4\n%5 - %6\n%7 - %8").arg(pl.min[1][0]).arg(pl.max[1][0]).arg(pl.min[1][1]).arg(pl.max[1][1]).arg(pl.min[1][2]).arg(pl.max[1][2]).arg(pl.min[1][3]).arg(pl.max[1][3]));
	QHBoxLayout *lo = new QHBoxLayout();
	lo->addWidget(leb);
	wgt->setLayout(lo);
	wgt->show();*/

calculating_mps calc;
	QSqlQuery query;
	QDate date;
	QString StrDate=date.currentDate().toString("dd.MM.yyyy");
	QString str=QString("SELECT name_ls, coordinates.x_coordinates, coordinates.y_coordinates, type_ls.excode_type_ls, \
							ls.short_name_ls, id_ls \
							FROM ls, coordinates, type_ls WHERE ls.id_coordinates=coordinates.id_coordinates AND \
							ls.id_type_ls=type_ls.id_type_ls	AND type_ls.excode_type_ls <> '' AND \
							coordinates.x_coordinates<>0 AND coordinates.y_coordinates<>0 AND \
							ls.short_name_ls <> '' and enimy_ls=FALSE");

//	QString str=QString("SELECT excode_region, name_region, id_region  FROM region WHERE excode_region<>0"); 
	long int a = mapwin->IsActive(mapwin->hMap);
	if (a)
	{
		if(query.exec(str))
		{
			while (query.next())
			{
				int id_ls = query.value(5).toInt();
				long int X_coord=query.value(1).toInt();
				long int Y_coord=query.value(2).toInt();
				QString excode_type_ls = query.value(3).toString();

				/*float MPS=calc.calculating(id_ls);*/
				People_Losses pl;
				pl.get_losses(id_ls);
				/*float MPS_kont=calc.get_mps_kont();
				float MPS_ofec=calc.get_mps_ofec();
				float MPS_priz=calc.get_mps_priz();*/
				
					QMap<int,QString> semantic_map;
					// показатель подразделения
						

					// принадлежность
					/*semantic_map[60028]="7";	*/
					
					semantic_map[60006]=StrDate;
					semantic_map[60007]=QString::number(pl.max[1][0]);				
					semantic_map[60008]=QString::number(pl.max[1][1]);	
					semantic_map[60009]=QString::number(pl.max[1][2]);
					semantic_map[60010]=QString::number(pl.max[1][3]);
					//// вид стрелки
					//if (MPS < 0.3)					semantic_map[60012]="4";
					//if ((MPS >= 0.3)&&(MPS <= 0.7))	semantic_map[60012]="5";
					//if ((MPS > 0.7)&&(MPS <= 1.0))	semantic_map[60012]="3";
						
					QString kodeX = "V0000000032";	

					// если значек формирований - авиа или военно-морские базы, то стрелка знака МПС рисуется правее и ниже
					if ((excode_type_ls=="1311701001")||(excode_type_ls=="0006701004")||(excode_type_ls=="0006701003")||(excode_type_ls=="V0000001118")) mapwin->createObject(hSite,X_coord+13500,Y_coord-7500,kodeX.toLocal8Bit().data(),1, semantic_map);
					// если значек - большой флаг
					if (excode_type_ls=="V00911000078")  mapwin->createObject(hSite,X_coord+17000,Y_coord-4500,kodeX.toLocal8Bit().data(),1, semantic_map);
					// иначе просто левее флажка формирования
					if ((excode_type_ls!="1311701001")&&(excode_type_ls!="0006701004")&&(excode_type_ls!="0006701003")&&(excode_type_ls!="V0000001118")&&(excode_type_ls!="V00911000078")) mapwin->createObject(hSite,X_coord+17000,Y_coord-4500,kodeX.toLocal8Bit().data(),1, semantic_map);
				
			}
			query.clear();
		}
	}
	showInformationDialog("Информация нанесена на карту");
	return;


}
