/***** MAPTRACE.H ************** Belenkov O.V.   ****** 14/04/97 ***
****** MAPTRACE.H ************** Dishlenko S.G.  ****** 23/07/99 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-1997              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*        ТРАССИРОВКА ВЫПОЛНЕНИЯ ПРОГРАММ ДЛЯ ПОИСКА ОШИБОК         *
*                                                                  *
*******************************************************************/

#ifndef MAPTRACE_H
#define MAPTRACE_H

/*
#ifndef MAPTRACER
  #define MAPTRACER FALSE   // Если трассировка нужна -
                            //  поставить TRUE
#endif


#if MAPTRACER != FALSE

  #include <stdio.h>

#ifndef LINUXAPI   // 23.07.99
  #define Tracer2(str, num, mes) \
          { FILE* hFile = fopen( "c:\\maptrace.log", "a+" );\
            fprintf( hFile, str, num, mes);\
            fclose( hFile );}

  #define Tracer1(str, mes) \
          { FILE* hFile = fopen( "c:\\maptrace.log", "a+" );\
            fprintf( hFile, str, mes);\
            fclose( hFile );}

  #define Tracer(message) \
          { FILE* HFile = fopen( "c:\\maptrace.log", "a+" );\
            fprintf( HFile, message);\
            fclose( HFile );}
#else                         // 23.07.99
  #define Tracer2(str, num, mes) \
          { FILE* hFile = fopen( "/maptrace.log", "a+" );\
            fprintf( hFile, str, num, mes);\
            fclose( hFile );}

  #define Tracer1(str, mes) \
          { FILE* hFile = fopen( "/maptrace.log", "a+" );\
            fprintf( hFile, str, mes);\
            fclose( hFile );}

  #define Tracer(message) \
          { FILE* HFile = fopen( "/maptrace.log", "a+" );\
            fprintf( HFile, message);\
            fclose( HFile );}
#endif
#else*/
  #define Tracer2(str, num, mes)
  #define Tracer1(str, mes)
  #define Tracer(message)
//#endif

//*******************************************************************
//  Пример вызова трассировки :
//   Tracer("0x%04X|message = 0x%04X BEGIN.\n", hwnd, uMsg);
//*******************************************************************


#endif
