#ifndef MAPSCROLL_H
#define MAPSCROLL_H

#include <QScrollArea>
#include <QtGui>
#include <mapapi.h>
#include <QMenu>
#include "calculating_mps.h"
#include "Calculate_K_omkrf.h"


#include "../my_mapaccess/my_mapaccess.h"
//#include <../saturn/calculatingproblemmanager/calculatingproblemmanager.h>
//#include <../saturn/calculatingproblemmanager/Calculate_K_omkrf.h>

// Окно для отображения карты
class MapScroll : public QScrollArea
{
    Q_OBJECT
public:
    MapScroll(QWidget * parent = 0);
    ~MapScroll();
    int			mapOpen( const char *name );
    void		ChangePos(long int dx,long int dy);
    void		ChangeScale(float);
    void		closeMap();
	void		changeFrame();
	long int	appendData(const char* sitname);
	void		findObject(double *x, double *y);
	long int	findObject1(double *x, double *y);
	void	findObject2(double *x, double *y);

	void		RegionAppendSemantic(HOBJ hobj, QString semantic);

	HSITE		dataOpen(HMAP hMap, const char * mapname,const char * rscname, CREATESITE * createsite);
	long int	createObject(long int hSit, double x, double y, const char * name_ff, long int id_obj);
	long int	createObject(long int hSit, double x, double y, const char * name_ff, long int id_obj, QMap<int,QString> semantic_map);
	long int	createLineObject(long int hSit, double x, double y, const char * name_ff, long int id_obj);
	void		IsObject(HOBJ hobj);
	long int	IsObject1(HOBJ hobj);

	long int	IsActive(HMAP hMap);
	long int	GetSiteLayerCount(HMAP hMap,HSITE hSite);
	long int	GetError();
	long int	UpdateScreen();
	QPoint		getXY(double x, double y);
	void		dataClose(HMAP hMap, HSITE hSite);
	double		getMapX1(HMAP hMap);
	double		getMapX2(HMAP hMap);
	double		getMapY1(HMAP hMap);
	double		getMapY2(HMAP hMap);
	long int	setObjectNoScale(HOBJ hobj);
	long int	objectTopScale(HOBJ hobj);
	long int	objectByMap(HMAP hMap, HOBJ object);
	void		mouseRightMenu();
	QStringList		getHobj(double *x, double *y);
	int			HScrollBarValue();
	int			VScrollBarValue();
	long int	getLayerCount();
	long int	setViewScale(float scale);
	long int	getmapBright();
	long int	setmapBright(long int bright);
	void		paint95();
	void		findObjectByCod();
    void        freeObject(HOBJ hobj);
    void        deleteObject(HOBJ hobj);

signals:
	void		signal_for_info(long int hobj);
	void		signal_for_right_button(HOBJ hobj, long int num_obj, long int id_object, QPoint pe, bool region);
	void		signal_for_info_2_arg(QString a, long int b);
    void		signal_for_change_scale(QPoint pe,HOBJ hobj, long int num_obj, long int id_object, bool region);
protected:
    bool  eventFilter (QObject * watched, QEvent * event);
    void  drawContents( QPainter* p, int cx, int cy, int cw, int ch);
	void  mousePressEvent(QMouseEvent * event);
	//void  addStatusBar();
public:
	MyMapAccess * map;
	HMAP		hMap;  // Идентификатор карты
	HSITE		hSite;    
	HIMAGE		himage;
	QTextCodec	* MainCodec;
	HINSTANCE	LibInst;
	DFRAME		dframe;
	MAPDFRAME	frame;
    RECT		RectDraw;          // Область отрисовки карты
    QWidget		* MyViewport;
	HSELECT		select;
	QRect		rect;
	long int	brightness;
	long int	for_info;
	HOBJ		info;
	HOBJ		hObj;
	//delete
	bool		if_create;
	QPoint		pe;
	double		screenX, screenY;
	//delete
	long int	flag1; //true - если семантика 17501 в объекте заполнена
	bool		flag2; //true - если открыт хоть один пользовательский слой
	bool		flag;  //если при нажатии правой клавишей мыши на нашем объекте - TRUE, иначе FALSE
	bool		if_open_sit;
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
	Calculate_K_omkrf calc1;
	calculating_mps		calc;
};

#endif
