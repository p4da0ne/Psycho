/****  AREALIST.H  ************  Belenkov  O.V.  *****  21/11/06 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2006              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*                FOR  WINDOWS ME & WINDOWS XP, LINUX               *
*                                                                  *
********************************************************************
*                                                                  *
*           СПИСОК РАЙОНОВ ЭЛЕКТРОННОЙ КАРТЫ  ВЕРСИЯ 9.х           *
*                                                                  *
*******************************************************************/

#ifndef AREALIST_H
#define AREALIST_H

#ifndef MAPTYPE_H
 #include "maptype.h"
#endif 

typedef struct ALSITEM     // ОПИСАНИЕ ЭЛЕМЕНТА СПИСКА РАЙОНОВ РАБОТ
{
  long Ident;              // Идентификатор элемента
  char Path[260];          // Полный путь к паспорту района
  char Name[32];           // Имя района (берется из паспорта)
  long Scale;              // Масштаб района (берется из паспорта)
  long Minimum;            // Нижняя граница перехода к району (например, 100 000)
  long Maximum;            // Верхняя граница перехода к району (например, 500 000)
  long Priority;           // Приоритет выбора для одинаковых масштабов
}
  ALSITEM;

typedef struct ALSITEMEX   // ОПИСАНИЕ ЭЛЕМЕНТА СПИСКА РАЙОНОВ РАБОТ
{                                                                     // 21/11/06
  long Ident;              // Идентификатор элемента
  long Scale;              // Масштаб района (берется из паспорта)
  long Minimum;            // Нижняя граница перехода к району (например, 100 000)
  long Maximum;            // Верхняя граница перехода к району (например, 500 000)
  long Priority;           // Приоритет выбора для одинаковых масштабов
  long Reserve;            // Резерв = 0
}
  ALSITEMEX;

typedef long int HALS;     // Идентификатор списка районов работ

// Список служебных семантик, описывающих район работ (элемент атласа)
#define SEMALSPATH   7701  // Путь к карте
#define SEMALSNAME   7702  // Имя района работ
#define SEMALSSCALE  7703  // Масштаб района работ
#define SEMALSMIN    7704  // Нижняя граница перехода
#define SEMALSMAX    7705  // Верхняя граница перехода
#define SEMALSMODE   7706  // Номер состояния данных
#define SEMALSPRIOR  7707  // Приоритет выбора

#define ALS_UP          1  // Поиск более мелкого масштаба
#define ALS_DOWN        2  // Поиск более крупного масштаба
#define ALS_ANY         0  // Поиск любой карты
#define ALS_BACK        4  // Вернуться к карте

// ----------------------------------------------------------------------
// Список структур произвольного размера                   
// ----------------------------------------------------------------------
typedef class _DLLCLASS TSimpleList
{
public:

  // Создать список
  // size    - размер элемента списка
  // reserve - минимальное число элементов
  TSimpleList(int size, int reserve = 256);

  ~TSimpleList() { Clear(); }

  // Добавить запись
  // item - адрес начала записи
  // size - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  int Append(const char * item, int size);

  // Число элементов
  int  Count() { return ItemCount; }

  // Очистить список
  void Clear();

  // Удалить список
  // number - номер элемента, начиная с 1
  void Delete(int number);

  // Запросить элемент
  // number - номер элемента, начиная с 1
  char * Item(int number);
  
private:

  int Allocate(int size);

public:

  int    ItemCount;
  char * ItemBegin;
  int    ItemSize;
  int    MemorySize;
  int    ItemReserve;
}
  TSimpleList;

// ----------------------------------------------------------------------
// Список районов работ
// ----------------------------------------------------------------------
typedef class _DLLCLASS TAreaList : public TSimpleList
{
public:

  // Создать список
  TAreaList() : TSimpleList(sizeof(ALSITEM)) {}

  // Добавить запись
  int Append(ALSITEM * item)
   { return TSimpleList::Append((char*)item, sizeof(ALSITEM)); }
  
  // Запросить элемент
  ALSITEM * Item(int number)
   { return (ALSITEM*) TSimpleList::Item(number); }
}  
  TAreaList;


extern "C"
{

// Открыть список районов работ
// listname - путь к списку районов работ (файлу "ALS")
// При ошибке возвращает ноль, иначе - идентификатор списка районов

HALS _MAPAPI alsOpenAreaList(const char * listname);

// Создать список районов работ
// listname - путь к списку районов работ (файлу "ALS")
// listtype - тип списка районов (0 - глобальный с поддержкой геодезических
//            координат, 1 - локальный в местной системе координат
// rscname -  имя классификатора для создания атласа (штатно - "arealist.rsc")
// При ошибке возвращает ноль, иначе - идентификатор списка районов

HALS _MAPAPI alsCreateAreaList(const char * listname, long int listtype);
HALS _MAPAPI alsCreateAreaListEx(const char * listname,
                                 const char * rscname, long int listtype);

// Закрыть список районов работ
// hals - идентификатор списка районов

void _MAPAPI alsCloseAreaList(HALS hals);

// Запросить является ли атлас глобальным
// hals - идентификатор списка районов (атласа)
// Если нет - возвращает ноль

HALS _MAPAPI alsIsGlobalAreaList(HALS hals);

// Запросить число элементов в списке
// hals - идентификатор списка районов

long int _MAPAPI alsGetItemCount(HALS hals);

// Запросить описание элемента списка районов
// hals   - идентификатор списка районов
// number - порядковый номер элемента, начиная с 1
// item   - заполняемая структура описания элемента
// При ошибке возвращает ноль

long int _MAPAPI alsGetItem(HALS hals, long int number, ALSITEM * item);

// Запросить описание элемента списка районов
// hals   - идентификатор списка районов
// number - порядковый номер элемента, начиная с 1
// item   - заполняемая структура описания элемента
// При ошибке возвращает ноль

long int _MAPAPI alsGetItem(HALS hals, long int number, ALSITEM * item);
long int _MAPAPI alsGetItemEx(HALS hals, long int number, ALSITEMEX * item); // 21/11/06

// Запросить имя элемента списка районов                 // 21/11/06
// hals   - идентификатор списка районов
// number - порядковый номер элемента, начиная с 1
// name   - адрес памяти для записи результата
// size   - длина области памяти в байтах (не менее 32)
// При ошибке возвращает ноль

long int _MAPAPI alsGetItemName(HALS hals, long int number, char * name, long int size);

// Запросить путь к паспорту для элемента из списка районов         // 21/11/06
// hals   - идентификатор списка районов
// number - порядковый номер элемента, начиная с 1
// path   - адрес памяти для записи результата
// size   - длина области памяти в байтах (не менее 260)
// При ошибке возвращает ноль

long int _MAPAPI alsGetItemPath(HALS hals, long int number, char * path, long int size);

// Запросить описание элемента списка районов
// hals   - идентификатор списка районов
// ident  - идентификатор элемента
// item   - заполняемая структура описания элемента
// При ошибке возвращает ноль

long int _MAPAPI alsGetItemByIdent(HALS hals, long int ident, ALSITEM * item);

// Запросить описание элемента списка районов                    // 21/11/06
// hals   - идентификатор списка районов
// ident  - идентификатор элемента
// item   - заполняемая структура описания элемента
// При ошибке возвращает ноль

long int _MAPAPI alsGetItemByIdentEx(HALS hals, long int ident, ALSITEMEX * item);

// Добавить элемент в список районов
// hals - идентификатор списка районов
// path - полный путь к паспорту района работ
// При ошибке возвращает ноль

long int _MAPAPI alsAppendItem(HALS hals, const char * path);


// Установить границы масштабов для автоматического перехода в район
// hals    - идентификатор списка районов
// ident   - идентификатор элемента, см. ALSITEM
// minimum - нижняя граница перехода к району (например, 100 000)
// maximum - верхняя граница перехода к району (например, 500 000)
// При ошибке возвращает ноль

long int _MAPAPI alsSetItemMiniMax(HALS hals, long int ident,
                                   long int minimum,
                                   long int maximum);

// Установить приоритет выбора карты
// hals     - идентификатор списка районов
// ident    - идентификатор элемента, см. ALSITEM
// priority - приоритет выбора района
// При ошибке возвращает ноль

long int _MAPAPI alsSetItemPriority(HALS hals, long int ident,
                                    long int priority);

// Установить границы масштабов для автоматического перехода в район
// hals   - идентификатор списка районов
// ident   - идентификатор элемента, см. ALSITEM
// При ошибке возвращает ноль

long int _MAPAPI alsDeleteItem(HALS hals, long int ident);


// Найти элемент расположенный в заданной точке ниже указанного
// элемента (в более крупном масштабе)
// hals   - идентификатор списка районов
// ident   - идентификатор элемента, см. ALSITEM
// point  - геодезические координаты заданной точки в радианах
// item   - заполняемая структура описания найденного элемента
// При ошибке возвращает ноль

long int _MAPAPI alsGoItemDown(HALS hals, long int ident,
                               DOUBLEPOINT * point, ALSITEM * item);

// Найти элемент расположенный в заданной точке ниже заданного
// элемента (в более крупном масштабе)
// hals   - идентификатор списка районов
// scale  - масштаб текущего элемента
// point  - геодезические координаты заданной точки в радианах
// item   - заполняемая структура описания найденного элемента
// При ошибке возвращает ноль

long int _MAPAPI alsGoItemDownByScale(HALS hals, long int scale,
                                      DOUBLEPOINT * point, ALSITEM * item);

// Найти элемент расположенный в заданной точке ниже заданного
// элемента (в более крупном масштабе)
// hals   - идентификатор списка районов
// scale  - масштаб текущего элемента
// frame  - координаты заданной области на атласе
// item   - заполняемая структура описания найденного элемента
// При ошибке возвращает ноль

long int _MAPAPI alsGoItemDownByScaleEx(HALS hals, long int scale,
                                        HOBJ frame, ALSITEM * target);

// Найти элемент расположенный в заданной точке выше указанного
// элемента (в более мелком масштабе)
// hals   - идентификатор списка районов
// ident   - идентификатор элемента, см. ALSITEM
// point  - геодезические координаты заданной точки в радианах
// item   - заполняемая структура описания найденного элемента
// При ошибке возвращает ноль

long int _MAPAPI alsGoItemUp(HALS hals, long int ident,
                             DOUBLEPOINT * point, ALSITEM * item);

// Найти элемент расположенный в заданной точке выше указанного
// элемента (в более мелком масштабе)
// hals   - идентификатор списка районов
// scale  - масштаб текущего элемента
// point  - геодезические координаты заданной точки в радианах
// item   - заполняемая структура описания найденного элемента
// При ошибке возвращает ноль

long int _MAPAPI alsGoItemUpByScale(HALS hals, long int scale,
                                    DOUBLEPOINT * point, ALSITEM * item);

// Найти элемент расположенный в заданной точке выше заданного
// элемента (в более мелком масштабе)
// hals   - идентификатор списка районов
// scale  - масштаб текущего элемента
// frame  - геодезические координаты заданной области на атласе
// item   - заполняемая структура описания найденного элемента
// При ошибке возвращает ноль

long int _MAPAPI alsGoItemUpByScaleEx(HALS hals, long int scale,
                                      HOBJ frame, ALSITEM * target);

// Установить масштаб отображения с учетом размещения изображения
// данных в пределах заданной области
// width  - ширина области в пикселах,
// height - высота области в пикселах

void _MAPAPI alsSetPictureSize(HALS hals, long int width,
                               long int height);

// Выделить на схеме указанный элемент атласа
// hdc   - контекст окна отображения атласа
// rect  - координаты окна в области полного изображения атласа
// ident - идентификатор элемента, см. ALSITEM
// color - цвет выделения элемента на атласе
// thick - толщина линии выделения в пикселах

void _MAPAPI alsPaintSelectItem(HALS hals, HDC hdc,
                                RECT * rect, long int ident,
                                COLORREF color, int thick);

// Запросить положение района в изображении атласа
// ident - идентификатор элемента (района), см. ALSITEM
// point - заполненные координаты в пикселах для текущего масштаба изображения
// При ошибке возвращает ноль

long int _MAPAPI alsGetItemPosition(HALS hals, long int ident, POINT * point);

// Выбрать в точке все районы
// hals   - идентификатор списка районов
// point  - геодезические координаты заданной точки в радианах
// При ошибке возвращает ноль

long int _MAPAPI alsItemList(HALS hals, TAreaList * list, DOUBLEPOINT * point);


// Запросить - есть ли в точке карта
// direct    - направление поиска (ALS_UP, ALS_DOWN, ALS_ANY)
// point     - геодезические координаты заданной точки в радианах
// Если карта есть - возвращает ненулевое значение
// При ошибке возвращает ноль

long int _MAPAPI alsEnableItem(HALS hals, long int scale,
                               DOUBLEPOINT * point, long int direct);

// Запросить - есть ли в заданной области карта
// direct    - направление поиска (ALS_UP, ALS_DOWN, ALS_ANY)
// frame     - координаты заданной области на атласе
// Если карта есть - возвращает ненулевое значение
// При ошибке возвращает ноль

long int _MAPAPI alsEnableItemEx(HALS hals, long int scale,
                                 HOBJ frame, long int direct);
                                      
}  // extern "C"


#endif  //  AREALIST_H

