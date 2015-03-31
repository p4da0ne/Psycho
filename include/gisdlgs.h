/**** GISDLGS.H *************** Belenkov O.V. ********* 06/06/07 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2007              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*      ОБЩИЕ ДИАЛОГИ ГИС КАРТА 2005 ДЛЯ ПРИЛОЖЕНИЙ НА GIS ToolKit  *
*                                                                  *
*                 ИМПОРТИРОВАНИЕ ФУНКЦИЙ :                         *
*                                                                  *
*   // Загрузка библиотеки                                         *
*   HINSTANCE libInst = ::LoadLibrary("gisdlgs.dll");              *
*                                                                  *
*   // Вызов функции                                               *
*   long int (WINAPI * tedInsertPoints)(HMAP hMap,                 *
*                                        TASKPARMEX* parm,         *
*                                        HOBJ info,                *
*                                        RECT * rect);             *
*   (FARPROC)tedInsertPoints =                                     *
*                   GetProcAddress(libInst, "tedInsertPoints");    *
*   long int Rezult = (*tedInsertPoints)(hMap, parm, info, 0);     *
*                                                                  *
*   // Выгрузка библиотеки                                         *
*   ::FreeLibrary(libInst);                                        *
*                                                                  *
*******************************************************************/

#ifndef GISDLGS_H
#define GISDLGS_H

#ifndef MAPTYPE_H
  #include "maptype.h"
#endif

extern "C"
{

 // Диалог "Редактирование метрики объекта"
 // hmap    - идентификатор открытой карты
 // parm    - параметры задачи (поле Handle должно содержать
 //           идентификатор главного окна)
 // info    - идентификатор редактируемого объекта
 // rect    - местоположение открываемого диалога или 0
 // При ошибке возвращает ноль

 long int WINAPI tedInsertPoints(HMAP hmap, TASKPARMEX *parm, HOBJ info, RECT *rect);

 // Диалог "Редактирование семантики объекта"
 // hmap    - идентификатор открытой карты
 // parm    - параметры задачи (поле Handle должно содержать
 //           идентификатор главного окна)
 // info    - идентификатор редактируемого объекта
 // При ошибке возвращает ноль

 long int WINAPI tedEditSemantic(HMAP hmap, TASKPARMEX *parm, HOBJ info);

 // Диалог "Отмена транзакций"
 // hmap    - идентификатор открытой карты
 // parm    - параметры задачи (поле Handle должно содержать
 //           идентификатор главного окна)
 // При ошибке возвращает ноль

 long int WINAPI tedUndoOperation(HMAP hmap, TASKPARMEX *parm);

 // Диалог "Открыть атлас карт"
 // parm    - параметры задачи (поле Handle должно содержать
 //           идентификатор главного окна)
 // name    - имя файла атласа карт, который открывается или создается
 // При ошибке возвращает ноль

 long int WINAPI tedOpenAtlas(TASKPARMEX *parm, char* name, long int size);

 // Диалог "Выбрать карту из списка"
 // parm    - параметры задачи (поле Handle должно содержать
 //           идентификатор главного окна)
 // item    - указатель на список элементов атласа, выбранных в текущей точке
 // count   - число элементов в списке
 // Возвращает номер выбранного пользователем элемента
 // При ошибке возвращает ноль

 long int WINAPI tedAtlasList(TASKPARMEX *parm, ALSITEM* item, long int count);

 // Диалог "Перейти в заданную точку по координатам"
 // hmap    - идентификатор открытой карты
 // parm    - параметры задачи (поле Handle должно содержать
 //           идентификатор главного окна)
 // place   - формат отображаемых в диалоге координат:
 // PLANEPOINT      = 1,  В метрах на местности
 // MAPPOINT        = 2,  В условных единицах карты (дискретах)
 // PICTUREPOINT    = 3,  В пикселах текущего полного изображения
 //                       эллипсоид Красовского 1942г.
 // GEORAD          = 4,  В радианах в соответствии с проекцией
 // GEOGRAD         = 5,  В градусах ...
 // GEOGRADMIN      = 6,  В градусах, минутах, секундах ...
 //                       общеземной эллипсоид WGS84
 // GEORADWGS84     = 7,  В радианах в соответствии с проекцией
 // GEOGRADWGS84    = 8,  В градусах ...
 // GEOGRADMINWGS84 = 9,  В градусах, минутах, секундах ...
 // PLANE42POINT    = 10, В метрах на местности по ближайшей зоне
 // GEORADPZ90      = 11, В радианах в соответствии с проекцией
 // GEOGRADPZ90     = 12, В градусах ...
 // GEOGRADMINPZ90  = 13, В градусах, минутах, секундах ...
 // point  - координаты текущей точки в метрах на карте
 // При успешном выполнении возвращает координаты выбранной точки в метрах
 // на карте
 // При ошибке возвращает ноль

 long int WINAPI tedGoPoint(HMAP hmap, TASKPARMEX *parm, long int place,
                            DOUBLEPOINT * point);

 // Диалог "Добавление в документ данных из каталога"
 // Могут быть добавлены ПК, Mtw, Rsw, Mtl, Mtq, Tin
 // hmap    - идентификатор открытой карты
 // parm    - параметры задачи (поле Handle должно содержать
 //           идентификатор главного окна)
 // title   - указатель на текст заголовка диалога или 0
 // flagSit - флаг запрета добавления ПК  (0-запрещено/1-разрешено)
 // flagMtw - флаг запрета добавления Mtw (0-запрещено/1-разрешено)
 // flagRsw - флаг запрета добавления Rsw (0-запрещено/1-разрешено)
 // flagMtl - флаг запрета добавления Mtl (0-запрещено/1-разрешено)
 // flagMtq - флаг запрета добавления Mtq (0-запрещено/1-разрешено)
 // flagTin - флаг запрета добавления Tin (0-запрещено/1-разрешено)
 // При ошибке возвращает ноль

 long int WINAPI tedAddDataFormDirEx(long int hmap, TASKPARMEX * parm, char * title,
                                     long int flagSit, long int flagMtw, long int flagRsw,
                                     long int flagMtl, long int flagMtq, long int flagTin);

} // extern "C"

#endif  // GISDLGS_H
