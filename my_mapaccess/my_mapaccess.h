#ifndef MY_MAPACCESS_H
#define MY_MAPACCESS_H
#include <QObject>

#ifndef config_my_mapaccess_h
	#include <config_my_mapaccess.h>
#endif

#ifndef MAPAPI_H
	#include <mapapi.h>
#endif


class __EXPORT_MY_MAPACCESS MyMapAccess : public QObject
{
    Q_OBJECT
public:
    MyMapAccess();
    ~MyMapAccess();
	int drawContents( HMAP hMap, BITMAPINFOHEADER *lpDibInfo, char *lpBits, RECT *RectDraw);
	HMAP mapOpen(  const char * name, int val);
	// Cоздать пустой объект векторной карты
	HOBJ mapCreateObject(HMAP hMap,long int val1=1,long int kind=IDDOUBLE2,long int val2=0);
	// Запросить общее число листов в районе
	long int mapGetListCount(HMAP hMap);
	// Запросить имя классификатор карты по имени карты
	long int mapGetRscByName(const char * name,  char * rscname,  long int size);
	// Запросить прямоугольные координаты габаритов карты
	double mapGetMapX1(HMAP hMap);
	double mapGetMapY1(HMAP hMap);
	double mapGetMapX2(HMAP hMap);
	double mapGetMapY2(HMAP hMap);
	// Запросить внутренний код (индекс) объекта
	long int mapRscObjectCode(HMAP hMap, long int excode,long int local);
	// Запросить число слоев на карте
	long int mapGetLayerCount(HMAP hMap);
	 // Запросить код локализации объекта
	long int mapRscObjectLocalInLayer(HMAP hMap, long int layer,long int number);
	 // Запросить название семантической характеристики объекта
	const char * mapRscSemanticName(HMAP hMap,long int code);
	 // Сформировать описание нового объекта (!!!)
    long int mapRegisterObjectByKey(HOBJ hObj, const char * name_ff);
	//добавить координату объекта
	long int mapAppendPointPlane(HOBJ hObj, double x,double g,long int val=0);
	long int mapCommitWithPlace(HOBJ hObj);
	long int mapSetBright(HMAP hmap, long int bright);
	void mapFreeObject(HOBJ hObj);
	void mapGetPictureSize(HMAP hMap,long int *mapW, long int *mapH);
	  // Создание буфера окна
	HIMAGE mapCreateImage(HWND hwnd);
	 // Создать новый район (новую векторную карту)
	HMAP mapCreateMap(const char * mapname, const char * rscname,MAPREGISTER * map,LISTREGISTER * sheet);
	HMAP mapCreateMapEx(const char * mapname, const char * rscname, MAPREGISTEREX * mapreg, LISTREGISTER * listreg);
	void mapCloseData(HMAP hMap);
	// Создать пользовательскую карту
	HMAP mapCreateSite(const char * mapname, const char * rscname,CREATESITE * createsite);
	HMAP mapCreateSiteEx(const char * mapname,const char * rscname,CREATESITEEX * createsite);
	HMAP mapCreateSiteUn(const WCHAR * mapname, const WCHAR * rscname,CREATESITEUN * createsite);
	//Открыть пользовательскую карту
	HMAP mapOpenData(const char * name, long int mode = 0);
	 // Запросить - может ли карта редактироваться
	long int mapGetSiteEditFlag(HMAP hMap, HSITE hSite);
	// Установить флаг редактирования карты (0 - не редактировать)
	long int mapSetSiteEditFlag(HMAP hMap,HSITE hSite, long int flag);
	// Создать пользовательскую карту по открытой карте
	HSITE mapCreateAndAppendSiteUn(HMAP hMap,  const WCHAR *  mapname,const WCHAR * rscname, CREATESITEUN * createsite);
	HSITE mapCreateAndAppendSite(HMAP hMap, const char * mapname,const char * rscname, CREATESITE * createsite);
	// Закрыть пользовательскую карту в заданном районе работ
	HSITE mapCloseSiteForMap(HMAP hMap, HSITE hSite);
	HSITE mapCloseSiteForMapByName(HMAP hMap,const char * name);



	// Заполнение справочных данных в зависимости от типа карты
	long int mapRegisterFromMapType(int maptype, MAPREGISTEREX * mapreg);
	//добавление объекта на катру MAP, SIT, MTW, MTQ, RSW, MPT
		long int mapAppendDataUn(HMAP hMap, const WCHAR * name, long int mode=0);
		long int mapAppendData(HMAP hMap, const char * name, long int mode=0);
		HOBJ  mapCreateSiteObject(HMAP hMap,HSITE hSite, long int kind = IDDOUBLE2, long int text = 0);
	//добавить и присоединить к карте
		 HSITE mapOpenSiteForMap(HMAP hMap, const char * sitename, long int mode);
	 // Запросить число слоев на карте
		 long int mapGetSiteLayerCount(HMAP hMap,  HSITE hSite);
		 // Запросить значение семантической характеристики объекта
		 double mapSemanticDoubleValue(HOBJ info, long int number);
		 // Запросить значение семантической характеристики объекта
		 double mapSemanticCodeDoubleValue(HOBJ info, long int code, long int number);
	//проверка корректности паспорта
		long int mapCheckAndUpdate(MAPREGISTEREX * mapreg, LISTREGISTER * listreg, long int priority);
	//имя главной карты в проекте
		const char * mapGetMainName(HMAP hMap);
	// Установить признак выделения объектов по обобщенным
		void mapSetTotalSelectFlag(HMAP hMap, long int flag=-1);
	// Установить масштаб отображения (знаменатель масштаба)
		long int mapSetViewScale(HMAP hMap, long int * x, long int * y, float scale);
		 // Запросить округленный масштаб отображения карты
		long int mapGetShowScale(HMAP);
	//поиск объекта
		//для поиска
		void mapChangeViewScale(HMAP hMap, long int *x, long int *y, float Change);
		long int mapGetViewSelect(HMAP hMap, HSELECT select);	
		HSELECT mapCreateMapSelectContext(HMAP hMap);
		//поиск
		HOBJ mapWhatObject(HMAP hMap, HOBJ info, MAPDFRAME * frame, long int flag = WO_LAST, long int place = PP_MAP);
		HOBJ mapWhatObjectBySelect(HMAP hMap, HOBJ info, MAPDFRAME * frame, HSELECT select, long int flag = WO_LAST, long int place = PP_MAP);
	 // Выделить на карте объекты, удовлетворющие заданным условим
		void mapViewSelect(HMAP hMap, HWND hWnd, DOUBLEPOINT * point, HSELECT select, COLORREF color, long int place = PP_MAP);
	  // Удалить контекст (описание условий) поиска/отображения
		void mapDeleteSelectContext(HSELECT select);
		//перевод в координаты в дискретах
		void mapPictureToMap(HMAP hMap,double *x, double *y);
		//из дискретов в метры
		void mapMapToPlane(HMAP hmap, double * x, double *y);
		//перевод в координаты в метрах
		void mapPictureToPlane(HMAP hMap, double * x, double * y);
	// Запросить название объекта по порядковому номеру (number)
		const char * mapRscObjectNameInLayer(HMAP hmap,long int layer,long int number);
		const char * mapObjectName(HOBJ info);
	// Запросить классификационный код объекта
	long int * mapRscObjectExcodeInLayer(HMAP hMap,long int layer,long int number);
	// Запросить - есть ли какие-либо открытые данные
	long int mapIsActive(HMAP hMap);
	// Запросить код ошибки последней операции доступа к данным
	long int mapGetAccessError();
	// Запросить уникальный номер объекта
	long int mapObjectKey(HOBJ info);
	 // Запросить название слоя объекта ("Layer" = "Segment")
	const char * mapSegmentName(HOBJ info);
	// Запросить текущее значение идентификатора окна для приема сообщений
	HWND mapGetHandleForEvent(HMAP hMap);
	// Запросить идентификатор классификатора карты
	 HRSC mapGetRscIdent(HMAP hMap,HSITE hSite);
	 // Запросить идентификатор классификатора карты, содержащей объект
	 HRSC mapGetRscIdentByObject(HOBJ info);
	 // Запросить количество видов семантик, которые еще могут быть
	 long int mapAvailableSemanticCount(HOBJ info);
	  // Запросить внешний код доступной семантики на объект
	 long int mapAvailableSemanticCode(HOBJ info,int number);
	 // Добавить новую характеристику в семантику объекта
	 long int mapAppendSemanticDouble(HOBJ info,long int code, double value);
	 long int mapAppendSemantic(HOBJ info,long int code, const char * value, long int size);
	 // Запросить значение семантической характеристики объекта
	  long int  mapSemanticValue(HOBJ info, long int number,  char * value,  long int size);
	  long int	mapSemanticCodeValue(HOBJ info,long int code, char * place,long int maxsize,long int number);
	 // Обновить изображение заданного фрагмента карты в образе экрана 
	 long int mapDrawImageMap(HIMAGE himage, HMAP hMap, RECT * rect,  POINT * position);
	 // Установить/сбросить/запросить масштабируемость объекта
	long int mapSetObjectScale(HOBJ info, long int scale);
	// Запросить/Установить диапазон масштабов видимости объекта (след 4 ф-ции)
	long int mapObjectTopScale(HOBJ info);
	long int mapSetObjectTopScale(HOBJ info, long int scale);
	long int mapObjectBotScale(HOBJ info);
	long int mapSetObjectBotScale(HOBJ info, long int scale);
	// Установить/сбросить/запросить признак "Не сжимать" объекта
	long int mapSetObjectPress(HOBJ info, long int press);
	long int mapGetObjectPress(HOBJ info);
	// Отобразить объект поверх карты местности в образе экрана
	long int mapDrawImageMapObject(HIMAGE himage, HMAP hMap, PAINTPARM * parm, HOBJ object);
	// Очистить содержимое объекта
	long int mapClearObject(HOBJ info,  long int sheetnumber = 1,  long int kind = IDFLOAT2);
	// Сформировать описание нового объекта (!!!)
	long int mapRegisterObject(HOBJ info, long int excode, long int local);
	long int mapDescribeObject(HOBJ info, long int code);
	// Запросить классификационный код объекта
	long int mapObjectExcode(HOBJ info);
	 // Запросить яркость карты (от -16 до +16)
	long int mapGetBright(HMAP hMap);
// Запросить контрастность (от -16 до +16)
	long int	mapGetContrast(HMAP hMap);
  // Установить контрастность (от -16 до +16)
	long int	mapSetContrast(HMAP hMap, long int contrast);
  // Запросить интенсивность заливки полигонов для       // 21/07/06
	long int	mapGetIntensity(HMAP hMap);
  // Установить интенсивность заливки полигонов для
	long int	mapSetIntensity(HMAP hMap, long int intensity);
	// Запросить контекст буфера окна
	HDC mapGetImageDC(HIMAGE hImage);
	 // Создать буфер карты в памяти для исключения мигания  
	HIMAGE mapCreateImageEx(long int width, long int height);
	 // Отобразить фрагмент карты на заданном устройстве
	void mapPaint95(HMAP hMap, HDC hdc,  long int erase, RECT * rect);
	  // Установить идентификатор окна для приема сообщений
	HWND mapGetHandleForMessage();
	 // Вывести изображение карты в Image (массив)
	long int	mapPaintToImage(HMAP hMap,  RGBQUAD * palette,  char * lpImage,  long int width, long int height,  RECT * rect);
	//------------------------------------------------------------
	// Преобразование из метров на местности (проекция карты) в геодезические координаты в радианах (общеземной эллипсоид WGS84)
	long int mapPlaneToGeoWGS84(HMAP hmap,double *Bx, double *Ly);
	long int mapPlaneToGeoWGS843D(HMAP hmap,double *Bx, double *Ly, double *H);
	 // Преобразование из дискретов на карте (районе работ) в пикселы на изображении
	void mapMapToPicture(HMAP hMap, double * x, double * y);
	//  Преобразование из геодезических координат в радианах
  long int mapGeoWGS84ToPlane3D(HMAP hmap, double *Bx, double *Ly, double * H);
   // Запрос - поддерживается ли пересчет к геодезическим координатам из плоских прямоугольных и обратно
	long int mapIsGeoSupported(HMAP hmap);
	// Преобразование координат из градусов в радианы
	void mapDegreeToRadian(GEODEGREE * degree, double * radian);
	 // Преобразование из геодезических координат в радианах  в метры на местности в соответствии с проекцией карты
	long int mapGeoToPlane(HMAP hmap,double *Bx, double *Ly);
	// Преобразование из метров на местности (проекция карты) в геодезические координаты в радианах (эллипсоид Красовского)
	long int mapPlaneToGeo42(HMAP hmap,double *Bx, double *Ly);
	long int mapPlaneToGeo423D(HMAP hmap,double *Bx, double *Ly, double *H);
	 // Преобразование координат из радиан в градусы
	void	 mapRadianToDegree(double * radian,  GEODEGREE * degree);
	 // Преобразование из метров на местности в дискреты на карте (районе работ)
	void	 mapPlaneToMap(HMAP hMap,double * x, double * y);
	HPRINTER prnLoadPrinter();
	// Изменить значение кода семантической характеристики объекта
	long int	mapSetSemanticCode(HOBJ info, long int number, long int code);
 // Изменить значение семантической характеристики объекта
	long int	mapSetSemanticValue(HOBJ info, long int number, char * place, long int maxsize);
	
	//РАСТР! ! ! ! ! ! ! ! ! ! ! ! ! !
	//long int _MAPAPI mapOpenRstForMap(HMAP hMap, const char * rstname, long int mode);
	 long int  openRstForMap(HMAP hMap, const char * rstname, long int mode);
	 long int  closeRstForMap(HMAP hMap, long int number);
	 // Запросить/Установить порядок отображения растра над картой
	 long int  setRstViewOrder(HMAP hMap, long int number, long int order);
protected:
    

public:
	HINSTANCE LibInst;
	
};



#endif
