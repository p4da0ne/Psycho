#ifndef MAPSCROLL_H
#define MAPSCROLL_H

#include <QScrollArea>
#include <QtGui>
#include <QTextCodec>
#include <QMap>
#include <QList>
#include <mapapi.h>
#include <QMenu>
#include "coord.h"

#include "../my_mapaccess/my_mapaccess.h"

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
	void			findObject(double *x, double *y);
	HSITE			openSit(HMAP hMap, const char * mapname,const char * rscname);
	
	long int		changeSitViewOrder(HSITE site, long int newNumber);
	
	
	long int		appendData(const char* sitname);
	long int		createV0(long int hSit, double x1, double y1, double x2, double y2, const char * name_ff, long int id_obj, int flag=0, long int id_coord=0);
	long int		createObject(long int hSit, double x, double y, const char * name_ff, long int id_obj=0, long int flag=0, bool if_shtab=FALSE, QString semantic="", long int id_coordintes=0, float angle=0);
	
	
	long int createObjectTest(long int hSit,  QList<Coord*> *coordinates, const char * rscKey, QMap<long int,QString> *semantics=0);
	
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////////
	
	long int		createLine(long int hSit, const char * name_ff, long int id_obj);
	long int		getScale();
	void			IsObject(HOBJ hobj);
	long int		IsActive(HMAP hMap);
	long int		GetSiteLayerCount(HMAP hMap,HSITE hsite);
	long int		GetError();
	long int		updateScreen();
	QPoint			getXY(double x, double y);
	void			closeSit(HMAP hMap, HSITE hSite);
	void			closeSitByName(HMAP hMap, const char * sitName);
	double			getMapX1(HMAP hMap);
	double			getMapX2(HMAP hMap);
	double			getMapY1(HMAP hMap);
	double			getMapY2(HMAP hMap);
	long int		setObjectNoScale(HOBJ hobj);
	long int		objectTopScale(HOBJ hobj);
	long int		objectByMap(HMAP hMap, HOBJ object);
	QStringList		getHobj(double *x, double *y);

	QStringList		getObjectIdAndTypeInfo(double *x, double *y);
	QList<QStringList>	getAllObjectsIdAndTypeInfo(double *x, double *y);  //поиск всех объектов в заданной точке

	int				HScrollBarValue();
	int				VScrollBarValue();
	long int		getLayerCount();
	long int		setViewScale(float scale);
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
	void			emitSignalForPlanner(long int  id_targeting_version, long int id);
	void			paintInDevice(HDC hdc, RECT * rect);
		// Изменить значение семантической характеристики объекта
	long int		changeSemanticValue(HOBJ info, long int number, char * place, long int maxsize);
	//РАСТР
	long int		openRstOnMap(const char * rstname);
	long int		closeRstForMap(long int number);
	long int		setRstOnMap(long int number);
private:
	void		Delete();
	void		changeObjCoord();
	void		changeHallCoord();
	void		appointWeapon(double *x, double *y);
	void		changeAngleWithMouse(double *x, double *y);
signals:
	void		signal_for_info(long int id_odject, long int flag);
	void		signal_for_right_button(long int hobj, QPoint pe, int flag_who, long int id_coordinates);
	void		signalFor1Action(double X, double Y);
	void		signalFor2Action(double X, double Y);
	void		signalFor3Action(long int id_odject);
	void		signalFor4Action(double X, double Y);
	void		signalForPlanner(long int id_targeting_version, long int id);
	void		signal_for_change_scale(QPoint pe);

	//------ Сигналы для обработки нажатий кнопок мыши --------------
	//void leftButtonClicked(QPoint pe, int idOdject=0, int objectType=0);
	//void rightButtonClicked(QPoint pe, int idOdject=0, int objectType=0);

	void leftButtonClicked(QPoint pe, QList<QStringList> objectsList);
	void rightButtonClicked(QPoint pe, QList<QStringList> objectsList);
	//---------------------------------------------------------------
protected:
    bool  eventFilter (QObject * watched, QEvent * event);
    void  drawContents( QPainter* p, int cx, int cy, int cw, int ch);
	void  mousePressEvent(QMouseEvent * event);
	//void  addStatusBar();

public:
	enum ReservedSemantics {ID_OBJECT = 17501, OBJECT_TYPE = 17502};
	MyMapAccess						* map;
	HMAP							hMap;  // Идентификатор карты
	CREATESITE						createsite;
	HSITE							hSite;   //объекты
	HSITE							hSite_s;  //стрелки
	HSITE							hSite_w;  //средства поражения
	HSITE							hSite_p;  //целераспределение значками
	HSITE							hSite_line;   //граница
	HSITE							hSite_corr;  //коридоры пролета
	HSITE							hSite_ok_weapon;  //выделить подходящие для уничтожения средства поражения
	HIMAGE							himage;
	QTextCodec						* MainCodec;
	HINSTANCE						LibInst;
	DFRAME							dframe;
	MAPDFRAME						frame;
    RECT							RectDraw;          // Область отрисовки карты
    QWidget							* MyViewport;
	HSELECT							select;
	QRect							rect;
	long int						brightness;
	long int						for_info;
	double							X[71];
	double							Y[71];
	double							X1[40];
	double							Y1[40];
	HOBJ							info;
	HOBJ							hObj;
	QPoint							pe;
	double							screenX, screenY;
    QList <QList<double> >			war_line_coord;
	//delete
	long int						flag1; //true - если семантика 17501 в объекте заполнена
	bool							flag2; //true - если открыт хоть один пользовательский слой
	int								flag;  //обработка нажатия правой мыши
	int								number_action; //номер выбранного действия для реакции левой клавиши мыши
	long int						x_for_change_angle, y_for_change_angle;
	int								dx, dy;
private:
	//delete
	QDialog		* info_dialog;
	QLabel		* info_label01;
	QLabel		* info_label02;
	QLabel		* info_label1;
	QLabel		* info_label11;
	QLabel		* info_label2;
	QLabel		* info_label21;
	QLabel		* info_label3;
	QLabel		* info_label31;
	//delete
	QMenu		* mouse_menu;
};

#endif
