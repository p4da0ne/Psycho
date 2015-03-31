/**** GISGPS.H ***************** Shabakov D.A. ******** 02/02/06 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2006              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*                  FOR  WINDOWS95 & WINDOWS NT                     *
*                                                                  *
********************************************************************
*                                                                  *
*          ОБРАБОТКА ДАННЫХ С GPS ПРИЕМНИКА ЧЕРЕЗ COM-ПОРТ         *
*                                                                  *
*******************************************************************/

//---------------------------------------------------------------------------
#ifndef GISGPS_H
#define GISGPS_H

#ifndef MAPAPI_H
  #include "mapapi.h"
#endif

extern "C"
{

  // Открыть диалог наблюдения за состоянием спутников В МОДАЛЬНОМ РЕЖИМЕ
  long int _export WINAPI GisDialogGpsTest(long int hmap,TASKPARMEX * parm);


  
//---------------------------------------------------------------------------
//      ФУНКЦИИ ДЛЯ ПОДКЛЮЧЕНИЯ ДИАЛОГА В НЕМОДАЛЬНОМ РЕЖИМЕ
//---------------------------------------------------------------------------

  // Открыть диалог наблюдения за состоянием спутников
  long int WINAPI _export gisGpsOpenMonitor(HMAP hmap, TASKPARMEX *parm);

  // Закрыть  диалог наблюдения за состоянием спутников
  long int WINAPI _export gisGpsCloseMonitor();

  // Известить диалог об изменении языка сообщений
  long int WINAPI _export gisGpsChangeLanguage(long int code);

  // Известить диалог об изменении состава данных
  long int WINAPI _export gisGpsChangeData();

} // extern "C"

#endif
