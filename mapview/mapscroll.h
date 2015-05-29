#ifndef MAPSCROLL_H
#define MAPSCROLL_H

#include <QScrollArea>
#include <QtGui>
#include <QTextCodec>
#include <QMap>
#include <QList>
#include <mapapi.h>
#include <QMenu>
#include "../coord/coord.h"

#include <my_mapaccess.h>

// Окно для отображения карты
class MapScroll : public QScrollArea
{
    Q_OBJECT
public:
    MapScroll(QWidget * parent = 0);
    ~MapScroll();
    int				mapOpen( const char *name );
    void			changePos(long int dx,long int dy);
    void			changeScale(float change);
    void			closeMap();
	void			changeFrame(int pixels=50);
	HSITE			openSit(HMAP hMap, const char * mapname, const char * rscname);
	long int		appendData(const char* sitname);
	long int		createObject(long int hSit,  QList<Coord*> *coordinates, const char * rscKey, QMap<long int,QString> *semantics=0);
	void			setMapCenter();
	long int		getScale();
	long int		IsActive(HMAP hMap);
	long int		GetSiteLayerCount(HMAP hMap,HSITE hsite);
	long int		updateScreen();
	QPoint			getXY(double x, double y);
	void			closeSit(HMAP hMap, HSITE hSite);
	void			closeSitByName(HMAP hMap, const char * sitName);
	double			getMapX1(HMAP hMap);
	double			getMapX2(HMAP hMap);
	double			getMapY1(HMAP hMap);
	double			getMapY2(HMAP hMap);
	long int		setObjectNoScale(HOBJ hobj);
	long int		setObjectTopScale(HOBJ hobj,int scale = 15000000);
	QStringList		getObjectIdAndTypeInfo(double *x, double *y);
	QList<QStringList>	getAllObjectsIdAndTypeInfo(double *x, double *y);  //поиск всех объектов в заданной точке
	int				HScrollBarValue();
	int				VScrollBarValue();
	long int		getLayerCount();
	long int		getMapBright();
	long int		setMapBright(long int bright);
	long int		getMapContrast();
	long int		setMapContrast(long int contrast);
	long int		getMapIntensity();
	long int		setMapIntensity(long int intensity);
	void			paint95();
	long int		objectInfoFromRsc(HOBJ Info, const char * name);
	long int		planeToGeo423D(double *Bx, double *Ly, double *H);
	long int		isGeoSupported();
	void			degreeToRadian(GEODEGREE * degree, double * radian);
	long int		geoWGS84ToPlane3D(HMAP hMap, double *Bx, double *Ly, double * H);
	void			radianToDegree(double * radian, GEODEGREE * degree);
	HPRINTER		loadPrinter();
	long int		planeToGeoWGS843D(double *Bx,  double *Ly,  double *H);
	GEODEGREEXY		pictureToWGS(double * X, double * Y, double * H);
	void			paintInDevice(HDC hdc, RECT * rect);
		// Изменить значение семантической характеристики объекта
	long int		changeSemanticValue(HOBJ info, long int number, char * place, long int maxsize);
	//РАСТР
	long int		openRstOnMap(const char * rstname);
	long int		closeRstForMap(long int number);
	long int		setRstOnMap(long int number);
signals:
	//------ Сигналы для обработки нажатий кнопок мыши --------------
	void cursorIsMoved(QPointF xyCoord);
	void selectedPoint(double X, double Y);
	void leftButtonClicked(QPoint pe, QList<QStringList> objectsList);
	void rightButtonClicked(QPoint pe, QList<QStringList> objectsList);
	//---------------------------------------------------------------
protected:
    bool  eventFilter (QObject * watched, QEvent * event);
    void  drawContents( QPainter* p, int cx, int cy, int cw, int ch);
	void  mousePressEvent(QMouseEvent * event);
	void  mouseMoveEvent(QMouseEvent * event);
public:
	enum ReservedSemantics {ID_OBJECT = 17501, OBJECT_TYPE = 17502};
	bool moveFlag;
	MyMapAccess						* map;
	HMAP							hMap;  // Идентификатор карты
	CREATESITE						createsite;
	QTextCodec						* MainCodec;
	DFRAME							dframe;
	MAPDFRAME						frame;
    RECT							RectDraw;          // Область отрисовки карты
    QWidget							*MyViewport;
	QRect							rect;
	HOBJ							info;
	QPoint							pe;
	double							screenX, screenY;
	int								mapBaseScale;
};

#endif
