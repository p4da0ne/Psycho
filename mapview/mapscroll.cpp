#include <QMessageBox>
#include <QPainter>
#include <QScrollBar>
#include <QImage>
#include <QPaintEvent>
#include <QMenu>
#include <math.h>
#include "mapscroll.h"
#include "coord.h"

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
	flag=0;
	number_action = 0;
	hSite=0;
	hSite_s=0;
	hSite_w=0;
	hSite_p=0;
	hSite_line=0;
	hSite_corr=0;
	hSite_ok_weapon=0;
	war_line_coord.clear();
	dx=0; dy=0;
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


bool MapScroll::eventFilter (QObject * watched, QEvent * event)
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
void MapScroll::drawContents( QPainter* p, int cx, int cy, int cw, int ch)//перерисовка
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
      lpDibInfo->biSizeImage     = lpDibInfo->biHeight *((lpDibInfo->biWidth * lpDibInfo->biBitCount + 31) / 32) * 4; //(long)cw*(long)ch*4;
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
}


//открытие карты
int	MapScroll::mapOpen(  const char *name )
{
  char        drive[10];
  char        dir[MAX_PATH];
  char        namefile[MAX_PATH];
  char        extname[10];
  char        newnamefile[MAX_PATH];
  char        sxfname[MAX_PATH];
  int         length;
  if (hMap)
   {
    closeMap();
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
	  //setViewScale(16000000);//установить масштаб, с каким изначально откроется карта
	  
	 
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
	  return hMap;
  }
  else
  {
	return 0;
  }
};


//=====================================================================
//====================== Метод закрытия карты =========================
//=====================================================================
void MapScroll::closeMap()
{
  if (hMap!=0)
  {
	  map->mapCloseData(hMap);
     hMap=0;
    MyViewport->hide();
  }
}

//====================================================================
//==== Метод изменения масштаба "<" ">" отображения карты ============
//====================================================================
void MapScroll::changeScale(float change)
{
	if (hMap == 0) return;
	long int X,Y;
	double x1,y1, a, b;
	long int mapW, mapH;
	//вычислим текущий центр
	X = horizontalScrollBar()->value() + viewport()->width() / 2;
	Y = verticalScrollBar()->value() + viewport()->height() / 2;
	
    map->mapChangeViewScale(hMap,&X,&Y,change);
	map->mapGetPictureSize(hMap,&mapW,&mapH);
	
	MyViewport->hide();
	//изменение размеров содержимого
	MyViewport->resize(mapW, mapH);
	
	
	//вычислим новый центр
	X = X - viewport()->width() / 2;
	if(X<0) X = 0;
	Y = Y - viewport()->height() / 2;
	if(Y<0) Y = 0;

	horizontalScrollBar()->setValue(X);
	verticalScrollBar()->setValue(Y);
	MyViewport->show();
}

//==================================================================
//========= Скроллинг карты по клавишам влево-вправо ===============
//==================================================================
void MapScroll::changePos(long int dx,long int dy)
{
  if (hMap == 0) return;
  long int X,Y;

  long int mapW, mapH;
  void (WINAPI * lpmapfn)(HMAP, long int *, long int *);
  (FARPROC&) lpmapfn = ::GetProcAddress(LibInst,"mapGetPictureSize");// Запросить размеры общего изображения карты в пикселах  // для текущего масштаба
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


//==========================================================================
//===== Метод обработки событий нажатия клавиш мыши ========================
//==========================================================================
void MapScroll::mousePressEvent(QMouseEvent * event)
{
	if (hMap)
	{
		if(event->button())
		{

			pe = event->pos();
			screenX= pe.x(); 
			screenY= pe.y(); 
			int xScroll = HScrollBarValue();
			int yScroll = VScrollBarValue();

			screenX+= xScroll; //horizontalScrollBar()->value();
			screenY+= yScroll; //verticalScrollBar()->value();
			
			map->mapPictureToPlane(hMap, &screenX, &screenY);
			pe = event->globalPos();
			
			//QStringList semList = getObjectIdAndTypeInfo(&screenX, &screenY);

			QList<QStringList> allObjectsList = getAllObjectsIdAndTypeInfo(&screenX, &screenY);

			if(!allObjectsList.isEmpty())
			{
				/*long int idOdject = semList.at(0).toInt();
				long int objectType = semList.at(1).toInt();*/
				
				if (event->button() == Qt::LeftButton)//левая клавиша мыши
				{					
					//emit leftButtonClicked(pe,idOdject,objectType); return;	
					emit leftButtonClicked(pe,allObjectsList); return;
				}
				if (event->button() == Qt::RightButton)  //правая клавиша мыши
				{
					//emit rightButtonClicked(pe,idOdject,objectType); return;	
					emit rightButtonClicked(pe,allObjectsList); return;
				}
			
			}
			else
			{
				if (event->button() == Qt::RightButton)  //правая клавиша мыши
				{
					//emit rightButtonClicked(pe,0,0); return; //клик на пустом месте (где нет объектов)	
					emit rightButtonClicked(pe,allObjectsList); return;
				}
				return;
			}
		}
	}
}

//для левой клавиши мыши
void		MapScroll::findObject(double *x, double *y)
{	
	select = map->mapCreateMapSelectContext(hMap);//создать условия поиска
	info=map->mapCreateObject(hMap);
	changeFrame();
	info=map->mapWhatObject(hMap,info,&frame,WO_LAST,PP_PLANE);
	//info=map->mapWhatActiveObject(hMap,info,&frame,WO_LAST,PP_PLANE);
	IsObject(info);
	double x1,y1;
	x1=0;
	y1=0;
	map->mapPictureToPlane(hMap, &x1, &y1);
//	HWND hwnd = map->mapGetHandleForEvent(hMap);

	long int a1 = map->mapClearObject(info);
	map->mapDeleteSelectContext(select);//удалить условия поиска
}
//для правой клавиши мыши
QStringList	MapScroll::getHobj(double *x, double *y)
{
	QStringList list;
	info=map->mapCreateObject(hMap);
	changeFrame();
	info=map->mapWhatObject(hMap,info,&frame,WO_LAST,PP_PLANE);
	long int g1 = map->mapAvailableSemanticCount(info);
			g1 = map->mapAvailableSemanticCode(info,g1);//код последней доступной семантики
		char value[32], value_flag[16], value_coord[18];
		long int a1=0,a2=0;
		g1=17501;
			a1 = map->mapSemanticCodeValue(info, g1, value, 32, 1);
		g1 = 17502;
			int a3 = map->mapSemanticCodeValue(info, g1, value_flag, 16, 1);
		g1 = 17503;
			a3 = map->mapSemanticCodeValue(info, g1, value_coord, 16, 1);
		/*char value_test[32];
		g1 = 25;//проверка семантики, где тип ракет
			a3 = map->mapSemanticCodeValue(info, g1, value_test, 32, 1);*/
		QString str_value_coord = value_coord;
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
				flag=1;
			}
			long int a1 = map->mapClearObject(info);
		}
		list.append(QString::number(a2));
		list.append(QString::number(value_flag[0]));
		list.append(str_value_coord);
		return list;
}

//=====================================================================
//===== Метод возвращает список значений семантик знака с ключами: ====
//===== 17501 - idObject; 17502 - тип объекта =========================
//=====================================================================
QStringList	MapScroll::getObjectIdAndTypeInfo(double *x, double *y)
{
	QStringList semList;
	info=map->mapCreateObject(hMap);
	changeFrame(50);  // расширение области поиска объекта
	info=map->mapWhatObject(hMap,info,&frame,WO_LAST,PP_PLANE);

	double idObject, objectType;
	int idObjectSemNumber, objectTypeSemNumber;

	if(info)
	{
		idObject = map->mapSemanticCodeDoubleValue(info,ID_OBJECT,1);
		objectType = map->mapSemanticCodeDoubleValue(info,OBJECT_TYPE,1);
		

		semList.append(QString::number((int)idObject));
		semList.append(QString::number((int)objectType));
	}
	return semList;
}


//==============================================================================
//===== Метод возвращает список списков значений семантик знаков с ключами: ====
//===== 17501 - idObject; 17502 - тип объекта ==================================
//===== Выполняется поиск всех объектов в заданной точке =======================
QList<QStringList>	MapScroll::getAllObjectsIdAndTypeInfo(double *x, double *y)
{
	QList<QStringList> signsList;
	
	QStringList semList;
	info=map->mapCreateObject(hMap);
	changeFrame(50);  // расширение области поиска объекта
	info=map->mapWhatObject(hMap,info,&frame,WO_LAST,PP_PLANE);

	double idObject, objectType;
	int idObjectSemNumber, objectTypeSemNumber;

	if(info)
	{
		idObject = map->mapSemanticCodeDoubleValue(info,ID_OBJECT,1);
		objectType = map->mapSemanticCodeDoubleValue(info,OBJECT_TYPE,1);
	
		if(idObject && objectType)
		{
			semList.append(QString::number((int)idObject));
			semList.append(QString::number((int)objectType));
			signsList.append(semList);
		}
		while(info)
		{
			HOBJ infoLast = info;
			info=map->mapCreateObject(hMap);
			info=map->mapWhatObject(hMap,infoLast,&frame,WO_BACK,PP_PLANE);

			if(info)
			{
				idObject = map->mapSemanticCodeDoubleValue(info,ID_OBJECT,1);
				objectType = map->mapSemanticCodeDoubleValue(info,OBJECT_TYPE,1);
	
				if(idObject && objectType)
				{
					QStringList semList;
					semList.append(QString::number((int)idObject));
					semList.append(QString::number((int)objectType));
					signsList.append(semList);
				}
			}
		}
	}


	return signsList;
}

//================================================================================
//==== Метод изменения области поиска объекта, начиная от координат клика мыши ===
//================================================================================
void MapScroll::changeFrame(int pixels)
{
	dframe.X1 = screenX+pixels;
	dframe.X2 = screenX-pixels;
	dframe.Y1 = screenY+pixels;
	dframe.Y2 = screenY-pixels;
	frame = dframe;
}

//открытие пользовательского слоя
HSITE		MapScroll::openSit(HMAP hMap, const char * mapname, const char * rscname)
{
	memset((void*)&createsite,0,sizeof(createsite));
	createsite.Length = sizeof(createsite);
	strcpy(createsite.MapName, mapname);

	createsite.MapType=COUNTGEOG;//обзорно-географическая
	createsite.MaterialProjection=CONICALORTHOMORPHIC;//каноническая равноугольная
	//long int scale = 5000000;//mapwin->getScale();
	long int scale = getScale();
	createsite.Scale = scale;
	map->mapCreateAndAppendSite(hMap,mapname,rscname,&createsite);
	return  map->mapOpenSiteForMap(hMap,mapname,0);
}

//Закрытие пользовательской карты
void		MapScroll::closeSit(HMAP hMap, HSITE hsite)
{
	if (hsite!=0) {
		map->mapCloseSiteForMap(hMap,hsite);
	}
		if (hsite==hSite) hSite=0;//чтоб проверять что слой закрыт
		if (hsite==hSite_s) hSite_s=0;
		if (hsite==hSite_line) hSite_line=0;
		if (hsite==hSite_w) hSite_w=0;
		if (hsite==hSite_p) hSite_p=0;
		if (hsite==hSite_ok_weapon) hSite_ok_weapon=0;
		flag2=0;
}

//======================================================================
//==== Закрытие пользовательской карты по её имени (пути к файлу) ======
//======================================================================
void MapScroll::closeSitByName(HMAP hMap, const char * sitName)
{
	map->mapCloseSiteForMapByName(hMap,sitName);
}


//добавление существующего пользовательского слоя
long int	MapScroll::appendData(const char* sitname)
{
	if (hMap)
	{
		return map->mapAppendData(hMap, sitname);
	}
	return 0;
}
//СОЗДАНИЕ ОБЪЕКТА!!
long int	MapScroll::createObject(long int hSit, double x, double y, const char * name_ff, long int id_obj, long int flag, bool if_shtab, QString semantic, long int id_coordintes, float angle)
{
	QString str_id_obj = QString::number(id_obj);
	QString str_flag = QString::number(flag);
	info = map->mapCreateSiteObject(hMap,hSit);
	long int a21;
	if (id_obj!=0)		    a21 = map->mapAppendSemantic(info, 17501, str_id_obj.toLocal8Bit().data(), 18);//добавить значение в семантику
	if (flag!=0)			
		a21 = map->mapAppendSemantic(info, 17502, str_flag.toLocal8Bit().data(), 18);//добавить значение в семантику

	if (id_coordintes!=0)		
	{
		str_flag = QString::number(id_coordintes);
		a21 = map->mapAppendSemantic(info, 17503, str_flag.toLocal8Bit().data(), 18);//добавить значение в семантику
	}
	if (semantic!="")		
	{
		QStringList sem_list = semantic.split("_");
		int list_i = sem_list.count();
		a21 = map->mapAppendSemantic(info, 17, sem_list.at(0).toLocal8Bit().data(), 255);//добавить значение в семантику	
		if(list_i>1)
		{
			a21 = map->mapAppendSemantic(info, 218, sem_list.at(1).toLocal8Bit().data(), 255);//добавить значение в семантику	
			a21 = map->mapAppendSemantic(info, 24, sem_list.at(2).toLocal8Bit().data(), 255);//добавить значение в семантику	
		}			
	}
	long int a1 = map->mapRegisterObjectByKey(info, name_ff);	
	long int a24 = map->mapAppendPointPlane(info, x, y);
	if (if_shtab) a24 = map->mapAppendPointPlane(info, x, y-10000);
	else 
	{
		double dx=10000, dy=0;
		if (angle!=0) 
		{
			if (angle>0&&angle<90)
			{
				dx=10000;
				dy=abs(dx*tan(M_PI*angle/180));	
			}
			if (angle>90&&angle<180)
			{
				dx=-10000;
				dy=abs(dx*tan(M_PI*angle/180));
			}
			if (angle>180&&angle<270)
			{
				dx=-10000;
				dy=-1*abs(dx*tan(M_PI*angle/180));
			}
			if (angle>270&&angle<360)
			{
				dx=10000;
				dy=-1*abs(dx*tan(M_PI*angle/180));
			}
			if (angle==90)
			{
				dy=0;
				dx=10000;
			}
			if (angle==180)
			{
				dy=-10000;
				dx=0;
			}
			if (angle==0||angle==360)
			{
				dy=10000;
				dx=0;
			}
			if (angle==270)
			{
				dy=0;
				dx=-10000;
			}
		}
			a24 = map->mapAppendPointPlane(info, x+dx, y+dy);//
	}
	long int a3 = objectTopScale(info);//задать max масштаб отображения 40000000 128000000
	//long int a4 = objectByMap(hMap,info); //не работает
	a24 = map->mapCommitWithPlace(info);

	map->mapClearObject(info);
	return a24;
}
//L0012345109

long int	MapScroll::createLine(long int hSit, const char * name_ff, long int id_obj)
{

	QString str_id_obj = QString::number(id_obj);
	info = map->mapCreateSiteObject(hMap,hSit);
	long int a21 = map->mapAppendSemantic(info, 17501, str_id_obj.toLocal8Bit().data(), 18);

	long int a1 = map->mapRegisterObjectByKey(info, name_ff);//сохранить данные об объекте
	Delete();
	QList<double> war_line_xy;
	double aa = war_line_coord.at(0).at(0), bb = war_line_coord.at(0).at(1);
	long int a24 = map->mapAppendPointPlane(info, aa, bb);
	long int a3 = objectTopScale(info);//задать max масштаб отображения 40000000
	long int a4 = objectByMap(hMap,info);
	for (int i=1; i<war_line_coord.count(); ++i)
	{
		aa = war_line_coord.at(i).at(0), bb = war_line_coord.at(i).at(1);
		a24 = map->mapAppendPointPlane(info, aa, bb);
	}
	a24 = map->mapCommitWithPlace(info);
	map->mapClearObject(info);

	a21 = map->mapAppendSemantic(info, 17501, str_id_obj.toLocal8Bit().data(), 18);
	a1 = map->mapRegisterObjectByKey(info, name_ff);//сохранить данные об объекте
	long int a_kaliningrad = map->mapAppendPointPlane(info, X1[0], Y1[0]);
	a3 = objectTopScale(info);//задать max масштаб отображения 40000000
	a4 = objectByMap(hMap,info);	
	for (int i=1; i<38; i++)
	{
		a_kaliningrad = map->mapAppendPointPlane(info, X1[i], Y1[i]);
	}
	a_kaliningrad = map->mapCommitWithPlace(info);
	map->mapClearObject(info);

	return a24;
}


void		MapScroll::Delete()
{
//======= Калининград =======
X1[0] = 6358089.537102;    Y1[0] = 7961678.445230;
X1[1] = 6340068.335689;    Y1[1] = 7978286.219081;
X1[2] = 6338301.551237;    Y1[2] = 7995247.349823;
X1[3] = 6333707.911661;    Y1[3] = 8026342.756184;
X1[4] = 6330527.699647;    Y1[4] = 8059204.946996;
X1[5] = 6328054.201413;    Y1[5] = 8082173.144876;
X1[6] = 6326994.130742;    Y1[6] = 8093127.208481;
X1[7] = 6328407.558304;    Y1[7] = 8109028.268551;
X1[8] = 6330527.699647;    Y1[8] = 8133409.893993;
X1[9] = 6330881.056537;    Y1[9] = 8152844.522968;
X1[10] = 6334414.625442;   Y1[10] = 8170865.724382;
X1[11] = 6335121.339223;   Y1[11] = 8177932.862191;
X1[12] = 6343601.904594;   Y1[12] = 8172632.508834;
X1[13] = 6351375.756184;   Y1[13] = 8169452.296820;
X1[14] = 6359149.607774;   Y1[14] = 8170159.010601;
X1[15] = 6365863.388693;   Y1[15] = 8174045.936396;
X1[16] = 6371517.098940;   Y1[16] = 8172632.508834;
X1[17] = 6378584.236749;   Y1[17] = 8178286.219081;
X1[18] = 6380704.378092;   Y1[18] = 8181113.074205;
X1[19] = 6386358.088339;   Y1[19] = 8181466.431095;
X1[20] = 6392365.155477;   Y1[20] = 8178639.575972;
X1[21] = 6396958.795053;   Y1[21] = 8172985.865724;
X1[22] = 6401199.077739;   Y1[22] = 8167332.155477;
X1[23] = 6401905.791519;   Y1[23] = 8165918.727915;
X1[24] = 6408619.572438;   Y1[24] = 8160971.731449;
X1[25] = 6410033.000000;   Y1[25] = 8160618.374558;
X1[26] = 6406852.787986;   Y1[26] = 8154964.664311;
X1[27] = 6408619.572438;   Y1[27] = 8137296.819788;
X1[28] = 6404732.646643;   Y1[28] = 8131643.109541;
X1[29] = 6406499.431095;   Y1[29] = 8125636.042403;
X1[30] = 6411446.427562;   Y1[30] = 8126696.113074;
X1[31] = 6410739.713781;   Y1[31] = 8121042.402827;
X1[32] = 6413213.212014;   Y1[32] = 8113268.551237;
X1[33] = 6419573.636042;   Y1[33] = 8103374.558304;
X1[34] = 6422047.134276;   Y1[34] = 8093127.208481;
X1[35] = 6430881.056537;   Y1[35] = 8086413.427562;
X1[36] = 6437594.837456;   Y1[36] = 8085353.356890;
X1[37] = 6439008.265018;   Y1[37] = 8082879.858657;
X1[38] = 6437241.480565;   Y1[38] = 8077579.505300;
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
long int	MapScroll::updateScreen()
{
	MyViewport->hide();
	MyViewport->show();
	return 0;
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
////получаем значение из семантики
void		MapScroll::IsObject(HOBJ hobj)
{
				//запрос количества возможных для редактирования семантик для данного объекта
		long int g1 = map->mapAvailableSemanticCount(hobj);
		g1 = map->mapAvailableSemanticCode(hobj,g1);//код последней доступной семантики
		//}
			long int flag=0;
		char value[32];
		long int a1=0;
		g1=17501;
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
		g1=17502;//flag
		a1 = map->mapSemanticCodeValue(hobj, g1, value, 32, 1);

		if (a1) flag=abs(int(value[0])-48);
		if (a1!=0) 
		{
			if (id_odject!=0)
			{
				flag1=1;
				emit signal_for_info(id_odject, flag);
				//mouseRightMenu();
			}

		}
}
//==============================================================
//== Методы возвращают прямоугольные координаты углов карты ====
//==============================================================
double MapScroll::getMapX1(HMAP hMap)
{
	return map->mapGetMapX1(hMap);
}
double MapScroll::getMapY1(HMAP hMap)
{
	return map->mapGetMapY1(hMap);
}
double MapScroll::getMapX2(HMAP hMap) 
{
	return map->mapGetMapX2(hMap);
}
double MapScroll::getMapY2(HMAP hMap)
{
	return map->mapGetMapY2(hMap);
}

//==============================================================
//=========== Сброс признака масштабируемости ==================
//==============================================================
long int MapScroll::setObjectNoScale(HOBJ hobj)
{
	//map->mapSetObjectPress(hobj,1);
	return map->mapSetObjectScale(hobj,0);
}
//запросить/установить границы видимости объектов
long int MapScroll::objectTopScale(HOBJ hobj)
{
	return map->mapSetObjectTopScale(hobj,150000000);
}
long int MapScroll::objectByMap(HMAP hMap, HOBJ object)
{
	long int a1=0;
	/*HWND hwnd = map->mapGetHandleForEvent(hMap);
	himage = map->mapCreateImage(hwnd);
	PAINTPARM parm;
	a1 = map->mapDrawImageMapObject(himage, hMap, &parm, object);*/
	return a1;
}

//=====================================================================
//== Метод возвращает текущее значение горизонтального скролбара ======
//=====================================================================
int	MapScroll::HScrollBarValue()
{
	return horizontalScrollBar()->value();
}
//=====================================================================
//== Метод возвращает текущее значение вертикального скролбара ========
//=====================================================================
int	MapScroll::VScrollBarValue()
{
	return verticalScrollBar()->value();
}

 // Запросить число слоев на карте
 // hmap - идентификатор открытых данных
 // При ошибке возвращает ноль
long int MapScroll::getLayerCount()
{
	return map->mapGetLayerCount(hMap);
}

//====================================================================
// ===== Установить масштаб карты ====================================
//====================================================================
long int MapScroll::setViewScale(float scale)
{
	long int x;
	long int y;
	x = 6000;   ///???????????????????????????????????????????????????????????????
	y=x;
	return map->mapSetViewScale(hMap,&x,&y,scale);
}

//===================================================================
//=========== Запросить яркость карты ===============================
//===================================================================
long int MapScroll::getMapBright()
{
	return map->mapGetBright(hMap);
}
//установить яркость
long int	MapScroll::setMapBright(long int bright)
{
	return map->mapSetBright(hMap,bright);
}
//запросить контраст
long int	MapScroll::getMapContrast()
{
	return map->mapGetContrast(hMap);
}
//установить контраст
long int	MapScroll::setMapContrast(long int contrast)
{
	return map->mapSetContrast(hMap, contrast);
}
//запросить яркость печати
long int	MapScroll::getMapIntensity()
{
	return map->mapGetIntensity(hMap);
}
//установить яркость печати
long int	MapScroll::setMapIntensity(long int intensity)
{
	return map->mapSetIntensity(hMap, intensity);
}
//перерисовка
void		MapScroll::paint95()
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
      lpDibInfo->biSizeImage     = lpDibInfo->biHeight *((lpDibInfo->biWidth * lpDibInfo->biBitCount + 31) / 32) * 4;;
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
//запростиь округленный масштаб
long int	MapScroll::getScale()
{
	return map->mapGetShowScale(hMap);
}
//создание линии из 2х точек
long int	MapScroll::createV0(long int hSit, double x1, double y1, double x2, double y2, const char * name_ff, long int id_obj, int flag, long int id_coord)
{
	QString str_id_obj = QString::number(id_obj), str_coord = QString::number(id_coord);
	info = map->mapCreateSiteObject(hMap,hSit);
	long int a21 = map->mapAppendSemantic(info, 17501, str_id_obj.toLocal8Bit().data(), 18);
	QString str_flag = QString::number(flag);
	a21 = map->mapAppendSemantic(info, 17502, str_flag.toLocal8Bit().data(), 18);
	a21 = map->mapAppendSemantic(info, 17503, str_coord.toLocal8Bit().data(), 18);
	//нанесение на карте
	long int a1 = map->mapRegisterObjectByKey(info, name_ff);//сохранить данные об объекте
	long int a24 = map->mapAppendPointPlane(info, x1, y1);
	long int a3 = objectTopScale(info);//задать max масштаб отображения
	long int a4 = objectByMap(hMap,info);
	a24 = map->mapAppendPointPlane(info, x2, y2);
	a24 = map->mapCommitWithPlace(info);
	map->mapClearObject(info);
	return a24;
}
//получтиь название объекта из классификатора по коду
long int	MapScroll::objectInfoFromRsc(HOBJ Info, const char * name)
{
	name = map->mapObjectName(Info);
	return 0;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
long int	MapScroll::createObjectTest(long int hSit,  QList<Coord*> *coordinates, const char * rscKey, QMap<long int,QString> *semantics)
{
	info = map->mapCreateSiteObject(hMap,hSit);
	objectTopScale(info);

	if(semantics)
	{
		QMap<long int,QString>::iterator it = semantics->begin(); 
		for (;it != semantics->end(); ++it) 
		{	
			long int semCode = it.key();
			QString semValue = it.value();
			map->mapAppendSemantic(info, semCode, semValue.toStdString().c_str(), semValue.size());
		}
	}

	map->mapRegisterObjectByKey(info, rscKey);

	//-- Добавление метрики объекта из списка координат ----
	for(int i=0;i<coordinates->count();i++)
	{
		double x,y;
		x = coordinates->at(i)->getX();
		y = coordinates->at(i)->getY();
		map->mapAppendPointPlane(info, x, y);
	}
	//-----------------------------------------------------


	long int commitFlag = map->mapCommitWithPlace(info);

	map->mapClearObject(info);
	
	return commitFlag;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////



//перевод координат
long int	MapScroll::planeToGeo423D(double *Bx, double *Ly, double *H)
{
	return map->mapPlaneToGeo423D(hMap, Bx, Ly, H);
}

void	MapScroll::degreeToRadian(GEODEGREE * degree, double * radian)
{
	map->mapDegreeToRadian(degree, radian);
}
long int	MapScroll::geoWGS84ToPlane3D(HMAP hMap, double *Bx, double *Ly, double * H)
{
	return map->mapGeoWGS84ToPlane3D(hMap, Bx, Ly, H);
}
void	MapScroll::radianToDegree(double * radian, GEODEGREE * degree)
{
		map->mapRadianToDegree(radian, degree);
}
//поддерживается ли перевод координат
long int	MapScroll::isGeoSupported()
{
	return map->mapIsGeoSupported(hMap);
}

HPRINTER	MapScroll::loadPrinter()
{
	return map->prnLoadPrinter();
}
 // Преобразование из метров на местности (проекция карты) в геодезические координаты в радианах (общеземной эллипсоид WGS84)
long int	MapScroll::planeToGeoWGS843D(double *Bx,  double *Ly,  double *H)
{
	 if (isGeoSupported())
    {
      return map->mapPlaneToGeoWGS843D(hMap, Bx, Ly, H);
    }
	return 0;
}
//перемещение объекта мышью
void		MapScroll::changeObjCoord()
{
		emit signalFor1Action(screenX, screenY);
}
void		MapScroll::changeHallCoord()
{
		emit signalFor2Action(screenX, screenY);
}
void		MapScroll::appointWeapon(double *x, double *y)
{
	select = map->mapCreateMapSelectContext(hMap);//создать условия поиска
	HOBJ info1=map->mapCreateObject(hMap);
	changeFrame();
	info1=map->mapWhatObject(hMap,info1,&frame,WO_LAST,PP_PLANE);
	//IsObject(info);
	long int g1 = map->mapAvailableSemanticCount(info1);
		g1 = map->mapAvailableSemanticCode(info1,g1);//код последней доступной семантики
		//}
			long int flag=0;
		char value[32];
		long int a1=0;
		g1=17501;//id_object
		a1 = map->mapSemanticCodeValue(info1, g1, value, 32, 1);
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
		g1=17502;//flag
		a1 = map->mapSemanticCodeValue(info1, g1, value, 32, 1);

		if (a1) flag=abs(int(value[0])-48);
		if (flag==5) //средство, подходящее для распределения
		{
			if (id_odject!=0)//дальше нужно узнать что за средство
			{
				emit signalFor3Action(id_odject);
			}
		}
}
void		MapScroll::changeAngleWithMouse(double *x, double *y)
{
	emit signalFor4Action(*x, *y);
}
//перевод из координат в метрах в WGS
GEODEGREEXY	 MapScroll::pictureToWGS(double * X, double * Y, double * H)
{
	GEODEGREEXY G_XY;
	GEODEGREE G_X1;
	GEODEGREE * G_X = &G_X1;
	G_X->Degree=0;
	G_X->Minute=0;
	G_X->Second=0;
	if (isGeoSupported())
		{
			long int a1 = map->mapPlaneToGeoWGS843D(hMap, X, Y, H);
			if (a1) 
			{
				radianToDegree(X, G_X);
			}
			G_XY.Degree_x = G_X->Degree;
			G_XY.Minute_x = G_X->Minute;
			G_XY.Second_x = G_X->Second;
			if (a1) 
			{
				radianToDegree(Y, G_X);
			}
			G_XY.Degree_y = G_X->Degree;
			G_XY.Minute_y = G_X->Minute;
			G_XY.Second_y = G_X->Second;
			G_XY.h = *H;
		}
	return G_XY;
}
void		MapScroll::emitSignalForPlanner(long int  id_targeting_version, long int id)
{

}
void		MapScroll::paintInDevice(HDC hdc, RECT * rect)
{
	map->mapPaint95(hMap, hdc, 0, rect);
}
// Изменить значение семантической характеристики объекта
long int	MapScroll::changeSemanticValue(HOBJ info, long int number, char * place, long int maxsize)
{
	return map->mapSetSemanticValue(info, number, place, maxsize);
}
//открытие растра
long int	MapScroll::openRstOnMap(const char * rstname)
{
	return map->openRstForMap(hMap, rstname, GENERIC_READ);
}	
//закрытие растра
long int	MapScroll::closeRstForMap(long int number)
{
	long int a = map->closeRstForMap(hMap, number);
	updateScreen();
	return a;
}
//растр над картой
long int	MapScroll::setRstOnMap(long int number)
{
	return map->setRstViewOrder(hMap, number, 1);
}
