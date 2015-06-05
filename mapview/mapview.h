#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QtGui>
#include <QObject>
#include <QLineEdit>
#include <QDialog>
#include <QPrinter>
#include <QCheckBox>

#include "config_mapview.h"
#include "mapscroll.h"
#include "view_manage.h"
#include "change_coord_dialog.h"


#define SETTINGS_ORGANIZATION "vka"
#define SETTINGS_APPLICATION "saturnMap"


class __EXPORT_MAPVIEW MapView : public QWidget
{
    Q_OBJECT

public:
    MapView(QWidget *parent=0);
    ~MapView();
	
protected:
	void			keyPressEvent(QKeyEvent *e);


private:
	void			initToolButtonsPanel();																
	void			initSaturnLeftMenu();
	QWidget*		createObjectPanel();
	QWidget*		createCalculatePanel();
	QWidget*		createEventPanel();
			
	Coord*			planeToWGS(long int hMap,Coord *coord);
	bool			openMap(QString mapFilepath = "");											
	HSITE			openMapSit(QString sitFileName, QString rscFilePath);						
	void			closeSitByName(QString sitFileName);										
	void			updateSite(int objectType);
	//------ Метод для отрисовки условных знаков на пользовательской карте -------------
	void			createSitObjects(HSITE hSite,QList<SignData*> signsList);					
	//------------------------------------------------------------------------
	QMenu*			createGreateLessScaleMenu();															
	QMenu*			createObjectsListMenu(QList<QStringList> objectsList);							    
	QMenu*			createObjectsListComplexMenu(QList<QStringList> objectsList);											
	void			showInformationDialog(QString information);
	void			showViewScale();
	void			changeObjectCoordInDB();
	//---- Меню по нажатию правой кнопки мыши (специфичные для конкретного программного комплекса) -------------------
	QMenu*			createFormationsMenu(QStringList objInfo);
	QMenu*			createSpecialConditionsMenu(QStringList objInfo);
	QMenu*			createSmiMeansMenu(QStringList objInfo);
	QMenu*			createFormationsMeansMenu(QStringList objInfo);
	QMenu*			createGroupsMeansMenu(QStringList objInfo);
	QMenu*			createRegionsMenu(QStringList objInfo);
	QMenu*			createPersonnelMenu(QStringList objInfo);
	QMenu*			createEventMenu(QStringList objInfo);
	//----------------------------------------------------------------------
	void			showMessageToUser(const QString);
private slots:
	void			openNewMap();
	void			openMapFromSettings();
	void			openRST();
	void			closeRST();
	void			closeSelectedRST();
	void			closeAllRST();
    void			lessScale();
    void			greateScale();
    void			closeMap();

	void			changeBrihgtUp();
	void			changeBrihgtDown();
	void			changeContrastUp();
	void			changeContrastDown();
	void			showCoordinates(QPointF xyCoord);
	void			slotObjectInfo();
	void			slotObjectReport();
	void			slotFormationPsiLooses();
	void			slotRegionMpos();
	void			slotMoveObject(); //слот обработки перемещения объекта
	void			changeObjectCoordInDB(double x, double y);
	//------------ Обработчики нажатий на кнопки мыши (общие для разных программных комплексов) ------------------
	void			slotMouseLeftButtonClicked(QPoint pe, QList<QStringList> objectsList);
	void			slotMouseRightButtonClicked(QPoint pe, QList<QStringList> objectsList);
	void			mouseRightSimpleMenu(QPoint pe);
	//======================================================
	void			showCheckedObjects();
	void			showCheckedCalcResults();
	void			showCheckedEvents();

	void			PrintMapSlot();
	void			PrintScreenSlot();

	void			slotSelectButtonToggled(bool checked);
	void			slotSearchObject();
	void			chooseSelectedObjects();
	void			chooseAllObjects();

	void			slotSelectedObjectsListViewCustomMenu(const QPoint &pe);
	void			slotRemoveOneObject();
	void			slotClearSelectedList();

private: 
    QTextCodec		* MainCodec;
    MapScroll		* mapwin;
	ViewManage		* model;
	QLineEdit		* cursor_coord;
	QDialog			* info_dialog;
	HDC				hdc;

	QVBoxLayout *vertLayout;
	QHBoxLayout *centralLayout;

	//---- Чекбоксы фильтра отображения -----
	QCheckBox *mpo_regions_checkbox;
	QCheckBox *smi_means_checkbox;
	QCheckBox *formation_means_checkbox;
	QCheckBox *organization_means_checkbox;
	QCheckBox *formations_checkbox;
	QCheckBox *conditions_checkbox;
	QCheckBox *persones_checkbox;
	//---- Чекбоксы фильтра расчетных задач -----
	QCheckBox *mps_our_Mil_checkbox;
	QCheckBox *mps_enemy_checkbox;
	QCheckBox *psi_looses_checkbox;
	//--------------------------------------

	//--- Элементы фильтра событий -------
	QDateTimeEdit *beginEventDateTime;
	QDateTimeEdit *endEventDateTime;
	QRadioButton *allObjectsButton;
	QRadioButton *selectObjectsButton;
	QListView *selectedObjectsListView;
	QStandardItemModel *selectedObjectsModel;
	QWidget *selectObjectsWidget;
	QLineEdit *searchObjectLineEdit;

	QListView *eventStatesView;
	QStandardItemModel *eventStatesModel;

	QStandardItemModel *eventTypesModel;
	QListView *eventTypesView;
	QDateTime *startDateTime;
	QDateTime *endDateTime;
	//---------------------------------------
	ChangeCoordDialog *dlg;
	QSettings *settings;
	QMenu			* mouse_menu;
	QDialog			* closeRSTdialog;
	QDialog			* searchResultsDialog;
	QLineEdit		* scale_info;
	QListView		* searchResultListView;
	QStandardItemModel *searchResultsModel;

	QStandardItemModel *rstModel;
	QListView *rstListView;

};

#endif //MAPVIEW_H
