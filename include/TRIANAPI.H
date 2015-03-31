/******  TRIANAPI.H  **********  Demidenko A.G. ******  01/09/2000 *
*******  TRIANAPI.H  **********  Shishkova L.A. ******  12/05/2003 *
*******  TRIANAPI.H  **********  Tarakanova I.R.******  27/06/2005 *
*******  TRIANAPI.H  **********  Gorbunov A.V.  ******  16/01/2006 *
*                                                                  *
********************************************************************
*                                                                  *
*                  FOR  WINDOWS95 & WINDOWS NT                     *
*                                                                  *
********************************************************************
*                                                                  *
*            ФУНКЦИИ БИБЛИОТЕКИ (GIS\GSS)DELONE.DLL                *
*                                                                  *
*******************************************************************/

#include "mapsyst.h"
#include "maptype.h"
#include "tritype.h"   //12/05/03

#ifndef TRIANAPI_H
#define TRIANAPI_H

extern "C"
{

// Функция выполняет построение триангуляции Делоне по массиву точек.
// Входные параметры: points - указатель на массив точек, для построения
// триангуляции, в массиве не должно быть повторяющихся точек;
// count- количество точек во входном массиве;
// frame - указатель на объект, полностью ограничиваищий массив точек 
// триангуляции, для создания триангуляции в замкнутом контуре
// После каждого вызова CreateDeloneTtiangle должен быть вызов
// DeleteDeloneTtiangle
// Общее число точек (рамки и входного массива) должно быть больше трех,
// иначе отсутствует физический смысл триангуляции (создания треугольников)
// Возвращает указатель на построенную триангуляцию

// Первый вариант создания триангуляции
// по двумерным координатам пикетных точек
HTRIANG _export
CreateDeloneTriangle(HWND parent,
                    XYDOUBLE* points,
                    unsigned long count,
                    HOBJ frame=0);

// Создание массива триангуляции с предварительной сортировкой точек
HTRIANG _export
CreateDeloneTriangleSort(HWND parent,
                    XYDOUBLE* Points,
                    unsigned long count,
                    HOBJ frame=0);

// Второй вариант создания триангуляции.
// По трехмерным координатам пикетных точек
HTRIANG   _export
CreateDeloneTriangleXYH(HWND parent,
                     XYHDOUBLE* points,
                     unsigned long count,
                     HOBJ frame=0);

// Создание массива триангуляции с учетом высот узлов
// и с предварительной сортировкой точек
HTRIANG   _export
CreateDeloneTriangleXYHSort(HWND parent,
                     XYHDOUBLE* Points,
                     unsigned long count,
                     HOBJ frame=0);

// Построение триангуляции (массива треугольников и массива рёбер)
// по массиву трехмерных точек
// parent - идентификатор окна диалога, которому посылаются
// сообщения о ходе процесса :
//   0x3000 - сообщение о проценте выполненных работ (в WPARAM),
//   если процесс должен быть принудительно завершен, в ответ
//   должно вернуться значение 0x3000.
// Если parent = 0 - сообщения не посылаются.
// points - массив исходных точек (структура VERTEX описана в tritype.h)
// count - количество точек в исходном массиве
// indexEdge - флаг выполнения индексации рёбер построенной
// триангуляции (0 - не выполнять, 1 - выполнять).
// Если indexEdge = 0, то построение триангуляции выполняется
// быстрее, но массив рёбер не содержит ссылок на треугольники - поля
// LTri,RTri структуры EDGE (структура EDGE описана в tritype.h)
// frame - идентификатор замкнутого объекта карты, ограничивающего
// триангуляцию. Возвращает указатель на построенную триангуляцию
// (HTRIANG описан в tritype.h)
// В случае ошибки возвращает 0
HTRIANG   _export
BuildDeloneTriangulation(HWND parent,
                         VERTEX* points,
                         unsigned long count,
                         long int indexEdge = 1,
                         HOBJ frame = 0);


// Построение триангуляции (массива рёбер)
// по массиву трехмерных точек
// parent - идентификатор окна диалога, которому посылаются
// сообщения о ходе процесса :
//   0x3000 - сообщение о проценте выполненных работ (в WPARAM),
//   если процесс должен быть принудительно завершен, в ответ
//   должно вернуться значение 0x3000.
// Если parent = 0 - сообщения не посылаются.
// points - массив исходных точек (структура VERTEX описана в tritype.h)
// count - количество точек в исходном массиве
// frame - идентификатор замкнутого объекта карты, ограничивающего
// триангуляцию. Возвращает указатель на построенную триангуляцию
// (HTRIANG описан в tritype.h)
// В случае ошибки возвращает 0

HTRIANG   _export
BuildDeloneEdges(HWND parent,
                 VERTEX* points,
                 unsigned long count,
                 HOBJ frame = 0);


// Удаление массива триангуляции
// HTRIANG htriangle - указатель на построенную триангуляцию
long int  _export
DeleteDeloneTriangle(HTRIANG htriangle);

// Получить номер треугольника, которому принадлежит  точка
// HTRIANG htriangle - указатель на построенную триангуляцию
// XYDOUBLE point - координаты точки
// Нумерация треугольников начинается с 0.
// В случае ошибки возвращает -1.
long int  _export
GetNumberTriangle(HTRIANG htriangle, XYDOUBLE point);

// Получить номер треугольника, которому принадлежит  точка
// HTRIANG htriangle - указатель на построенную триангуляцию
//   (триангуляция должна быть построена функцией BuildDeloneTriangulation)
// XYDOUBLE point - координаты точки
// Нумерация треугольников начинается с 0.
// В случае ошибки возвращает -1.
long int  _export
GetNumberTriangleAdd(HTRIANG htriangle, XYDOUBLE point);  // 23/12/05

// Получить информацию о треугольнике по его номеру в массиве
// HTRIANG htriangle - указатель на построенную триангуляцию
// TRIANGLE triangle - информация о треугольнике с номером number
// long int number - номер треугольника в массиве триангуляции
// Нумерация треугольников начинается с 0.
// В случае ошибки возвращает 0.
long int  _export
GetTriangle(HTRIANG htriangle, TRIANGLE* triangle, long int number);

// Получить информацию о ребре AB,BC,CA треугольника numTriangle
// HTRIANG htriangle - указатель на построенную триангуляцию
// edge - информация о ребре AB,BC,CA треугольника numTriangle
// (структура EDGE описана в tritype.h)
// В случае ошибки возвращает 0.
long int  _export
GetTriangleEdgeAB(HTRIANG htriangle, EDGE* edge, long int numTriangle);

long int  _export
GetTriangleEdgeBC(HTRIANG htriangle, EDGE* edge, long int numTriangle);

long int  _export
GetTriangleEdgeCA(HTRIANG htriangle, EDGE* edge, long int numTriangle);

// Получить информацию о ребре по его номеру в массиве
// HTRIANG htriangle - указатель на построенную триангуляцию
// EDGE edge - информация о ребре с номером number
// long int number - номер треугольника в массиве триангуляции
// Нумерация ребер начинается с 0.
// В случае ошибки возвращает 0.
long int  _export
GetEdge(HTRIANG htriangle, EDGE* edge, long int number);


// Получить информацию о вершине треугольника по ее номеру в массиве
// HTRIANG htriangle - указатель на построенную триангуляцию
// XYDOUBLE vertex - информация о вершине треугольника с номером number
// long int number - номер вершины треугольника в массиве пикетных точек
// Нумерация вершин треугольников начинается с 0.
// В случае ошибки возвращает 0.
long int  _export
GetVertex(HTRIANG htriangle, XYDOUBLE* vertex, long int number);

// Получить информацию о вершине треугольника по ее номеру в массиве
// htriangle - указатель на построенную триангуляцию
// vertex - информация о вершине треугольника с номером number
// (структура VERTEX описана в tritype.h)
// number - номер вершины треугольника в массиве пикетных точек
// Нумерация вершин треугольников начинается с 0.
// В случае ошибки возвращает 0.
long int  _export
GetVertexXYH(HTRIANG htriangle, VERTEX* vertex, long int number);

// Получить информацию о характеристике вершины треугольника (третьей
// координате) по ее номеру в массиве пикетных точек
// HTRIANG htriangle - указатель на построенную триангуляцию
// double heigth - значение характеристики вершины треугольника с номером number
// long int number - номер вершины треугольника в массиве пикетных точек
// Нумерация вершин треугольников начинается с 0.
// В случае ошибки возвращает 0.
long int  _export
GetHeigth(HTRIANG htriangle, double* heigth, long int number);

// Получить информацию о характеристике вершины треугольника (третьей
// координате) по ее номеру в массиве пикетных точек
// HTRIANG htriangle - указатель на построенную триангуляцию
//   (триангуляция должна быть построена функцией BuildDeloneTriangulation)
// double heigth - значение характеристики вершины треугольника с номером number
// long int number - номер вершины треугольника в массиве пикетных точек
// Нумерация вершин треугольников начинается с 0.
// В случае ошибки возвращает 0.
long int  _export
GetHeigthAdd(HTRIANG htriangle, double* heigth, long int number);

// Получить число треугольников.
// HTRIANG htriangle - указатель на построенную триангуляцию
// Возвращает число треугольников в массиве триангуляции
// В случае ошибки возвращает 0.
long int  _export
GetQuantTriangle(HTRIANG htriangle);

// Получить число вершин.
// HTRIANG htriangle - указатель на построенную триангуляцию
// Возвращает число вершин в массиве триангуляции
// В случае ошибки возвращает 0.
long int  _export
GetQuantVertex(HTRIANG htriangle); 

// Получить число ребер.
// HTRIANG htriangle - указатель на построенную триангуляцию
// Возвращает число ребер в массиве триангуляции
// В случае ошибки возвращает 0.
long int  _export
GetQuantEdges(HTRIANG htriangle);

// Выполнить сгущение триангуляции
// HTRIANG htriangle - указатель на построенную триангуляцию
// В случае ошибки возвращает 0.
long int  _export
ThickenTriangle(HTRIANG htriangle);

// Выполнить разложение триангуляции
// HTRIANG htriangle - указатель на построенную триангуляцию
// В случае ошибки возвращает 0.
long int  _export
FacrtorizationTriangle(HTRIANG htriangle);

// Получить протокол бракованых точек
// Возвращает количество бракованных точек
long int GetControlTriangle(HTRIANG htriangle, int* control);


// Добавление в триангуляцию структурных линий (линий перегиба)
// HTRIANG htriangle - указатель на построенную триангуляцию  27/06/05
// В случае ошибки возвращает 0.
long int  _export
 AddBreakLinesInTriangle(HTRIANG htriangle,HOBJ info);

// Перестроить триангуляцию с учетом структурных линий    27/06/05
// HTRIANG htriangle - указатель на построенную триангуляцию
// В случае ошибки возвращает 0.
long int  _export
 RebuildTriangle(HTRIANG htriangle,HOBJ info);

// Добавить в массив исходных точек точки пересечения отрезков метрики
// структурной линии info с рёбрами ранее построенной триангуляции htriangle
long int  _export
 AddPointsByLine(HTRIANG htriangle, HOBJ info);

// Выставить флаг "структурная линия" рёбрам триангуляции htriangle,
// лежащим на отрезках структурной линии info
long int  _export
 MarkEdgesByLine(HTRIANG htriangle, HOBJ info);

} // extern "C"
#endif




