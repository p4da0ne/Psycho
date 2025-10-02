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
   	this->setMouseTracking(true);  //�������� ����� ������������ ������� �������� ���� ��� ������� �������
	

	MainCodec = QTextCodec::codecForName("CP1251");
	MyViewport = 0;
    hMap = 0;
	map = new MyMapAccess();
	rect = frameRect();

	/////////////////////////
	moveFlag = false;
	////////////////////////
}
//==============================================================
//=============== ���������� ===================================
//==============================================================
MapScroll::~MapScroll()
{
  if (hMap)
   {
    map->mapCloseData(hMap);
    hMap = 0;
   }
}

//=============================================================================
//============ ������ ������� ����������� ���� ����� ==========================
//=============================================================================
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

//=======================================================================================
//====================== ����������� ���� ===============================================
//=======================================================================================
void MapScroll::drawContents( QPainter* p, int cx, int cy, int cw, int ch)
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


//=====================================================================
//============= �������� ����� ========================================
//=====================================================================
int	MapScroll::mapOpen(const char *name)
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

  if ((_stricmp(extname,".map") == 0) || (_stricmp(extname,".sit") == 0) ||
      (_stricmp(extname,".rsw") == 0) || (_stricmp(extname,".mtw") == 0))
  {
    hMap=map->mapOpen(name, 0);
  }            
                                                                  
 
   if (hMap)
   {
      long int	mapW, mapH;
	  
	  mapBaseScale = getScale();  //����������� �������� �������� �����
	 
	 //setViewScale(16000000);//���������� �������, � ����� ���������� ��������� �����  (����� ������ �� ��������)

	  map->mapGetPictureSize(hMap,&mapW,&mapH);
     
      if (MyViewport == 0)  
      {
        MyViewport = new QWidget(viewport());
        MyViewport->setAttribute(Qt::WA_NoSystemBackground);
		MyViewport->setMouseTracking(true);
        setWidget(MyViewport);
      }
      else MyViewport->show();
      //��������� �������� �����������
      MyViewport->setGeometry(0, 0, mapW, mapH);

	  setFocus();
      MyViewport->repaint();
	  return hMap;
  }
  else
  {
	return 0;
  }
}


//=====================================================================
//====================== ����� �������� ����� =========================
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
//==== ����� ��������� ����������� � ����� ����� =====================
//====================================================================
void MapScroll::setMapCenter()
{
	if (hMap == 0) return;
	long int X,Y;
	long int mapW, mapH;

	map->mapGetPictureSize(hMap,&mapW,&mapH);
	
	horizontalScrollBar()->setMaximum(mapW);
	verticalScrollBar()->setMaximum(mapH);

	//�������� �����
	X = mapW/2;
	if (X < 0) X = 0;
	Y = mapH/2;
	if (Y < 0) Y = 0;

	horizontalScrollBar()->setValue(X);
	verticalScrollBar()->setValue(Y);

}


//====================================================================
//==== ����� ��������� �������� "<" ">" ����������� ����� ============
//====================================================================
void MapScroll::changeScale(float change)
{
	if (hMap == 0) return;
	long int X,Y;
	double x1,y1, a, b;
	long int mapW, mapH;
	//�������� ������� �����

	int hMaximum = horizontalScrollBar()->maximum();
	int vMaximum = verticalScrollBar()->maximum();


	X = horizontalScrollBar()->value() + viewport()->width() / 2;
	Y = verticalScrollBar()->value() + viewport()->height() / 2;
	
    map->mapChangeViewScale(hMap,&X,&Y,change);
	map->mapGetPictureSize(hMap,&mapW,&mapH);
	
	MyViewport->hide();
	//��������� �������� �����������
	MyViewport->resize(mapW, mapH);
	horizontalScrollBar()->setMaximum(mapW);
	verticalScrollBar()->setMaximum(mapH);
	
	//�������� ����� �����
	X = X - viewport()->width() / 2;
	if(X<0) X = 0;
	Y = Y - viewport()->height() / 2;
	if(Y<0) Y = 0;

	horizontalScrollBar()->setValue(X);
	verticalScrollBar()->setValue(Y);
	MyViewport->show();
}

//==================================================================
//========= ��������� ����� �� �������� �����-������ ===============
//==================================================================
void MapScroll::changePos(long int dx,long int dy)
{
  if (hMap == 0) return;
  long int X,Y;

  long int mapW, mapH;
  map->mapGetPictureSize(hMap,&mapW,&mapH);
  
  X = horizontalScrollBar()->value() + dx;
  Y = verticalScrollBar()->value() + dy;
  if (X > mapW - viewport()->width()) X = mapW - viewport()->width();
  if (Y > mapH - viewport()->height()) Y = mapH - viewport()->height();
  if (X < 0) X = 0;
  if (Y < 0) Y = 0;

  horizontalScrollBar()->setValue(X);
  verticalScrollBar()->setValue(Y);
  updateScreen();
}


//==========================================================================
//===== ����� ��������� ������� ������� ������ ���� ========================
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
				
				if (event->button() == Qt::LeftButton)//����� ������� ����
				{					
					if(moveFlag)
					{
						emit selectedPoint(screenX, screenY);  //������ ��� �������� ����� ����� ��� ����������� ������� 
					}
					else
					{
						emit leftButtonClicked(pe,allObjectsList); return;
					}
				}
				if (event->button() == Qt::RightButton)  //������ ������� ����
				{
					//emit rightButtonClicked(pe,idOdject,objectType); return;	
					emit rightButtonClicked(pe,allObjectsList); return;
				}
			
			}
			else
			{
				if (event->button() == Qt::LeftButton)//����� ������� ����
				{					
					if(moveFlag)
					{
						emit selectedPoint(screenX, screenY);  //������ ��� �������� ����� ����� ��� ����������� ������� 
					}
				}

				if (event->button() == Qt::RightButton)  //������ ������� ����
				{
					//emit rightButtonClicked(pe,0,0); return; //���� �� ������ ����� (��� ��� ��������)	
					emit rightButtonClicked(pe,allObjectsList); return;
				}
				return;
			}
		}
	}
}


//======================================================================
//======= ����� ��������� �������� ���� �� ����� =======================
//======= ������� ���������� � ������ ��������� =======================
//======================================================================
void MapScroll::mouseMoveEvent(QMouseEvent * event)
{
	if(event->Move)
	{
		QPointF a,mouse_pos;
		double x,y;
		if (hMap)
		{
			mouse_pos = event->pos();
			x = mouse_pos.x();
			y = mouse_pos.y();
			a = getXY(x,y);
			emit cursorIsMoved(a);
		}
		
	}
}



//=====================================================================
//===== ����� ���������� ������ �������� �������� ����� � �������: ====
//===== 17501 - idObject; 17502 - ��� ������� =========================
//=====================================================================
QStringList	MapScroll::getObjectIdAndTypeInfo(double *x, double *y)
{
	QStringList semList;
	info=map->mapCreateObject(hMap);
	changeFrame(50);  // ���������� ������� ������ �������
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
//===== ����� ���������� ������ ������� �������� �������� ������ � �������: ====
//===== 17501 - idObject; 17502 - ��� ������� ==================================
//===== ����������� ����� ���� �������� � �������� ����� =======================
QList<QStringList>	MapScroll::getAllObjectsIdAndTypeInfo(double *x, double *y)
{
	QList<QStringList> signsList;
	
	QStringList semList;
	info=map->mapCreateObject(hMap);
	changeFrame(512);  // ���������� ������� ������ �������
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
//==== ����� ��������� ������� ������ �������, ������� �� ��������� ����� ���� ===
//================================================================================
void MapScroll::changeFrame(int pixels)
{
	dframe.X1 = screenX+pixels;
	dframe.X2 = screenX-pixels;
	dframe.Y1 = screenY+pixels;
	dframe.Y2 = screenY-pixels;
	frame = dframe;
}



//============================================================================
//====�����  �������� ����������������� ���� =================================
//============================================================================
HSITE MapScroll::openSit(HMAP hMap, const char * mapname, const char * rscname)
{
	memset((void*)&createsite,0,sizeof(createsite));
	createsite.Length = sizeof(createsite);
	strcpy(createsite.MapName, mapname);

	createsite.MapType=COUNTGEOG;//�������-��������������
	createsite.MaterialProjection=CONICALORTHOMORPHIC;//������������ �������������
	long int scale = mapBaseScale;

	createsite.Scale = scale;
	map->mapCreateAndAppendSite(hMap,mapname,rscname,&createsite);
	return  map->mapOpenSiteForMap(hMap,mapname,0);
}



//=======================================================================
//============= �������� ���������������� ����� =========================
//=======================================================================
void MapScroll::closeSit(HMAP hMap, HSITE hsite)
{
	if (hsite!=0)
	{
		map->mapCloseSiteForMap(hMap,hsite);
	}
}

//======================================================================
//==== �������� ���������������� ����� �� �� ����� (���� � �����) ======
//======================================================================
void MapScroll::closeSitByName(HMAP hMap, const char * sitName)
{
	map->mapCloseSiteForMapByName(hMap,sitName);
}


//=======================================================================
//======= ���������� ������������ ���������������� ����� ================
//=======================================================================
long int MapScroll::appendData(const char* sitname)
{
	if (hMap)
	{
		return map->mapAppendData(hMap, sitname);
	}
	return 0;
}


//=======================================================================
//======= �������� ���������� ����� =====================================
//=======================================================================
long int	MapScroll::IsActive(HMAP hMap)
{
	return map->mapIsActive(hMap);
}


//=======================================================================================
//======== ���������� ����� ����� ���������������� ����� ================================
//=======================================================================================
long int MapScroll::GetSiteLayerCount(HMAP hMap,HSITE hSite)
{
	return map->mapGetSiteLayerCount(hMap,hSite);
}


//=======================================================================
//========= �������� ����������� � �������� ������ ======================
//=======================================================================
long int MapScroll::updateScreen()
{
	MyViewport->hide();
	MyViewport->show();
	return 0;
}

//======================================================================
//======= �������� �� ��������� ������ � ������������� �� ����� ========
//======================================================================
QPoint MapScroll::getXY(double x, double y)
{
	QPoint a;
	x += horizontalScrollBar()->value();
	y += verticalScrollBar()->value();		
	map->mapPictureToPlane(hMap, &x, &y);
	a.setX(x);
	a.setY(y);
	return a;
}


//==============================================================
//== ������ ���������� ������������� ���������� ����� ����� ====
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
//=========== ����� �������� ���������������� ==================
//==============================================================
long int MapScroll::setObjectNoScale(HOBJ hobj)
{
	return map->mapSetObjectScale(hobj,0);
}

//���������/���������� ������� ��������� ��������
long int MapScroll::setObjectTopScale(HOBJ hobj, int scale)
{
	return map->mapSetObjectTopScale(hobj,scale);
}



//=====================================================================
//== ����� ���������� ������� �������� ��������������� ��������� ======
//=====================================================================
int	MapScroll::HScrollBarValue()
{
	return horizontalScrollBar()->value();
}
//=====================================================================
//== ����� ���������� ������� �������� ������������� ��������� ========
//=====================================================================
int	MapScroll::VScrollBarValue()
{
	return verticalScrollBar()->value();
}

 // ��������� ����� ����� �� �����
 // hmap - ������������� �������� ������
 // ��� ������ ���������� ����
long int MapScroll::getLayerCount()
{
	return map->mapGetLayerCount(hMap);
}


//===================================================================
//=========== ��������� ������� ����� ===============================
//===================================================================
long int MapScroll::getMapBright()
{
	return map->mapGetBright(hMap);
}

//==================================================================
//=========== ���������� ������� ==================================
//==================================================================
long int	MapScroll::setMapBright(long int bright)
{
	return map->mapSetBright(hMap,bright);
}

//==================================================================
//=========== ��������� �������� ==================================
//==================================================================
long int	MapScroll::getMapContrast()
{
	return map->mapGetContrast(hMap);
}


//==================================================================
//=========== ���������� �������� ==================================
//==================================================================
long int MapScroll::setMapContrast(long int contrast)
{
	return map->mapSetContrast(hMap, contrast);
}


//==================================================================
//============= ��������� ������� ������ ===========================
//==================================================================
long int MapScroll::getMapIntensity()
{
	return map->mapGetIntensity(hMap);
}


//==================================================================
//=============== ���������� ������� ������ ========================
//==================================================================
long int MapScroll::setMapIntensity(long int intensity)
{
	return map->mapSetIntensity(hMap, intensity);
}


//======================================================================
//============= ����������� ============================================ ?????????????????????????????????????????????????????????????
//======================================================================
void MapScroll::paint95()
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


//===================================================================================
//=========== ��������� ����������� ������� ����������� ����� =======================
//===================================================================================
long int MapScroll::getScale()
{
	return map->mapGetShowScale(hMap);
}


//===================================================================================
//============== �������� �������� ������� �� �������������� �� ���� ================
//===================================================================================
long int MapScroll::objectInfoFromRsc(HOBJ Info, const char * name)
{
	name = map->mapObjectName(Info);
	return 0;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
long int	MapScroll::createObject(long int hSit,  QList<Coord*> &coordinates, const char * rscKey, QMap<long int,QString> semantics)
{
	info = map->mapCreateSiteObject(hMap,hSit);

	map->mapRegisterObjectByKey(info, rscKey);
	
    if(!semantics.isEmpty())
	{
        QMap<long int,QString>::iterator it = semantics.begin();
        for (;it != semantics.end(); ++it)
		{	
			long int semCode = it.key();
			QString semValue = it.value();
			map->mapAppendSemantic(info, semCode, semValue.toStdString().c_str(), semValue.size());
		}
	}



	//-- ���������� ������� ������� �� ������ ��������� ----
    for(int i=0;i<coordinates.count();i++)
	{
		double x,y;
        x = coordinates.at(i)->getX();
        y = coordinates.at(i)->getY();
		map->mapAppendPointPlane(info, x, y);
	}
	//-----------------------------------------------------


	long int commitFlag = map->mapCommitWithPlace(info);

	map->mapClearObject(info);
	
	return commitFlag;
}




//������� ���������
long int MapScroll::planeToGeo423D(double *Bx, double *Ly, double *H)
{
	return map->mapPlaneToGeo423D(hMap, Bx, Ly, H);
}

void MapScroll::degreeToRadian(GEODEGREE * degree, double * radian)
{
	map->mapDegreeToRadian(degree, radian);
}
long int MapScroll::geoWGS84ToPlane3D(HMAP hMap, double *Bx, double *Ly, double * H)
{
	return map->mapGeoWGS84ToPlane3D(hMap, Bx, Ly, H);
}
void MapScroll::radianToDegree(double * radian, GEODEGREE * degree)
{
	map->mapRadianToDegree(radian, degree);
}
//�������������� �� ������� ���������
long int MapScroll::isGeoSupported()
{
	return map->mapIsGeoSupported(hMap);
}

HPRINTER MapScroll::loadPrinter()
{
	return map->prnLoadPrinter();
}
 // �������������� �� ������ �� ��������� (�������� �����) � ������������� ���������� � �������� (���������� ��������� WGS84)
long int MapScroll::planeToGeoWGS843D(double *Bx,  double *Ly,  double *H)
{
	 if (isGeoSupported())
    {
      return map->mapPlaneToGeoWGS843D(hMap, Bx, Ly, H);
    }
	return 0;
}


//������� �� ��������� � ������ � WGS
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

////////////////////////////////////////////////////////////????????????????????????????????????????????????????
void MapScroll::paintInDevice(HDC hdc, RECT * rect)
{
	map->mapPaint95(hMap, hdc, 0, rect);
}

//=====================================================================================================
//============ �������� �������� ������������� �������������� ������� =================================
//=====================================================================================================
long int MapScroll::changeSemanticValue(HOBJ info, long int number, char * place, long int maxsize)
{
	return map->mapSetSemanticValue(info, number, place, maxsize);
}

//================================================================================
//========== �������� ������ =====================================================
//================================================================================
long int MapScroll::openRstOnMap(const char * rstname)
{
	return map->openRstForMap(hMap, rstname, GENERIC_READ);
}	

//===============================================================================
//============= �������� ������ =================================================
//===============================================================================
long int MapScroll::closeRstForMap(long int number)
{
	long int a = map->closeRstForMap(hMap, number);
	updateScreen();
	return a;
}

//================================================================================
//=============== ���������� ����� ��� ������ ====================================
//================================================================================
long int MapScroll::setRstOnMap(long int number)
{
	return map->setRstViewOrder(hMap, number, 1);
}
