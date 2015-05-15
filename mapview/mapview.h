#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QDialog>
#include <QStatusBar>
#include <QCheckBox>

#include "config_mapview.h"
#include "mapscroll.h"
#include "view_manage.h"
#include <QPrinter>

#define SETTINGS_ORGANIZATION "vka"
#define SETTINGS_APPLICATION "saturnMap"


class __EXPORT_MAPVIEW MapView : public QWidget
{
    Q_OBJECT

public:
    MapView( QWidget *parent=0, const char *name=0 );
    ~MapView();
	void			keyPressEvent(QKeyEvent *e);
	void			mouseMoveEvent(QMouseEvent * event);
	void			selectAllObject();
private:
	void initToolButtonsPanel();																//++++++
	void initSaturnLeftMenu();																	//++++++			

	bool			openMap(QString mapFilepath = "");											//++++++
	HSITE			openMapSit(QString sitFileName, QString rscFilePath);						//++++++
	void			closeSitByName(QString sitFileName);										//++++++

	//------ Метод для отрисовки условных знаков на пользовательской карте -------------
	void			createSitObjects(HSITE hSite,QList<SignData*> signsList);					//++++++
	

	//------------------------------------------------------------------------
	QMenu* createGreateLessScaleMenu();															//++++++
	QMenu* createObjectsListMenu(QList<QStringList> objectsList);							    //++++++
	QMenu* createObjectsListComplexMenu(QList<QStringList> objectsList);											//++++++



	void			showInformationDialog(QString information);
	
	void			setAdditionalInfo();
	void			setStatusInfo(QString status);

	HSITE   		openMapSitWeapon();
	HSITE   		openMapSitForLine();
	HSITE			openMapSitForCorridors();
	HSITE			openMapSitForPicture();
	HSITE			openMapSitForOkWeapon();
	HSITE			openMapSitFor();
	bool			ifShtab(QString code);
	//int				get_targeting_version();
	//int				get_operation_combo();
	//int				getDirectionCombo();
	void			showOpenMapDialog();					
	void			shortOfGunDialog();			
	//inline bool		get_object_checkbox();
	//inline bool		get_weapon_checkbox();
	//inline bool		get_corr_checkbox();
	//inline bool		get_lbs_checkbox();
	//inline bool		get_znaki_checkbox();//знаки - mark
	//inline bool		get_line_checkbox();
	//inline bool		getMarkDirCheckbox();
	//inline bool		getLineDirCheckbox();
	void			paintAllObject();
	void			paintWeapon();
	void			paintLine();
	void			paintCorridors();
	void			paintPlaner();
	void			paintPlanerDirection();
	void			joinObjectWithWeapon();
	void			joinObjectWithWeaponDirection();
	void			WGS_to_other();
	void			apdateInDatabase();
	GEODEGREEXY		doubleToGeodegree(double *x, double *y, double *h);

public slots://на запросы
	void			errors_message(QString str);
	void			showInfoAboutObject();

	void			mouseRightMenu(long int id_obj, QPoint pe, int semantic_flag, long int id_coordinates);
	


	void			showPositionWGSMouseSlot(double X, double Y, double H=0);
	void			showPositionHallMouseSlot(double X, double Y, double H=0);
	void			showAppointMouseSlot(long int id_object);

private slots:

	void			openNewMap();
	void			openMapFromSettings();
	void			openRST();
	void			closeRST();
	void			closeRST1();
    void			lessScale();
    void			greateScale();
    void			closeMap();
	void			closeAllRST();
	void			changeBrihgtUp();
	void			changeBrihgtDown();
	void			changeContrastUp();
	void			changeContrastDown();

	void			slotObjectInfo(); //слот - обработчик выбора в контекстном меню объекта
	void			slotObjectDescription();
	void			slotObjectReport();
	void			slotFormationPsiLooses();
	void			slotRegionMpos();
	//==========================
	//------------ Обработчики нажатий на кнопки мыши (общие для разных программных комплексов) ------------------
	void			slotMouseLeftButtonClicked(QPoint pe, QList<QStringList> objectsList);
	void			slotMouseRightButtonClicked(QPoint pe, QList<QStringList> objectsList);
	void			mouseRightSimpleMenu(QPoint pe);
	//======================================================

	//---- Меню по нажатию правой кнопки мыши (специфичные для конкретного программного комплекса) -------------------
	QMenu*			createFormationsMenu(QStringList objInfo);
	QMenu*			createSpecialConditionsMenu(QStringList objInfo);
	QMenu*			createSmiMeansMenu(QStringList objInfo);
	QMenu*			createFormationsMeansMenu(QStringList objInfo);
	QMenu*			createGroupsMeansMenu(QStringList objInfo);
	QMenu*			createRegionsMenu(QStringList objInfo);
	
	//----------------------------------------------------------------------



	void			showShortInformationObject(long int id_obj, long int flag);
	void			moreButtonClicked();
	void			test();

	void			appendSit_Test();

	void showCheckedObjects();
	void showCheckedCalcResults();
	
	void			showOnlyUnAllocation();
	void			checkPaintCel();
	void			checkPaintDirection();
	void			changeObjectCoord();
	void			redrawWithNewAngle(double X, double Y);
	void			changeAngleWithMouse();
	void			changeHallCoord();
	void			PrintMapSlot();
	void			PrintScreenSlot();
	void			closeDhangeDoordDialog();
	void			closeAppointDialog();
	void			closeChangeHollCoordDialog();
public:
    QTextCodec		* MainCodec;
    MapScroll		* mapwin;
	ViewManage		* model;
	QLineEdit		* cursor_coord;
	QPoint			mouse_pos;
	QStatusBar		* statusBar;
	QDialog			* info_dialog;
	QDialog			* create_object_dialog;
	QDialog			* bright_dialog;
	QDialog			* openMapDialog;
	QDialog			* change_coord_dialog;
	long int		Id_obj;
	long int		Id_coordinates;
	bool			if_open_sit;
	QPoint			pe;
	HDC				hdc;

protected:
	double			koef_mah1;
	double			koef_mah2;

private: 
	QVBoxLayout *vertLayout;
	QHBoxLayout *centralLayout;

	//---- Чекбоксы фильтра отображения -----
	QCheckBox *smi_means_checkbox;
	QCheckBox *formation_means_checkbox;
	QCheckBox *organization_means_checkbox;
	QCheckBox *formations_checkbox;
	QCheckBox *conditions_checkbox;
	//---- Чекбоксы фильтра расчетных задач -----
	QCheckBox *mpo_regions_checkbox;
	QCheckBox *mps_our_Mil_checkbox;
	QCheckBox *mps_enemy_checkbox;
	QCheckBox *psi_looses_checkbox;
	//--------------------------------------

	QSettings *settings;

	QMenu			* mouse_menu;
	QLabel			* mouse_x; 
	QLabel			* mouse_y; 
	QDialog			* info_close_dialog;
	QDialog			* short_info_dialog;
	QDialog			* appointWeaponDialog;
	QDialog			* weapon_listdialog;
	QDialog			* short_gun_dialog;
	QDialog			* closeRSTdialog;
	QAction			* showAction;
	QDialog			* change_hall_dialog;
	QAction			* changeCoordObjectAction;
	QAction			* appointWeapon;
	QAction			* dissolutionPlanAction;
	QAction			* change_angle_action;
	QGroupBox		* secondary_group_box;
	QGroupBox		* group_box1;
	QGroupBox		* group_box2;
	QGroupBox		* group_box3;
	QLineEdit		* additional_info;
	QLineEdit		* status_bar;
	QPushButton		* more_but;
	QComboBox		* weapon_combobox;
	QComboBox		* version_combobox;
	QComboBox		* operation_combobox;
	QComboBox		* direction_combobox;
	QString			curFile;
	QLineEdit		* lineEdit;

	QLineEdit		* x_edit_g;//--
	QLineEdit		* x_edit_m;// --
	QLineEdit		* x_edit_s;//  --
	QLineEdit		* y_edit_g;//   -- для перемещения объекта
	QLineEdit		* y_edit_m;//  --
	QLineEdit		* y_edit_s;// --
	QLineEdit		* h_edit_h;//--
	QTableWidget	* tableWidget;
	QScrollArea		* scrollarea;
	QList <int>		group_weapon_list;
	QList <int>		number_int;
	QStringList		rst_name_and_number;  //открытых растров
	GEODEGREEXY		G_XY_c;//центр при повороте знака
	
	


	
};

#endif
