#include <QMessageBox>
#include <QPainter>
#include <QTextCodec>
#include <QScrollBar>
#include <QImage>
#include <QPaintEvent>
#include <QMenu>
#include <QtSql>
#include "mapscroll.h"


MapScroll::MapScroll(QWidget * parent)
          :QScrollArea(parent)
{
    MainCodec = QTextCodec::codecForName("CP1251");
	MyViewport = 0;
    hMap = 0;
    LibInst = 0;
	map = new MyMapAccess();
	rect = frameRect();
	//delete
	flag1=0;
	flag2=FALSE;
	flag=FALSE;
}
//-------------------------------------------------------------
// Деструктор
//-------------------------------------------------------------
MapScroll::~MapScroll()
{
  if (hMap)
   {
    map->mapCloseData(hMap);
    hMap = 0;
   }
}
bool		MapScroll::eventFilter (QObject * watched, QEvent * event)
{
  if (event->type() == QEvent::Paint && MyViewport != 0 && MyViewport == watched)
  {
    QPainter p;

    p.begin(MyViewport);
    drawContents(&p, ((QPaintEvent *)event)->rect().left(), ((QPaintEvent *)event)->rect().top(),
                 ((QPaintEvent *)event)->rect().right() - ((QPaintEvent *)event)->rect().left() + 1, 
		 ((QPaintEvent *)event)->rect().bottom() - ((QPaintEvent *)event)->rect().top() + 1);
    p.end();
    return true;
  }
  else return QScrollArea::eventFilter(watched, event);
}

//-------------------------------------------------------------
// Перерисовка окна
//-------------------------------------------------------------
void		MapScroll::drawContents( QPainter* p, int cx, int cy, int cw, int ch)//перерисовка
{
   if (hMap)
   {
      RectDraw.left = cx ;
      RectDraw.top  = cy ;
      RectDraw.right  = cx + cw;
      RectDraw.bottom = cy + ch;
      
	  int dibsize = sizeof(BITMAPINFOHEADER) +
                   (long)sizeof(RGBQUAD)*256L;
 
	  char *lpImage = (char *)::AllocateTheMemory(cw * ch * 4 + dibsize);
      memset(lpImage, 0xff, cw * ch * 4 + dibsize);

	  BITMAPINFOHEADER *lpDibInfo = (BITMAPINFOHEADER *)lpImage;
	  lpDibInfo->biSize          = sizeof(BITMAPINFOHEADER);
      lpDibInfo->biWidth         = cw;
      lpDibInfo->biHeight        = ch;
      lpDibInfo->biPlanes        = 1;
      lpDibInfo->biBitCount      = (unsigned short)(8 * 4);
      lpDibInfo->biCompression   = BI_RGB;
      lpDibInfo->biSizeImage     = (long)cw*(long)ch*4;
      lpDibInfo->biXPelsPerMeter = 0;
      lpDibInfo->biYPelsPerMeter = 0;
      lpDibInfo->biClrUsed       = 256;
      lpDibInfo->biClrImportant  = 0; 
	  char *lpBits = lpImage + dibsize;

	  int ret= map->drawContents(hMap, lpDibInfo, lpBits, &RectDraw);
	  
	  char *string = (char *)AllocateTheMemory(cw * 4);
      for (int i = 0; i < ch/2; i++)
	  {
		char *pointE  = lpBits + (ch - i - 1) * cw * 4;
        char *pointB  = lpBits + i * cw * 4;


		memcpy(string, pointB, cw * 4);
        memcpy(pointB, pointE, cw * 4);
        memcpy(pointE, string, cw * 4);
	  }
      FreeTheMemory(string);

	  QImage img((uchar *) lpBits, cw, ch, QImage::Format_RGB32);	  
      p->drawImage(cx, cy, img, 0, 0, cw, ch);
	  FreeTheMemory(lpImage);
   }
};

//открытие карты
int			MapScroll::mapOpen(  const char *name )
{
  char        drive[10];
  char        dir[MAX_PATH];
  char        namefile[MAX_PATH];
  char        extname[10];
  char        newnamefile[MAX_PATH];
  char        sxfname[MAX_PATH];
  int         length;
  if_create=0;
  if (hMap)
   {
    void (WINAPI * lpmapfn)(int);
    (FARPROC&) lpmapfn = ::GetProcAddress(LibInst,"mapCloseData");
    (*lpmapfn)(hMap);
    hMap = 0;
   }
  
  SplitThePath(name, drive, dir, namefile, extname);

  if ((stricmp(extname,".map") == 0) || (stricmp(extname,".sit") == 0) || 
      (stricmp(extname,".rsw") == 0) || (stricmp(extname,".mtw") == 0))
  {
    hMap=map->mapOpen(name, 0);
  }            
                                                                  
 
   if (hMap)
   {
      long int	mapW, mapH;
	  map->mapGetPictureSize(hMap,&mapW,&mapH);
     
      if (MyViewport == 0)  
      {
        MyViewport = new QWidget(viewport());
		MyViewport->setAttribute(Qt::WA_NoBackground);
        setWidget(MyViewport);
      }
      else MyViewport->show();
      //изменение размеров содержимого
      MyViewport->setGeometry(0, 0, mapW, mapH);

	  setFocus();
      MyViewport->repaint();
  }
  map->mapSetBright(hMap, 5); // ===== установка яркости карты
  if_open_sit = FALSE;
  return 0;
};

void		MapScroll::closeMap()
{
  if (hMap!=0)
  {
	  map->mapCloseData(hMap);
     hMap=0;
    MyViewport->hide();
  }
}

//изменение масштаба
void		MapScroll::ChangeScale(float Change)
{
	if (hMap == 0) return;
	long int X,Y;
	long int mapW, mapH;
	//вычислим текущий центр
	X = horizontalScrollBar()->value() + viewport()->width() / 2;
	Y = verticalScrollBar()->value() + viewport()->height() / 2;
	map->mapChangeViewScale(hMap,&X,&Y,Change);
	map->mapGetPictureSize(hMap,&mapW,&mapH);
		
	MyViewport->hide();
	//изменение размеров содержимого
	MyViewport->resize(mapW, mapH);

	//вычислим новый центр - (!) центр не по мыши, а середина MapScroll (!)
	X = X - viewport()->width() / 2;
	if (X < 0) X = 0;
	Y = Y - viewport()->height() / 2;
	if (Y < 0) Y = 0;

	horizontalScrollBar()->setValue(X);
	verticalScrollBar()->setValue(Y);
	MyViewport->show();
	
	
}

//скроллинг карты по клавишам
void		MapScroll::ChangePos(long int dx,long int dy)
{
  if (hMap == 0) return;
  long int X,Y;

  long int mapW, mapH;
  void (WINAPI * lpmapfn)(HMAP, long int *, long int *);
  (FARPROC&) lpmapfn = ::GetProcAddress(LibInst,"mapGetPictureSize");
  (*lpmapfn)(hMap, &mapW, &mapH);

  X = horizontalScrollBar()->value() + dx;
  Y = verticalScrollBar()->value() + dy;
  if (X > mapW - viewport()->width()) X = mapW - viewport()->width();
  if (Y > mapH - viewport()->height()) Y = mapH - viewport()->height();
  if (X < 0) X = 0;
  if (Y < 0) Y = 0;

  horizontalScrollBar()->setValue(X);
  verticalScrollBar()->setValue(Y);
}


//нажатие клавиш мыши//delete
void		MapScroll::mousePressEvent(QMouseEvent * event)
{
	if (if_open_sit)
	{
		if (!if_create)
		{
			if (event->button())
			{
					pe = event->pos();
					screenX= pe.x(); 
					screenY= pe.y(); 
					screenX+= horizontalScrollBar()->value();
					screenY+= verticalScrollBar()->value();
					map->mapPictureToPlane(hMap, &screenX, &screenY);

				if (event->button() == Qt::LeftButton)//левая клавиша мыши
				{					
					findObject(&screenX, &screenY);

					//QString b = map-> objectName();//  mapClearObject(info);

			/*		int ret = QMessageBox::warning (this, tr("My Application"),  b,
                           QMessageBox::Save | QMessageBox::Discard
                           | QMessageBox::Cancel,
                           QMessageBox::Save);
*/			
				}

				if (event->button() == Qt::RightButton)   //Правая клавиша мыши
				{

				
					findObject2(&screenX, &screenY);
				//	long int hobj = getHobj(&screenX, &screenY);


		//			
		//long int a1=0;
		//long int g3=32822;
		//char value3[32];

		//a1 = map->mapSemanticCodeValue(hobj, g3, value3, 32, 1);


		//long int id_obj = atoi(value3);





		//			long int id_odject = findObject1(&screenX, &screenY);
		//			pe = event->globalPos();
		//			/*if (flag2)
		//			{
		//				
		//				if (flag)
		//				{*/
		//		

		//					emit signal_for_right_button(id_odject, pe);




						/*}
						flag=FALSE;
					}*/
				}
			}
		}
	}
}

//поиск объекта по щелчку левой клавишей мыши (**** видимо клик только на слое с нанесенными объектами НЕ ПОЛЬЗОВАТЕЛЬСКИЙ слой с регионами!!! ***)
void		MapScroll::findObject(double *x, double *y)
{	
	select = map->mapCreateMapSelectContext(hMap);//создать условия поиска
	info=map->mapCreateObject(hMap);
	changeFrame();
	info=map->mapWhatObject(hMap,info,&frame,WO_LAST,PP_PLANE);
	IsObject(info);





	double x1,y1;
	x1=0;
	y1=0;
	map->mapPictureToPlane(hMap, &x1, &y1);
//	HWND hwnd = map->mapGetHandleForEvent(hMap);

	long int a1 = map->mapClearObject(info);
	map->mapDeleteSelectContext(select);//удалить условия поиска
}





long int		MapScroll::findObject1(double *x, double *y)
{	
	select = map->mapCreateMapSelectContext(hMap);//создать условия поиска
	info=map->mapCreateObject(hMap);
	changeFrame();
	info=map->mapWhatObject(hMap,info,&frame,WO_LAST,PP_PLANE);
	long int sss = IsObject1(info);

	double x1,y1;
	x1=0;
	y1=0;
	map->mapPictureToPlane(hMap, &x1, &y1);
//	HWND hwnd = map->mapGetHandleForEvent(hMap);

	long int a1 = map->mapClearObject(info);
	map->mapDeleteSelectContext(select);//удалить условия поиска
return sss;

}

	
				/*	Правая кнопка мыши!				*/

void		MapScroll::findObject2(double *x, double *y)
{	
	select = map->mapCreateMapSelectContext(hMap);//создать условия поиска
	info=map->mapCreateObject(hMap);
	changeFrame();
	info=map->mapWhatObject(hMap,info,&frame,WO_LAST,PP_PLANE);
	long int num_obj = IsObject1(info);


		bool region = false;			
		long int a=0, b=0, c=0;
		long int g1=60011;	//  для РЕГИОНОВ!!!
		long int g2=32812;	//	ls - формирования,	mpo_pso - средства
		long int g3=32822;	//	ID - объекта в таблице (ls или	mpo_pso)
		char value[32];
		char value2[32];
		char value3[32];
		
		c = map->mapSemanticCodeValue(info, g2, value2, 32, 1);
		
		if (!((QString(value2)=="mpo_pso")||(QString(value2)=="special_conditions")))
		{
		a = map->mapSemanticCodeValue(info, g3, value3, 32, 1);


		long int id_obj = atoi(value3);



		b = map->mapSemanticCodeValue(info, g1, value, 32, 1);
		
		

		// только для РЕГИОНОВ!!!				   
		if (b!=0) 
		{
			
		region = true;

		}

			if ((b!=0)||(a!=0))
			{

				//	long int id_odject = findObject1(&screenX, &screenY);

				//	pe = event->globalPos();
				
			emit signal_for_right_button(info, num_obj, id_obj,  pe, region);
			}
		

		}


	double x1,y1;
	x1=0;
	y1=0;
	map->mapPictureToPlane(hMap, &x1, &y1);
//	HWND hwnd = map->mapGetHandleForEvent(hMap);

	long int a1 = map->mapClearObject(info);
	map->mapDeleteSelectContext(select);//удалить условия поиска
return;

}


long int	MapScroll::IsObject1(HOBJ hobj)
{
	
	//запрос количества возможных для редактирования семантик для данного объекта
		long int g1 = map->mapAvailableSemanticCount(hobj);
	
		char value[32];
		long int a1=0;
//		g1=17501;
		g1=60011;
		// получаем номер объекта
		long int ff = map->mapObjectKey(hobj);\

		long int ff1 = map->mapObjectKey(hobj);
		
		


		//g1=20;
			a1 = map->mapSemanticCodeValue(hobj, g1, value, 32, 1);
			int k=0, id_odject=0, step=1;


			for (int i=0; i<32; i++)
		{
			if ((int)value[i]!=0)
				k++;
			else break;
		}
		for (int i=k-1; i>=0; i--)
		{
			id_odject = id_odject + abs(((int)value[i]-48))*step;
			step*=10;
		}

		if (a1!=0) 
		{


		}

		return ff1;
}





































long int	MapScroll::getHobj(double *x, double *y)
{
	info=map->mapCreateObject(hMap);
	changeFrame();
	info=map->mapWhatObject(hMap,info,&frame,WO_LAST,PP_PLANE);
	long int g1 = map->mapAvailableSemanticCount(info);
			g1 = map->mapAvailableSemanticCode(info,g1);//код последней доступной семантики
		char value[32];
		long int a1=0,a2=0;
		g1=17501;
			a1 = map->mapSemanticCodeValue(info, g1, value, 32, 1);
			int k=0, step=1;
		for (int i=0; i<32; i++)
		{
			if ((int)value[i]!=0)
				k++;
			else break;
		}
		for (int i=k-1; i>=0; i--)
		{
			a2 = a2 + abs(((int)value[i]-48))*step;
			step*=10;
		}
		if (a1!=0) 
		{
			if (a2!=0)
			{
				flag=TRUE;
			}
			long int a1 = map->mapClearObject(info);
		}
		return a2;
}
void		MapScroll::changeFrame()
{
	dframe.X1 = screenX+1;
	dframe.X2 = screenX-1;
	dframe.Y1 = screenY+1;
	dframe.Y2 = screenY-1;
	frame = dframe;
}

//создание пользовательского слоя
HSITE		MapScroll::dataOpen(HMAP hMap, const char * mapname, const char * rscname, CREATESITE * createsite)
{
	if (!if_open_sit)
	{
	if_open_sit = TRUE;
	hSite = map->mapCreateAndAppendSite(hMap,mapname,rscname,createsite);
	}
	return  map->mapOpenSiteForMap(hMap,mapname,0);
}
//закрытие пользовательского слоя
void		MapScroll::dataClose(HMAP hMap, HSITE hSite)
{
	if (if_open_sit)
	{
		map->mapCloseSiteForMap(hMap,hSite);
	}
		if_open_sit = FALSE;
		flag2=0;
}

//добавление существующего пользовательского слоя
long int	MapScroll::appendData(const char* sitname)
{
	if (hMap)
	{
		long int a;
		a = map->mapAppendData(hMap, sitname);
		return a;
	}
	return 0;
}
//СОЗДАНИЕ ОБЪЕКТА!! из базы
long int	MapScroll::createObject(long int hSit, double x, double y, const char * name_ff, long int id_obj)
{
	//QString str_id_obj = QString::number(id_obj);
	//info = map->mapCreateSiteObject(hMap,hSit);
	//long int a21 = map->mapAppendSemantic(info, 17501, str_id_obj.toLocal8Bit().data(), 18);

	//long int a1 = map->mapRegisterObjectByKey(info, name_ff);
	//long int a24 = map->mapAppendPointPlane(info, x, y);

	//long int a3 = objectTopScale(info);//задать max масштаб отображения
	//long int a4 = objectByMap(hMap,info);
	////сохранить данные об объекте
	//a24 = map->mapCommitWithPlace(info);
	//map->mapAppendPointPlane(info, x,y+100);
	//map->mapClearObject(info);
	return info;
}

//СОЗДАНИЕ ОБЪЕКТА C СЕМАНТИКОЙ
long int	MapScroll::createObject(long int hSit, double x, double y, const char * name_ff, long int id_obj, QMap<int,QString> semantic_map)
{
	QString str_id_obj = QString::number(id_obj);
	info = map->mapCreateSiteObject(hMap,hSit);

	map->mapRegisterObjectByKey(info, name_ff);
	map->mapAppendPointPlane(info, x, y);
	objectTopScale(info);//задать max масштаб отображения
	objectByMap(hMap,info);

	long int a321;
	double Radius;
	double Angle;

	QMapIterator<int, QString> i(semantic_map);
    while (i.hasNext()) 
	{
		i.next();
		if (i.key()==32811) // дальность действия 
		{
			//Radius = i.value().toDouble()*250;
			Radius = i.value().toDouble()*250;
		}
		if (i.key()==32852) // угол (направление) действия 
		{
			//Angle = i.value().toDouble();
			Angle = 5*3.14/2 - (i.value().toDouble()*3.14/180);
		}

		a321 = map->mapAppendSemantic(info, i.key(), i.value().toLocal8Bit().data(), 255);
	}
 
	// если зачек радио-теле центра (свой или вражеский), то добавляем вторую метрику
	if ((QString(name_ff)=="V0000169007")||(QString(name_ff)=="V0000169029"))
	{	
		map->mapAppendPointPlane(info, x,y+Radius);
	}


	if (QString(name_ff)=="L00000060504") // самолет
	{	
		map->mapAppendPointPlane(info, x+120000,y+140000);
		map->mapAppendPointPlane(info, x+120000+70000,y+140000-80000);
	}


	// если передвижная звуковещательная станция, то добавляем вторую метрику 
	// (получаем ее как угол места и длину радиус-вектора, направленного из первой точки метрики)
	if (QString(name_ff)=="V0000060505")
	{	
		//double xx=4*Radius*cos(Angle);
		//double yy=4*Radius*sin(Angle);

		double xx=4*Radius*cos(Angle);
		double yy=4*Radius*sin(Angle);

		map->mapAppendPointPlane(info, x+xx,y+yy);
	}


	
//	if (QString(name_ff)!="V0000169007")&&(QString(name_ff)!="V0000060505"))
//	{
		//a321 = map->mapAppendSemantic(info, i.key(), i.value().toLocal8Bit().data(), 255);
//    }

	//сохранить данные об объекте
	map->mapCommitWithPlace(info);
	map->mapClearObject(info);
	return info;
}

//L0012345109 создание линейного объекта
long int	MapScroll::createLineObject(long int hSit, double x, double y, const char * name_ff, long int id_obj)
{
	QString str_id_obj = QString::number(id_obj);
	info = map->mapCreateSiteObject(hMap,hSit);
	long int a21 = map->mapAppendSemantic(info, 17501, str_id_obj.toLocal8Bit().data(), 18);

	long int a1 = map->mapRegisterObjectByKey(info, name_ff);//сохранить данные об объекте
	long int a24 = map->mapAppendPointPlane(info, x, y);
	long int a3 = objectTopScale(info);//задать max масштаб отображения 40000000
	long int a4 = objectByMap(hMap,info);
	double X[10], Y[10];
	X[0]=x; Y[0]=y;
	int i=1;
	while (i<9)
	{
		X[i] = X[i-1] - 990000;
		Y[i] = Y[i-1] - 90000;
		i++;
		X[i] = X[i-1] - 99000;
		Y[i] = Y[i-1] + 90000;
		i++;
	}
	for (i=1; i<9; i++)
	{
		a24 = map->mapAppendPointPlane(info, X[i], Y[i]);
	}
	a24 = map->mapCommitWithPlace(info);
	map->mapClearObject(info);
	return a24;
}
long int	MapScroll::IsActive(HMAP hMap)
{
	return map->mapIsActive(hMap);
}
long int	MapScroll::GetError()
{
	return map->mapGetAccessError();
	
}
long int	MapScroll::GetSiteLayerCount(HMAP hMap,HSITE hSite)
{
	return map->mapGetSiteLayerCount(hMap,hSite);
}
//обновить изображение в размерах экрана
long int	MapScroll::ApdateScreen()
{
	long int a1;
	HWND hwnd = map->mapGetHandleForMessage();
	//= map->mapGetHandleForEvent(hMap);
	himage = map->mapCreateImage(hwnd);
	POINT position;
	RECT rect;
	position.x = horizontalScrollBar()->value();
	position.y = verticalScrollBar()->value();
	rect.bottom = position.x+1500;
	rect.left = 0;
	rect.top = 0;
	rect.right = position.y+1500;
	a1 = map->mapDrawImageMap(himage, hMap, &RectDraw, &position);
	return a1;
}

//пересчет из координат экрана в прямоугольные на карте
QPoint		MapScroll::getXY(double x, double y)
{
	QPoint a;
	x += horizontalScrollBar()->value();
	y += verticalScrollBar()->value();		
	map->mapPictureToPlane(hMap, &x, &y);
	a.setX(x);
	a.setY(y);
	return a;
}
//
void		MapScroll::IsObject(HOBJ hobj)
{
				//запрос количества возможных для редактирования семантик для данного объекта
		//long int g1 = map->mapAvailableSemanticCount(hobj);
	//	if (g1) 
	//	{
	//		g1 = map->mapAvailableSemanticCode(hobj,g1);//код последней доступной семантики
	//	}
		char value[32];
		long int a1=0, a2=0, a3=0;
		long int g1=60011;	//  для РЕГИОНОВ!!!
		long int g2=32812;	//	ls - формирования,	mpo_pso - средства
		long int g3=32822;	//	ID - объекта в таблице (ls или	mpo_pso)
		int k=0, id_object=0, step=1;
		char value2[32];
		char value3[32];

		a1 = map->mapSemanticCodeValue(hobj, g1, value, 32, 1);
		
		

		// только для РЕГИОНОВ!!!				   
		if (a1!=0) 
		{
			// получаем номер объекта
			long int ff = map->mapObjectKey(hobj);

		int id_object=QString::number(ff).toInt();

			flag1=1;



			
			emit signal_for_info_2_arg("region", ff);
		
		}
		// для ФОРМИРОВАНИЙ или СРЕДСТВ!!!
		a2 = map->mapSemanticCodeValue(hobj, g2, value2, 32, 1);
		a3 = map->mapSemanticCodeValue(hobj, g3, value3, 32, 1);		

		

		if ((a2!=0)&&(a3!=0)) 
		{

		long int id_object = atoi(value3);
		
		// вызываем окошко с информацией об объекте!
		//emit signal_for_info(id_object);
		emit signal_for_info_2_arg(QString("%1").arg(value2), id_object);
////////////////////////////////////////////////////////////////////////////////////
/*
// Запрос информации об объекте из базы
			QSqlQuery query;
			QString Name_ls;
			long int a11111=0;
			QString str=QString("SELECT name_ls FROM ls WHERE id_ls=20");
			if(query.exec(str))
			{
				while (query.next())
				{
					Name_ls = query.value(0).toString();
				}
				query.clear();
			}

e=QString("Информация об объекте: ")+Name_ls;
ret232323232 = QMessageBox::warning (this, tr("My Application"),  e,
                           QMessageBox::Save | QMessageBox::Discard
                           | QMessageBox::Cancel,
                           QMessageBox::Save);
*/
////////////////////////////////////////////////////////////////////////////////////

		}

}
//запросы прямоугольных координат углов карты
double		MapScroll::getMapX1(HMAP hMap)
{
	return map->mapGetMapX1(hMap);
}
double		MapScroll::getMapY1(HMAP hMap)
{
	return map->mapGetMapY1(hMap);
}
double		MapScroll::getMapX2(HMAP hMap) 
{
	return map->mapGetMapX2(hMap);
}
double		MapScroll::getMapY2(HMAP hMap)
{
	return map->mapGetMapY2(hMap);
}
//сброс признака масштабируемости
long int	MapScroll::setObjectNoScale(HOBJ hobj)
{
	map->mapSetObjectPress(hobj,1);
	return map->mapSetObjectScale(hobj,0);
}
//запросить/установить границы видимости объектов
long int	MapScroll::objectTopScale(HOBJ hobj)
{
	return map->mapSetObjectTopScale(hobj,100000000);
}
long int	MapScroll::objectByMap(HMAP hMap, HOBJ object)
{
	long int a1=0;
	/*HWND hwnd = map->mapGetHandleForEvent(hMap);
	himage = map->mapCreateImage(hwnd);
	PAINTPARM parm;
	a1 = map->mapDrawImageMapObject(himage, hMap, &parm, object);*/
	return a1;
}
//delete
void		MapScroll::mouseRightMenu()
{
		mouse_menu = new QMenu(this); 
		mouse_menu->addAction("&Red"); 
		mouse_menu->addAction("&Green"); 
		mouse_menu->addAction("&Blue"); 
	//	mouse_menu->exec(pe);
}
int			MapScroll::HScrollBarValue()
{
	return horizontalScrollBar()->value();
}
int			MapScroll::VScrollBarValue()
{
	return verticalScrollBar()->value();
}
long int	MapScroll::getLayerCount()
{
	return map->mapGetLayerCount(hMap);
}
long int	MapScroll::setViewScale(float scale)
{
	long int x;
	long int y;
	x = 20000;
	y=x;
	return map->mapSetViewScale(hMap,&x,&y,scale);
}
//запросить яркость
long int	MapScroll::getmapBright()
{
	return map->mapGetBright(hMap);
}
//установить яркость
long int	MapScroll::setmapBright(long int bright)
{
	return map->mapSetBright(hMap,bright);
}
void		MapScroll::paint95()//перерисовка
{
  if (hMap)
   {	
	QPainter p;
    p.begin(MyViewport);
	  long int cx, cy, cw, ch;
	  cx =  this->rect.left();
	  cy = this->rect.top() ;
	  cw = this->rect.right() - this->rect.left() +1;
	  ch = this->rect.bottom() - this->rect.top() +1;
      RectDraw.left = 	cx;
      RectDraw.top  =   cy;
      RectDraw.right  = cx + cw;
      RectDraw.bottom = cy + ch;
      
	  int dibsize = sizeof(BITMAPINFOHEADER) +
                   (long)sizeof(RGBQUAD)*256L;
 
	  char *lpImage = (char *)::AllocateTheMemory(cw * ch * 4 + dibsize);
      memset(lpImage, 0xff, cw * ch * 4 + dibsize);

	  BITMAPINFOHEADER *lpDibInfo = (BITMAPINFOHEADER *)lpImage;
	  lpDibInfo->biSize          = sizeof(BITMAPINFOHEADER);
      lpDibInfo->biWidth         = cw;
      lpDibInfo->biHeight        = ch;
      lpDibInfo->biPlanes        = 1;
      lpDibInfo->biBitCount      = (unsigned short)(8 * 4);
      lpDibInfo->biCompression   = BI_RGB;
      lpDibInfo->biSizeImage     = (long)cw*(long)ch*4;
      lpDibInfo->biXPelsPerMeter = 0;
      lpDibInfo->biYPelsPerMeter = 0;
      lpDibInfo->biClrUsed       = 256;
      lpDibInfo->biClrImportant  = 0; 
	  char *lpBits = lpImage + dibsize;
	  int ret= map->drawContents(hMap, lpDibInfo, lpBits, &RectDraw);
	  char *string = (char *)AllocateTheMemory(cw * 4);
      for (int i = 0; i < ch/2; i++)
	  {
		char *pointE  = lpBits + (ch - i - 1) * cw * 4;
        char *pointB  = lpBits + i * cw * 4;
		memcpy(string, pointB, cw * 4);
        memcpy(pointB, pointE, cw * 4);
        memcpy(pointE, string, cw * 4);
	  }
      FreeTheMemory(string);

	  QImage img((uchar *) lpBits, cw, ch, QImage::Format_RGB32);	  
      p.drawImage(cx, cy, img, 0, 0, cw, ch);
	  FreeTheMemory(lpImage);
	  p.end();
	   }
}

//==============================================================================================================================================
void		MapScroll::findObjectByCod()//поиск объекта по коду
{

	//map->mapSetSemanticValue



	  // Поиск объекта по уникальному номеру на карте
  // info     - идентификатор существующего объекта,
  // созданного функцией CreateObject() или CreateSiteObject(),
  // в котором будет размещен результат поиска.
  // hMap   - идентификатор открытой основной карты
  // hSite  - идентификатор открытой пользовательской карты
  // key   - идентификатор объекта на карте
  // При ошибке возвращает ноль

//_MAPIMP HOBJ _MAPAPI mapSeekSiteObject(HMAP hMap,HSITE hSite,HOBJ info,
//                                       long int key);
}


//Изменение семантики региона
void MapScroll::RegionAppendSemantic(HOBJ hobj, QString semantic)
{

						//Изменение семантики региона (изменение цвета заливки региона)

						map->mapAppendSemantic(hobj, 60011, semantic.toLocal8Bit().data(), 32);

						//сохранить данные об объекте
						map->mapCommitWithPlace(hobj);
						map->mapClearObject(hobj);
						MyViewport->hide();
						MyViewport->show();
						return;
}