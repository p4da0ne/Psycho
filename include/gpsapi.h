/****** gpsapi.h *************** Belenkov  O.V. ******* 08/07/08 ***
******* gpsapi.h *************** Shabakov  D.A. ******* 19/02/07 ***
******* gpsapi.h *************** Dishlenko S.G. ******* 03/08/05 ***
******* gpsapi.h *************** Vitsko    D.A. ******* 12/04/06 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2008              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*             ЧТЕНИЕ ДАННЫХ С GPS, РАБОТА С COM-ПОРТОМ ИЛИ USB     *
*                                                                  *
*******************************************************************/

#ifndef GPSAPI_H
#define GPSAPI_H

#ifndef MAPTYPE_H
  #include "maptype.h"
#endif

#ifdef LINUXAPI // 03/08/05
  // Управление терминалом
  #include <termios.h>
#endif

typedef long int HCOMPORT;

typedef struct TIMEDATA    // 05/09/06
{
  double Time;                // Время UTC
  long int Day;               // День (01-31)
  long int Month;             // Месяц (01-12)
  long int Year;              // Год
  long int ZoneForHours;      // Часовой пояс, смещение от GMT (0+- 13 часов)
  long int ZoneForMinutes;    // Часовой пояс, смещение от GMT (минуты)
  char Rez[20];               // Резерв
}
  TIMEDATA;

// Параметры спутника
typedef struct SATELLITEPARM  // 29/01/07
{
  char SatelliteNumber[4];    // PRN номер спутника
  long int Height;            // Высота, градусы(90 - максимум)
  long int Azimuth;           // Азимут истинный, градусы(0-359)
  long int SignalNoise;       // Отношение сигнал/шум от 0-99 дБ, ноль-нет сигнала
}
  SATELLITEPARM;


extern "C"
{

// Открыть COM или USB порт для чтения GPS - сообщений в формате NMEA O183
// Для Windows:
// portname - имя COM-порта (например, "COM1:" или путь к USB-порту)
// baudrate - частота обмена с портом (например, 9600 или 0)
// Для Linux:
//   portname - имя COM-порта (например, "/dev/ttyS0"("COM1:" в Windows))
//   baudrate - частота обмена с портом (например, B9600 или B4800)
// При ошибке возвращает 0
_MAPIMP HCOMPORT _MAPAPI gpsOpen(LPTSTR portname, long int baudrate);

// Закрыть COM-порт
_MAPIMP void _MAPAPI gpsClose(HCOMPORT hcomport);

// Открыть COM-порт для чтения/записи
// portname - имя COM-порта (например, "COM1:")
// baudrate - частота обмена с портом (например, 4800)
// При ошибке возвращает 0
_MAPIMP HCOMPORT _MAPAPI gpsOpenComPort(LPTSTR portname, long int baudrate);
_MAPIMP HCOMPORT _MAPAPI gpsOpenComPortUn(const WCHAR * portname, long int baudrate);

// Закрыть COM-порт
_MAPIMP void _MAPAPI gpsCloseComPort(HCOMPORT hcomport);

// Запросить - обновились ли координаты точки
// Это можно делать по таймеру, например, каждую секунду
_MAPIMP long int _MAPAPI gpsIsUpdate(HCOMPORT hcomport);

// Запросить координаты текущей точки в радианах в системе GPS (WGS-84)  // 19/02/07
// Признак обновления координат сбрасывается
_MAPIMP long int _MAPAPI gpsGetPoint(HCOMPORT hcomport, DOUBLEPOINT * point,
                                     double * height);

// Запросить координаты текущей точки в градусах в системе GPS (WGS-84)
// Признак обновления координат сбрасывается
_MAPIMP long int _MAPAPI gpsGetSourcePoint(HCOMPORT hcomport, DOUBLEPOINT * point,
                                           double * height);

// Запросить время и дату определения местоположения // 05/09/06
// timedata - структура, заполняемая параметрами времени и даты
_MAPIMP long int _MAPAPI gpsCurrentTime(HCOMPORT hcomport, TIMEDATA * timedata);

// Запросить число используемых спутников
_MAPIMP long int _MAPAPI gpsSatelliteCount(HCOMPORT hcomport);

//  Запросить число видимых спутников   // 04/02/05
_MAPIMP long int _MAPAPI gpsVisibleSatelliteCount(HCOMPORT hcomport);

// Запросить Скорость над поверхностью, км/ч     // 30/08/06
_MAPIMP long int _MAPAPI gpsSpeed(HCOMPORT hcomport, double * speed);

// Запросить геометрический фактор PDOP // 25/01/07
// PDOP = 1   - идеальное расположение спутников
// PDOP < 6.0 - пригодно для навигации
_MAPIMP long int _MAPAPI gpsFactorPDOP(HCOMPORT hcomport, double * factor);

// Запросить горизонтальный геометрический фактор HDOP // 25/01/07
// PDOP = 1   - идеальное расположение спутников
// PDOP < 4.0 - пригодно для навигации
_MAPIMP long int _MAPAPI gpsFactorHDOP(HCOMPORT hcomport, double * factor);

// Запросить вертикальный геометрический фактор HDOP // 25/01/07
// PDOP = 1   - идеальное расположение спутников
// PDOP < 4.5 - пригодно для навигации
_MAPIMP long int _MAPAPI gpsFactorVDOP(HCOMPORT hcomport, double * factor);

// Запросить режим функционирования устройства // 29/01/07
// Возвращает: 1-местоположение не определено, 2 - 2D, 3 - 3D
// При ошибке возвращает 0
_MAPIMP long int _MAPAPI gpsDeviceMode3D(HCOMPORT hcomport);

// Запросить количество спутников, для которых известны параметры // 29/01/07
// (PRN номер спутника; Высота, градусы; Азимут истинный; Отношение сигнал/шум)
// Параметры запрашивать функцией gpsSatelliteParam()
// При ошибке возвращает 0
_MAPIMP long int _MAPAPI gpsSatelliteParamCount(HCOMPORT hcomport);

// Запросить параметры спутников для альманаха // 29/01/07
// (PRN номер спутника; Высота, градусы; Азимут истинный; Отношение сигнал/шум)
// parm  - указатель на структуру SATELLITEPARM
// (необходимое количество элементов структуры запрашивать функцией gpsSatelliteParamCount() )
// count - количество объявленных элементов структуры SATELLITEPARM
// При ошибке возвращает 0
_MAPIMP long int _MAPAPI gpsSatelliteParam(HCOMPORT hcomport, SATELLITEPARM* parm,  long int count);

// Запросить PRN номера спутников, используемых при решении навигационной задачи // 30/01/07
// Сообщение приемника GPS - $GPGGA
// numbers - указатель на массив, в который будут занесены PRN номера спутников
// size    - размер массива (количество элементов массива)
// Предусмотреть выделение массива для 12-ти спутников.
// При ошибке возвращает 0
_MAPIMP long int _MAPAPI gpsSatelliteNumbers(HCOMPORT hcomport, long int * numbers, long int size);

// Включить формирование протокола входного буфера // 06/02/05
_MAPIMP long int _MAPAPI gpsCreateFileProtocol(HCOMPORT hcomport, char *fileName, long int sizeFileName);

// Отключить протокол входного буфера // 06/02/05
_MAPIMP long int _MAPAPI gpsCloseFileProtocol(HCOMPORT hcomport);

// Запросить качество приема данных
_MAPIMP long int _MAPAPI gpsQuality(HCOMPORT hcomport);

// Запросить имя открытого COM-порта
_MAPIMP LPTSTR _MAPAPI gpsPortName(HCOMPORT hcomport);
_MAPIMP long int _MAPAPI gpsPortNameUn(HCOMPORT hcomport, WCHAR * name, long int size);

// Запросить идентификатор открытого COM-порта
// Если порт закрыт - возвращает ноль
_MAPIMP HANDLE _MAPAPI gpsPortHandle(HCOMPORT hcomport);

// Запросить частоту обмена с портом
// Если порт закрыт - возвращает ноль
_MAPIMP long int _MAPAPI gpsBaudrate(HCOMPORT hcomport);

// Запросить содержимое последней записи из GPS
// buffer - адрес буфера для размещения результата
// size   - размер буфера
// При ошибке возвращает ноль
_MAPIMP long int _MAPAPI gpsGetRecord(HCOMPORT hcomport, char * buffer, long int size);
_MAPIMP long int _MAPAPI gpsGetRecordUn(HCOMPORT hcomport, WCHAR * buffer, long int size);

// Передать запись в порт (некоторые GPS-устройства активизируются
// после получения команды)
// buffer - адрес буфера с отправляемой записью
// size   - размер отправляемой записи
// При ошибке возвращает ноль
_MAPIMP long int _MAPAPI gpsPutRecord(HCOMPORT hcomport, char * buffer, long int size);

#ifdef WIN32API     // 03/08/05
// Запросить код ошибки последней операции
// При отсутствии ошибки возвращает ноль
_MAPIMP long int _MAPAPI gpsGetLastError(HCOMPORT hcomport);


// Установить Функции обратного вызова  // 15/02/05
// mask - команда(сообщение устройства), соответствующая вызываемой функции
// call - адрес вызываемой функции (см. maptype.h),
// parm - параметр, который будет передан вызываемой функции.
// При ошибке возвращает ноль
_MAPIMP long int _MAPAPI gpsSetMask(HCOMPORT hcomport,
                            const char * mask, MASKCALL call, long int parm);
#endif

#ifdef WINCE
// Определить имя доступного COM-порта для подключения к GPS-приемнику
// Работает в среде Windows Mobile 5.х и выше
// При ошибке возвращает ноль
_MAPIMP long int _MAPAPI gpsGetDefaultGPSPort(WCHAR * portname, long int size);
#endif

} // extern "C"

#endif  // MAPPORT_H
