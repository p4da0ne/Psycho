#include "my_mapaccess.h"
#include "qdebug.h"
#include "mapsyst.h"

#ifdef WIN32API
#define LIBMAPACCESNAME "mapacces.dll"
#else
#define LIBMAPACCESNAME "libqdmapacces.so"
#endif

MyMapAccess::MyMapAccess()
:QObject()     
{
    LibInst = 0;
	#ifdef UNICODE
    TCHAR tname[MAX_PATH];
    StringToUnicode(LIBMAPACCESNAME,tname,sizeof(tname));
#endif
    if (LibInst == 0)
#ifdef UNICODE
      LibInst = ::LoadLibrary(tname);
    if(!LibInst){
        DWORD err=GetLastError();
        printf("%lu", err);
    }
#else
      LibInst = ::LoadLibrary(LIBMAPACCESNAME);
#endif
}

//-------------------------------------------------------------
// Деструктор
//-------------------------------------------------------------
MyMapAccess::~MyMapAccess()
{
    ::FreeLibrary(LibInst);
    LibInst = 0;  
}

//-------------------------------------------------------------
// Перерисовка окна
//-------------------------------------------------------------
 // Вывести изображение карты в DIB
 // Данная функция реализована только для платформы Windows !
 // Выводится фрагмент карты, заданный параметром rect.
 // Заполнется палитра,расположенная за BITMAINFOHEADER
 // Изображение строится в текущем масштабе и составе объектов
 // Размер точки 1,2,3 или 4 байт.
 // Если размеры заданного DIB меньше размеров фрагмента -
 // изображение строится в пределах размеров DIB.
 //  hmap      - идентификатор открытых данных
 //  dibinfo   - описатель создаваемого DIB в памяти
 //  lpDibBits - адрес первого байта битовой области.
 //  rect      - координаты фрагмента карты (Draw)
 //              в изображении (Picture).
 // Размер области DIB, задаваемый параметром dibinfo->biSizeImage,
 // должен учитывать то, что каждая строка DIB должна быть
 // кратна 4 байтам (32 битам):
 //  dibinfo->biSizeImage = dibinfo->biHeight *
 //    ((dibinfo->biWidth * dibinfo->biBitCount + 31) / 32) * 4;
 // При ошибке в параметрах возвращает ноль
int MyMapAccess::drawContents( HMAP hMap, 
							  BITMAPINFOHEADER *lpDibInfo, 
							  char *lpBits, 
							  RECT *RectDraw)
{
   
	  long int (WINAPI * lpmapfn2)(HMAP, BITMAPINFOHEADER *,char *, RECT *);
      (FARPROC&) lpmapfn2 = ::GetProcAddress(LibInst,"mapPaintToDib");
      int ret = (*lpmapfn2)(hMap, lpDibInfo, lpBits, RectDraw);
	  return ret;
}

//открытие карты
HMAP MyMapAccess::mapOpen(const char * name, 
						  int val)
{
    if (!LibInst) {
        qDebug() << "Library not loaded";
        return 0;
    }
    
    if (!name) {
        qDebug() << "Invalid file name";
        return 0;
    }
    
    using MapOpenFunc = HMAP (WINAPI *)(const char *, int);
    auto lpmapfn = reinterpret_cast<MapOpenFunc>(::GetProcAddress(LibInst, "mapOpenMap"));
    
    if (!lpmapfn) {
        qDebug() << "Function mapOpenMap not found in library";
        return 0;
    }
    
    HMAP hMap = (*lpmapfn)(name, val);
    if (!hMap) {
        qDebug() << "Failed to open map:" << name;
    }
    
    return hMap;
}

// Запросить общее число листов в районе
 // hmap - идентификатор открытых данных
 // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapGetListCount(HMAP hmap);
long int MyMapAccess::mapGetListCount(HMAP hMap)
{
	long int (WINAPI * lpfn_mapGetListCount)(HMAP);          
	(FARPROC&)lpfn_mapGetListCount = ::GetProcAddress(LibInst, "mapGetListCount");   
	return (*lpfn_mapGetListCount)(hMap); 
}
  // Запросить имя классификатор карты по имени карты   21/02/06
  // name    - имя файла паспорта карты (MAP,SIT)
  // rscname - имя файла RSC
  // size    - размер буфера для размещения имени RSC
  // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapGetRscByName(const char * name, char * rscname, long int size);
long int MyMapAccess::mapGetRscByName(const char * name,
                                         char * rscname, 
										 long int size)
{
	long int (WINAPI * lpfn_mapGetRscByName)( const char * , char * , long int);          
	(FARPROC&)lpfn_mapGetRscByName = ::GetProcAddress(LibInst, "mapGetRscByName");   
	return (*lpfn_mapGetRscByName)(name, rscname, size); 
}

long int MyMapAccess::mapRegisterObjectByKey(HOBJ hObj, 
											 const char * name_ff)
{
	long int (WINAPI * lpfn_mapRegisterObjectByKey)(HOBJ, const char *);          
	(FARPROC&)lpfn_mapRegisterObjectByKey = ::GetProcAddress(LibInst, "mapRegisterObjectByKey");   
	return (*lpfn_mapRegisterObjectByKey)(hObj,name_ff); 
}

long int MyMapAccess::mapAppendPointPlane(HOBJ hObj, 
										  double x,
										  double y,
										  long int val)
{
	long int (WINAPI * lpfn_mapAppendPointPlane)(HOBJ, double,double,long int);
	(FARPROC&)lpfn_mapAppendPointPlane = ::GetProcAddress(LibInst, "mapAppendPointPlane");   
	return (*lpfn_mapAppendPointPlane)(hObj,x,y,val);
}

long int MyMapAccess::mapCommitWithPlace(HOBJ hObj)
{
	long int (WINAPI * lpfn_mapCommitWithPlace)(HOBJ);
	(FARPROC&)lpfn_mapCommitWithPlace = ::GetProcAddress(LibInst, "mapCommitWithPlace");
	return (*lpfn_mapCommitWithPlace)(hObj);
}

void MyMapAccess::mapFreeObject(HOBJ hObj)
{
		void (WINAPI * lpfn_mapFreeObject)(HOBJ);
	   (FARPROC&)lpfn_mapFreeObject = ::GetProcAddress(LibInst, "mapFreeObject");   
	   (*lpfn_mapFreeObject)(hObj); 
}
 // Сформировать описание нового объекта (!!!)
 // по внешнему коду и локализации
 // info - идентификатор объекта карты в памяти
 // excode - внешний код объекта (числовой),
 // local  - локализация (LOCAL_LINE, LOCAL_POINT...)
 // Вызывается после mapCreateObject(...) и добавления
 // семантики (если она есть)
 // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapRegisterObject(HOBJ info, long int excode, long int local);
long int MyMapAccess::mapRegisterObject(HOBJ info, long int excode, long int local)
{
	long int (WINAPI * lpfn_mapRegisterObject)(HOBJ, long int, long int);
	(FARPROC&)lpfn_mapRegisterObject = ::GetProcAddress(LibInst, "mapRegisterObject");
	return (*lpfn_mapRegisterObject)(info, excode, local);
}

void MyMapAccess::mapChangeViewScale(HMAP hMap, 
									 long int *x, 
									 long int *y, 
									 float Change)
{

void (WINAPI * lpmapfn)(HMAP, long int *, long int *, float ch);
(FARPROC&) lpmapfn = ::GetProcAddress(LibInst,"mapChangeViewScale");
  (*lpmapfn)(hMap, x,y,Change);
}

void MyMapAccess::mapGetPictureSize(HMAP hMap,
									long int *mapW, 
									long int *mapH)
{
  void (WINAPI * lpmapfn1)(HMAP, long int *, long int *);
  (FARPROC&) lpmapfn1 = ::GetProcAddress(LibInst,"mapGetPictureSize");
  (*lpmapfn1)(hMap, mapW, mapH);
}
  // Установить яркость карты (от -16 до +16)
  // hmap - идентификатор открытых данных
long int MyMapAccess::mapSetBright(HMAP hmap, 
								   long int bright)
{
	
 long int (WINAPI * lpfn_mapSetBright)(HMAP,long int);
	(FARPROC&)lpfn_mapSetBright = ::GetProcAddress(LibInst, "mapSetBright");
	return (*lpfn_mapSetBright)(hmap,bright);
}
  // Запросить яркость карты (от -16 до +16)
  // hmap - идентификатор открытых данных
//_MAPIMP long int _MAPAPI mapGetBright(HMAP hmap);
long int MyMapAccess::mapGetBright(HMAP hMap)
{
  long int (WINAPI * lpfn_mapGetBright)(HMAP);
  (FARPROC&) lpfn_mapGetBright = ::GetProcAddress(LibInst,"mapGetBright");
  return (*lpfn_mapGetBright)(hMap);
}
// Запросить контрастность (от -16 до +16)
  // hmap - идентификатор открытых данных
//_MAPIMP long int _MAPAPI mapGetContrast(HMAP hmap);
long int MyMapAccess::mapGetContrast(HMAP hMap)
{
  long int (WINAPI * lpfn_mapGetContrast)(HMAP);
  (FARPROC&) lpfn_mapGetContrast = ::GetProcAddress(LibInst,"mapGetContrast");
  return (*lpfn_mapGetContrast)(hMap);
}
  // Установить контрастность (от -16 до +16)
  // hmap - идентификатор открытых данных
//_MAPIMP long int _MAPAPI mapSetContrast(HMAP hmap, long int contrast);
long int MyMapAccess::mapSetContrast(HMAP hMap, 
								   long int contrast)
{	
 long int (WINAPI * lpfn_mapSetContrast)(HMAP,long int);
	(FARPROC&)lpfn_mapSetContrast = ::GetProcAddress(LibInst, "mapSetContrast");
	return (*lpfn_mapSetContrast)(hMap, contrast);
}
  // Запросить интенсивность заливки полигонов для       // 21/07/06
  // принтерного отображения (от 0 до 100)
  // hmap - идентификатор открытых данных
//_MAPIMP long int _MAPAPI mapGetIntensity(HMAP hmap);
long int MyMapAccess::mapGetIntensity(HMAP hMap)
{
  long int (WINAPI * lpfn_mapGetIntensity)(HMAP);
  (FARPROC&) lpfn_mapGetIntensity = ::GetProcAddress(LibInst,"mapGetIntensity");
  return (*lpfn_mapGetIntensity)(hMap);
}
  // Установить интенсивность заливки полигонов для
  // принтерного отображения (от 0 до 100)
  // hmap - идентификатор открытых данных
//*_MAPIMP long int _MAPAPI mapSetIntensity(HMAP hmap, long int intensity);
long int MyMapAccess::mapSetIntensity(HMAP hMap, 
								   long int intensity)
{	
 long int (WINAPI * lpfn_mapSetIntensity)(HMAP,long int);
	(FARPROC&)lpfn_mapSetIntensity = ::GetProcAddress(LibInst, "mapSetIntensity");
	return (*lpfn_mapSetIntensity)(hMap,intensity);
}
void  MyMapAccess::mapCloseData(HMAP hMap)
{

    void (WINAPI * lpmapfn)(int);
#ifdef WIN32API
    (FARPROC&) lpmapfn = ::GetProcAddress(LibInst,"mapCloseData");
#else
    (void *)lpmapfn = dlsym((void *)LibInst, "mapCloseData");
#endif
    (*lpmapfn)(hMap);
}
 // Отобразить фрагмент карты на заданном устройстве
 // в текущих масштабе и составе объектов
 // hmap  - идентификатор открытых данных
 // hdc   - идентификатор контекста устройства вывода,
 // erase - признак стирания фона перед выводом,
 //        (0 - фон не стирать, !=0 - очистить фрагмент цветом фона,
 //        для экранного способа вывода (VT_SCREEN) всегда стирает
 //        цветом фона, кроме значения -2 (минус 2))
 // rect - координаты фрагмента карты (Draw) в изображении (Picture)
 // Корректно работает с большими изображениями под Windows95 и NT,
 // но требует перед вызовом установки
 //              ::SetViewportOrgEx(hDC, dx , dy, 0),
 // где dx,dy - положение отображаемого фрагмента в клиентной
 // области !
 // Размер картинки, рисуемой за один вызов, не более
 // текущих размеров экрана! Иначе - см. PaintToDib,PaintToImage...
//_MAPIMP void _MAPAPI mapPaint95(HMAP hMap, HDC hdc,  long int erase, RECT * rect);
void MyMapAccess::mapPaint95(HMAP hMap, HDC hdc,  long int erase, RECT * rect)
{
	void (WINAPI * lpfn_mapPaint95)(HMAP, HDC,  long int, RECT *);
      (FARPROC&) lpfn_mapPaint95 = ::GetProcAddress(LibInst,"mapPaint95");
      (*lpfn_mapPaint95)(hMap, hdc,  erase, rect);
}

void MyMapAccess::mapPictureToMap(HMAP hMap,
								  double *x, 
								  double *y)
{
	void (WINAPI * lpfn_PictToMap)(HMAP hmap,double *x, double *y);
      (FARPROC&) lpfn_PictToMap = ::GetProcAddress(LibInst,"mapPictureToMap");
      (*lpfn_PictToMap)(hMap, x , y);

}
 /*********************************************************
  *                                                        *
  *          ОТКРЫТЬ/ЗАКРЫТЬ ДОСТУП К ДАННЫМ КАРТЫ         *
  *                                                        *
  *********************************************************/
//создание объекта на карте
 // Cоздать пустой объект векторной карты
  // (создание подобъекта - см. редактирование метрики)
  // По умолчанию метрика FLOAT 2-ух мерная
  // sheetnumber - номер листа в котором будет расположен
  // kind - тип создаваемой метрики, описан в maptype.h
  // создаваемый объект
  // text - признак метрики с текстом (объекты типа "подпись")
  // (устанавливается автоматически при вызове mapPutText(...))
  // После вызова функций типа What...() и Seek...() все параметры
  // полученного объекта могут измениться (text,kind,list и т.п.)
  // При ошибке возвращает ноль

HOBJ MyMapAccess::mapCreateObject(HMAP hMap,
								  long int val1,
								  long int kind,
								  long int val2)
{
	HOBJ (WINAPI * lpfn_mapCreateObject)(int,long int,long int,long int);          
	(FARPROC&)lpfn_mapCreateObject = ::GetProcAddress(LibInst, "mapCreateObject");   
	return(*lpfn_mapCreateObject)(hMap,val1,kind,val2); 
	//hObj = mapCreateObject( hMap,1,IDDOUBLE2,0);
}
   // Запросить число слоев на карте
  // hMap   - идентификатор открытой основной карты
  // hSite  - идентификатор открытой пользовательской карты
  // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapGetSiteLayerCount(HMAP hMap,HSITE hSite);
long int MyMapAccess::mapGetSiteLayerCount(HMAP hMap,
										   HSITE hSite)
{
	long int (WINAPI * lpfn_mapGetSiteLayerCount)(HMAP ,HSITE );          
	(FARPROC&)lpfn_mapGetSiteLayerCount = ::GetProcAddress(LibInst, "mapGetSiteLayerCount");   
	return( * lpfn_mapGetSiteLayerCount)(hMap,hSite); 
}
// Проверка корректности паспортных данных и, если надо, то заполнение
  // координат по признаку приоритета 0 - расчет прямоугольных координат
  //                                  1 - геодезических
  // Струтктуры MAPREGISTEREX, LISTREGISTER описаны в mapcreate.h
  // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapCheckAndUpdate(MAPREGISTEREX * mapreg, LISTREGISTER * listreg, long int priority);
long int MyMapAccess::mapCheckAndUpdate(MAPREGISTEREX * mapreg, 
										LISTREGISTER * listreg,
										long int priority)
{
	long int (WINAPI * lpfn_mapCheckAndUpdate)(MAPREGISTEREX * , LISTREGISTER * , long int);          
	(FARPROC&)lpfn_mapCheckAndUpdate = ::GetProcAddress(LibInst, "mapCheckAndUpdate");   
	return( * lpfn_mapCheckAndUpdate)(mapreg, listreg, priority); 
}
 // Cоздать пустой объект пользовательской карты (обстановки)
  // (создание подобъекта - см. редактирование метрики объекта)
  // По умолчанию метрика 2-ух мерная
  // hMap  - идентификатор открытой основной карты
  // hSite - идентификатор открытой пользовательской карты
  //         в которой будет расположен создаваемый объект
  // text  - признак метрики с текстом (объекты типа "подпись")
  // kind  - формат метрики
  // После вызова функций типа What...() и Seek...() все параметры
  // полученного объекта могут измениться (text,kind и т.п.)
  // Для каждого полученного и больше не используемого
  // идентификатора HOBJ необходим вызов функции FreeObject()
  // При ошибке возвращает ноль
//_MAPIMP HOBJ _MAPAPI mapCreateSiteObject(HMAP hMap,HSITE hSite, long int kind = IDDOUBLE2, long int text = 0);
HOBJ MyMapAccess::mapCreateSiteObject(HMAP hMap,
									  HSITE hSite, 
									  long int kind, 
									  long int text)
{
	HOBJ (WINAPI * lpfn_mapCreateSiteObject)(HMAP ,HSITE , long int , long int );          
	(FARPROC&)lpfn_mapCreateSiteObject = ::GetProcAddress(LibInst, "mapCreateSiteObject");   
	return( * lpfn_mapCreateSiteObject)(hMap,hSite,kind,text); 
}
// Заполнение справочных данных в зависимости от типа карты           // 22/12/05
  // Струтктуры MAPREGISTEREX, LISTREGISTER описаны в mapcreate.h
  // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapRegisterFromMapType(int maptype, MAPREGISTEREX * mapreg);
long int MyMapAccess::mapRegisterFromMapType(int maptype,
											 MAPREGISTEREX * mapreg)
{
	long int (WINAPI * lpfn_mapRegisterFromMapType)(int, MAPREGISTEREX *);          
	(FARPROC&)lpfn_mapRegisterFromMapType = ::GetProcAddress(LibInst, "mapRegisterFromMapType");   
	return( * lpfn_mapRegisterFromMapType)(maptype,mapreg); 
}
// Запросить состав отображаемых объектов
  // hmap - идентификатор открытых данных
  // select - идентификатор контекста поиска/отображения
  // в который будут помещены текущие условия отображения
  // см. mapCreateMapSelectContext(...)
  // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapGetViewSelect(HMAP hmap,HSELECT select);
long int MyMapAccess::mapGetViewSelect(HMAP hMap,
									   HSELECT select)
{
	long int (WINAPI * lpfn_mapGetViewSelect)(HMAP,HSELECT);          
	(FARPROC&)lpfn_mapGetViewSelect = ::GetProcAddress(LibInst, "mapGetViewSelect");   
	return( * lpfn_mapGetViewSelect)(hMap,select); 
}

  // Открыть данные с автоматическим определением их типа
  // (векторные,растровые,матричные...)
  // name - имя открываемого файла (MAP, SIT, MTW, RSW, MPT)
  // mode - режим чтения/записи (GENERIC_READ, GENERIC_WRITE или 0)
  // GENERIC_READ - все данные только на чтение, при этом не открываются
  // файлы \Log\name.log и \Log\name.tac - протокол работы и журнал транзакций
  // При ошибке возвращает ноль
//_MAPIMP HMAP _MAPAPI mapOpenData(const char * name, long int mode = 0);
HMAP MyMapAccess::mapOpenData(const char * name, long int mode )
{
	HMAP (WINAPI * lpfn_mapOpenData)(const char * , long int );          
	(FARPROC&) lpfn_mapOpenData = ::GetProcAddress(LibInst, "mapOpenData");   
	return( * lpfn_mapOpenData)(name, mode);
}
//===================================================================================
//===================================================================================
 // ЗАПРОС ОБ'ЕКТА ПО ЗАДАННЫМ КООРДИНАТАМ										=====
 // hMap - идентификатор открытой карты.										=====
 // info - идентификатор объекта в памяти,										=====
 //        предварительно созданного функцией mapCreateObject()					=====
 //        или mapCreateSiteObject(),											=====
 //        в котором будет размещен результат поиска.							=====
 // Поиск выполнется среди тех объектов,которые									=====
 // соответствуют условиям HSELECT !											=====	
 // select- контекст условий выбора объектов,									=====
 // place - система координат,													=====
 // frame - прямоугольная область поиска объекта в системе координат,			=====
 //         заданной переменной place (PP_PLANE,PP_GEO, ...)					=====
 // flag  - порядок поиска объектов (WO_FIRST, WO_NEXT...)						=====
 // Выбор объекта в "точке" лучше начинать с последнего, то						=====
 // есть того, что нарисован поверх остальных (это чуть медленнее				=====
 // прямого поиска).															=====
 // При поиске с флажками WO_NEXT,WO_BACK параметр info должен					=====
 // содержать результат предыдущего поиска.										=====
 // Если объект не найден - возвращает ноль,									=====
 // иначе - возвращает значение info !											=====
//===================================================================================
//===================================================================================
//#ifdef __cplusplus
/*_MAPIMP HOBJ _MAPAPI mapWhatObjectBySelect(HMAP hMap,	HOBJ info, MAPDFRAME * frame,  HSELECT select,  long int flag = WO_LAST,  long int place = PP_MAP);*/
HOBJ MyMapAccess::mapWhatObjectBySelect(HMAP hMap,	
										HOBJ info, 
										MAPDFRAME * frame, 
										HSELECT select, 
										long int flag, 
										long int place)
{
	HOBJ (WINAPI * lpfn_mapWhatObjectBySelect)(HMAP ,HOBJ, MAPDFRAME *,  HSELECT, long int,  long int);          
	(FARPROC&)lpfn_mapWhatObjectBySelect = ::GetProcAddress(LibInst, "mapWhatObjectBySelect");   
	return(*lpfn_mapWhatObjectBySelect)(hMap,info,frame,select,flag,PP_MAP); 
}
 // Запросить уникальный номер объекта
 // info - идентификатор объекта карты в памяти
 // При ошибке возвращает 0 (ноль допустим)
//_MAPIMP long int _MAPAPI mapObjectKey(HOBJ info);
long int MyMapAccess::mapObjectKey(HOBJ info)
{
	long int (WINAPI * lpfn_mapObjectKey)(HOBJ);          
	(FARPROC&)lpfn_mapObjectKey = ::GetProcAddress(LibInst, "mapObjectKey");   
	return( * lpfn_mapObjectKey)(info); 
}
 // Запросить название слоя объекта ("Layer" = "Segment")
 // info - идентификатор объекта карты в памяти
 // name - адрес буфера для результата запроса
 // size - размер буфера
 // При ошибке возвращает ноль
//_MAPIMP const char * _MAPAPI mapSegmentName(HOBJ info);
const char * MyMapAccess::mapSegmentName(HOBJ info)
{
	const char * (WINAPI * lpfn_mapSegmentName)(HOBJ);          
	(FARPROC&)lpfn_mapSegmentName = ::GetProcAddress(LibInst, "mapSegmentName");   
	return( * lpfn_mapSegmentName)(info); 
}
  // Запросить текущее значение идентификатора окна для приема сообщений
//_MAPIMP HWND _MAPAPI mapGetHandleForEvent(HMAP hmap);
 HWND MyMapAccess::mapGetHandleForEvent(HMAP hMap)
{
	 HWND (WINAPI * lpfn_mapGetHandleForEvent)(HMAP);          
	(FARPROC&)lpfn_mapGetHandleForEvent = ::GetProcAddress(LibInst, "mapGetHandleForEvent");   
	return( * lpfn_mapGetHandleForEvent)(hMap); 
}

 // Создать новый район (новую векторную карту)
  // mapname - полное имя файла карты
  // rscname - полное имя файла ресурсов
  // Возвращает идентификатор открытой векторной карты (TMapAccess*)
  // Структуры MAPREGISTER и LISTREGISTER описаны в mapcreat.h
  // При ошибке возвращает ноль
//_MAPIMP HMAP _MAPAPI mapCreateMap(const char * mapname, const char * rscname,MAPREGISTER * map,LISTREGISTER * sheet);
HMAP MyMapAccess::mapCreateMap(const char * mapname, 
							   const char * rscname,
							   MAPREGISTER * map,
							   LISTREGISTER * sheet)
{
	HMAP (WINAPI * lpfn_mapCreateMap)(const char * , const char * ,MAPREGISTER * ,LISTREGISTER * );          
	(FARPROC&)lpfn_mapCreateMap = ::GetProcAddress(LibInst, "mapCreateMap");   
	return( * lpfn_mapCreateMap)(mapname,rscname,map,sheet); 
}
//_MAPIMP HMAP _MAPAPI mapCreateMapEx(const char * mapname, const char * rscname, MAPREGISTEREX * mapreg, LISTREGISTER * listreg);
HMAP MyMapAccess::mapCreateMapEx(const char * mapname,
								 const char * rscname,
								 MAPREGISTEREX * mapreg,
								 LISTREGISTER * listreg)
{
	HMAP (WINAPI * lpfn_mapCreateMapEx)(const char * , const char * ,MAPREGISTEREX * ,LISTREGISTER * );          
	(FARPROC&)lpfn_mapCreateMapEx = ::GetProcAddress(LibInst, "mapCreateMapEx");   
	return( * lpfn_mapCreateMapEx)(mapname,rscname,mapreg,listreg); 
}
// Создать пользовательскую карту
  // mapname - полное имя файла карты
  // rscname - полное имя файла ресурсов
  // Возвращает идентификатор открытой векторной карты (TMapAccess*)
  // Структура CREATESITE описана в maptype.h
  // При ошибке возвращает ноль
//_MAPIMP HMAP _MAPAPI mapCreateSite(const char * mapname, const char * rscname,CREATESITE * createsite);
/*int MyMapAccess::mapCreateSite()
{
	int (WINAPI * lpfn_)();          
	(FARPROC&)lpfn_ = ::GetProcAddress(LibInst, "mapCreateSite");   
	return( * lpfn_)(); 
}*/
HMAP MyMapAccess::mapCreateSite(const char * mapname, 
								const char * rscname,
								CREATESITE * createsite)
{
	HMAP (WINAPI * lpfn_mapCreateSite)(const char * , const char * ,CREATESITE * );          
	(FARPROC&) lpfn_mapCreateSite = ::GetProcAddress(LibInst, "mapCreateSite");   
	return( * lpfn_mapCreateSite)(mapname, rscname,createsite); 
}
//_MAPIMP HMAP _MAPAPI mapCreateSiteEx(const char * mapname,const char * rscname,CREATESITEEX * createsite);//11/05/07
HMAP MyMapAccess::mapCreateSiteEx(const char * mapname,
								  const char * rscname,
								  CREATESITEEX * createsite)
{
	HMAP (WINAPI * lpfn_mapCreateSiteEx)(const char * ,const char * ,CREATESITEEX * );          
	(FARPROC&)lpfn_mapCreateSiteEx = ::GetProcAddress(LibInst, "mapCreateSiteEx");   
	return( * lpfn_mapCreateSiteEx)(mapname,rscname,createsite); 
}
// Установить признак выделения объектов по обобщенным
 // условиям поиска
 // hMap - идентификатор открытой карты,
 // flag = -1, поиск будет выполняться по всем картам
 // (0 - карта местности, 1...n - пользовательские карты)
 // Никакого действия кроме сохранения значения не производит
 // Применяется для связи между различными модулями
//_MAPIMP void _MAPAPI mapSetTotalSelectFlag(HMAP hMap, long int flag);
void MyMapAccess::mapSetTotalSelectFlag(HMAP hMap, 
										long int flag)
{
	void (WINAPI * lpfn_mapSetTotalSelectFlag)(HMAP, long int);          
	(FARPROC&)lpfn_mapSetTotalSelectFlag = ::GetProcAddress(LibInst, "mapSetTotalSelectFlag");   
	return( * lpfn_mapSetTotalSelectFlag)(hMap, flag); 
}
  // Удалить контекст (описание условий) поиска/отображения
  // объектов карты
  // select - контекст (описание условий) поиска/отображения.
//_MAPIMP  void _MAPAPI mapDeleteSelectContext(HSELECT select);
void MyMapAccess::mapDeleteSelectContext(HSELECT select)
{
	void (WINAPI * lpfn_mapDeleteSelectContext)(HSELECT);          
	(FARPROC&)lpfn_mapDeleteSelectContext = ::GetProcAddress(LibInst, "mapDeleteSelectContext");   
	return( * lpfn_mapDeleteSelectContext)(select); 
}
	// Создать пользовательскую карту
  // mapname - полное имя файла карты в кодировке UNICODE
  // rscname - полное имя файла ресурсов в кодировке UNICODE
  // Возвращает идентификатор открытой векторной карты (TMapAccess*)
  // Структура CREATESITEUN описана в maptype.h
  // При ошибке возвращает ноль
//MAPIMP HMAP _MAPAPI mapCreateSiteUn(const WCHAR * mapname, const WCHAR * rscname,CREATESITEUN * createsite); // 11/05/07
HMAP MyMapAccess::mapCreateSiteUn(const WCHAR * mapname, 
								  const WCHAR * rscname,
								  CREATESITEUN * createsite)
{
	HMAP (WINAPI * lpfn_mapCreateSiteUn)(const WCHAR *, const WCHAR *,CREATESITEUN *);          
	(FARPROC&)lpfn_mapCreateSiteUn = ::GetProcAddress(LibInst, "mapCreateSiteUn");   
	return( * lpfn_mapCreateSiteUn)( mapname, rscname,createsite); 
}


  // Установить порядок отображения карты   // 18/12/02
  // hMap  - идентификатор открытой карты
  // number - номер пользовательской карты в цепочке
  // order  - флаг(0 - под основной картой, 1 - над основной картой)
  // При ошибке возвращает 0

long int MyMapAccess::mapSetSiteViewOrder(HMAP hMap, long int number, long int order)
{
	HMAP (WINAPI * lpfn_mapSetSiteOrder)(HMAP, long int, long int);          
	(FARPROC&)lpfn_mapSetSiteOrder = ::GetProcAddress(LibInst, "mapSetSiteViewOrder");   
	return( * lpfn_mapSetSiteOrder)( hMap, number,order); 
}


  // Определить номер пользовательской карты в цепочке
  // по ее идентификатору
  // hMap  - идентификатор открытой карты
  // hSite - идентификатор открытой пользовательской карты
  // При ошибке возвращает ноль

long int MyMapAccess::mapGetSiteNumber(HMAP hMap,HSITE hSite)
{
	HMAP (WINAPI * lpfn_mapSitOrder)(HMAP, HSITE);          
	(FARPROC&)lpfn_mapSitOrder = ::GetProcAddress(LibInst, "mapGetSiteNumber");   
	return( * lpfn_mapSitOrder)( hMap, hSite); 
}


// Создать пользовательскую карту по открытой карте
  // hmap -  идентификатор открытых данных
  // mapname - полное имя файла карты
  // rscname - полное имя файла ресурсов
  // Структура CREATESITE описана в maptype.h
  // Возвращает идентификатор открытой пользовательской карты
  // При ошибке возвращает ноль
//_MAPIMP HSITE _MAPAPI mapCreateAndAppendSite(HMAP hmap, const char * mapname,const char * rscname, CREATESITE * createsite);
HSITE MyMapAccess::mapCreateAndAppendSite(HMAP hMap, 
											const char * mapname,
											const char * rscname, 
											CREATESITE * createsite)
{
	HSITE (WINAPI * lpfn_mapCreateAndAppendSite)(HMAP , const char * ,const char * , CREATESITE * );          
	(FARPROC&)lpfn_mapCreateAndAppendSite = ::GetProcAddress(LibInst, "mapCreateAndAppendSite");   
	return ( * lpfn_mapCreateAndAppendSite)(hMap,mapname,rscname,createsite); 
}
// Создать пользовательскую карту по открытой карте
  // hmap -  идентификатор открытых данных
  // mapname - полное имя файла карты в кодировке UNICODE
  // rscname - полное имя файла ресурсов в кодировке UNICODE
  // Структура CREATESITEUN описана в maptype.h
  // Возвращает идентификатор открытой пользовательской карты
  // При ошибке возвращает ноль
//_MAPIMP HSITE _MAPAPI mapCreateAndAppendSiteUn(HMAP hmap,  const WCHAR *  mapname,const WCHAR * rscname, CREATESITEUN * createsite);          // 11/05/07 
HSITE MyMapAccess::mapCreateAndAppendSiteUn(HMAP hMap,  
											const WCHAR *  mapname,
											const WCHAR * rscname, 
											CREATESITEUN * createsite)
{
	HSITE (WINAPI * lpfn_mapCreateAndAppendSiteUn)(HMAP ,  const WCHAR *  ,const WCHAR * , CREATESITEUN * );          
	(FARPROC&)lpfn_mapCreateAndAppendSiteUn = ::GetProcAddress(LibInst, "mapCreateAndAppendSiteUn");   
	return( * lpfn_mapCreateAndAppendSiteUn)(hMap, mapname,rscname, createsite); 
}
 // Выделить на карте объекты, удовлетворющие заданным условим
 // hmap   - идентификатор открытых данных
 // hwnd   - идентификатор окна вывода
 // point - координаты верхнего левого угла окна на карте
 // в соответсвующей параметру place системе координат
 // select - условия отбора объектов,
 // color  - цвет, которым будут выделяться объекты на карте
//_MAPIMP void _MAPAPI mapViewSelect(HMAP hmap, HWND hwnd, DOUBLEPOINT * point, HSELECT select, COLORREF color, long int place = PP_MAP);  
void MyMapAccess::mapViewSelect(HMAP hMap, HWND hWnd, DOUBLEPOINT * point, HSELECT select, COLORREF color, long int place)
{
	void (WINAPI * lpfn_mapViewSelect)(HMAP, HWND, DOUBLEPOINT *, HSELECT, COLORREF, long int);          
	(FARPROC&)lpfn_mapViewSelect = ::GetProcAddress(LibInst, "mapViewSelect");   
	return( * lpfn_mapViewSelect)(hMap, hWnd, point, select, color, place); 
}
  /*********************************************************
  //*                                                        *
  //* ФУНКЦИИ СОЗДАНИЯ/УДАЛЕНИЯ КОНТЕКСТА ПОИСКА/ОТОБРАЖЕНИЯ *
  //*                                                        *
  *********************************************************/
  // Создать контекст (описание условий) поиска/отображения
  // объектов карты
  // �� состав условий отбора объектов входят : лист, слой,
  // локализация, диапазон номеров объектов, характеристики
  // (семантика) объекта, область расположения (метрика) объекта
  // В созданном контексте доступны все объекты карты без исключений
  // Запрашивается минимум 10 Кб памти,
  // если заданы условия поиска по метрике и семантике - до 300 Кб
  // Каждый созданный контекст должен быть удален, когда
  // он больше не используется
  // hMap   - идентификатор открытой карты,
  // Параметр hmap необходим, когда устанавливаются условия поиска/
  // отображения по семантике
  // При ошибке возвращает ноль
//_MAPIMP  HSELECT _MAPAPI mapCreateMapSelectContext(HMAP hmap);}
 HSELECT MyMapAccess::mapCreateMapSelectContext(HMAP hMap)
{
	HSELECT (WINAPI * lpfn_mapCreateMapSelectContext)(HMAP);          
	(FARPROC&)lpfn_mapCreateMapSelectContext = ::GetProcAddress(LibInst, "mapCreateMapSelectContext");   
	return ( * lpfn_mapCreateMapSelectContext)(hMap); 
}
 
 // Поиск обектов в окрестности точки, заданной прямоугольной рамкой
 // Применяется для выбора объекта при нажатии левой кнопки мыши на карте
 // hMap - идентификатор открытой карты.
 // info - идентификатор объекта в памяти,
 //        предварительно созданного функцией mapCreateObject()
 //        или mapCreateSiteObject(),
 //        в котором будет размещен результат поиска.
 // Поиск выполнется среди тех объектов,которые видны на экране !
 // frame - прямоугольная область поиска объекта в системе координат,
 //         заданной переменной place (PP_PLANE,PP_GEO, ...)
 // Площадные объекты выбираются в пределах рамки               // 30/10/06
 // размером 512х512 пикселов в текущем масштабе изображения
 // flag - порядок поиска объектов (WO_FIRST, WO_NEXT...)
 // Выбор объекта в "точке" лучше начинать с последнего, то
 // есть того, что нарисован поверх остальных (это чуть медленнее
 // прямого поиска).
 // При поиске с флажками WO_NEXT,WO_BACK параметр info должен
 // содержать результат предыдущего поиска.
 // place - система координат.
 // Если объект не найден - возвращает ноль,
 // иначе - возвращает значение info !
//#ifdef __cplusplus
//_MAPIMP HOBJ _MAPAPI mapWhatObject(HMAP hMap,HOBJ info,MAPDFRAME * frame,long int flag = WO_LAST,long int place = PP_MAP);
HOBJ MyMapAccess::mapWhatObject(HMAP hMap,
								HOBJ info,
								MAPDFRAME * frame,
								long int flag,
								long int place)
{
	HOBJ (WINAPI * lpfn_mapWhatObject)(HMAP,HOBJ,MAPDFRAME *,long int,long int place);          
	(FARPROC&)lpfn_mapWhatObject = ::GetProcAddress(LibInst, "mapWhatObject");   
	return ( * lpfn_mapWhatObject)(hMap,info,frame,flag,place); 
}
// Преобразование из дискретов на карте (районе работ)
 // в метры на местности
 // на входе дискреты, на выходе - метры.
//_MAPIMP void _MAPAPI mapMapToPlane(HMAP hmap,double * x, double * y);
void MyMapAccess::mapMapToPlane(HMAP hMap, 
								double * x,
								double * y)
{
	void (WINAPI * lpfn_mapMapToPlane)(HMAP ,double *, double *);          
	(FARPROC&)lpfn_mapMapToPlane = ::GetProcAddress(LibInst, "mapMapToPlane");   
	( * lpfn_mapMapToPlane)(hMap, x, y); 
}
 // Преобразование из пикселов в изображении в координаты
 // на местности в метрах
 // Применение :
 // xmet = xdis; ymet = ydis;
 // mapPictureToPlane(xmet,ymet);
 // hmap - идентификатор открытых данных
 // x,y  - преобразуемые координаты
//_MAPIMP void _MAPAPI mapPictureToPlane(HMAP hmap,double * x, double * y);
void MyMapAccess::mapPictureToPlane(HMAP hMap,
									double * x, 
									double * y)
{
	void (WINAPI * lpfn_mapPictureToPlane)(HMAP ,double *, double *);          
	(FARPROC&)lpfn_mapPictureToPlane = ::GetProcAddress(LibInst, "mapPictureToPlane");   
	( * lpfn_mapPictureToPlane)(hMap, x, y); 
}
//===================================================================================
//===================================================================================
 // Запросить имя главной карты в проекте										=====
 // При ошибке возвращает пустую строку											=====
//===================================================================================
//===================================================================================
//_MAPIMP const char * _MAPAPI mapGetMainName(HMAP hMap);
const char * MyMapAccess::mapGetMainName(HMAP hMap)
{
	const char * (WINAPI * lpfn_mapGetMainName)(HMAP);          
	(FARPROC&)lpfn_mapGetMainName = ::GetProcAddress(LibInst, "mapGetMainName");   
	return( * lpfn_mapGetMainName)(hMap); 
}
//======================================================================================
//
//
//                  Запросы
//
//========================================================================================
 // Запросить название объекта по порядковому номеру (number)
 // в заданном слое
 // hmap - идентификатор открытых данных
 // layer - номер слоя в классификаторе
 // При ошибке возвращает ноль или пустую строку
//_MAPIMP const char * _MAPAPI mapRscObjectNameInLayer(HMAP hmap,long int layer,long int number);
const char * MyMapAccess::mapRscObjectNameInLayer(HMAP hmap,long int layer,long int number)
{
	const char * (WINAPI * lpfn_mapRscObjectNameInLayer)(HMAP ,long int ,long int );          
	(FARPROC&)lpfn_mapRscObjectNameInLayer = ::GetProcAddress(LibInst, "mapRscObjectNameInLayer");   
	return( * lpfn_mapRscObjectNameInLayer)(hmap,layer,number); 
}
// Запросить классификационный код объекта
 // по порядковому номеру в заданном слое (number)
 // hmap - идентификатор открытых данных
 // layer - номер слоя в классификаторе
 // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapRscObjectExcodeInLayer(HMAP hmap,long int layer,long int number);
long int * MyMapAccess::mapRscObjectExcodeInLayer(HMAP hmap,long int layer,long int number)
{
	long int * (WINAPI * lpfn_mapRscObjectExcodeInLayer)(HMAP ,long int ,long int );          
	(FARPROC&)lpfn_mapRscObjectExcodeInLayer = ::GetProcAddress(LibInst, "mapRscObjectExcodeInLayer");   
	return( * lpfn_mapRscObjectExcodeInLayer)(hmap,layer,number); 
}
 // Запросить условное название объекта
 // info - идентификатор объекта карты в памяти
 // name - адрес буфера для ��езультата запроса
 // size - размер буфера
 // При ошибке возвращает ноль
//_MAPIMP const char * _MAPAPI mapObjectName(HOBJ info);
const char * MyMapAccess::mapObjectName(HOBJ info)
{
	const char * (WINAPI * lpfn_mapObjectName)(HOBJ);          
	(FARPROC&)lpfn_mapObjectName = ::GetProcAddress(LibInst, "mapObjectName");   
	return( * lpfn_mapObjectName)(info); 
}
  // Запросить - есть ли какие-либо открытые данные
  // Данные - векторные, растровые, матричные...
  // hmap -  идентификатор открытых данных
  // Если открытых данных нет или ошибка - возвращает ноль
//_MAPIMP long int _MAPAPI mapIsActive(HMAP hMap);
long int MyMapAccess::mapIsActive(HMAP hMap)
{
	long int (WINAPI * lpfn_mapIsActive)(HMAP);          
	(FARPROC&)lpfn_mapIsActive = ::GetProcAddress(LibInst, "mapIsActive");   
	return( * lpfn_mapIsActive)(hMap); 
}
// Запросить код ошибки последней операции доступа к данным
  // Коды ошибок - см. maperr.rh
//_MAPIMP long int _MAPAPI mapGetAccessError();
 long int MyMapAccess::mapGetAccessError()
{
	long int (WINAPI * lpfn_mapGetAccessError)();          
	(FARPROC&)lpfn_mapGetAccessError = ::GetProcAddress(LibInst, "mapGetAccessError");   
	return( * lpfn_mapGetAccessError)(); 
}
 // Запросить значение семантической характеристики объекта
 // Значение преобразуется в символьный вид без раскодирования
 // info   - идентификатор объекта карты в памяти
 // number - последовательный номер характеристики (c 1),
 // value  - адрес размещения строки,
 // size   - максимальная длина строки
 // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapSemanticValue(HOBJ info, long int number,  char * value,  long int size);
 long int MyMapAccess::mapSemanticValue(HOBJ info, long int number,  char * value,  long int size)
{
	long int (WINAPI * lpfn_mapSemanticValue)(HOBJ, long int,  char *,  long int);          
	(FARPROC&)lpfn_mapSemanticValue = ::GetProcAddress(LibInst, "mapSemanticValue");   
	return( * lpfn_mapSemanticValue)(info, number,  value,  size); 
}
  // Запросить значение семантической характеристики объекта
 // в виде числа с плавающей точкой двойной точности
 // info    - идентификатор объекта карты в памяти
 // number  - последовательный номер характеристики (c 1)
 // Если значение семантики не может быть преобразовано
 // к числовому виду или не найдено - возвращает ноль
//_MAPIMP double _MAPAPI mapSemanticDoubleValue(HOBJ info,long int number);
 double MyMapAccess::mapSemanticDoubleValue(HOBJ info, 
											long int number)
{
	double (WINAPI * lpfn_mapSemanticDoubleValue)(HOBJ, long int);          
	(FARPROC&)lpfn_mapSemanticDoubleValue = ::GetProcAddress(LibInst, "mapSemanticDoubleValue");   
	return( * lpfn_mapSemanticDoubleValue)(info, number); 
}
  // Запросить прямоугольные координаты габаритов карты
  // в метрах (система координат PLANE)
  // X - снизу вверх, Y - слева направо
  // т.1 - нижний левый угол,
  // т.2 - верхний правый
  // hmap - идентификатор открытых данных
//===========================================================================
//_MAPIMP double _MAPAPI mapGetMapX1(HMAP hmap);
double MyMapAccess::mapGetMapX1(HMAP hMap)
{
	double (WINAPI * lpfn_mapGetMapX1)(HMAP);          
	(FARPROC&)lpfn_mapGetMapX1 = ::GetProcAddress(LibInst, "mapGetMapX1");   
	return( * lpfn_mapGetMapX1)(hMap); 
}
//_MAPIMP double _MAPAPI mapGetMapY1(HMAP hmap);
double MyMapAccess::mapGetMapY1(HMAP hMap)
{
	double (WINAPI * lpfn_mapGetMapY1)(HMAP);          
	(FARPROC&)lpfn_mapGetMapY1 = ::GetProcAddress(LibInst, "mapGetMapY1");   
	return( * lpfn_mapGetMapY1)(hMap); 
}
//_MAPIMP double _MAPAPI mapGetMapX2(HMAP hmap);
double MyMapAccess::mapGetMapX2(HMAP hMap)
{
	double (WINAPI * lpfn_mapGetMapX2)(HMAP);          
	(FARPROC&)lpfn_mapGetMapX2 = ::GetProcAddress(LibInst, "mapGetMapX2");   
	return( * lpfn_mapGetMapX2)(hMap); 
}
//_MAPIMP double _MAPAPI mapGetMapY2(HMAP hmap);
double MyMapAccess::mapGetMapY2(HMAP hMap)
{
	double (WINAPI * lpfn_mapGetMapY2)(HMAP);          
	(FARPROC&)lpfn_mapGetMapY2 = ::GetProcAddress(LibInst, "mapGetMapY2");   
	return( * lpfn_mapGetMapY2)(hMap); 
}

/*int MyMapAccess::mapCreateSite()
{
	int (WINAPI * lpfn_)();          
	(FARPROC&)lpfn_ = ::GetProcAddress(LibInst, "mapCreateSite");   
	return( * lpfn_)(); 
}*/

 //========================================================================================================
 //==================                                                                          ============
 //==================                      пользовательская карта                              ============
 //==================                                                                          ============
 //========================================================================================================
  // Добавить данные к открытой карте (карту, растр, матрицу)
  // name - имя открываемого файла (MAP, SIT, MTW, MTQ, RSW, MPT) в кодировке UNICODE
  // mode - режим чтения/записи (GENERIC_READ, GENERIC_WRITE или 0)
  // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapAppendDataUn(HMAP hMap, const WCHAR * name, long int mode);
long int MyMapAccess::mapAppendDataUn(HMAP hMap, 
									  const WCHAR * name, 
									  long int mode)
{
	long int (WINAPI * lpfn_mapAppendDataUn)(HMAP, const WCHAR *, long int);          
	(FARPROC&)lpfn_mapAppendDataUn = ::GetProcAddress(LibInst, "mapAppendDataUn");   
	return( * lpfn_mapAppendDataUn)(hMap,name,mode); 
}
 // Добавить данные к открытой карте (карту, растр, матрицу)
  // name - имя открываемого файла (MAP, SIT, MTW, MTQ, RSW, MPT)
  // mode - режим чтения/записи (GENERIC_READ, GENERIC_WRITE или 0)
  // Возвращает идентификатор типа данных (FILE_MAP - для пользовательской
  // карты, FILE_RSW - для растра, FILE_MTW - для матрицы, FILE_MTL - для
  // матрицы слоев, FILE_MTQ - для матрицы качеств), данные добавляются в
  // список последними, если данные уже были открыты, число открытых данных
  // (карт, растров, матриц) не меняется
  // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapAppendData(HMAP hMap, const char * name,long int mode = 0);
long int MyMapAccess::mapAppendData(HMAP hMap, 
									  const char * name, 
									  long int mode)
{
	long int (WINAPI * lpfn_mapAppendData)(HMAP, const char *, long int);          
	(FARPROC&)lpfn_mapAppendData = ::GetProcAddress(LibInst, "mapAppendData");   
	return( * lpfn_mapAppendData)(hMap,name,mode); 
}
 // Открыть пользовательскую карту в заданном районе работ
  // (добавить в цепочку пользовательских карт (в обстановку))
  // Возвращает идентификатор открытой пользовательской карты
  // hMap     - идентификатор открытой карты
  // sitename - имя открываемого файла пользовательской карты
  // mode     - режим чтения/записи (GENERIC_READ, GENERIC_WRITE 
  //            или 0) GENERIC_READ - все данные только на чтение
  // При ошибке возвращает ноль
//_MAPIMP HSITE _MAPAPI mapOpenSiteForMap(HMAP hMap, const char * sitename,long int mode);
 HSITE MyMapAccess::mapOpenSiteForMap(HMAP hMap, 
									  const char * sitename, 
									  long int mode)
{
	 HSITE (WINAPI * lpfn_mapOpenSiteForMap)(HMAP, const char *, long int);          
	(FARPROC&)lpfn_mapOpenSiteForMap = ::GetProcAddress(LibInst, "mapOpenSiteForMap");   
	return( * lpfn_mapOpenSiteForMap)(hMap,sitename,mode); 
} 
 // Закрыть пользовательскую карту в заданном районе работ
  // hMap - идентификатор открытой карты
  // hSite - идентификатор открытой пользовательской карты
  // Если hSite == 0, закрываются все данные обстановки
  // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapCloseSiteForMap(HMAP hMap, HSITE hSite);
 HSITE MyMapAccess::mapCloseSiteForMap(HMAP hMap, 
										HSITE hSite)
{
	 HSITE (WINAPI * lpfn_mapCloseSiteForMap)(HMAP, HSITE);          
	(FARPROC&)lpfn_mapCloseSiteForMap = ::GetProcAddress(LibInst, "mapCloseSiteForMap");   
	return( * lpfn_mapCloseSiteForMap)(hMap, hSite); 
} 

  // Закрыть пользовательскую карту в заданном районе работ
  // hMap - идентификатор открытой карты
  // name - имя паспорта пользовательской карты
  // При ошибке возвращает ноль

HSITE MyMapAccess::mapCloseSiteForMapByName(HMAP hMap, const char * sitename)
{
	HSITE (WINAPI * lpfn_mapCloseSiteForMapByName)(HMAP, const char *);          
	(FARPROC&)lpfn_mapCloseSiteForMapByName = ::GetProcAddress(LibInst, "mapCloseSiteForMapByName");   
	return( * lpfn_mapCloseSiteForMapByName)(hMap,sitename); 
}


 // Преобразование из метров на местности (проекция карты)
 // в геодезические координаты в радианах (общеземной эллипсоид WGS84)
 // (поддерживается не для всех карт !)
 // Наличие высоты повышает точность расчетов,
 // функция mapPlaneToGeoWGS84() пытается
 // определить высоту из матрицы
 // Применение :
 // if (mapIsGeoSupported())
 //   {
 //     B = Xmet; L = Ymet;
 //     mapPlaneToGeoWGS84(hMap,B,L);
 //   }
 // hmap  - идентификатор открытых данных
 // Bx,Ly - преобразуемые координаты
 // на входе метры, на выходе - радианы
 // H     - высота в точке (метры)
 // При ошибке возвращает 0

long int MyMapAccess::mapPlaneToGeoWGS84(HMAP hmap,
										 double *Bx, 
										 double *Ly)
{
	long int (WINAPI * lpfn_mapPlaneToGeoWGS84)(HMAP ,double *, double *);          
	(FARPROC&)lpfn_mapPlaneToGeoWGS84 = ::GetProcAddress(LibInst, "mapPlaneToGeoWGS84");   
	return ( * lpfn_mapPlaneToGeoWGS84)(hmap, Bx, Ly); 
}


long int MyMapAccess::mapPlaneToGeoWGS843D(HMAP hmap,
										   double *Bx, 
										   double *Ly, 
										   double *H)
{
	long int (WINAPI * lpfn_mapPlaneToGeoWGS843D)(HMAP ,double *, double *, double *);          
	(FARPROC&)lpfn_mapPlaneToGeoWGS843D = ::GetProcAddress(LibInst, "mapPlaneToGeoWGS843D");   
	return ( * lpfn_mapPlaneToGeoWGS843D)(hmap, Bx, Ly, H); 
}

//  Преобразование из геодезических координат в радианах
 // (общеземной эллипсоид WGS84)
 //  в метры на местности в проекции карты
 // (поддерживается не для всех карт !)
 // hmap - идентификатор открытых данных
 // Bx,Ly,H  - преобразуемые координаты
 // на входе радианы, на выходе - метры
 // При ошибке возвращает ноль

long int MyMapAccess::mapGeoWGS84ToPlane3D(HMAP hmap, 
										   double *Bx, 
										   double *Ly, 
										   double *H)
{
	long int (WINAPI * lpfn_mapGeoWGS84ToPlane3D)(HMAP ,double *, double *, double *);          
	(FARPROC&)lpfn_mapGeoWGS84ToPlane3D = ::GetProcAddress(LibInst, "mapGeoWGS84ToPlane3D");   
	return ( * lpfn_mapGeoWGS84ToPlane3D)(hmap, Bx, Ly, H); 

}

 // Запрос - поддерживается ли пересчет к геодезическим
 // координатам из плоских прямоугольных и обратно
 // hmap - идентификатор открытых данных
 // Если нет - возвращает ноль

long int MyMapAccess::mapIsGeoSupported(HMAP hmap)
{
	long int (WINAPI * lpfn_mapIsGeoSupported)(HMAP);          
	(FARPROC&)lpfn_mapIsGeoSupported = ::GetProcAddress(LibInst, "mapIsGeoSupported");   
	return ( * lpfn_mapIsGeoSupported)(hmap); 
}

	// Преобразование координат из градусов в радианы
 // (для положительного значения)
 // degree - структура, содержащая координаты в градусах, минутах,
 // секундах. Описана в maptype.h
 // radian - значение в радианах

void MyMapAccess::mapDegreeToRadian(GEODEGREE * degree,
									double * radian)
{
	void (WINAPI * lpfn_mapDegreeToRadian)(GEODEGREE * ,double *);          
	(FARPROC&)lpfn_mapDegreeToRadian = ::GetProcAddress(LibInst, "mapDegreeToRadian");   
	( * lpfn_mapDegreeToRadian)(degree, radian); 
}
 // Преобразование из метров на местности в дискреты
 // на карте (районе работ)
 // hmap - идентификатор открытых данных
 // x,y  - преобразуемые координаты
//_MAPIMP void _MAPAPI mapPlaneToMap(HMAP hmap,double * x, double * y);
void MyMapAccess::mapPlaneToMap(HMAP hMap, double * x, double * y)
{
	void (WINAPI * lpfn_mapPlaneToMap)(HMAP ,double *, double *);          
	(FARPROC&)lpfn_mapPlaneToMap = ::GetProcAddress(LibInst, "mapPlaneToMap");   
	( * lpfn_mapPlaneToMap)(hMap, x, y); 
}
 // Преобразование координат из радиан в градусы
 // (для положительного значения)
 // radian - значение в радианах
 // degree - структура, содержащая координаты в градусах, минутах,
 // секундах. Описана в maptype.h

void MyMapAccess::mapRadianToDegree(double * radian,
									GEODEGREE * degree)
{
	void (WINAPI * lpfn_mapRadianToDegree)(double *, GEODEGREE *);          
	(FARPROC&)lpfn_mapRadianToDegree = ::GetProcAddress(LibInst, "mapRadianToDegree");   
	( * lpfn_mapRadianToDegree)(radian, degree); 

}

 // Преобразование из геодезических координат в радианах
 // в метры на местности в соответствии с проекцией карты
 // (поддерживается не для всех карт !)
 // hmap - идентификатор открытых данных
 // Bx,Ly  - преобразуемые координаты
 // на входе радианы, на выходе - метры
 // При ошибке возвращает 0

long int MyMapAccess::mapGeoToPlane(HMAP hmap,
									double *Bx, 
									double *Ly)
{
	long int (WINAPI * lpfn_mapGeoToPlane)(HMAP ,double *, double *);          
	(FARPROC&)lpfn_mapGeoToPlane = ::GetProcAddress(LibInst, "mapGeoToPlane");   
	return ( * lpfn_mapGeoToPlane)(hmap, Bx, Ly); 
}

	// Преобразование из метров на местности (проекция карты)
 // в геодезические координаты в радианах (эллипсоид Красовского)
 // (поддерживается не для всех карт !)
 // hmap  - идентификатор открытых данных
 // Bx,Ly - преобразуемые координаты
 // на входе метры, на выходе - радианы
 // H     - высота в точке (метры)
 // При ошибке возвращает ноль

long int MyMapAccess::mapPlaneToGeo42(HMAP hmap,
									  double *Bx, 
									  double *Ly)
{
	long int (WINAPI * lpfn_mapPlaneToGeo42)(HMAP ,double *, double *);          
	(FARPROC&)lpfn_mapPlaneToGeo42 = ::GetProcAddress(LibInst, "mapPlaneToGeo42");   
	return ( * lpfn_mapPlaneToGeo42)(hmap, Bx, Ly); 	
}

long int MyMapAccess::mapPlaneToGeo423D(HMAP hMap,
										double *Bx, 
										double *Ly, 
										double *H)
{
	long int (WINAPI * lpfn_mapPlaneToGeo423D)(HMAP ,double *, double *, double *);          
	(FARPROC&)lpfn_mapPlaneToGeo423D = ::GetProcAddress(LibInst, "mapPlaneToGeo423D");   
	return ( * lpfn_mapPlaneToGeo423D)(hMap, Bx, Ly, H); 
}

  // Запросить - может ли карта редактироваться
  // hMap  - идентификатор открытой карты
  // hSite - идентификатор открытой пользовательской карты
  // Если нет - возвращает ноль
//_MAPIMP long int _MAPAPI mapGetSiteEditFlag(HMAP hMap,HSITE hSite);
long int MyMapAccess::mapGetSiteEditFlag(HMAP hMap,
										 HSITE hSite)
{
	long int (WINAPI * lpfn_mapGetSiteEditFlag)(HMAP, HSITE);          
	(FARPROC&) lpfn_mapGetSiteEditFlag = ::GetProcAddress(LibInst, "mapGetSiteEditFlag");   
	return ( * lpfn_mapGetSiteEditFlag)(hMap, hSite); 
}

  // Установить флаг редактирования карты (0 - не редактировать)
  // hMap  - идентификатор открытой карты
  // hSite - идентификатор открытой пользовательской карты
  // flag  - признак возможности редактирования
  // Возвращает новое значение флага
//_MAPIMP long int _MAPAPI mapSetSiteEditFlag(HMAP hMap, HSITE hSite, long int flag);  //28/11/02
long int MyMapAccess::mapSetSiteEditFlag(HMAP hMap,
										 HSITE hSite, 
										 long int flag)
{
	long int (WINAPI * lpfn_mapSetSiteEditFlag)(HMAP, HSITE, long int);          
	(FARPROC&) lpfn_mapSetSiteEditFlag = ::GetProcAddress(LibInst, "mapSetSiteEditFlag");   
	return ( * lpfn_mapSetSiteEditFlag)(hMap, hSite, flag); 
}
//========================================================================================
//=========                                                                       ========
//=========                             Обновить                                  ========
//=========                                                                       ========
//========================================================================================
 // Обновить изображение заданного фрагмента карты в образе экрана      // 12/01/08
  // После обновления карты изображение перемещаемых объектов стирается
  // в пределах заданного фрагмента (но для стирания объектов текущим
  // видом карты достаточно вызвать mapClearImageObjects).
  // himage  - идентификатор буфера окна
  // onlymap - признак обновления только буфера карты (оптимизация скорости отрисовки
  //           при дальнейшем вызове mapClearImageObjects)
  // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapDrawImageMap(HIMAGE himage, HMAP hMap, RECT * rect,  POINT * position);
 long int MyMapAccess::mapDrawImageMap(HIMAGE himage, 
										HMAP hMap, 
										RECT * rect,  
										POINT * position)
{
	long int (WINAPI * lpfn_mapDrawImageMap)(HIMAGE, HMAP, RECT *,  POINT *);          
	(FARPROC&)lpfn_mapDrawImageMap = ::GetProcAddress(LibInst, "mapDrawImageMap");   
	return ( * lpfn_mapDrawImageMap)(himage, hMap, rect,  position); 
 }
				/*********************************************************
				 *                                                        *
				 * ФУНКЦИИ ЗАПРОСА ИНФОРМАЦИИ ИЗ КЛАССИФИКАТОРА ОБ'ЕКТОВ  *
				 *                                                        *
				 *********************************************************/
   // Очистить содержимое объекта
  // (для повторного заполнения, как пустого объекта)
  // hmap - идентификатор открытых данных
  // sheetnumber - номер листа в котором будет расположен
  // kind - тип создаваемой метрики, описан в maptype.h
  // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapClearObject(HOBJ info,  long int sheetnumber = 1,  long int kind = IDFLOAT2);
long int MyMapAccess::mapClearObject(HOBJ info,  
									 long int sheetnumber,  
									 long int kind)
{
	long int (WINAPI * lpfn_mapClearObject)(HOBJ,  long int,  long int);          
	(FARPROC&)lpfn_mapClearObject = ::GetProcAddress(LibInst, "mapClearObject");   
	return ( * lpfn_mapClearObject)(info,  sheetnumber, kind); 
}
  // Запросить идентификатор классификатора карты
  // hMap  - идентификатор открытой карты
  // hSite - идентификатор открытой пользовательской карты
  // При ошибке возвращает ноль
  //_MAPIMP  HRSC _MAPAPI mapGetRscIdent(HMAP hMap,HSITE hSite);
 HRSC MyMapAccess::mapGetRscIdent(HMAP hMap,
									HSITE hSite)
{
	HRSC (WINAPI * lpfn_mapGetRscIdent)(HMAP, HSITE);          
	( FARPROC&) lpfn_mapGetRscIdent = ::GetProcAddress(LibInst, "mapGetRscIdent");   
	return ( * lpfn_mapGetRscIdent)(hMap, hSite); 
 }
  // Запросить идентификатор классификатора карты, содержащей объект             //19/06/06
 // info - идентификатор объекта карты в памяти
 // При ошибке возвращает ноль
//_MAPIMP HRSC _MAPAPI mapGetRscIdentByObject(HOBJ info);
HRSC MyMapAccess::mapGetRscIdentByObject(HOBJ info)
{
	HRSC (WINAPI * lpfn_mapGetRscIdentByObject)(HOBJ);          
	( FARPROC&) lpfn_mapGetRscIdentByObject = ::GetProcAddress(LibInst, "mapGetRscIdentByObject");   
	return ( * lpfn_mapGetRscIdentByObject)(info); 
 }
 // Запросить количество видов семантик, которые еще могут быть
 // добавлены для данного объекта
 // Изменяется в процессе редактирования семантики объекта !
 // (некоторые характеристики могут присваиваться только один раз)
 // info    - идентификатор объекта карты в памяти
 // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapAvailableSemanticCount(HOBJ info);
 long int MyMapAccess::mapAvailableSemanticCount(HOBJ info)
{
	long int (WINAPI * lpfn_mapAvailableSemanticCount)(HOBJ);          
	( FARPROC&) lpfn_mapAvailableSemanticCount = ::GetProcAddress(LibInst, "mapAvailableSemanticCount");   
	return ( * lpfn_mapAvailableSemanticCount)(info); 
 }
  // Запросить внешний код доступной семантики на объект
 // по последовательному номеру доступных семантик
 // Изменяется в процессе редактирования семантики объекта !
 // info    - идентификатор объекта карты в памяти
 // number - последовательный номер доступных семантик (1,2,3...)
 // При ошибке возвращает ноль
 //_MAPIMP long int _MAPAPI mapAvailableSemanticCode(HOBJ info,int number);
long int MyMapAccess::mapAvailableSemanticCode(HOBJ info,
											   int number)
{
	long int (WINAPI * lpfn_mapAvailableSemanticCode)(HOBJ, int);          
	( FARPROC&) lpfn_mapAvailableSemanticCode = ::GetProcAddress(LibInst, "mapAvailableSemanticCode");   
	return ( * lpfn_mapAvailableSemanticCode)(info, number); 
 }
 // Изменить значение кода семантической характеристики объекта
 // info    - идентификатор объекта карты в памяти
 // number  - последовательный номер характеристики
 // code    - внешний код характеристики
 // При ошибке возвращает ноль,
 // иначе - внутренний код семантики
//_MAPIMP long int _MAPAPI mapSetSemanticCode(HOBJ info, long int number, long int code);
long int MyMapAccess::mapSetSemanticCode(HOBJ info, long int number, long int code)
{
	long int (WINAPI * lpfn_mapSetSemanticCode)(HOBJ, long int, long int);          
	( FARPROC&) lpfn_mapSetSemanticCode = ::GetProcAddress(LibInst, "mapSetSemanticCode");   
	return ( * lpfn_mapSetSemanticCode)(info, number, code); 
 }
 // Изменить значение семантической характеристики объекта
 // info    - идентификатор объекта карты в памяти
 // number  - последовательный номер характеристики,
 // place   - адрес строки, содержащей новое значение
 //           в символьном виде; Для семантики типа "классификатор"
 //           передается код значения в виде строки чисел,
 //           то же - для типа "ссылка на объект".
 // maxsize - длина передаваемой строки (для контроля)
 // При ошибке возвращает ноль

//_MAPIMP long int _MAPAPI mapSetSemanticValue(HOBJ info,long int number, char * place,long int maxsize);
long int MyMapAccess::mapSetSemanticValue(HOBJ info, long int number, char * place, long int maxsize)
{
	long int (WINAPI * lpfn_mapSetSemanticValue)(HOBJ, long int, char *, long int);          
	( FARPROC&) lpfn_mapSetSemanticValue = ::GetProcAddress(LibInst, "mapSetSemanticValue");   
	return ( * lpfn_mapSetSemanticValue)(info, number, place, maxsize); 
 }
 // Запросить число слоев на карте
 // hmap - идентификатор открытых данных
 // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapGetLayerCount(HMAP hmap);
long int MyMapAccess::mapGetLayerCount(HMAP hMap)
{
	long int (WINAPI * lpfn_mapGetLayerCount)(HMAP);          
	( FARPROC&) lpfn_mapGetLayerCount = ::GetProcAddress(LibInst, "mapGetLayerCount");   
	return ( * lpfn_mapGetLayerCount)(hMap); 
 }
 // Запросить код локализации объекта
 // по порядковому номеру в заданном слое (number)
 // hmap - идентификатор открытых данных
 // layer - номер слоя в классификаторе
 // При ошибке возвращает ноль (ноль допустим)
//{ LOCAL_LINE = 0,  LOCAL_SQUARE = 1, LOCAL_POINT = 2, LOCAL_TITLE = 3, LOCAL_VECTOR = 4, LOCAL_MIXED = 5 };
//_MAPIMP long int  _MAPAPI mapRscObjectLocalInLayer(HMAP hmap, long int layer, long int number);
long int MyMapAccess::mapRscObjectLocalInLayer(HMAP hMap, 
											   long int layer,
											   long int number)
{
	long int (WINAPI * lpfn_mapRscObjectLocalInLayer)(HMAP, long int, long int);          
	( FARPROC&) lpfn_mapRscObjectLocalInLayer = ::GetProcAddress(LibInst, "mapRscObjectLocalInLayer");   
	return ( * lpfn_mapRscObjectLocalInLayer)(hMap, layer, number); 
 }
 // Запросить название семантической характеристики объекта
 // hmap - идентификатор открытых данных
 // code    - код характеристики
 // При ошибке возвращает ноль или пустую строку
//_MAPIMP const char * _MAPAPI mapRscSemanticName(HMAP hmap,long int code);
const char * MyMapAccess::mapRscSemanticName(HMAP hMap,
											 long int code)
{
	const char * (WINAPI * lpfn_mapRscSemanticName)(HMAP, long int);          
	( FARPROC&) lpfn_mapRscSemanticName = ::GetProcAddress(LibInst, "mapRscSemanticName");   
	return ( * lpfn_mapRscSemanticName)(hMap, code); 
 }
// Запросить внутренний код (индекс) объекта
 // по внешнему коду (excode) и локализации (local)
 // hmap - идентификатор открытых данных
 // При ошибке возвращает ноль
//_MAPIMP long int  _MAPAPI mapRscObjectCode(HMAP hMap, long int excode,long int local);
long int MyMapAccess::mapRscObjectCode(HMAP hMap, 
									   long int excode,
									   long int local)
{
	long int (WINAPI * lpfn_mapRscObjectCode)(HMAP, long int, long int);          
	( FARPROC&) lpfn_mapRscObjectCode = ::GetProcAddress(LibInst, "mapRscObjectCode");   
	return ( * lpfn_mapRscObjectCode)(hMap, excode, local); 
 }
// Запросить значение семантической характеристики объекта
 // в виде числа с плавающей точкой двойной точности
 // info    - идентификатор объекта карты в памяти
 // code    - код характеристики,для которой ищется значение,
 // number  - последовательный номер найденного значения,
 //  не равен последовательному номеру характеристики !
 //  например : код code имеют 3-я и 6-я характеристики,
 //             соответственно для них number = 1 и 2,
 //             а при number = 3  - код возврата будет ноль.
 // Если значение семантики не может быть преобразовано
 // к числовому виду или не найдено - возвращает ноль
//_MAPIMP double _MAPAPI mapSemanticCodeDoubleValue(HOBJ info,long int code,long int number);
double MyMapAccess::mapSemanticCodeDoubleValue(HOBJ info,
											   long int code,
											   long int number)
{
	double (WINAPI * lpfn_mapSemanticCodeDoubleValue)(HOBJ, long int, long int);          
	( FARPROC&) lpfn_mapSemanticCodeDoubleValue = ::GetProcAddress(LibInst, "mapSemanticCodeDoubleValue");   
	return ( * lpfn_mapSemanticCodeDoubleValue)(info, code, number); 
 }
 // Запросить значение семантической характеристики объекта
 // Значение преобразуется в символьный вид
 // info    - идентификатор объекта карты в памяти
 // code    - код характеристики,для которой ищется значение,
 // place   - адрес размещения строки,
 // maxsize - максимальная длина строки
 // number  - последовательный номер найденного значения,
 //  не равен последовательному номеру характеристики !
 //  например : код code имеют 3-я и 6-я характеристики,
 //             соответственно для них number = 1 и 2,
 //             а при number = 3  - код возврата будет ноль.
 // При ошибке возвращает ноль,
 // при успешном выполнении - последовательный номер
 // найденной характеристики
//_MAPIMP long int _MAPAPI mapSemanticCodeValue(HOBJ info,long int code, char * place,long int maxsize,long int number);
long int MyMapAccess::mapSemanticCodeValue(HOBJ info,
										   long int code, 
										   char * place,
										   long int maxsize,
										   long int number)
{
	long int (WINAPI * lpfn_mapSemanticCodeValue)(HOBJ, long int, char *,long int, long int );          
	( FARPROC&) lpfn_mapSemanticCodeValue = ::GetProcAddress(LibInst, "mapSemanticCodeValue");   
	return ( * lpfn_mapSemanticCodeValue)(info, code, place, maxsize, number); 
 }

 // Запросить классификационный код объекта
 // info - идентификатор объекта карты в памяти
 // При ошибке возвращает 0 (ноль допустим для нового объекта)
//_MAPIMP long int _MAPAPI mapObjectExcode(HOBJ info);
long int MyMapAccess::mapObjectExcode(HOBJ info)
{
	long int (WINAPI * lpfn_mapObjectExcode)(HOBJ);          
	( FARPROC&) lpfn_mapObjectExcode = ::GetProcAddress(LibInst, "mapObjectExcode");   
	return ( * lpfn_mapObjectExcode)(info); 
 }
  // Отобразить объект поверх карты местности в образе экрана
  // himage  - идентификатор буфера окна
  // onlymap - признак обновления только буфера карты (оптимизация скорости отрисовки
  //           при дальнейшем вызове mapClearImageObjects)
  // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapDrawImageMapObject(HIMAGE himage, HMAP hMap, PAINTPARM * parm, HOBJ object);
long int MyMapAccess::mapDrawImageMapObject(HIMAGE himage, 
											HMAP hMap, 
											PAINTPARM * parm, 
											HOBJ object)
{
	long int (WINAPI * lpfn_mapDrawImageMapObject)(HIMAGE, HMAP, PAINTPARM *, HOBJ);          
	( FARPROC&) lpfn_mapDrawImageMapObject = ::GetProcAddress(LibInst, "mapDrawImageMapObject");   
	return ( * lpfn_mapDrawImageMapObject)(himage, hMap, parm, object); 
 }
 // Добавить новую характеристику в семантику объекта,   // 12/02/06
 // info    - идентификатор объекта карты в памяти
 // code    - внешний код характеристики
 // value   - значение в виде числа двойной точности
 // При ошибке возвращает ноль,
 // при успешном выполнении - последовательный номер
 // созданной характеристики
//_MAPIMP long int _MAPAPI mapAppendSemanticDouble(HOBJ info, long int code, double value);
long int MyMapAccess::mapAppendSemanticDouble(HOBJ info,
											  long int code, 
											  double value)
{
	long int (WINAPI * lpfn_mapAppendSemanticDouble)(HOBJ, long int, double);          
	( FARPROC&) lpfn_mapAppendSemanticDouble = ::GetProcAddress(LibInst, "mapAppendSemanticDouble");   
	return ( * lpfn_mapAppendSemanticDouble)(info, code, value); 
 }

 // Добавить новую характеристику в семантику объекта
 // info    - и��ентификатор объекта карты в памяти
 // code    - внешний код характеристики
 // value   - адрес строки, содержащей новое значение
 //           в символьном виде, числа с плавающей точкой могут иметь
 //           разделителем только символ точка "."
 // size    - максимальная длина строки
 // (числовые значения будут преобразовываться в двоичный вид)
 // Если такая семантика была и она не повторяемая - значение заменяется  // 10/07/06
 // При ошибке возвращает ноль,
 // при успешном выполнении - последовательный номер
 // созданной характеристики
 // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapAppendSemantic(HOBJ info,long int code, const char * value, long int size);
long int MyMapAccess::mapAppendSemantic(HOBJ info,
										long int code, 
										const char * value,
										long int size)
{
	long int (WINAPI * lpfn_mapAppendSemantic)(HOBJ, long int, const char *, long int);          
	( FARPROC&) lpfn_mapAppendSemantic = ::GetProcAddress(LibInst, "mapAppendSemantic");   
	return ( * lpfn_mapAppendSemantic)(info, code, value, size); 
 }
// Установить/сбросить/запросить масштабируемость объекта
 // scale = 1 для установки масштабируемости
 //         0 для сброса признака масштабируемости
 // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapSetObjectScale(HOBJ info, long int scale);
long int MyMapAccess::mapSetObjectScale(HOBJ info, 
										long int scale)
{
	long int (WINAPI * lpfn_mapSetObjectScale)(HOBJ, long int);          
	( FARPROC&) lpfn_mapSetObjectScale = ::GetProcAddress(LibInst, "mapSetObjectScale");   
	return ( * lpfn_mapSetObjectScale)(info, scale); 
 }
 // Запросить/Установить диапазон масштабов видимости объекта
 // scale - масштаб отображения от 1:1 до 1:40 млн.
 // info - идентификатор объекта карты в памяти
//_MAPIMP long int _MAPAPI mapObjectTopScale(HOBJ info);
long int MyMapAccess::mapObjectTopScale(HOBJ info)
{
	long int (WINAPI * lpfn_mapObjectTopScale)(HOBJ);          
	( FARPROC&) lpfn_mapObjectTopScale = ::GetProcAddress(LibInst, "mapObjectTopScale");   
	return ( * lpfn_mapObjectTopScale)(info); 
 }
//_MAPIMP long int _MAPAPI mapSetObjectTopScale(HOBJ info,long int scale);
long int MyMapAccess::mapSetObjectTopScale(HOBJ info,
										   long int scale)
{
	long int (WINAPI * lpfn_mapSetObjectTopScale)(HOBJ, long int);          
	( FARPROC&) lpfn_mapSetObjectTopScale = ::GetProcAddress(LibInst, "mapSetObjectTopScale");   
	return ( * lpfn_mapSetObjectTopScale)(info, scale); 
 }
//_MAPIMP long int _MAPAPI mapObjectBotScale(HOBJ info);
long int MyMapAccess::mapObjectBotScale(HOBJ info)
{
	long int (WINAPI * lpfn_mapObjectBotScale)(HOBJ);          
	( FARPROC&) lpfn_mapObjectBotScale = ::GetProcAddress(LibInst, "mapObjectBotScale");   
	return ( * lpfn_mapObjectBotScale)(info); 
 }
//_MAPIMP long int _MAPAPI mapSetObjectBotScale(HOBJ info,long int scale);
long int MyMapAccess::mapSetObjectBotScale(HOBJ info, 
										   long int scale)
{
	long int (WINAPI * lpfn_mapSetObjectBotScale)(HOBJ, long int);          
	( FARPROC&) lpfn_mapSetObjectBotScale = ::GetProcAddress(LibInst, "mapSetObjectBotScale");   
	return ( * lpfn_mapSetObjectBotScale)(info, scale); 
 }
  // Запросить округленный масштаб отображения карты
  // hmap - идентификатор открытых данных
  // Возвращает значение знаменателя масштаба
//_MAPIMP long int _MAPAPI mapGetShowScale(HMAP hMap);
long int MyMapAccess::mapGetShowScale(HMAP hMap)
{
	long int (WINAPI * lpfn_mapGetShowScale)(HMAP);          
	( FARPROC&) lpfn_mapGetShowScale = ::GetProcAddress(LibInst, "mapGetShowScale");   
	return ( * lpfn_mapGetShowScale)(hMap); 
 }
 // Установить/сбросить/запросить признак "Не сжимать" объекта     14/12/05
 // press = 1 для установки признака "Не сжимать"
 //         0 для сброса признака
 // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapSetObjectPress(HOBJ info, long int press);

long int MyMapAccess::mapSetObjectPress(HOBJ info, 
										long int press)
{
	long int (WINAPI * lpfn_mapSetObjectPress)(HOBJ, long int);          
	( FARPROC&) lpfn_mapSetObjectPress = ::GetProcAddress(LibInst, "mapSetObjectPress");   
	return ( * lpfn_mapSetObjectPress)(info, press); 
 }
//_MAPIMP long int _MAPAPI mapGetObjectPress(HOBJ info);   // 27/12/05
long int MyMapAccess::mapGetObjectPress(HOBJ info)
{
	long int (WINAPI * lpfn_mapGetObjectPress)(HOBJ);          
	( FARPROC&) lpfn_mapGetObjectPress = ::GetProcAddress(LibInst, "mapGetObjectPress");   
	return ( * lpfn_mapGetObjectPress)(info); 
 }
 // Сформировать описание нового объекта (!!!)
 // по внутреннему коду объекта (см. mapRscObjectCode() и т.п.)
 // info - идентификатор объекта карты в памяти
 // Вызывается после mapCreateObject(...) и добавления
 // семантики (если она есть)
 // При ошибке возвращает ноль
//_MAPIMP long int _MAPAPI mapDescribeObject(HOBJ info, long int code);
long int MyMapAccess::mapDescribeObject(HOBJ info, 
										long int code)
{
	long int (WINAPI * lpfn_mapDescribeObject)(HOBJ, long int);          
	( FARPROC&) lpfn_mapDescribeObject = ::GetProcAddress(LibInst, "mapDescribeObject");   
	return ( * lpfn_mapDescribeObject)(info, code); 
 }
  // Установить масштаб отображения (знаменатель масштаба)
  //  Параметры:
  //          hmap - идентификатор открытых данных
  //          x, y - координаты предполагаемого "центра изображения"
  //                 (любой точки привязки) в окне в текущем масштабе
  //         scale - реальный масштаб отображения, который желают получить
  //  Возвращает:   0 - масштаб не изменился,  1 - масштаб изменился
  //          x, y - координаты предполагаемого "центра изображения"
  //                 в окне относительно всей картинки
  //                 в новом масштабе отображения
//_MAPIMP long int _MAPAPI mapSetViewScale(HMAP hmap, long int * x, long int * y, float scale);
long int MyMapAccess::mapSetViewScale(HMAP hMap, 
									  long int * x, 
									  long int * y, 
									  float scale)
{
	long int (WINAPI * lpfn_mapSetViewScale)(HMAP, long int *, long int *, float);          
	( FARPROC&) lpfn_mapSetViewScale = ::GetProcAddress(LibInst, "mapSetViewScale");   
	return ( * lpfn_mapSetViewScale)(hMap, x, y, scale); 
 }
  // Запросить контекст буфера окна
  // himage - идентификатор буфера окна
  // При ошибке возвращает ноль
//_MAPIMP HDC _MAPAPI mapGetImageDC(HIMAGE hImage);
HDC MyMapAccess::mapGetImageDC(HIMAGE hImage)
{
	HDC (WINAPI * lpfn_mapGetImageDC)(HIMAGE);          
	( FARPROC&) lpfn_mapGetImageDC = ::GetProcAddress(LibInst, "mapGetImageDC");   
	return ( * lpfn_mapGetImageDC)(hImage); 
 }

  /*********************************************************
  *                                                        *
  *          ФОРМИРОВАНИЕ БУФЕРА ОКНА КАРТЫ                *
  *                                                        *
  *********************************************************/

  // Создать буфер карты в памяти для исключения мигания     // 12/01/08
  // перемещаемых по карте объектов
  // width  - ширина клиентской части окна карты в точках,
  // height - высота клиентской части окна карты в точках.
  // Создается первый буфер экрана, второй создается при первом вызове
  // функции отображения объекта в буфер (Draw) -  для оптимального
  // применения функций при отображении карты и без перемещаемых объектов
  // Размер буфера =  (width * height * 4)
  // Всего может быть параллельно открыто до 256 образов экранов одновременно
  // При успешном выполнении возвращает идентификатор образа экрана
  // При ошибке возвращает ноль
//_MAPIMP HIMAGE _MAPAPI mapCreateImageEx(long int width, long int height);
HIMAGE MyMapAccess::mapCreateImageEx(long int width, 
									 long int height)
{
	HIMAGE (WINAPI * lpfn_mapCreateImageEx)(long int, long int);          
	( FARPROC&) lpfn_mapCreateImageEx = ::GetProcAddress(LibInst, "mapCreateImageEx");   
	return ( * lpfn_mapCreateImageEx)(width, height); 
 }

  // Создание буфера окна
  // hwnd - идентификатор окна, в котором рисуется карта
  // При ошибке возвращает ноль
//_MAPIMP HIMAGE _MAPAPI mapCreateImage(HWND hwnd);
 HIMAGE MyMapAccess::mapCreateImage(HWND hwnd)
{
	 HIMAGE (WINAPI * lpfn_mapCreateImage)(HWND);          
	(FARPROC&)lpfn_mapCreateImage = ::GetProcAddress(LibInst, "mapCreateImage");   
	return( * lpfn_mapCreateImage)(hwnd); 
}
  // Установить идентификатор окна для приема сообщений
  // от "затяжных" процессов (перекодировка при открытии
  // карты, нарезка объектов по заданной границе,...)
  // Код сообщения - 0x590,
  // wparm : -1 старт процесса,-2 завершение,0-100 процент выполнения
  // lparm : текст сообщения
  // Для отмены сообщений - установить идентификатор в ноль
  // (Идентификатор закрытого окна может привести к сбою в системе)
  // Для завершения процесса вернуть число 0x590
  // Возвращает предыдущее значение идентификатора
	//_MAPIMP HWND _MAPAPI mapSetHandleForMessage(HWND hwnd);
//_MAPIMP HWND _MAPAPI mapGetHandleForMessage();
HWND MyMapAccess::mapGetHandleForMessage()
{
	HWND (WINAPI * lpfn_mapGetHandleForMessage)();          
	( FARPROC&) lpfn_mapGetHandleForMessage = ::GetProcAddress(LibInst, "mapGetHandleForMessage");   
	return ( * lpfn_mapGetHandleForMessage)(); 
 }
 // Преобразование из дискретов на карте (районе работ)
 // в пикселы на изображении
 // hmap - идентификатор открытых данных
 // x,y  - преобразуемые координаты
 // на входе дискреты, на выходе - пикселы.
 // Применение :
 // xpix = xdis; ypix = ydis;
 // mapMapToPicture(xpix,ypix);
//_MAPIMP void _MAPAPI mapMapToPicture(HMAP hmap,double * x, double * y);
void MyMapAccess::mapMapToPicture(HMAP hMap, double * x, double * y)
{
	void (WINAPI * lpfn_mapMapToPicture)(HMAP, double *, double *);          
	( FARPROC&) lpfn_mapMapToPicture = ::GetProcAddress(LibInst, "mapMapToPicture");   
	( * lpfn_mapMapToPicture)(hMap, x, y); 
 }

 // Загрузить принтер
//_MAPIMP HPRINTER _EXPORTAPI prnLoadPrinter();
HPRINTER  MyMapAccess::prnLoadPrinter()
{
	HPRINTER  (WINAPI * lpfn_prnLoadPrinter)();          
	( FARPROC&) lpfn_prnLoadPrinter = ::GetProcAddress(LibInst, "prnLoadPrinter");   
	return ( * lpfn_prnLoadPrinter)(); 
 }
 // Вывести изображение карты в Image (массив)
 // Данная функция реализована только для платформы Windows !
 // Выводится фрагмент карты, заданный параметром rect.
 // Заполняется палитра в поле palette.
 // Изображение строится в текущем масштабе и составе объектов
 // Палитра Image только 256 цветов, размер точки 1 байт !
 // Если размеры заданного Image меньше размеров фрагмента -
 // изображение строится в пределах размеров Image.
 //  hmap          - идентификатор открытых данных
 //  palette       - адрес палитры (256 RGBQUAD-цветов)
 //  lpImage       - адрес первого байта области изображения.
 //  width, height - ширина и высота Image.
 //  rect          - координаты фрагмента карты (Draw)
 //                  в изображении (Picture).
 // При ошибке в параметрах возвращает ноль
//_MAPIMP long int _MAPAPI mapPaintToImage(HMAP hmap, RGBQUAD * palette, char * lpImage, long int width,long int height,  RECT * rect);
long int MyMapAccess::mapPaintToImage(HMAP hMap, 
									  RGBQUAD * palette, 
									  char * lpImage, 
									  long int width,
									  long int height,  
									  RECT * rect)
{
	long int (WINAPI * lpfn_mapPaintToImage)(HMAP, RGBQUAD *, char *, long int, long int, RECT *);          
	( FARPROC&) lpfn_mapPaintToImage = ::GetProcAddress(LibInst, "mapPaintToImage");   
	return ( * lpfn_mapPaintToImage)(hMap, palette, lpImage, width, height, rect); 
 }

//=====================================================================================================

							//РАСТР! ! ! ! ! ! ! ! ! ! ! ! ! !

//=====================================================================================================
// Открыть растровые данные в заданном районе работ
  // (добавить в цепочку растров)
  // Возвращает номер файла в цепочке растров
  // hMap    - идентификатор открытой векторной карты
  // rstname - имя файла растровой карты
  // mode    - режим чтения/записи (GENERIC_READ, GENERIC_WRITE или 0)
  // GENERIC_READ - все данные только на чтение
  // При ошибке возвращает ноль
//long int _MAPAPI mapOpenRstForMap(HMAP hMap, const char * rstname, long int mode);
long int  MyMapAccess::openRstForMap(HMAP hMap,
						const char * rstname, 
						long int mode)
{
	long int (WINAPI * lpfn_openRstForMap)(HMAP, const char *, long int);          
	( FARPROC&) lpfn_openRstForMap = ::GetProcAddress(LibInst, "mapOpenRstForMap");   
	return ( * lpfn_openRstForMap)(hMap, rstname, mode); 
 }
  // Закрыть растровые данные в заданном районе работ
  // hMap   - идентификатор открытой векторной карты
  // number - номер растрового файла в цепочке
  // Если number == 0, закрываются все растровые данные
  // При ошибке возвращает ноль
//_MAPIMP  long int _MAPAPI closeRstForMap(HMAP hMap, long int number);
long int  MyMapAccess::closeRstForMap(HMAP hMap, long int number)
{
	long int (WINAPI * lpfn_closeRstForMap)(HMAP, long int);          
	( FARPROC&) lpfn_closeRstForMap = ::GetProcAddress(LibInst, "mapCloseRstForMap");   
	return ( * lpfn_closeRstForMap)(hMap, number); 
 }
// Запросить/Установить порядок отображения растра
  // hMap   - идентификатор открытой векторной карты
  // number - номер растрового файла в цепочке
  //  (0 - под картой, 1 - над картой)
  // При ошибке возвращает 0
//_MAPIMP  long int _MAPAPI mapSetRstViewOrder(HMAP hMap, long int number, long int order);
//_MAPIMP  long int _MAPAPI mapGetRstViewOrder(HMAP hMap, long int number);
long int  MyMapAccess::setRstViewOrder(HMAP hMap, 
									   long int number,
										long int order)
{
	long int (WINAPI * lpfn_setRstViewOrder)(HMAP, long int, long int);          
	( FARPROC&) lpfn_setRstViewOrder = ::GetProcAddress(LibInst, "mapSetRstViewOrder");   
	return ( * lpfn_setRstViewOrder)(hMap, number, order); 
 }
