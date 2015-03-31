/****  VECEXAPI.H    **********  Savochkin V.N.  *****  11/04/03 ***
*****  VECEXAPI.H    **********  Belenkov  O.V.  *****  30/11/06 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2006              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*                   FOR  WINDOWS95 & WINDOWS NT                    *
*                                                                  *
********************************************************************
*                                                                  *
*            ОПИСАНИЕ КЛАССА ДОСТУПА К ОБРАБОТКЕ ДАННЫХ            *
*        ИНТЕРФЕЙС ДЛЯ ПРОГРАММ НА ЯЗЫКАХ C, PASCAL, BASIC         *
*                                                                  *
*******************************************************************/

#if !defined(VECEXAPI_H)
#define VECEXAPI_H

#ifndef MAPAPI_H
  #include "mapapi.h"    // Параметры функций визуализации
#endif

#ifndef Mmstruct_H
  #include "mmstruct.h"      // 30/11/06
#endif

#ifdef WIN32API
   #define HMESSAGE HWND
#else
   #define HMESSAGE MSGHANDLER
#endif

typedef struct INFOLIST {
  char Nomenclature[32];     // номенклатура
  long   Scale;              // масштаб
  long   CountObject;        // количество объектов
  int   Type;                // Тип карты
                  } INFOLIST;



typedef struct INFOSXF {
  char  SheetName[32];
  char  Nomenclature[32];
  long  RealCoordinate;
  long  Scale;
  long  RecordCount;
  long  MapType;
                  } INFOSXF;

typedef struct INFODIR {
  char  SheetName[32];
  char  NameRsc[MAX_PATH];
  long  CountList;
                  } INFODIR;

//#include "vecexdll.h"

extern "C"
{

//---------------------------------------------------------------------------
//  Структурный контроль листа
//   hMap,hSite - идентификаторы карты (см.MAPAPI.H),
//   mode       - режим работы (0 - контроль, 1 - редактирование),
//   handle     - идентификатор окна диалога процесса обработки.
//
//  Идентификатору посылаются следующие сообщения :
//  WM_LIST   = 0x586   WParam - количество листов в районе
//                      LParam - номер текущего листа
//  WM_OBJECT = 0x585   WParam - процент обработанных объектов
//  WM_ERROR  = 0x587   WParam - порядковый номер объекта или 0
//                      LParam = 1 - ошибка в карте
//                             = 2 - ошибка в классификаторе
//                             = 3 - ошибка в описании объекта
//                             = 4 - ошибка в метрике
//                             = 5 - ошибка в семантике
//                             = 6 - ошибка в графике
//  Возвращает количество ошибок в районе
//---------------------------------------------------------------------------

long int WINAPI _export mapStructControl(HMAP hMap,HSITE hSite,
                                         long mode,HMESSAGE handle);

//---------------------------------------------------------------------------
//       Сортировка всех карт и обстановок, входящих в документ
//
//           mapdoc - Сортируемый документ
//           handle - Идентификатор окна диалога процесса обработки
//           mode   - Способ обработки карты (0 - сортировать все листы,
//                                            1 - только несортированные)
//                                  2 - сохранять файлы отката  08/10/00

//---------------------------------------------------------------------------

long int _export WINAPI MapSortProcess(HMAP mapdoc,HMESSAGE handle,
                                       long int mode);


//========================================================================
//   Функции загрузки карты
//========================================================================
//  Идентификатору посылаются следующие сообщения :
//  WM_LIST   = 0x586   WParam - количество листов в районе
//                      LParam - номер текущего листа
//  WM_OBJECT = 0x585   WParam - процент обработанных объектов
//  WM_INFOLIST = 0x584 LParam - адрес структуры INFOLIST
// ----------------------------------------------------------------
// Загрузить карту из файла SXF
// ----------------------------------------------------------------

long int WINAPI _export LoadSxfToMap(char * namesxf,
                                     char * namemap,
                                     char * namersc,
                                     HMESSAGE handle);

// ----------------------------------------------------------------
// Загрузить карту из файла SXF с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export LoadSxfToMapSelect(char * namesxf,
                                           char * namemap,
                                           char * namersc,
                                           HMESSAGE handle,
                                           HSELECT select);

// ----------------------------------------------------------------
// Загрузить карту из файла SXF с использованием Select 
// и изменением имени карты
// ----------------------------------------------------------------

long int WINAPI _export LoadSxfToMapSelectName(char * namesxf,
                                               char * namemap,
                                               char * namersc,
                                               HMESSAGE handle,
                                               HSELECT select,
                                               char * name,
                                               char * namehdr);

// ----------------------------------------------------------------
// Загрузить карту из файла TXT
// ----------------------------------------------------------------

long int WINAPI _export LoadTxtToMap(char * nametxt,
                                     char * namemap,
                                     char * namersc,
                                     HMESSAGE handle);

// ----------------------------------------------------------------
// Загрузить карту из файла TXT с испльзованием Select
// ----------------------------------------------------------------

long int WINAPI _export LoadTxtToMapSelect(char * nametxt,
                                           char * namemap,
                                           char * namersc,
                                           HMESSAGE handle,
                                           HSELECT select);


// ----------------------------------------------------------------
// Загрузить карту из файла TXT с испльзованием Select и изменением имени
// ----------------------------------------------------------------

long int WINAPI _export LoadTxtToMapSelectName(char * nametxt,
                                               char * namemap,
                                               char * namersc,
                                               HMESSAGE handle,
                                               HSELECT select,
                                               char * name,
                                               char * namehdr);


// ----------------------------------------------------------------
// Загрузить карту по списку DIR
// ----------------------------------------------------------------

long int WINAPI _export LoadDirToMap(char* namedir,
                                     char* namemap,
                                     HMESSAGE handle);

// ----------------------------------------------------------------
// Загрузить карту по списку DIR с использованием  Select
// ----------------------------------------------------------------

long int WINAPI _export LoadDirToMapSelect(char* namedir,
                                           char* namemap,
                                           HMESSAGE handle,
                                           HSELECT select);


// ----------------------------------------------------------------
// Загрузить карту по списку DIR с использованием  Select и 
// изменением имени района и имени файла ресурсов
// ----------------------------------------------------------------

long int WINAPI _export LoadDirToMapSelectName(char* namedir,
                                               char* namemap,
                                               HMESSAGE handle,
                                               HSELECT select,
                                               char * name,
                                               char* namersc);


// ----------------------------------------------------------------
// Загрузить карту по данным из другого района
// ----------------------------------------------------------------

long int WINAPI _export LoadMapToMap(HMAP  map,
                                     char * namemap,
                                     HMESSAGE handle);

// ----------------------------------------------------------------
// Загрузить карту по данным другого района с использованием  Select
// ----------------------------------------------------------------

long int WINAPI _export LoadMapToMapSelect(HMAP map,
                                           char* namemap,
                                           HMESSAGE handle,
                                           HSELECT select);


//========================================================================
//   Функции обновления карты
//========================================================================

// ----------------------------------------------------------------
// Обновить карту из файла SXF
// ----------------------------------------------------------------

long int WINAPI _export UpdateMapFromSxf(char* namesxf,
                                         char* namemap,
                                         HMESSAGE handle);

// ----------------------------------------------------------------
// Обновить карту из файла SXF с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export UpdateMapFromSxfSelect(char* namesxf,
                                         char* namemap,
                                         HMESSAGE handle,
                                         HSELECT select);

// ----------------------------------------------------------------
// Добавить в карту данные из файла SXF с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export AppendMapFromSxfSelect(char* namesxf,
                                         char* namemap,
                                         HMESSAGE handle,
                                         HSELECT select);


// ----------------------------------------------------------------
// Обновить карту из файла TXT
// ----------------------------------------------------------------

long int WINAPI _export UpdateMapFromTxt(char* nametxt,
                                         char* namemap,
                                         HMESSAGE handle);

// ----------------------------------------------------------------
// Обновить карту из файла TXT с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export UpdateMapFromTxtSelect(char* nametxt,
                                         char* namemap,
                                         HMESSAGE handle,
                                         HSELECT select);


// ----------------------------------------------------------------
// Добавить в карту данные из файла TXT с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export AppendMapFromTxtSelect(char* namesxf,
                                         char* namemap,
                                         HMESSAGE handle,
                                         HSELECT select);

// ----------------------------------------------------------------
// Обновить карту из файла DIR с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export UpdateMapFromDirSelect(char* namedir,
                                         char* namemap,
                                         HMESSAGE handle,
                                         HSELECT select);


// ----------------------------------------------------------------
// Добавить в карту данные из файла DIR с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export AppendMapFromDirSelect(char* namedir,
                                         char* namemap,
                                         HMESSAGE handle,
                                         HSELECT select);

//========================================================================
//   Функции сохранения карты
//========================================================================
//  Идентификатору посылаются следующие сообщения :
//  WM_LIST   = 0x586   WParam - количество листов в районе
//                      LParam - номер текущего листа
//  WM_OBJECT = 0x585   WParam - процент обработанных объектов

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат SXF(Windows)
// ----------------------------------------------------------------

long int WINAPI _export SaveSxfFromMap(char * namemap,
                                       int list,
                                       char * namesxf,
                                       HMESSAGE handle);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат SXF(Windows) с
//          использованием  Select
// ----------------------------------------------------------------

long int WINAPI _export SaveSxfFromMapSelect(char * namemap,
                                             int list,
                                             char * namesxf,
                                             HMESSAGE handle,
                                             HSELECT select,
                                             int flag = 0,
                                             char * nameregion = NULL);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат SXF(Windows) с
//          использованием  Select
// ----------------------------------------------------------------

long int WINAPI _export SaveSxfFromHMapSelect(HMAP map,
                                             int list,
                                             char * namesxf,
                                             HMESSAGE handle,
                                             HSELECT select,
                                             int flag = 0,
                                             char * nameregion = NULL);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат SXF(DOS)
// ----------------------------------------------------------------

long int WINAPI _export SaveSxfDosFromMap(char * namemap,
                                          int list,
                                          char * namesxf,
                                          HMESSAGE handle);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат SXF(Dos) с
//          использованием  Select
// ----------------------------------------------------------------

long int WINAPI _export SaveSxfDosFromMapSelect(char * namemap,
                                                int list,
                                                char * namesxf,
                                                HMESSAGE handle,
                                                HSELECT select,
                                                char * nameregion=NULL);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат SXF(Dos) с
//          использованием  Select
// ----------------------------------------------------------------

long int WINAPI _export SaveSxfDosFromHMapSelect(HMAP map,
                                                int list,
                                                char * namesxf,
                                                HMESSAGE handle,
                                                HSELECT select,
                                                char * nameregion=NULL);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат TXT(XY)
// ----------------------------------------------------------------


long int WINAPI _export SaveTxtFromMap(char * namemap,
                                       int   list,
                                       char * nametxt,
                                       HMESSAGE handle);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат TXT(XY) с
//      использованием Select
// ----------------------------------------------------------------

long int WINAPI _export SaveTxtFromMapSelect(char * namemap,
                                             int list,
                                             char * nametxt,
                                             HMESSAGE handle,
                                             HSELECT select,
                                             char * nameregion = NULL);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат TXT(XY) с
// использованием Select
// namemap - имя выгружаемой карты,
// list    - номер листа,
// nametxt - имя файла TXF,
// handle  - идентификатор окна, которому посылаются сообщения о ходе процесса
//           (WM_INFOLIST, WM_OBJECT)
// nameregion - имя района (карты)
// precision  - число знаков после запятой для прямоугольных координат
// ----------------------------------------------------------------

long int WINAPI _export SaveTxtFromMapSelectEx(const char * namemap,
                                               long int list,
                                               const char * nametxt,
                                               HMESSAGE handle,
                                               HSELECT select,
                                               const char * nameregion,
                                               long int precision);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат TXT(BL)
// ----------------------------------------------------------------


long int WINAPI _export SaveTxtGeoFromMap(char * namemap,
                                          int list,
                                          char * nametxt,
                                          HMESSAGE handle);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат TXT(BL) с
//      использованием Select
// ----------------------------------------------------------------

long int WINAPI _export SaveTxtGeoFromMapSelect(char * namemap,
                                                int list,
                                                char * nametxt,
                                                HMESSAGE handle,
                                                HSELECT select,
                                                char * nameregion = NULL);
long int WINAPI _export SaveTxtGeoGradFromMapSelect(char * namemap,
                                                int list,
                                                char * nametxt,
                                                HMESSAGE handle,
                                                HSELECT select,
                                                char * NameRegion = NULL);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат SXF(WINDOWS) по DIR
// ----------------------------------------------------------------

long int WINAPI _export SaveDirSxfFromMap(char* namemap,
                                     char* namedir,
                                     HMESSAGE handle);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат SXF(WINDOWS) по DIR
//   с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export SaveDirSxfFromMapSelect(char* namemap,
                                     char* namedir,
                                     HMESSAGE handle,
                                     HSELECT select);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат SXF(WINDOWS) по DIR
//   с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export SaveDirSxfIntFromMapSelect(char* namemap,
                                     char* namedir,
                                     HMESSAGE handle,
                                     HSELECT select);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат SXF(DOS) по DIR
// ----------------------------------------------------------------

long int WINAPI _export SaveDirSxfDosFromMap(char* namemap,
                                     char* namedir,
                                     HMESSAGE handle);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат SXF(DOS) по DIR
//   с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export SaveDirSxfDosFromMapSelect(char* namemap,
                                     char* namedir,
                                     HMESSAGE handle,
                                     HSELECT select);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат TXT(XY) по DIR
// ----------------------------------------------------------------

long int WINAPI _export SaveDirTxtFromMap(char* namemap,
                                     char* namedir,
                                     HMESSAGE handle);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат TXT(XY) по DIR
//   с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export SaveDirTxtFromMapSelect(char* namemap,
                                     char* namedir,
                                     HMESSAGE handle,
                                     HSELECT select);
// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат TXT(BL) по DIR
// ----------------------------------------------------------------

long int WINAPI _export SaveDirTxtGeoFromMap(char* namemap,
                                     char* namedir,
                                     HMESSAGE handle);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат TXT(BL) по DIR
//   с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export SaveDirTxtGeoFromMapSelect(char* namemap,
                                     char* namedir,
                                     HMESSAGE handle,
                                     HSELECT select);

// ----------------------------------------------------------------
// Выгрузка из внутреннего формата в формат TXT(BL градусы) по DIR
//   с использованием Select
// ----------------------------------------------------------------

long int WINAPI _export SaveDirTxtGeoGradFromMapSelect(char* namemap,
                                     char* namedir,
                                     HMESSAGE handle,
                                     HSELECT select);

// ----------------------------------------------------------------
//   Трансформирование района по рамкам
// ----------------------------------------------------------------

int WINAPI _export TransVector(HOBJ FrameReal , HOBJ FrameTeor);

//-------------------------------------------------------------------
//  Получение справочной информации о листе из SXF
// ------------------------------------------------------------------
long int _MAPAPI _export GetInfoSxf(char * namesxf,
                                  INFOSXF * infosxf);

//-------------------------------------------------------------------
//  Получение справочной информации из DIR
// ------------------------------------------------------------------
long int _MAPAPI _export GetInfoDir(char * namedir,
                                  INFODIR * infodir);

// ------------------------------------------------------------------
// Получение списка файлов SXF в DIR
// ------------------------------------------------------------------
long int _MAPAPI _export GetDir(char * namedir,
                                NAMESARRAY * sxfdir, long int length);

// ------------------------------------------------------------------
// Запросить паспортные данные векторной карты
// по имени файла SXF
// Структуры MAPREGISTER и LISTREGISTER описаны в mapcreat.h
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int _MAPAPI mapGetSxfInfoByName(const char * name,        //11/04/03
                                     MAPREGISTER * map,
                                     LISTREGISTER * sheet);

}       // extern "C"

#endif  // VECEXAPI_H


