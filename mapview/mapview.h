#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QDialog>
#include <QStatusBar>
#include <QDate>


#include <mapapi.h>
#include "config_mapview.h"
#include "mapscroll.h"
#include "view_manage.h"
#include "calculating_mps.h"
#include "People_Losses.h"
#include "../my_mapaccess/my_mapaccess.h"
//#include "Calculate_K_omkrf.h"


class __EXPORT_MAPVIEW MapView : public QWidget
{
    Q_OBJECT

public:
    MapView( QWidget *parent=0, const char *name=0 );
//    ~MapView();
    void	keyPressEvent(QKeyEvent *e);
	void	mouseMoveEvent(QMouseEvent * event);
	void	selectAllObject();
	void	selectAllObject1();
	void	selectAllObject2();

private:
	void	showInformationDialog(QString information);
public slots://на запросы
	void	errors_message(QString str);
	void	showInfoAboutObject(long int id_obj);
	void	mouseRightMenu(HOBJ hobj, long int id_object, long int id_obj, QPoint pe, bool region);
private slots:
	void	selectLineObject();
    void	open();
    void	LessScale();
    void	GreateScale();
    void	closeMap();
	void	showCloseSitInfo();
	void	changeBrihgtUp();
	void    appendSit();
	void	changeBrihgtDown();

	void	calc_mps();
	void	calc_mps2();
	void	People_Losse();
	//==========================
	void	opacityMap();
	/////////////////////////////
	void test_view_manage();
	void test_view_manage2();
	void showShortInformationObject(long int id_obj);
	void showShortInformationObject_2arg(QString a,long int id_obj);
	HSITE   openMapSit();
	HSITE   openMapSit1();
public:
	MyMapAccess * map;
    QTextCodec  * MainCodec;
    MapScroll	* mapwin;
	ViewManage  * model;
	CREATESITE    createsite;
	HSITE		  hSite;
	HSITE		  hSite1;

	//HMAP		hMap;  // Идентификатор карты
	//HSELECT		select;
	HOBJ		info;

	QLineEdit	* cursor_coord;
	QPoint		  mouse_pos;
	QWidget		* MyViewport;
	QStatusBar  * statusBar;
	QDialog		* info_dialog;
	QDialog		* create_object_dialog;
	QDialog		* bright_dialog;
		long int	flag1; //true - если семантика 17501 в объекте заполнена
	bool		flag2;//true - если открыт хоть один пользовательский слой
	bool		flag;//если при нажатии правой клавишей мыши на нашем объекте - TRUE, иначе FALSE
	bool		if_open_sit;
	bool		if_create;
	QPoint		pe;
	double		screenX, screenY;
protected:
	QString		curFile;
	QString		File;
	QLineEdit	* lineEdit;
private: 
	QMenu		* mouse_menu;
	QLabel		* mouse_x; 
	QLabel		* mouse_y; 
	QDialog		* info_close_dialog;
	QDialog		* short_info_dialog;
	Calculate_K_omkrf calc1;
	/*calculating_mps		calc;*/
//	QLabel      * lbl;
};

#endif
