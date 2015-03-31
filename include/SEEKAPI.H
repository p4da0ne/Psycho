/****** SEEKAPI.H ************** Belenkov O.V.   ****** 05/08/08 ***
******* SEEKAPI.H ************** Sokolova T.O.   ****** 29/01/03 ***
******* SEEKAPI.H ************** Nefedieva O.A.  ****** 02/09/04 ***
******* SEEKAPI.H ************** Gustinovich N.A.****** 02/10/02 ***
******* SEEKAPI.H ************** Tarakanova I.R. ****** 24/05/07 ***
******* SEEKAPI.H ************** Kruzhkov A.E.   ****** 10/11/06 ***
******* SEEKAPI.H ************** Vitsko D.A.     ****** 22/11/05 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2008              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*            ОПИСАНИЕ ФУНКЦИЙ ПРОИСКА ОБ'ЕКТОВ КАРТЫ               *
*        ИНТЕРФЕЙС ДЛЯ ПРОГРАММ НА ЯЗЫКАХ C, PASCAL, BASIC         *
*                                                                  *
********************************************************************
*                                                                  *
*    Импортирование функций в Windows :                            *
*                                                                  *
*    // Загрузка библиотеки                                        *
*    HINSTANCE libInst = ::LoadLibrary("gisacces.dll");            *
*                                                                  *
*    // Вызов функции                                              *
*    HMAP (WINAPI * lpfn_OpenMap)(const char*, long int);          *
*    (FARPROC)lpfn_OpenMap = GetProcAddress(libInst, "OpenMap");   *
*    HMAP hMap = (*lpfn_OpenMap)("Noginsk.map",0);                 *
*                                                                  *
*    // Выгрузка библиотеки                                        *
*    ::FreeLibrary(libInst);                                       *
*                                                                  *
*******************************************************************/

#ifndef SEEKAPI_H
#define SEEKAPI_H

#ifdef __cplusplus
extern "C"
{
#endif

  /*********************************************************
  *                                                        *
  *        ФУНКЦИИ ПОИСКА ОБ'ЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ       *
  *                                                        *
  *     ПОИСК МОЖЕТ ВЫПОЛНЯТЬСЯ В СИСТЕМАХ КООРДИНАТ :     *
  *       ИЗОБРАЖЕНИЯ (Picture)  - в пикселах,             *
  *       КАРТЫ,ИЛИ РАЙОНА (Map) - в дискретах,            *
  *       В ПРЯМОУГОЛЬНОЙ НА                               *
  *       МЕСТНОСТИ (Plane)      - в метрах,               *
  *       В ГЕОДЕЗИЧЕСКОЙ НА                               *
  *       МЕСТНОСТИ (Geo)        - в радианах.             *
  *                                                        *
  *********************************************************/

 // Флажки, определяющие ПОРЯДОК ПОИСКА ОБЪЕКТОВ
 // Первый в цепочке,последний,следующий за найденым ранее,
 // предыдущий, вместе c удаленными, только в заданной карте.
 // Если указано WO_INMAP - номер карты определяется
 // из HSELECT !
 // Поиск начинается с флажков WO_FIRST или WO_LAST (если нет
 // объекта с которого начинается поиск), затем применяются
 // флажки WO_NEXT или WO_BACK (например, в цикле).
 // enum SEEKTYPE { WO_FIRST  = 0,  WO_LAST  = 2,
 //                 WO_NEXT   = 4,  WO_BACK  = 8,
 //                 WO_CANCEL = 16, WO_INMAP = 32 };


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
#ifdef __cplusplus
_MAPIMP HOBJ _MAPAPI mapWhatObject(HMAP hMap,HOBJ info,MAPDFRAME * frame,
                                   long int flag = WO_LAST,
                                   long int place = PP_MAP);
#else
_MAPIMP HOBJ _MAPAPI mapWhatObject(HMAP hMap,HOBJ info,MAPDFRAME * frame,
                                   long int flag,
                                   long int place);
#endif

#ifdef __cplusplus
 // Поиск обектов в окрестности точки, заданной прямоугольной рамкой,
 // среди активных объектов
 // Применяется для выбора объекта при нажатии левой кнопки мыши на карте
 // Поиск выполнется среди тех объектов,которые видны на экране !
 // Активные объекты - те, что доступны для интерактивного выбора (оператором)
 // Установка условий поиска выполняется функцией mapSetSiteActiveSelect()
 // hMap  - идентификатор открытой карты,
 // info  - идентификатор объекта в памяти,
 //         предварительно созданного функцией mapCreateObject()
 //         или mapCreateSiteObject(),
 //         в котором будет размещен результат поиска.
 // frame - прямоугольная область поиска объекта в системе координат,
 //         заданной переменной place (PP_PLANE,PP_GEO, ...)
 // flag  - порядок поиска объектов (WO_FIRST, WO_NEXT...)
 // place - система координат.
 // Если объект не найден - возвращает ноль

_MAPIMP long int _MAPAPI mapWhatActiveObject(HMAP hMap,HOBJ info,MAPDFRAME * frame,
                                             long int flag = WO_LAST,
                                             long int place = PP_MAP);
#endif

 // Установить порядок выбора подписей
 // hMap  - идентификатор открытой карты.
 // place - порядок выбора подписи,
 // если равен нулю, то при переборе в обратном порядке (WO_LAST,
 // WO_PREV) сначала будут идти подписи,
 // если не равен нулю, то при прямом выборе (WO_FIRST, WO_NEXT)
 // подписи будут первыми
 // По-умолчанию подписи в точке выбираются последними (для WO_LAST),
 // т.е. place = 1

_MAPIMP void _MAPAPI mapSetTextPlace(HMAP hMap, long int place);
_MAPIMP long int _MAPAPI mapGetTextPlace(HMAP hMap);

 // ЗАПРОС ОБ'ЕКТА ПО ЗАДАННЫМ КООРДИНАТАМ
 // hMap - идентификатор открытой карты.
 // info - идентификатор объекта в памяти,
 //        предварительно созданного функцией mapCreateObject()
 //        или mapCreateSiteObject(),
 //        в котором будет размещен результат поиска.
 // Поиск выполнется среди тех объектов,которые
 // соответствуют условиям HSELECT !
 // select- контекст условий выбора объектов,
 // place - система координат,
 // frame - прямоугольная область поиска объекта в системе координат,
 //         заданной переменной place (PP_PLANE,PP_GEO, ...)
 // flag  - порядок поиска объектов (WO_FIRST, WO_NEXT...)
 // Выбор объекта в "точке" лучше начинать с последнего, то
 // есть того, что нарисован поверх остальных (это чуть медленнее
 // прямого поиска).
 // При поиске с флажками WO_NEXT,WO_BACK параметр info должен
 // содержать результат предыдущего поиска.
 // Если объект не найден - возвращает ноль,
 // иначе - возвращает значение info !
#ifdef __cplusplus
_MAPIMP HOBJ _MAPAPI mapWhatObjectBySelect(HMAP hMap,HOBJ info,
                                    MAPDFRAME * frame,
                                    HSELECT select,
                                    long int flag = WO_LAST,
                                    long int place = PP_MAP);
#else
_MAPIMP HOBJ _MAPAPI mapWhatObjectBySelect(HMAP hMap,HOBJ info,
                                    MAPDFRAME * frame,
                                    HSELECT select,
                                    long int flag,
                                    long int place);
#endif

 // Поиск объекта по имени листа (номенклатуре) и номеру объекта
 // среди всех карт
 // hMap - идентификатор открытой карты.
 // info - идентификатор существующего объекта,
 //        в котором будет размещен результат поиска.
 // listname - название (номенклатура) листа,
 // key      - идентификатор объекта в листе,
 // При ошибке возвращает ноль

_MAPIMP HOBJ _MAPAPI mapSeekObject(HMAP hMap,HOBJ info,
                            const char * listname, long int key);

_MAPIMP HOBJ _MAPAPI mapSeekObjectInList(HMAP hMap, HSITE hSite, HOBJ info,
                                  long int list, long int key);


 // Поиск номера объекта по имени листа (номенклатуре) и номеру объекта
 // среди всех карт (в отличие от поиска объекта возвращает номер объекта
 // и для удаленных объектов, пока не было сортировки карты)
 // hMap     - идентификатор открытой карты,
 // listname - название (номенклатура) листа,
 // key      - идентификатор объекта в листе.
 // При ошибке возвращает ноль

_MAPIMP long int _MAPAPI mapSeekObjectNumber(HMAP hMap,
                                      const char * listname, long int key);


 // Поиск объектов по заданным условиям среди всех объектов
 // hMap     - идентификатор открытой карты,
 // info     - идентификатор существующего объекта,
 //            в котором будет размещен результат поиска.
 // select   - условия поиска объекта
 // flag     - порядок поиска объектов (WO_FIRST, WO_NEXT...)
 // При поиске с флажками WO_NEXT,WO_BACK параметр info должен
 // содержать результат предыдущего поиска.
 // Если объект не найден - возвращает ноль
#ifdef __cplusplus
_MAPIMP HOBJ _MAPAPI mapSeekSelectObject(HMAP hMap,HOBJ info,HSELECT select,
                                  long int flag = WO_FIRST);
#else
_MAPIMP HOBJ _MAPAPI mapSeekSelectObject(HMAP hMap,HOBJ info,HSELECT select,
                                  long int flag);
#endif

 // Запрос числа объектов на карте, удовлетворяющих условиям поиска
 // Подсчет выполняется на той карте, для которой был создан контекст поиска!
 // select - условия поиска объекта
 // При ошибке возвращает ноль

_MAPIMP long int _MAPAPI mapSeekSelectObjectCount(HMAP hMap, HSELECT select);

 // Поиск объектов по заданным условиям среди отображаемых объектов
 // (пересечение заданных условий с условиями отображения)
 // hMap     - идентификатор открытой карты,
 // info     - идентификатор существующего объекта,
 //            в котором будет размещен результат поиска.
 // select   - условия поиска объекта
 // flag     - порядок поиска объектов (WO_FIRST, WO_NEXT...)
 // Если объект не найден - возвращает ноль

#ifdef __cplusplus
_MAPIMP HOBJ _MAPAPI mapSeekViewObject(HMAP hMap,HOBJ info,HSELECT select,
                                long int flag = WO_FIRST);
#else
_MAPIMP HOBJ _MAPAPI mapSeekViewObject(HMAP hMap,HOBJ info,HSELECT select,
                                long int flag);
#endif


 //  ПОИСК ОБ'ЕКТА, ИМЕЮЩЕГО С ДАННЫМ ОБ'ЕКТОМ СМЕЖНЫЙ УЧАСТОК .
 //  Поиск ведется в карте, где находится выбранный объект
 //  Если объект не найден - возвращает ноль.
 //  hMap     - идентификатор открытой карты,
 //  info     - идентификатор существующего объекта,
 //             для которого надо найти смежные участки
 //  target   - указатель на существующий объект TObjectInfo,
 //             в котором будет размещен результат поиска.
 //  При поиске с флажками WO_NEXT,WO_BACK параметр target должен
 //  указывать на результат предыдущего поиска.
 //  select   - условия поиска объекта
 //  delta    - допуск до объекта в метрах (может равняться 0)
 //             в структуре MAPADJACENTSECTION поле number 
 //             для начального поиска должно быть равным 0.
 //  flag     - порядок поиска объектов (WO_FIRST, WO_NEXT...)
 //  При поиске с флажками WO_NEXT,WO_BACK параметр info должен
 //  содержать результат предыдущего поиска.
 //  Если объект не найден - возвращает ноль,
 //  иначе - возвращает номер участка и заполненную структуру MAPADJACENTSECTION
 //  (см. MAPTYPE.H)
 //  subject = 0 искать соседей только с внешним контуром
 // ( 1- с учетом подобъектов, нумерация точек объекта и подобъектов - сквозная)
#ifdef __cplusplus
_MAPIMP long int _MAPAPI mapSeekAdjacentObject(HMAP hMap,HOBJ info,
                                    HOBJ target,
                                    MAPADJACENTSECTION * section,
                                    HSELECT select,
                                    double delta = 0.0,
                                    long int flag = WO_FIRST | WO_INMAP,
                                    long int subject = 0);
#else
_MAPIMP long int _MAPAPI mapSeekAdjacentObject(HMAP hMap,HOBJ info,
                                    HOBJ target,
                                    MAPADJACENTSECTION * section,
                                    HSELECT select,
                                    double delta,
                                    long int flag,
                                    long int subject);
#endif


 //  ПОИСК СОСЕДЕЙ, ИМЕЮЩИХ С ДАННЫМ ОБ'ЕКТОМ СМЕЖНЫЙ УЧАСТОК .
 //  Поиск ведется в карте, где находится выбранный объект
 //  Если соседи не найдены - возвращает ноль,
 //  иначе - количество соседей.
 //  hMap  - идентификатор открытой карты,
 //  info  - идентификатор существующего объекта,
 //          для которого надо найти смежные участки
 //  MAPADJACENTLISTEX - память для соседей
 //  count - максимальное количество смежных участков
 //  select - условия поиска объектов для списка
 //  delta - допуск до объекта в метрах
 //  point - обработка сторон,образуемых повторяющимися точками:
 //  0 - заносить в список, 1 - нет
 //  subject - 0 искать соседей только с внешним контуром
 // ( 1- с учетом подобъектов, нумерация точек объекта и подобъектов - сквозная)
#ifdef __cplusplus
_MAPIMP long int _MAPAPI mapSeekAdjacentListEx(HMAP hMap,HOBJ info,
                                       MAPADJACENTLISTEX * list,
                                       long int count,
                                       HSELECT select,
                                       double delta = 0.0,
                                       long int point = 0,
                                       long int subject = 0);
#else
_MAPIMP long int _MAPAPI mapSeekAdjacentListEx(HMAP hMap,HOBJ info,
                                       MAPADJACENTLISTEX * list,
                                       long int count,
                                       HSELECT select,
                                       double delta,
                                       long int point,
                                       long int subject);
#endif


 // Проверить - подходит ли объект под условия поиска/отображения
 // Если подходит возвращает ненулевое значение
 // hMap     - идентификатор открытой карты,
 // select   - условия поиска/отображения
 // При ошибке возвращает ноль

_MAPIMP long int _MAPAPI mapTestObject(HOBJ info,HSELECT select);


 // Проверить - подходит ли объект под условия поиска/выделения
 // заданные на карте
 // Если подходит возвращает ненулевое значение
 // hMap     - идентификатор открытой карты,
 // select   - условия поиска/отображения
 // При ошибке возвращает ноль                 // 04/10/07

_MAPIMP long int _MAPAPI mapTotalTestObject(HMAP hMap, HOBJ info);


 // Выбор объекта по номеру листа и последовательному номеру объекта
 // (прямой доступ к объекту без перебора)
 // hMap     - идентификатор открытой карты,
 // hSite    - идентификатор открытой пользовательской карты,
 // info     - идентификатор существующего объекта,
 //            в котором будет размещен результат поиска;
 // list     - номер листа (для пользовательской карты всегда 1);
 // object   - последовательный номер объекта в листе
 // (начиная с 1 до mapGetObjectCount(...) или mapGetSiteObjectCount(...)).
 // Если объект имеет признак "удален" - возвращает 1 !!!
 // При успешном выполнении возвращает значение info.
 // При ошибке возвращает ноль

_MAPIMP HOBJ _MAPAPI mapReadObjectByNumber(HMAP hMap,HSITE hSite,HOBJ info,
                                    long int list,long int object);

 // Выбор объекта по номеру листа и уникальному номеру объекта
 // Если объект имеет признак "удален" - возвращает 1 !!!
 // При успешном выполнении возвращает значение info.
 // hMap     - идентификатор открытой карты,
 // hSite    - идентификатор открытой пользовательской карты,
 // info     - идентификатор существующего объекта,
 //            в котором будет размещен результат поиска;
 // list     - номер листа (для пользовательской карты всегда 1);
 // key      - уникальный номер объекта.
 // При ошибке возвращает ноль

_MAPIMP HOBJ _MAPAPI mapReadObjectByKey(HMAP hMap, HSITE hSite, HOBJ info,
                                 long int list, long int key);


  /*********************************************************
  *                                                        *
  * ФУНКЦИИ ОБОБЩЕННОГО ПОИСКА ОБ'ЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ  *
  *                                                        *
  *********************************************************/

 // Запросить правило обобщенного поиска по картам
 // hMap - идентификатор открытой карты,
 // если результат = -1, поиск будет выполняться по всем картам
 // (0 - карта местности, 1...n - пользовательские карты)
 // При ошибке возвращает число (-2)

_MAPIMP long int _MAPAPI mapGetTotalSeekMapRule(HMAP hMap);

 // Запросить имеются ли на основной или пользовательской карте объекты,
 // удовлетворяющие заданным условиям поиска
 // hMap   - идентификатор открытой карты,
 // number - номер карты
 // (0 - карта местности, 1...n - пользовательские карты)
 // Если на карте есть подходящие объекты - возвращает ненулевое значение

_MAPIMP long int _MAPAPI mapIsTotalSeekMapRule(HMAP hMap, long int number);

 // Установить правило обобщенного поиска по картам
 // hMap     - идентификатор открытой карты,
 // number   - номер карты, по которой выполняется поиск,
 // если number == -1, поиск будет выполняться по всем картам
 // (0 - карта местности, 1...n - пользовательские карты)
#ifdef __cplusplus
_MAPIMP void _MAPAPI mapSetTotalSeekMapRule(HMAP hMap,long int number = -1);
#else
_MAPIMP void _MAPAPI mapSetTotalSeekMapRule(HMAP hMap,long int number);
#endif


 // Установить правило обобщенного поиска для
 // отображаемых объектов карты
 // hMap - идентификатор открытой карты,
 // Если view == 0, поиск выполняется среди всех объектов
 // карты, иначе - среди отображаемых (SeekViewObject())
#ifdef __cplusplus
_MAPIMP void _MAPAPI mapSetTotalSeekViewRule(HMAP hMap,long int view = 0);
#else
_MAPIMP void _MAPAPI mapSetTotalSeekViewRule(HMAP hMap,long int view);
#endif
 long int _MAPAPI mapGetTotalSeekViewRule(HMAP hMap);


 // Установить условия поиска/выделения объектов по имени листа
 // и номеру объекта в карте
 // hMap     - идентификатор открытой карты,
 // listname - имя листа карты (карта выбирается автоматически)
 // key      - номер объекта в карте (BaseKey)
 // Если для карты установлены условия поиска по номерам объектов,
 // то остальные условия (слой,локализация...) игнорируются.
 // Если карта не найдена - возвращает ноль

_MAPIMP long int _MAPAPI mapSetTotalSeekSample(HMAP hMap,const char* listname,
                                        long int key);


 // Установить условия поиска/выделения объектов по всем картам
 // (пользовательским и карте местности)
 // hMap     - идентификатор открытой карты,
 // acceess  - признак отбора объектов :
 // =  0 - отключить отбор всех объектов всех карт (отключает все локализации),
 // != 0 - все объекты доступны при поиске в mapTotalSeekObject().
 // (Альтернатива перебору условий поиска для всех карт;
 //  перед применением mapSetTotalSeekSample доступ может быть отключен)
 // При ошибке возвращает ноль

_MAPIMP long int _MAPAPI mapSetTotalSeekAccess(HMAP hMap, long int access);


 // Выделить на карте объекты, удовлетворяющие условиям
 // обобщенного поиска
 // hMap     - идентификатор открытой карты,
 // hDC      - контекст устройства отображения,  
 // rect     - координаты фрагмента карты (Draw) в изображении (Picture)
 // color    - цвет, которым будут выделяться объекты на карте
 // Требует перед вызовом установки
 //              ::SetViewportOrgEx(hDC, dx , dy, 0),
 // где dx,dy - положение отображаемого фрагмента в клиентной
 // области !
 // При ошибке возвращает ноль

_MAPIMP long int _MAPAPI mapTotalPaintSelect95(HMAP hMap,HDC hDC, RECT * rect,
                                        COLORREF color);


 // Выделить на карте объекты, удовлетворяющие условиям
 // обобщенного поиска
 // hMap   - идентификатор открытых данных,
 // hwnd   - идентификатор окна вывода,
 // color  - цвет, которым будут выделяться объекты на карте,
 // point - координаты верхнего левого угла окна на карте
 // в соответсвующей параметру place системе координат.
 // При ошибке возвращает ноль

_MAPIMP long int _MAPAPI mapTotalViewSelect(HMAP hMap, HWND hwnd,
                                     DOUBLEPOINT * point,
                                     COLORREF color,
                                     long int place /*= PP_PLANE*/);


 // Подсчитать сколько объектов удовлетворяет условиям
 // обобщенного поиска
 // hMap     - идентификатор открытой карты,
 // При ошибке возвращает ноль

_MAPIMP long int _MAPAPI mapTotalSeekObjectCount(HMAP hMap);


 // Обобщенный поиск объектов по заданным условиям
 // Условия обобщенного поиска вводятся заранее
 // (используются функции mapGetSiteViewSelect(),mapGetSiteSeekSelect()...)
 // hMap - идентификатор открытой карты,
 // info - указатель на существующий объект TObjectInfo
 // flag - порядок поиска объектов (WO_FIRST, WO_NEXT...)
 // При поиске с флажками WO_NEXT,WO_BACK параметр info должен
 // указывать на результат предыдущего поиска.
 // Если объект не найден - возвращает ноль

_MAPIMP HOBJ _MAPAPI mapTotalSeekObject(HMAP hMap,HOBJ info,
                                 long int flag);


 // Установить признак выделения объектов по обобщенным
 // условиям поиска
 // hMap - идентификатор открытой карты,
 // flag = -1, поиск будет выполняться по всем картам
 // (0 - карта местности, 1...n - пользовательские карты)
 // Никакого действия кроме сохранения значения не производит
 // Применяется для связи между различными модулями

_MAPIMP void _MAPAPI mapSetTotalSelectFlag(HMAP hMap, long int flag);


 // Запросить признак выделения объектов по обобщенным
 // условиям поиска
 // hMap - идентификатор открытой карты,
 // если результат = -1, поиск будет выполняться по всем картам
 // (0 - карта местности, 1...n - пользовательские карты)

_MAPIMP long int _MAPAPI mapGetTotalSelectFlag(HMAP hMap);


 // Определить общие габариты объектов, соответствующие заданным
 // условиям
 // Габариты рассчитываются в метрах
 // hMap   - идентификатор открытой карты,
 // border - координаты прямоугольного участка под объектом.
 // При ошибке возвращает ноль

_MAPIMP long int _MAPAPI mapGetTotalSeekBorder(HMAP hMap, DFRAME * border);



 /*********************************************************
 *                                                        *
 * ФУНКЦИИ СОЗДАНИЯ/УДАЛЕНИЯ КОНТЕКСТА ПОИСКА/ОТОБРАЖЕНИЯ *
 *                                                        *
 *********************************************************/

  // Создать контекст (описание условий) поиска/отображения
  // объектов карты
  // В состав условий отбора объектов входят : лист, слой,
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

_MAPIMP  HSELECT _MAPAPI mapCreateMapSelectContext(HMAP hmap);


  // Создать копию контекста (описания условий) поиска/отображения
  // объектов карты
  // select - исходный контекст (описание условий) поиска/отображения.
  // Каждый созданный контекст должен быть удален, когда
  // он больше не используется
  // При ошибке возвращает ноль

_MAPIMP  HSELECT _MAPAPI mapCreateCopySelectContext(HSELECT select);


  // Копировать контекст (описание условий) поиска/отображения
  // в существующий контекст поиска/отображения
  // target - контекст условий поиска, куда выполняется копирование;
  // source - копируемый контекст поиска.
  // (Каждый созданный контекст должен быть удален, когда
  // он больше не используется)
  // При ошибке возвращает ноль

_MAPIMP  long int _MAPAPI mapCopySelectContext(HSELECT target, HSELECT source);


  // Установить доступ ко всем видам данных
  // контекста поиска/отображения
  // select - контекст (описание условий) поиска/отображения.
  // При ошибке возвращает ноль

_MAPIMP  long int _MAPAPI mapClearSelectContext(HSELECT select);


  // Удалить контекст (описание условий) поиска/отображения
  // объектов карты
  // select - контекст (описание условий) поиска/отображения.

_MAPIMP  void _MAPAPI mapDeleteSelectContext(HSELECT select);


  // Установить/запросить признак инвертирования условий поиска
  // select - контекст (описание условий) поиска/отображения.
  // flag   - признак инвертирования

_MAPIMP  long int _MAPAPI mapSetInversionSelect(HSELECT select, long int flag);

_MAPIMP  long int _MAPAPI mapGetInversionSelect(HSELECT select);


  // Установить доступ к объектам с заданным номером
  // слоя (сегмента)
  // select - контекст условий поиска
  // layer  - номер слоя(сегмента), начинается с 0 (!),
  // если равен -1 (0xFFFFFFFF), то устанавливается
  // доступ ко всем слоям сразу;
  // check  - доступность слоя (0 - нет доступа, != 0 - есть)
  // Термин Layer явлется синонимом слова Segment

_MAPIMP  void _MAPAPI mapSelectLayer(HSELECT select, long int layer,
                              long int check);


  // Запросить доступны ли объекты с заданным номером
  // слоя (сегмента)
  // select - контекст условий поиска
  // layer  - номер слоя(сегмента)
  // если равен -1 (0xFFFFFFFF), то проверяется
  // доступ ко всем слоям сразу (0 - что-то недоступно);
  // Термин Layer явлется синонимом слова Segment
  // Доступность слоя : 0 - нет доступа, != 0 - есть.

_MAPIMP  long int _MAPAPI mapCheckLayer(HSELECT select, long int layer);


  // Установить доступ к объектам в заданном листе
  // select - контекст условий поиска
  // list   - номер листа карты, начинается с 1,
  // если равен -1 (0xFFFFFFFF), то устанавливается
  // доступ ко всем листам сразу;
  // check  - доступность листа (0 - нет доступа, != 0 - есть)

_MAPIMP  void _MAPAPI mapSelectList(HSELECT select, long int list,
                             long int check);


  // Запросить доступны ли объекты в заданном листе
  // select - контекст условий поиска
  // list   - номер листа
  // если равен -1 (0xFFFFFFFF), то проверяется
  // доступ ко всем листам сразу (0 - что-то недоступно);
  // Доступность листа : 0 - нет доступа, != 0 - есть.

_MAPIMP  long int _MAPAPI mapCheckList(HSELECT select, long int list);


  // Установить доступ к объектам c заданным индексом
  // (внутренним кодом)
  // select - контекст условий поиска
  // object - индекс объекта карты, начинается с 1,
  // если равен -1 (0xFFFFFFFF), то устанавливается
  // доступ ко всем объектам сразу;
  // check  - доступность объекта (0 - нет доступа, != 0 - есть)

_MAPIMP  void _MAPAPI mapSelectObject(HSELECT select, long int object,
                               long int check);


  // Запросить доступны ли объекты с заданным индексом
  // (внутренним кодом)
  // select - контекст условий поиска
  // object - индекс объекта
  // если равен -1 (0xFFFFFFFF), то проверяется
  // доступ ко всем объектам сразу (0 - что-то недоступно);
  // Доступность объекта : 0 - нет доступа, != 0 - есть.

_MAPIMP  long int _MAPAPI mapCheckObject(HSELECT select, long int object);


  // Установить доступ к объектам c заданной локализацией
  // (0 - линейный,1 - площадной,2-точечный,3-подпись,
  //  4 - векторный (линия с 2-мя точками),
  //  5 - шаблон (сложная подпись) )
  // select - контекст условий поиска
  // local  - код локализации, начинается с 0,
  // если равен -1 (0xFFFFFFFF), то устанавливается
  // доступ ко всем локализациям сразу;
  // check  - доступность локализации (0 - нет доступа, != 0 - есть)

_MAPIMP  void _MAPAPI mapSelectLocal(HSELECT select, long int local,
                              long int check);


  // Запросить доступны ли объекты с заданной локализацией
  // select - контекст условий поиска
  // local  - код локализации, начинается с 0 (линейный,...),
  // если равен -1 (0xFFFFFFFF), то проверяется
  // доступ ко всем локализациям сразу (0 - что-то недоступно);
  // Доступность локализации : 0 - нет доступа, != 0 - есть.

_MAPIMP  long int _MAPAPI mapCheckLocal(HSELECT select, long int local);


  // Установить доступ к объектам с заданными номерами
  // select  - контекст условий поиска
  // min,max - диапазон номеров поиска, начинается с 0,
  // если оба числа равны -1 (0xFFFFFFFF), то устанавливается
  // доступ ко всем номерам сразу;

_MAPIMP  void _MAPAPI mapSelectKey(HSELECT select,
                            unsigned long int min,
                            unsigned long int max);


  // Запросить доступны ли объекты с заданным номером
  // select - контекст условий поиска
  // key    - номер объекта,
  // если равен -1 (0xFFFFFFFF), то проверяется
  // доступ ко всем номерам сразу (0 - что-то недоступно);
  // Доступность номера : 0 - нет доступа, != 0 - есть.

_MAPIMP  long int _MAPAPI mapCheckKey(HSELECT select,
                               unsigned long int key);


  // Запросить минимальный номер в диапазоне поиска
  // select - контекст условий поиска
  // Может равняться 0, когда доступны все номера объектов

_MAPIMP  unsigned long int _MAPAPI mapGetMinKey(HSELECT select);


  // Запросить максимальный номер в диапазоне поиска
  // select - контекст условий поиска

_MAPIMP  unsigned long int _MAPAPI mapGetMaxKey(HSELECT select);


  // Добавить условие в список
  // select - контекст условий поиска
  // code   - коды условий (1-LESS,3-MORE, ... - см. MAPTYPE.H)
  // value  - значение для условия
  // При ошибке возвращает ноль, иначе - номер условия

_MAPIMP  long int _MAPAPI mapSelectSemanticAppend(HSELECT select,long int code,
                                           long int semcode,
                                           const char * value);
_MAPIMP  long int _MAPAPI mapSelectSemanticAppendUn(HSELECT select,long int code,
                                           long int semcode,
                                           const WCHAR * value);

  // Удалить все условия из списка семантик
  // select - контекст условий поиска

_MAPIMP  void _MAPAPI mapSelectSemanticClear(HSELECT select);


  // Запросить количество установленных условий по семантике
  // select - контекст условий поиска

_MAPIMP  long int _MAPAPI mapSelectSemanticCount(HSELECT select);


  // Запросить код условия для семантики по последовательному номеру
  // select - контекст условий поиска
  // number - последовательный номер семантики
  // При ошибке возвращает ноль

_MAPIMP  long int _MAPAPI mapSelectSemanticCondition(HSELECT select,
                                              long int number);

  // Запросить код семантики по последовательному номеру
  // Например : 4 - абс.высота, 9 - название, ...
  // select - контекст условий поиска
  // number - последовательный номер семантики
  // При ошибке возвращает ноль

_MAPIMP  long int _MAPAPI mapSelectSemanticCode(HSELECT select,
                                         long int number);


  // Удалить условие из списка
  // select - контекст условий поиска
  // number - номер условия в списке (от 1 до mapSelectSemanticCount())
  // При ошибке возвращает ноль

_MAPIMP  long int _MAPAPI mapSelectSemanticDelete(HSELECT select,
                                           long int number);


  // Установить/Запросить обобщающее условие для набора семантик
  // select - контекст условий поиска
  // code - код условия :
  // 16 - CMOR : выполняется хотя бы одно,
  // 32 - CMAND : выполняются все, см. MAPTYPE.H

_MAPIMP  void _MAPAPI mapSelectSemanticLink(HSELECT select,
                                     long int code);

_MAPIMP  long int _MAPAPI mapGetSelectSemanticLink(HSELECT select);


  // Запросить значение семантики по последовательному номеру
  // select - контекст условий поиска
  // number - последовательный номер семантики
  // place  - адрес строки, в которой разместиться результат
  // size   - размер выходной строки
  // При ошибке возвращает ноль

_MAPIMP  long int _MAPAPI mapSelectSemanticValue(HSELECT select,long int number,
                                          char * place,long int size);


  // Запросить количество установленных условий по измерениям объектов
  // select - контекст условий поиска
  // (длина, периметр, площадь, высота)

_MAPIMP  long int _MAPAPI mapSelectMeasureCount(HSELECT select);


  // Удалить все данные из списка измерений
  // select - контекст условий поиска

_MAPIMP  void _MAPAPI mapSelectMeasureClear(HSELECT select);


  // Добавить измерение в список
  // select - контекст условий поиска
  // measurecode - код измерения объекта : длина, периметр, площадь, высота (см. MAPTYPE.H)
  // condition - коды условий (1-CMLESS,3-CMMORE, ... - см. MAPTYPE.H)
  // value - значение измерения в метрах, для площади - в кв. метрах.
  // Если заданы только condition1 и value1, а condition2 = 0,
  // добавляется единственное значение измерения.
  // Для задания диапазона значений, дополнительно указываются condition2 и value2.
  // В этом случае condition1 должно равняться CMMOREEQ (>=),
  // condition2 - CMLESSEQ (<=) !
  // При ошибке возвращает ноль, иначе - номер условия в списке

_MAPIMP  long int _MAPAPI mapSelectMeasureAppend(HSELECT select, long int measurecode,
                                          long int condition1,   // первое условие отбора(>,<,>=,...)
                                          double   value1,       // первое значение(в дискретах, для высоты в метрах)
                                          long int condition2,   // второе условие отбора(>,<,>=,...)
                                          double   value2) ;     // второе значение(в дискретах, для высоты в метрах)


  // Запросить установлен ли для измерения диапазон значений
  // по последовательному номеру в списке.
  // select - контекст условий поиска
  // number - номер измерения
  // Возвращает 1 - установлено единственное значение;
  //            2 - установлен диапазон значений;
  //            0 - при ошибке.

_MAPIMP  long int _MAPAPI mapIsSelectMeasureRange(HSELECT select, long int number);


  // Установить/Запросить обобщающее условие для набора измерений
  // select   - контекст условий поиска
  // linkcode - код условия :
  // 16 - CMOR : выполняется хотя бы одно,
  // 32 - CMAND : выполняются все (см. MAPTYPE.H)

_MAPIMP  void _MAPAPI mapSelectMeasureLink(HSELECT select, long int linkcode);

_MAPIMP  long int _MAPAPI mapGetSelectMeasureLink(HSELECT select);


  // Запросить код измерения по последовательному номеру
  // select - контекст условий поиска
  // number - номер измерения
  // Например : площадь, длина и т.д. (см. MAPTYPE.H)
  // При ошибке возвращает ноль

_MAPIMP  long int _MAPAPI mapSelectMeasureCode(HSELECT select, long int number);


  // Запросить значение измерения по последовательному номеру в списке
  // select - контекст условий поиска
  // number - номер измерения
  // pvalue1,pvalue2 - адреса для размещения результата.
  // pvalue2 заполняется, если установлен диапазон значений
  // Возвращает 1 - передается одно значение;
  //            2 - передаются два значения;
  //            0 - при ошибке.

_MAPIMP  long int _MAPAPI mapSelectMeasureValue(HSELECT select, long int number,
                                         double *pvalue1,double *pvalue2);


  // Запросить код условия для измерения по последовательному номеру
  // select - контекст условий поиска
  // number - номер измерения
  // pcondition1,pcondition2 - адреса для размещения результата.
  // Возвращает 1 - передается одно значение;
  //            2 - передаются два значения;
  //            0 - при ошибке.

_MAPIMP  long int _MAPAPI mapSelectMeasureCondition(HSELECT select, long int number,
                                             long int *pcondition1,
                                             long int *pcondition2);


  // Запросить размер записи, необходимый для сохранения условий поиска
  // select - контекст условий поиска
  // При ошибке возвращает ноль

_MAPIMP  long int _MAPAPI mapGetSelectRecordSize(HSELECT select);


  // Сформировать запись для сохранения условий поиска
  // select - контекст условий поиска
  // buffer - адрес записи
  // length - длина буфера,для размещения записи
  // При ошибке возвращает ноль

_MAPIMP  long int _MAPAPI mapGetSelectRecord(HSELECT select,char * buffer,
                                      long int length);


  // Заполнить условия поиска из записи
  // select - контекст условий поиска
  // buffer - адрес записи
  // length - длина записи или буфера,содержащего запись (не меньше записи)
  // При ошибке возвращает ноль

_MAPIMP  long int _MAPAPI mapPutSelectRecord(HSELECT select,char * buffer,
                                      long int length);


  // Опросить наличие списка объектов в контексте условий поиска/отображения
  // Список объектов содержит номер листа и номер объекта
  // в листе
  // select - контекст условий поиска
  // Если список объектов не установлен,возвращает ноль

_MAPIMP  long int _MAPAPI mapIsSample(HSELECT select);


  // Запросить число объектов в списке для указанного листа карты
  // select - контекст условий поиска
  // list   - номер листа
  // Если список объектов не установлен,возвращает ноль

_MAPIMP  long int _MAPAPI mapGetSampleCount(HSELECT select, long int list);


  // Запросить уникальный номер объекта из списке по порядковому номеру и
  // указанному листу карты
  // select - контекст условий поиска
  // list   - номер листа в карте
  // number - порядковый номер объекта в списке выбранных объектов листа
  // Если список объектов не установлен,возвращает ноль

_MAPIMP  long int _MAPAPI mapGetSampleByNumber(HSELECT select, long int list,
                                        long int number);


  // Очистить список объектов в контексте условий поиска/отображения
  // Список объектов содержит номер листа и номер объекта
  // в листе
  // select - контекст условий поиска
  // Применяется для отбора объектов,атрибуты которых расположены
  // во внешних базах данных

_MAPIMP  void _MAPAPI mapClearSample(HSELECT select);


  // Установить признак совместной обработки номеров объектов с
  // условиями по локализации, слоям, семантике и пр.
  // select - контекст условий поиска
  // complex - признак совместной обработки (0/1)
  // Возвращает предыдущее значение

_MAPIMP  long int _MAPAPI mapSetSampleComplex(HSELECT select, long int complex);


  // Инвертировать список отобранных объектов в контексте условий поиска // 02/09/04
  // select - контекст условий поиска
  // При ошибке возвращает ноль

_MAPIMP  long int _MAPAPI mapInvertSample(HSELECT select);


  // Заполнить список объектов всеми номерами объектов, которые есть
  // на листе
  // select - контекст условий поиска
  // list   - номер листа, с которого берутся объекты
  // Связь с картой (hMap, hSite) устанавливается при создании контекста
  // или при чтении контекста с карты
  // Возвращает число объектов, занесенных в список для листа
  // При ошибке возвращает ноль

_MAPIMP  long int _MAPAPI mapSetSampleAllObjects(HSELECT select, long int list);


  // Установить условия отображения объекта по имени листа карты
  // и номеру объекта в карте.
  // hMap     - идентификатор открытой карты,
  // listname - имя листа карты,
  // key - уникальный номер объекта в карте.
  // При ошибке возвращает ноль.

_MAPIMP  long int _MAPAPI mapSelectViewSample(HMAP hMap, const char * listname,
               
                        long int key);

  // Установить условия поиска объекта по имени листа карты
  // и номеру объекта в карте.
  // hMap     - идентификатор открытой карты,
  // listname - имя листа карты,
  // key - уникальный номер объекта в карте.
  // При ошибке возвращает ноль.

_MAPIMP  long int _MAPAPI mapSelectSeekSample(HMAP hMap,const char * listname,
                                       long int key);


  // Установить доступ к заданному объекту заданного листа карты.
  // select   - контекст условий поиска
  // listname - имя листа карты,
  // key - уникальный номер объекта в карте.
  // При ошибке возвращает ноль.

_MAPIMP  long int _MAPAPI mapSelectSample(HSELECT select, const char * listname,
                                          long int key);


  // Установить доступ к заданному объекту заданного листа карты.
  // select - контекст условий поиска
  // list   - номер листа карты,
  // key    - уникальный номер объекта в карте.
  // При ошибке возвращает ноль.

_MAPIMP  long int _MAPAPI mapSelectSampleByList(HSELECT select, long int list,
                                         long int key);


  // Установить доступ к заданному объекту заданного листа карты.
  // select - контекст условий поиска
  // list   - номер листа карты,
  // number - номер объекта в листе.
  // В контексте условий поиска должна быть установлена карта.
  // При ошибке возвращает ноль.

_MAPIMP  long int _MAPAPI mapSelectSampleByNumber(HSELECT select, long int list,
                                            long int number);


  // Отключить доступ к заданному объекту заданного листа карты
  // select   - контекст условий поиска
  // listname - имя листа карты
  // key      - уникальный номер объекта в карте
  // В контексте условий поиска должна быть установлена карта.
  // При ошибке возвращает ноль.

_MAPIMP  long int _MAPAPI mapUnselectSample(HSELECT select, const char * listname,
                                     long int key);


  // Отключить доступ к заданному объекту заданного листа карты.
  // select - контекст условий поиска
  // list   - номер листа карты,
  // key    - уникальный номер объекта в карте
  // В контексте условий поиска должна быть установлена карта.
  // При ошибке возвращает ноль.

_MAPIMP  long int _MAPAPI mapUnselectSampleByList(HSELECT select, long int list,
                                           long int key);


  // Отключить доступ к заданному объекту заданного листа карты.
  // select - контекст условий поиска
  // list   - номер листа карты,
  // number - номер объекта в листе.
  // В контексте условий поиска должна быть установлена карта.
  // При ошибке возвращает ноль.

_MAPIMP  long int _MAPAPI mapUnselectSampleByNumber(HSELECT select, long int list,
                                             long int number);


  // Установить параметры поиска объектов по области.
  // hMap     - идентификатор открытой карты,
  // object   - объект-область поиска
  // distance - расстояние поиска в метрах
  // флажки,описывающие критерии поиска :
  // filter  - учитывать/не учитывать(1/0) фильтр объектов
  //           (параметры фильтра должны быть установлены заранее
  //           в контексте поиска)
  // inside  - границы поиска объектов по области :
  //           1 - внутри области, 2 - целиком внутри области,
  //           0 - по расстоянию.
  // visible - с учетом/без учета(1/0) видимости объектов на карте
  // listname - имя карты поиска. listname = 0,если устанавливается
  //            поиск по всем картам.
  // action   - порядок поиска объектов :
  //            0 - последовательный поиск по мере запроса объектов,
  //            1 - предварительный отбор всех объектов (главному
  //            окну приложения посылается WM_PROGRESSBAR - maptype.h),
  //            ускоряет многократный запрос отобранных объектов;
#ifdef __cplusplus
_MAPIMP  long int _MAPAPI mapSelectArea(HMAP hMap, HOBJ object,
                                 double distance = 0.0,
                                 long int filter = 0,
                                 long int inside = 1,
                                 long int visible = 0,
                                 const char * listname = 0,
                                 long int action = 0);
#else
_MAPIMP  long int _MAPAPI mapSelectArea(HMAP hMap, HOBJ object,
                                 double distance,
                                 long int filter,
                                 long int inside,
                                 long int visible,
                                 const char * listname,
                                 long int action);
#endif


  // Установить параметры поиска объектов по области.
  // hMap     - идентификатор открытой карты,
  // object   - объект-область поиска
  // distance - расстояние поиска в метрах
  // nmap     - номер карты поиска; устанавливает границы поиска
  //            по картам.Если nmap=-1,поиск по всем картам
  // флажки,описывающие критерии поиска :
  // filter  - учитывать/не учитывать(1/0) фильтр объектов
  //           (параметры фильтра должны быть установлены заранее
  //           в контексте поиска)
  // inside  - границы поиска объектов по области :
  //           1 - внутри области, 2 - целиком внутри области,
  //           0 - по расстоянию.
  // visible - с учетом/без учета(1/0) видимости объектов на карте
  // action   - порядок поиска объектов :
  //            0 - последовательный поиск по мере запроса объектов,
  //            1 - предварительный отбор всех объектов (главному
  //            окну приложения посылается WM_PROGRESSBAR - maptype.h),
  //            ускоряет многократный запрос отобранных объектов;

_MAPIMP  int _MAPAPI  mapSelectAreaEx(HMAP hMap, HOBJ object,
                             double distance, long int filter,
                             long int inside, long int visible,
                             long int action, long int nmap);


  // Сбросить параметры поиска объектов по области
  // hMap     - идентификатор открытой карты

_MAPIMP  void _MAPAPI mapUnselectArea(HMAP hMap);


  // Установить в контексте параметры поиска объектов по области.
  // hselect - контекст поиска об'ектов
  // object - объект-область поиска
  // distance - расстояние поиска в метрах
  // флажки,описывающие критерии поиска :
  // filter  - учитывать/не учитывать(1/0) фильтр объектов
  //           (параметры фильтра должны быть установлены заранее
  //           в контексте поиска)
  // inside  - границы поиска объектов по области :
  //           1 - внутри области, 2 - целиком внутри области,
  //           0 - по расстоянию.
  // visible - с учетом/без учета(1/0) видимости объектов на карте
  // action   - порядок поиска объектов :
  //            0 - последовательный поиск по мере запроса объектов,
  //            1 - предварительный отбор всех объектов (главному
  //            окну приложения посылается WM_PROGRESSBAR - maptype.h),
  //            ускоряет многократный запрос отобранных объектов;
#ifdef __cplusplus
_MAPIMP  long int _MAPAPI mapSelectSeekArea(HSELECT hselect, HOBJ object,
                                     double distance = 0.0,
                                     long int filter = 0,
                                     long int inside = 1,
                                     long int visible = 0,
                                     long int action = 0);
#else
_MAPIMP  long int _MAPAPI mapSelectSeekArea(HSELECT hselect, HOBJ object,
                                     double distance,
                                     long int filter,
                                     long int inside,
                                     long int visible,
                                     long int action);
#endif


  // Сбросить в контексте параметры поиска объектов по области
  // hselect - контекст поиска объектов

_MAPIMP  void _MAPAPI mapUnselectSeekArea(HSELECT hselect);


  // Зафиксировать в контексте поиска количественный состав карты
  // При ошибке возвращает ноль
  // hMap     - идентификатор открытой карты,
  // (Используется при редактировании карты для исключения из поиска вновь
  // созданных объектов)                                      // 24/06/02

_MAPIMP  long int _MAPAPI mapFreezeMapContents(HMAP hMap);


  // Сбросить в контексте поиска данные о количественном составе карты
  // hMap     - идентификатор открытой карты,
  // При ошибке возвращает ноль                               // 24/06/02

_MAPIMP  long int _MAPAPI mapDefreezeMapContents(HMAP hMap);



 /*********************************************************
 *                                                        *
 *      ФУНКЦИИ ПОИСКА ПО МЕТРИКЕ ОБЪЕКТОВ                *
 *                                                        *
 *********************************************************/

  // Найти точку метрики подобъекта, ближайшую к заданной
  // info    - идентификатор объекта в памяти
  // point   - координаты точки в прямоугольной
  //           системе координат , в метрах на местности
  // subject - последовательный номер подобъекта
  //           (0 - объект, 1 - первый подобъект и т.д.,
  //           если равен -1  - поиск по всей метрике)
  // Для определения номера подобъекта при поиске по всей метрике
  // применяется mapGetCurrentSubject()
  // Возвращает номер точки (номер первой точки равен 1)
  // При ошибке возвращает 0

_MAPIMP  long int _MAPAPI mapSeekNearPoint(HOBJ info,
                                    DOUBLEPOINT * point,
                                    long int subject);


  // Найти точку на контурах объекта и подобъектов,
  // ближайшую к заданной
  // hMap    - идентификатор открытой карты,
  // info    - идентификатор объекта в памяти
  // pointin - координаты точки в прямоугольной
  //           системе координат , в метрах на местности
  // Возвращает номер точки метрики за которой расположена
  // точка на контуре или ноль при ошибке
  // Координаты точки (в метрах) помещаются по адресу pointout

_MAPIMP  long int _MAPAPI mapSeekNearVirtualPoint(HMAP hMap, HOBJ info,
                                         DOUBLEPOINT * pointin,
                                         DOUBLEPOINT * pointout);


  // Найти точку на контуре подобъекта,
  // ближайшую к заданной
  // координаты точки pointin заданы в прямоугольной
  // системе координат , в метрах на местности
  // hMap    - идентификатор открытой карты,
  // info    - идентификатор объекта в памяти
  // subject - последовательный номер подобъекта
  // ( 0 - объект, 1 - первый подобъект и т.д.)
  // Возвращает номер точки метрики за которой расположена
  // точка на контуре или ноль при ошибке
  // Координаты точки (в метрах) помещаются по адресу pointout

_MAPIMP  long int _MAPAPI mapSeekNearVirtualPointSubject(HMAP hMap,
                                  HOBJ obj,long int subject,
                                  DOUBLEPOINT * pointin,
                                  DOUBLEPOINT * pointout);


  // Запросить номер текущего подобъекта (после поиска)
  // hObj - идентификатор объекта в памяти
  // При ошибке возвращает ноль, если текущим подобъектом
  // является сам объект - тоже возвращает ноль

_MAPIMP  long int _MAPAPI mapGetCurrentSubject(HOBJ hobj);


  // Определение координат точки, лежащей на заданном
  // расстоянии (по периметру) от заданной точки
  // info   - идентификатор объекта в памяти
  // number - номер начальной точки
  // distance - расстояние
  // если distance > = 0 - поиск по направлению цифрования
  //               <   0 - поиск против направления цифрования
  // point - координаты выходной точки
  //         (в прямоугольной системе в метрах на местности)
  // subject - номер подобъекта
  // Возвращает номер точки, за которой находится или
  // c которой совпадает
  // Если точки совпадают - возвращает отрицательный номер точки
  // При ошибке возвращает 0

_MAPIMP  long int _MAPAPI mapSeekVirtualPointByDistance(HOBJ info,
                                    long int number, double distance,
                                    DOUBLEPOINT *point, long int subject);



 /*******************************************************************
 *     ФУНКЦИИ ПЕРЕСЕЧЕНИЯ ОБ'ЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ               *
 *                                                                  *
 * Пересечение двух объектов, т.е. нахождение общей части объектов, *
 * один из которых (ПЕРВЫЙ объект карты) - РЕЗАК(по которому режут) *
 * другой (ВТОРОЙ объект карты) - ОБЪЕКТ, КОТОРЫЙ РЕЖУТ.            *
 * Только для ПЛОЩАДНЫХ или ЛИНЕЙНЫХ объектов !!!                   *
 ********************************************************************/

 // *********************************************************
 //
 // Схема запуска:
 // =============
 // HCROSS hCross = mapCreateObjectsCross(info1,info2,method,precision)
 // if (hCross)
 //    {
 //    while(mapGetNextCross(hCross,info))
 //         {
 //         ...
 //         }
 //    mapFreeObjectsCross(hCross);
 //    }
 //**********************************************************

 // Пересечение двух объектов, т.е. нахождение общей части объектов,
 // один из которых (ПЕРВЫЙ объект карты) - РЕЗАК(по которому режут)
 // другой (ВТОРОЙ объект карты) - ОБЪЕКТ, КОТОРЫЙ РЕЖУТ.
 //
 // Создание класса пересечения
 // Только для ПЛОЩАДНЫХ или ЛИНЕЙНЫХ объектов !!!
 // info1 - первый объект карты - РЕЗАК (произвольный замкнутый контур, подобъекты не учитываются)
 // info2 - второй объект карты (площадной объект с подобъектами)
 // или
 // info1 - первый объект карты - РЕЗАК (произвольный замкнутый контур с подобъектами)
 // info2 - второй объект карты (линейный объект с подобъектами)      // 09/08/04
 // Пересечение произвольных площадных объектов с подобъектами - mapCreateObjectsCrossSquare(...)
 // method - тип результирующих объектов
 //          LOCAL_SQUARE - площадной, LOCAL_LINE - линейный
 //          тип результирующих объектов зависит от типа
 //          второго объекта:
 //          - если второй объект незамкнутый, то тип только LOCAL_LINE,
 //          - если второй объект замкнутый, то тип может быть LOCAL_LINE или LOCAL_SQUARE.
 // При логическом сложении method и флага проверки входимости (FLAGINSIDEOBJECTS = 32
 // см. maptype.h) в класс пересечения будут добавлены объекты, которые полностью входят
 // в объект - РЕЗАК  (напр. LOCAL_SQUARE | FLAGINSIDEOBJECTS)     //24/08/04
 // precision - точность при дотягивании (в метрах), при precision=0 устанавливается точность    //24/05/07
 //             0.001 для карт масштаба <= 500000,
 //             0.01 для карт масштаба более 500000, если precision больше предложенной, то
 //             устанавливается большее значение
 // Возвращает указатель на класс пересечения
 // При отсутствии пересечения или при ошибке возвращает 0


_MAPIMP HCROSS _MAPAPI mapCreateObjectsCross(HOBJ info1, HOBJ info2,
                                      long int method,
                                      double precision);

 // Пересечение двух объектов
 // Освобождение класса пересечения

_MAPIMP void _MAPAPI mapFreeObjectsCross(HCROSS hCross);

 // Пересечение двух объектов
 // Запросить объект
 // info - результат
 // При ошибке возвращает 0

_MAPIMP HOBJ _MAPAPI mapGetNextCross(HCROSS hCross, HOBJ info);

 // Запрос на пересечение двух объектов
 // Только для ПЛОЩАДНЫХ или ЛИНЕЙНЫХ объектов !!!
 // info1 - первый объект карты
 // info2 - второй объект карты
 // Если объекты пересекаются, возвращает 1
 // При ошибке или неверном типе объектов возвращает 0

_MAPIMP long int _MAPAPI mapGetObjectsCross(HOBJ info1, HOBJ info2);


 // Объединение(сшивка) двух объектов
 // Только для ПЛОЩАДНЫХ или ЛИНЕЙНЫХ объектов !!!
 // Не допускается сшивать замкнутый и незамкнутый объекты !!!
 // info1 - первый объект карты
 // info2 - второй объект карты
 // info -  результат
 // method - тип результирующего объекта
 //          LOCAL_SQUARE - площадной
 //          (на входе только два площадных или линейных замкнутых объекта),
 //          LOCAL_LINE - линейный
 //          (на входе два площадных или линейных замкнутых объекта,
 //           или два незамкнутых объекта)
 // precision - точность при дотягивании (в метрах), при precision=0 устанавливается точность
 //             0.001 для карт масштаба <= 500000,
 //             0.01 для карт масштаба более 500000, если precision больше предложенной, то
 //             устанавливается большее значение
 // Возвращает указатель на результирующий объект (info)
 // При ошибке возвращает 0

_MAPIMP HOBJ _MAPAPI mapGetObjectsUnion(HOBJ info1, HOBJ info2,  HOBJ info,
                                 long int method, double precision);

// Объединение (сшивка) двух площадных объектов (типа "Кварталы")
// Только для ПЛОЩАДНЫХ (типа "Кварталы") !!!
// info1     - первый объект карты
// info2     - второй объект карты
// info      - результат
// precision - допуск при дотягивании (в метрах);
//             при превышении допуска сшивка выполняется через три
//             ближайших точки (точка одного объекта, отрезок - другого)
// always    - флаг принудительной сшивки
//             1 - сшивает всегда
//             0 - сшивает если ближайшая связь меньше precision и     // 10/11/06
//                 пара связей не образует треугольник
// Возвращает указатель на результирующий объект (info)
// При ошибке возвращает 0

_MAPIMP HOBJ _MAPAPI mapSquareObjectsUnion(HOBJ info1, HOBJ info2, HOBJ info,
                                   double precision, long int always);  // 03/11/06


 /*************************************************************
 *     ФУНКЦИИ СОГЛАСОВАНИЯ ОБ'ЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ        *
 *                                                            *
 * Согласование двух объектов, т.е. нахождение внешней части  *
 * исходного объекта, которая примыкает к другому объекту и   *
 * имеет с ним общую часть контура.                           *
 * Только для ПЛОЩАДНЫХ или ЛИНЕЙНЫХ объектов !!!             * 
 **************************************************************/

 // *********************************************************
 // Схема запуска:
 // =============
 // HCROSS hCross = mapCreateObjectsConsent(info1,info2,method,precision)
 // if (hCross)
 //    {
 //    while(mapGetNextConsent(hCross,info))
 //         {
 //         ...
 //         }
 //    mapFreeObjectsConsent(hCross);
 //    }
 // **********************************************************

 // Согласование двух объектов, т.е. нахождение внешней части
 // исходного объекта, которая примыкает к другому объекту и
 // имеет с ним общую часть контура.

 // Создание класса согласования
 // Только для ПЛОЩАДНЫХ или ЛИНЕЙНЫХ объектов !!!
 // info1 - первый объект карты, по которому согласовывают
 //         внешний контур второго объекта
 //         (это произвольный замкнутый контур без подобъектов)
 // info2 - второй объект карты, у которого нужно найти внешнюю
 //         часть, примыкающую к первому объекту и имеющему с ним
 //         общую часть контура (это произвольный линейный
 //         или площадной объект с подобъектами)
 // method - тип результирующих объектов
 //          LOCAL_SQUARE - площадной, LOCAL_LINE - линейный
 //          тип результирующих объектов зависит от типа
 //          второго объекта:
 //          - если второй объект незамкнутый, то тип только LOCAL_LINE,
 //          - если второй объект замкнутый, то тип может быть LOCAL_LINE или LOCAL_SQUARE.
 // precision - точность при дотягивании (в метрах), при precision=0 устанавливается точность
 //             0.001 для карт масштаба <= 500000,
 //             0.01 для карт масштаба более 500000, если precision больше предложенной, то
 //             устанавливается большее значение
 // Возвращает указатель на класс согласования
 // При ошибке возвращает 0


_MAPIMP HCROSS _MAPAPI mapCreateObjectsConsent(HOBJ info1, HOBJ info2,
                                        long int method,
                                        double precision);

 // Согласование двух объектов
 // Освобождение класса согласования
 // hCross - указатель на класс согласования

_MAPIMP void _MAPAPI mapFreeObjectsConsent(HCROSS hCross);


 // Согласование двух объектов
 // Запросить объект
 // hCross - указатель на класс согласования
 // info   - результат
 // При ошибке возвращает 0

_MAPIMP HOBJ _MAPAPI mapGetNextConsent(HCROSS hCross, HOBJ info);




 /****************************************************************
 *  ФУНКЦИИ ЗАПРОСА ТОЧЕК ПЕРЕСЕЧЕНИЯ ОБ'ЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ *
 *                  ОБРАБАТЫВАЮТСЯ ВСЕ ПОДОБ'ЕКТЫ !!!            *
 *****************************************************************/

 // **********************************************************
 // Схема запуска:
 // =============
 // HCROSS hCross = mapCreateObjectCrossPoints(info1,info2)
 // CROSSPOINT point;
 // if (hCross)
 //    {
 //    int count = mapGetCrossCount(hCross);
 //    for(int i = 1; i <= count; i++)
 //         {
 //         mapGetCrossPoint(hCross,i,(HPOINT)&point);
 //         ...
 //         }
 //    mapFreeCrossPoints(hCross);
 //    }
 // **********************************************************

 // Нахождение точек пересечения двух объектов
 // Создание класса точек пересечения
 // info1 - первый объект карты ( линейный,площадной )
 // info2 - второй объект карты ( линейный,площадной )
 // precision - точность при дотягивании (в метрах), при precision=0 устанавливается точность
 //             0.001 для карт масштаба <= 500000,
 //             0.01 для карт масштаба более 500000, если precision больше предложенной, то
 //             устанавливается большее значение
 // Возвращает указатель на класс пересечения
 // При ошибке возвращает 0

_MAPIMP HCROSS _MAPAPI mapCreateObjectCrossPointsEx(HOBJ info1, HOBJ info2,
                                             double precision);


 // Нахождение точек пересечения двух объектов
 // Только для ПЛОЩАДНЫХ или ЛИНЕЙНЫХ объектов !!!
 // Создание класса точек пересечения
 // info1 - первый объект карты
 // info2 - второй объект карты 
 // Возвращает указатель на класс пересечения
 // При ошибке возвращает 0

_MAPIMP HCROSS _MAPAPI mapCreateObjectCrossPoints(HOBJ info1, HOBJ info2);


 // Нахождение точек пересечения двух объектов
 // Запросить количество точек пересечения
 // hCross - указатель на класс согласования
 // При ошибке возвращает 0

_MAPIMP int _MAPAPI mapGetCrossCount(HCROSS hCross);


 // Нахождение точек пересечения двух объектов
 // Запросить точку
 // hCross - указатель на класс согласования
 // number - номер точки (с 1)
 // point  - точка (КООРДИНАТЫ В МЕТРАХ НА МЕСТНОСТИ)
 // При ошибке возвращает 0

_MAPIMP int _MAPAPI
     mapGetCrossPoint(HCROSS hCross,long int number,HPOINT point);


 // Нахождение точек пересечения двух объектов
 // Освобождение класса
 // hCross - указатель на класс согласования

_MAPIMP void _MAPAPI mapFreeCrossPoints(HCROSS hCross);




 /**********************************************************
 *  ФУНКЦИИ ЗАПРОСА ТОЧЕК ПЕРЕСЕЧЕНИЯ ОБ'ЕКТОВ/ПОДОБ'ЕКТОВ *
 *                    ЭЛЕКТРОННОЙ КАРТЫ                    *
 *                  ПОДОБ'ЕКТЫ НЕ ОБРАБАТЫВАЮТСЯ !!!       *
 **********************************************************/

 // *********************************************************
 // Схема запуска:
 // =============
 // HCROSS hCross = mapCreateSubjectCrossPoints(info1,info2,
 //                                             subject1, subject2)
 // CROSSPOINT point;
 // if (hCross)
 //    {
 //    int count = mapGetCrossCount(hCross);
 //    for(int i = 1; i <= count; i++)
 //         {
 //         mapGetCrossPoint(hCross,i,(HPOINT)&point);
 //         ...
 //         }
 //    mapFreeCrossPoints(hCross);
 //    }
 //**********************************************************

 // Нахождение точек пересечения двух объектов/подобъектов
 // Создание класса точек пересечения
 // info1 - первый объект карты ( произвольный )
 // info2 - второй объект карты ( произвольный )
 // subject1 - номер подобъекта info1
 // subject2 - номер подобъекта info2
 // precision - точность при дотягивании (в метрах), при precision=0 устанавливается точность
 //             0.001 для карт масштаба <= 500000,
 //             0.01 для карт масштаба более 500000, если precision больше предложенной, то
 //             устанавливается большее значение
 // Возвращает указатель на класс пересечения
 // При ошибке возвращает 0

_MAPIMP HCROSS _MAPAPI
        mapCreateSubjectCrossPointsEx(HOBJ info1, HOBJ info2,
                                      long int subject1,long int subject2,
                                      double precision);
                                      
 // Нахождение точек пересечения двух объектов/подобъектов
 // Создание класса точек пересечения
 // info1 - первый объект карты ( произвольный )
 // info2 - второй объект карты ( произвольный )
 // subject1 - номер подобъекта info1
 // subject2 - номер подобъекта info2
 // Возвращает указатель на класс пересечения
 // При ошибке возвращает 0

_MAPIMP HCROSS _MAPAPI
        mapCreateSubjectCrossPoints(HOBJ info1, HOBJ info2,
                                    long int subject1, long int subject2);


 //  Добавить точки пересечения объектов в метрику
 //  При ошибке или отсутствии пересечения возвращает 0
 //  info1 - первый объект карты
 //  info2 - второй объект карты
 //  30/03/00

_MAPIMP int _MAPAPI mapInsertPointCross(HOBJ info1, HOBJ info2);


 //  Определить взаиморасположение двух объектов
 //  (только для замкнутых объектов)
 //  info1 - первый объект карты
 //  info2 - второй объект карты
 //  Возвращает: 1 - первый объект внутри второго,
 //              2 - второй объект внутри первого,
 //              3 - объекты пересекаются,
 //              4 - объекты не пересекаются
 //  При ошибке возвращает 0
 //  20/04/00

_MAPIMP int _MAPAPI mapCheckInsideObject(HOBJ info1, HOBJ info2);


 //  Определить взаиморасположение двух объектов (включая подобъекты)
 //  (только для замкнутых объектов)
 //  info1 - первый объект карты
 //  info2 - второй объект карты
 //  Возвращает: 1 - первый объект внутри второго
 //                 (второй объект д.б. обязательно замкнутым),
 //            - 1 - первый объект внутри подобъекта второго объекта
 //                 (второй объект д.б. обязательно замкнутым),
 //              2 - второй объект внутри первого
 //                 (первый объект д.б. обязательно замкнутым),
 //            - 2 - второй объект внутри подобъекта первого объекта
 //                 (первый объект д.б. обязательно замкнутым),
 //              3 - объекты пересекаются,
 //              4 - объекты не пересекаются
 //  При ошибке возвращает 0
 //  09/06/01

_MAPIMP int _MAPAPI mapCheckInsideObjectEx(HOBJ info1, HOBJ info2);


 //  Определить взаиморасположение объекта и точки
 //  (только для замкнутых объектов)
 //  info    - идентификатор объекта,
 //  subject - номер объекта(0) или подобъекта,
 //  point   - координаты проверяемой точки в метрах.
 //  Возвращает: 1 - точка внутри объекта(подобъекта),
 //              2 - точка за пределами объекта(подобъекта),
 //              3 - точка совпадает с точкой метрики,
 //              4 - точка лежит на отрезке.
 //  При ошибке возвращает 0

_MAPIMP int _MAPAPI mapCheckInsidePoint(HOBJ info, int subject,
                                 DOUBLEPOINT * point);


 //  Определить взаиморасположение области и объекта
 //  info - идентификатор объекта,
 //  area - область (обязательно замкнута и без подобъектов,
 //                  т.к. подобъекты области не учитываются )
 //  precision - точность (в метрах)
 //  Возвращает: 1 - область внутри объекта
 //                 (в этом случае объект д.б. обязательно замкнутым),
 //              2 - объект внутри области
 //              3 - область и объект пересекаются,
 //              4 - область и объект не пересекаются
 //              5 - объект касается области и лежит внутри нее
 //              6 - объект касается области и лежит снаружи
 //  При ошибке возвращает 0
 //  21/02/01

#ifdef __cplusplus
_MAPIMP int _MAPAPI mapCheckOverlap(HOBJ area, HOBJ info, double precision = 0);
#else
_MAPIMP int _MAPAPI mapCheckOverlap(HOBJ area, HOBJ info, double precision);
#endif

#ifdef __cplusplus


 // Найти пересечение двух отрезков
 // xy1,xy2 - первый отрезок,
 // xy3,xy4 - второй отрезок
 // point1,point2  - точки пересечения
 // precision - точность (в метрах)
 // Возврат : 1 - одна точка пересечения,
 //           2 - отрезок xy1,xy2 лежит  на отрезке xy3,xy4
 // При отсутствии точки пересечения или ошибке возвращает ноль

_MAPIMP long int _MAPAPI mapCrossCutData(DOUBLEPOINT *xy1, DOUBLEPOINT *xy2,
                                  DOUBLEPOINT *xy3, DOUBLEPOINT *xy4,
                                  DOUBLEPOINT *point1, DOUBLEPOINT *point2,
                                  double precision = DELTANULL);

 // ----------------------------------------------------------
 // Пересечение отрезка и метрики объекта / подобъекта
 // xy1, xy2 - координаты отрезка
 // data - метрика объекта/подобъекта
 // subject - номер объекта/подобъекта
 // ( 0 - объект, 1 - первый подобъект и т.д.)
 // first - номер первой точки участка
 // last  - номер последней точки участка
 // Возвращает две структуры NUMBERPOINT, включающие
 // номер точки метрики, после которой
 // находится точка пересечения и
 // структуру DOUBLEPOINT - координаты точки пересечения
 // Две структуры заполняются если отрезок xy1,xy2 лежит
 // на отрезке метрики
 // При отсутствии точек пересечения или ошибке возвращает ноль
 // ----------------------------------------------------------
_MAPIMP long int _MAPAPI mapCrossCutAndSubject(HOBJ info,            // 31/01/06
                             DOUBLEPOINT * xy1, DOUBLEPOINT * xy2,
                             int first, int last,
                             NUMBERPOINT * point1,
                             NUMBERPOINT * point2, int subject,
                             double precision);

 // Положение точки относительно отрезка
 // point     - координаты 
 // xy1,xy2   - координаты концов отрезка
 // precision - точность
 // Возвращает POINTPOSITION (PS_FIRST, PS_LEFT... - maptype.h)
 // При ошибке возвращает 0

_MAPIMP long int _MAPAPI mapGetPointPosition(DOUBLEPOINT *point,
                                          DOUBLEPOINT *xy1,
                                          DOUBLEPOINT *xy2,
                                          double precision = DELTANULL);


 // Определение двух точек по перпендикуляру от отрезка (point1-point2)
 // на расстоянии (+/-) distance от произвольной точки point (по умолчанию point1)
 // pointout1, pointout2 - координаты требуемых точек
 // point1,point2        - координаты концов отрезка
 // point                - произвольная точка, от которой производиться расчет
 // distance             - растояние от точки point
 // При ошибке возвращает 0

_MAPIMP long int _MAPAPI mapSeekPointNormalLine(DOUBLEPOINT *point1,
                                         DOUBLEPOINT *point2,
                                         DOUBLEPOINT *pointout1,
                                         DOUBLEPOINT *pointout2,
                                         double distance, DOUBLEPOINT * point = 0);
#endif




  /**********************************************************
  *                                                         *
  *    ФУНКЦИИ РАССЕЧЕНИЕ ЗАМКНУТОГО ОБ'ЕКТА ПО ЛИНИИ       *
  *                                                         *
  **********************************************************/

 // *********************************************************
 // Схема запуска:
 // =============
 // HCROSS hCross = mapCreateObjectCutByLine(info1,info2,method,precision)
 // if (hCross)
 //   {
 //     while(mapGetNextCut(hCross,info))
 //     {
 //        ...
 //     }
 //     mapFreeObjectsCut(hCross);
 //   }
 //**********************************************************

 // Рассечение замкнутого объекта по линии
 // Создание класса рассечения
 // info1 - линия без подобъектов, по которой режут
 // info2 - замкнутый объект, который режут (произвольный,c подобъектами)
 // method - метод нарезки объектов:
 //          LOCAL_LINE - линейный,
 //          LOCAL_SQUARE - площадной
 // precision - точность при дотягивании (в метрах), при precision=0 устанавливается точность
 //             0.001 для карт масштаба <= 500000,
 //             0.01 для карт масштаба более 500000, если precision больше предложенной, то
 //             устанавливается большее значение
 // Возвращает указатель на класс разрезания
 // При ошибке возвращает 0

_MAPIMP HCROSS _MAPAPI mapCreateObjectCutByLine(HOBJ info1, HOBJ info2,
                                         long int method,
                                         double precision);


 // Рассечение замкнутого объекта по линии
 // Освобождение класса рассечения
 // hCross - указатель на класс разрезания

_MAPIMP void _MAPAPI mapFreeObjectsCut(HCROSS hCross);


 // Рассечение замкнутого объекта по линии
 // Запросить объект
 // hCross - указатель на класс разрезания
 // info   - результат
 // При ошибке возвращает 0

_MAPIMP HOBJ _MAPAPI mapGetNextCut(HCROSS hCross, HOBJ info);




  /*********************************************************************
  *                                                                    *
  *     ФУНКЦИИ ПЕРЕСЕЧЕНИЯ ПЛОЩАДНЫХ ОБ'ЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ       *
  *                                                                    *
  * Пересечение двух площадных объектов(с учетом ВСЕХ подобъектов),    *
  * т.е. нахождение их общей части                                     *
  * один из которых (ПЕРВЫЙ объект карты) - РЕЗАК(по которому режут)   *
  * другой (ВТОРОЙ объект карты) - ОБЪЕКТ, КОТОРЫЙ РЕЖУТ.              *
  * Только для ПЛОЩАДНЫХ объектов !!!                                  *
  **********************************************************************/

 // *********************************************************
 // Схема запуска:
 // =============
 // HCROSS hCross = mapCreateObjectsCrossSquare(info1,info2,method,precision)
 // if (hCross)
 //   {
 //    while(mapGetNextCross(hCross,info))
 //    {
 //      ...
 //    }
 //    mapFreeObjectsCross(hCross);
 //   }
 //**********************************************************

 // Создание класса пересечения двух площадных объектов(с учетом ВСЕХ подобъектов)
 // Только для ПЛОЩАДНЫХ объектов !!!
 // info1 - первый объект карты - РЕЗАК (произвольный площадной объект с подобъектами)
 // info2 - второй объект карты (произвольный площадной объект с подобъектами)
 // precision - точность при дотягивании (в метрах), при precision=0 устанавливается точность
 //             0.001 для карт масштаба <= 500000,
 //             0.01 для карт масштаба более 500000, если precision больше предложенной, то
 //             устанавливается большее значение
 // Возвращает указатель на класс пересечения
 // При ошибке возвращает 0

_MAPIMP HCROSS _MAPAPI mapCreateObjectsCrossSquare(HOBJ info1, HOBJ info2,
                                            double precision);


 // Установить допуск при удалении одинаковых точек  13/01/03
 // info - объект карты
 // precision - предлагаемый допуск или 0 (в метрах)
 // возвращает реальный допуск

_MAPIMP double _MAPAPI mapSetPrecision(HOBJ info, double precision);




  /*******************************************************************
  *                                                                  *
  *     ФУНКЦИИ ДЛЯ НАБОРА ОБЪЕКТОВ, ОБЪЕДИНЕННЫХ ПО СЕМАНТИЧЕСКОЙ   *
  *                      ХАРАКТЕРИСТИКЕ                              *
  *                                                                  *
  *  Коды семантик:                                                  *
  *                                                                  *
  *  32801 - признак объекта, имеющего ссылку на подчиненный объект; *
  *  32802 - признак объекта, на который имеется ссылка от главного  *
  *          объекта;                                                *
  *  32803 - признак объекта, входящего в группу равноправных        *
  *          объектов.                                               *
  *                                                                  *
  *    Значением семантики является уникальный номер группы, который *
  * формируется автоматически по номеру главного объекта или первого *
  * объекта в группе равноправных объектов.                          *
  *                                                                  *
  ********************************************************************/

 // *********************************************************
 // Схема запуска:
 // =============
 // HOBJSET hObjSet = mapCreateObjectSet()
 // if (hObjSet)
 //   {
 //     ... функции класса
 //     mapFreeObjectSet(hObjSet);
 //   }
 //**********************************************************

#ifdef __cplusplus          // Не переносить на С

 // Создание класса набора объектов, объединенных по семантической
 // характеристике
 // При ошибке возвращает 0

_MAPIMP HOBJSET _MAPAPI mapCreateObjectSet();


 // Освобождение класса набора объектов, объединенных по семантической
 // характеристике
 // hobjset - указатель на набора объектов

_MAPIMP void _MAPAPI mapFreeObjectSet(HOBJSET hobjset);


 // Построение группы из объектов карты
 // по существующей в объекте групповой семантике
 // hobjset - указатель на набора объектов
 // info    - идентификатор объекта карты
 // При ошибке возвращает 0

_MAPIMP long int _MAPAPI mapBuildObjectSet(HOBJSET hobjset, HOBJ info);


 // Построение группы из объектов карты
 // по определенному типу существующей в объекте групповой семантике
 // hobjset - указатель на набора объектов
 // info    - идентификатор объекта карты
 // type    - тип семантической характеристики для поиска в info
 //           GROUPLEADER, GROUPSLAVE, GROUPPARTNER
 // если type = 0 - ищет первую попавшуюся групповую семантику
 // ( функция mapBuildObjectSet );
 // При ошибке возвращает 0

_MAPIMP long int _MAPAPI mapBuildObjectSetByType(HOBJSET hobjset, HOBJ info, int type);


 // Заполнить контекст поиска из объектов набора
 // hobjset - указатель на набора объектов
 // hselect - условия поиска
 // При ошибке возвращает 0

_MAPIMP long int _MAPAPI mapBuildSelect(HOBJSET hobjset, HSELECT hselect);


 // Запросить количество объектов в наборе
 // hobjset - указатель на набора объектов
 // При ошибке возвращает 0

_MAPIMP long int _MAPAPI mapObjectSetCount(HOBJSET hobjset);


 // Запросить объект из набора по номеру (начиная с 1)
 // Объект HOBJ не нужно создавать !!! 
 // hobjset - указатель на набора объектов
 // number  - номер объекта из набора
 // При ошибке возвращает 0

_MAPIMP HOBJ _MAPAPI mapObjectSetObject(HOBJSET hobjset, long int number);


 // Запросить габариты объектов набора
 // hobjset - указатель на набора объектов
 // frame   - координаты (в метрах) прямоугольной области габаритов набора
 // При ошибке возвращает 0

_MAPIMP long int _MAPAPI mapObjectSetFramePlane(HOBJSET hobjset, DFRAME *frame);


 // Назначить главный объект в наборе (приписать семантику 32801)
 // hobjset - указатель на набора объектов
 // number - порядковый номер объекта в листе (из числа объектов в наборе)
 // если number = 0, назначается первый попавшийся объект в наборе
 // При ошибке возвращает 0

_MAPIMP long int _MAPAPI mapObjectSetNominateLeader(HOBJSET hobjset, int number);


 // Удалить объект из набора по порядковому номеру (с 1)
 // Удаленный заменяется на последний
 // Если объект главный в группе - удаляется вся группа
 // hobjset - указатель на набора объектов
 // number - порядковый номер объекта 
 // save - сохранить изменения в файл
 //  =  0 - не сохранять, 1 - сохранять
 // При ошибке возвращает 0

_MAPIMP long int _MAPAPI mapObjectSetClearObject(HOBJSET hobjset, long int number,
                                          long int save);


 // Удалить все объекты из набора
 // hobjset - указатель на набора объектов
 // save    - сохранить изменения в файл
 //           =  0 - не сохранять, 1 - сохранять

_MAPIMP void _MAPAPI mapObjectSetClear(HOBJSET hobjset, long int save);


 // Запрос признака группового объекта (по семантике)
 // groupnumber - переменная, куда помещается номер группы
 // (если он нужен) или 0
 // hobjset - указатель на набора объектов
 // hobj    - идентификатор объекта
 // Возвращает код групповой семантики (GROUPLEADER,GROUPSLAVE,GROUPPARTNER) или 0

_MAPIMP long int _MAPAPI mapObjectSetIsGroup(HOBJSET hobjset, HOBJ hobj,
                                      long int *groupnumber);


 // Запросить наличие групповой семантики данного типа у объекта
 // hobjset - указатель на набора объектов
 // hobj    - идентификатор объекта
 // type -  тип групповой семантики
 //        GROUPLEADER, GROUPSLAVE, GROUPPARTNER
 // Возвращает номер группы или 0 при отсутствии

_MAPIMP long int _MAPAPI mapObjectSetGetTypeSemn(HOBJSET hobjset, HOBJ hobj, int type);


 // Удалить все объекты набора с карты
 // hobjset - указатель на набор объектов

_MAPIMP long int _MAPAPI mapObjectSetDelete(HOBJSET hobjset);


 // Удалить групповую семантику из объекта
 // group - номер набора
 // если group == 0, то используется номер группы класса hobjset
 // hobjset - указатель на набора объектов
 // hobj    - идентификатор объекта

_MAPIMP long int _MAPAPI mapObjectSetDeleteSemantic(HOBJSET hobjset, HOBJ hobj,
                                             long int group);

 // Отобразить набор
 // hobjset - указатель на набор объектов
 // hmap    - идентификатор открытой карты
 // hDC     - контекст устройства отображения
 // Rect    - область отображения

_MAPIMP long int _MAPAPI mapPaintObjectSet(HOBJSET hobjset, HMAP hmap,
                                    HDC hDC, RECT& rect);

 // Сохранить набор
 // hobjset - указатель на набора объектов
 // always  - сохранять всегда или только, если были изменения
 //         = 0 - были изменения
 //         = 1 - всегда

_MAPIMP long int _MAPAPI mapObjectSetSave(HOBJSET hobjset, long int always);


 // Удалить объект из группы по его номеру на листе (с 1)
 // hobjset - указатель на набор объектов
 // number  - порядковый номер объекта
 // save - сохранение в файл
 //      =  0 - не сохранять, 1 - сохранять

_MAPIMP int _MAPAPI mapObjectSetRemoveNumber(HOBJSET hobjset, int number,
                                      int save);


 // Добавить главный объект группу (с приписыванием семантики 32801)
 // hobjset - указатель на набор объектов
 // save - сохранение в файл : = 0 - не сохранять, 1 - сохранять
 // Если hobj - вновь созданный объект (Key = 0) и save = 1
 //             вначале info будет сохранен, а потом добавлен в группу
 // Если объект hobj содержит групповую семантику и является главным
 // - добавляет в набор, не изменяя групповую семантику
 // Если объект hobj содержит групповую семантику и является подчиненным
 // - добавляет групповую семантику (создает новую группу не разрушая старую)
 // Возвращает порядковый номер объекта или 0

_MAPIMP int _MAPAPI mapObjectSetAppendGeneral(HOBJSET hobjset, HOBJ hobj, int save); // 02/10/02


 // Добавить главный объект группу (с приписыванием семантики 32801)
 // hobjset - указатель на набор объектов
 // save - сохранение в файл : = 0 - не сохранять, 1 - сохранять
 // Если hobj - вновь созданный объект (Key = 0) и save = 1
 //             вначале info будет сохранен, а потом добавлен в группу
 // Если объект hobj содержит групповую семантику и является главным
 // - разрушает существующую группу и создает новую
 // Возвращает порядковый номер объекта или 0

_MAPIMP int _MAPAPI mapObjectSetAppendGeneralNew(HOBJSET hobjset, HOBJ hobj, int save); // 02/10/02


 // Добавить подчиненный объект группу (с приписыванием семантики 32802)
 // hobjset - указатель на набор объектов
 // save - сохранение в файл : = 0 - не сохранять, 1 - сохранять
 // Если hobj - вновь созданный объект (Key = 0) и save = 1
 //             вначале info будет сохранен, а потом добавлен в группу
 // Если объект hobj содержит групповую семантику и является главным
 // - добавляет групповую семантику (создает новую группу не разрушая старую)
 // Если объект hobj содержит групповую семантику и является подчиненным
 // - добавляет в набор, изменяя групповую семантику
 // Возвращает порядковый номер объекта или 0

_MAPIMP int _MAPAPI mapObjectSetAppendSubordinate(HOBJSET hobjset, HOBJ hobj,
                                           int save);


 // Добавить подчиненный объект группу (с приписыванием семантики 32802)
 // hobjset - указатель на набор объектов
 // save - сохранение в файл : = 0 - не сохранять, 1 - сохранять
 // Если hobj - вновь созданный объект (Key = 0) и save = 1
 //             вначале info будет сохранен, а потом добавлен в группу
 // Если объект hobj содержит групповую семантику и является подчиненным
 // - разрушает существующую группу и создает новую
 // Возвращает порядковый номер объекта или 0

_MAPIMP int _MAPAPI mapObjectSetAppendSubordinateNew(HOBJSET hobjset, HOBJ hobj,
                                              int save);


 // Найти главный объект в группе
 // hobjset - указатель на набор объектов
 // group - номер группы
 //         при group = 0 - номер группы устанавливается из набора hobjset
 // Объект HOBJ не нужно создавать !!!

_MAPIMP HOBJ _MAPAPI mapObjectSetFindGeneral(HOBJSET hobjset, long int group);


 // Объединить наборы hobjset и set
 // regime -
 //    = 1 -  добавить отдельный объект
 //           если объект главный - создать иерархию, включив только его
 //           в текущий набор
 //           если объект подчиненный или равноправный - включить его в текущий набор,
 //           удалив из set
 //    = 2 -  весь набор
 //           разрушается набор set и все объекты включаются в текущий набор
 //    = 3 -  создать иерархию
 //           найти главный объект набора set и включить его как подчиненный
 //           в текущий набор, создав иерархию
 // info - по какому объекту набора set ориентироваться
 // Результат помещается в набор hobjset
 // save - сохранение в файл
 //      =  0 - не сохранять, 1 - сохранять

_MAPIMP int _MAPAPI mapObjectSetUnion(HOBJSET hobjset, HOBJSET set,
                               int regime, HOBJ info, int save);


 /*******************************************************************
 *                                                                  *
 *     ФУНКЦИЯ ДЛЯ РАСЧЕТА СЕМАНТИЧЕСКОЙ ХАРАКТЕРИСТИКИ  В ТОЧКЕ    *
 *                    ПО ВЕКТОРНОЙ КАРТЕ                            *
 *                                                                  *
 ********************************************************************/

 // Вычисление значения характеристики в заданной точке
 // по данным векторной карты.]
 // hMap - идентификатор открытой векторной карты,
 // Характеристика задаётся кодом семантики - semanticCode,поиск заданной
 // характеристики выполняется по всем объектам векторной карты.
 // Координаты точки (point->X,point->Y) задаются в метрах в
 // системе координат векторной карты.
 // Вычисленное значение характеристики заносится в value.
 // В случае ошибки возвращает 0
 // 06/10/05
_MAPIMP long int _MAPAPI  mapCalcCharacteristic(HMAP hMap,
                                        DOUBLEPOINT* point,
                                        long int semanticCode,
                                        double*  value);

#endif

#ifdef __cplusplus
}       // extern "C"
#endif


#endif  // SEEKAPI_H
