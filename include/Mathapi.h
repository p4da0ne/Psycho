/****   MATHAPI.H    **********  Shabakov D.A.   *****  22/10/04 ***
*****   MATHAPI.H    **********  Konon V.N.      *****  01/07/04 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2004              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*                   FOR  WINDOWS95 & WINDOWS NT                    *
*                                                                  *
********************************************************************
*                                                                  *
*            ОПИСАНИЕ ФУНКЦИЙ ОБРАБОТКИ ДАННЫХ                     *
*        ИНТЕРФЕЙС ДЛЯ ПРОГРАММ НА ЯЗЫКАХ C, PASCAL, BASIC         *
*                                                                  *
*******************************************************************/

#ifndef MATHAPI_H
#define MATHAPI_H

#ifndef MAPTYPE_H
  #include "maptype.h"   // Описание структур интерфейса MAPAPI
#endif
#include "mapapi.h"
#include <math.h>
#include <stdio.h>
typedef long int  HCUT;    // ИДЕНТИФИКАТОР ОБ'ЕКТА

 // *********************************************************
 // Схема запуска:
 // =============
 // HCUT hCut = mathCreateCutProcess(hMap, info, precision);
 // if (hCut)
 //    {
 //    while(mathGetNextTriangularObject(hCut, infoNew))
 //         {
 //         ...
 //         }
 //    mathFreeCutProcess(hCut);
 //    }
 // **********************************************************

extern "C"
{

// Отсечение элементарного треугольного объекта от сложного площадного бъекта карты
// Создание класса (Конструктор)
// info - площадной бъект карты
// Возвращает указатель на класс
// При ошибке возвращает 0
HCUT _export  WINAPI  mathCreateCutProcess(HMAP Map, HOBJ info, double precision);

// Отсечение элементарного треугольного объекта от сложного площадного бъекта карты
// Запросить следующий треугольный объект
// info - результат (треугольный объект)
// Для заполнения высот точек(если исходный объект имеет 3-х мерную метрику)    // 22/10/04
//     info необходимо создавать с 3-х мерной метрикой
// Возвращает остаток сложного площадного бъекта карты
// При ошибке возвращает 0
HOBJ _export  WINAPI  mathGetNextTriangularObject(HCUT hcut, HOBJ info);

// Отсечение элементарного треугольного объекта от сложного площадного бъекта карты
// Освобождение класса (Деструктор)
long int _export  WINAPI  mathFreeCutProcess(HCUT hcut);

//******************************************************************
//        ФУНКЦИИ РАСЧЁТА ПОЛОЖЕНИЯ ТОЧКИ ДЛЯ НАНЕСЕНИЯ            *
//        НА ВЫБРАННЫЙ ПЛОЩАДНОЙ ОБЪЕКТ                            *
//******************************************************************
//  ИМПОРТИРОВАНИЕ ФУНКЦИЙ :                                       *
//                                                                 *
//  // Загрузка библиотеки                                         *
//  HINSTANCE libInst;                                             *
//                                                                 *
//  // Вызов функции                                               *
//  int (WINAPI *GetObjectCenter)(HMAP hmap, HOBJ info, double *x, *
//                                double *y);                      *
//                                                                 *
//  (FARPROC) GetObjectCenter = mapLoadLibrary("mapmath.dll",      *
//                                          &libInst,              *
//                                          "mathGetObjectCenter");*          *
//                                                                 *
//  int code = (GetObjectCenter*)(hmap,info,&x,&y);                *
//    ...                                                          *
//                                                                 *
//  // Выгрузка библиотеки                                         *
//  ::FreeLibrary(libInst);                                        *
//                                                                 *
//*****************************************************************/

// Пересечение линии с площадным объектом
long int _export  WINAPI  mathCreateLineCross(HMAP hMap, HOBJ inobj, HOBJ outinfo,
                                          double step);

// Интерполирование линии
long int _export  WINAPI  mathLineInterpolate(HOBJ inobj, HOBJ outobj,
                                          double step);

// Установить центр площадного объекта
long int _export  WINAPI  mathGetObjectCenter(HMAP hmap, HOBJ info,
                                          double *x,double *y);

// Установить длину отрезка между точками с координатами x1,y1 и x2,y2
// delta - шаг приближения
// number - номер точки
// При ошибке возвращает 0
long int _export  WINAPI  mathSetLineLength(double *x1,double *y1,double *x2,double *y2,
                                       double delta,long int number);

// Создание списка номенклатур по заданным габаритам района
// hmap - идентификатор открытых данных
// info  - заданная область расчёта номенклатур листов карты (метрика
//                                              должна быть замкнутой)
// scale - знаменатель масштаба карты (25000 - 1000000)
// listreg - адрес массива структур паспортных данных листа карты
// sheet - максимальное число номенклатур
// Возвращаемое значение: при ошибке - 0,
// при нормальном завершении заполнятся поля структуры LISTREGISTER для
// каждого листа
long int _export  WINAPI  mathSheetFromFrame(HMAP  hmap,
                                  HOBJ info, long scale,
                                  LISTREGISTER* listreg,
                                  long sheet);

// Контроль габаритов района создания номенклатур
// info  - заданная область построения листов карты
// выходные параметры:
// frame - координаты 2-х углов области построения листа
// regim - параметр учёта прохождения через осевой меридиан
//                    (0 - осевой меридиан не пересекается
//                     1 - осевой меридиан пересекается)
// При ошибке возвращает 0
long int _export  WINAPI  mathCheckFrame(HMAP hmap, HOBJ info, FRAME*frame,
                                         int &regim);

// Заполнение полей массива структур LISTREGISTER паспортных данных
// array - адрес списка номенклатур,рассчитанных по заданной области
// listreg - адрес массива структур паспортных данных листа карты
// scale   - знаменатель масштаба (25000 - 1000000)
// countname - число рассчитанных номенклатур
// При ошибке возвращает 0
long int _export  WINAPI  mathSetListRegister(char* array,
                                              LISTREGISTER*listreg,
                                              long scale, long countname);

// Отбраковка номенклатур,не принадлежащих заданной области
// info  - заданная область построения листов карты
// listreg - адрес массива структур паспортных данных листа карты
// count   - число рассчитанных номенклатур после отбраковки
// При ошибке возвращает 0, иначе  число номенклатур после отбраковки
long int _export  WINAPI  mathSetBelongNomenclature(HMAP hmap, HOBJ info,
                                                    LISTREGISTER*listreg,
                                                    long count);


// Определить номенклатуру листа по масштабу scale и геодезическим координатам
// центра b,l (в градусах)
// name - номенклатура листа
// comp - признак компановки(создавать сдвоенные листы - 1
//                           не создавать              - 0)
// При ошибке возвращает 0, иначе 1

long int _export  WINAPI  mathGetRuleSheetName(long int scale,double b,double l,
                                               char *name,long int comp);

}       // extern "C"

#endif  // MATHAPI_H
