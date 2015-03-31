/****** PICEXPRM.H ************* Shabakov D.A.   ****** 01/11/05 ***
*                                                                  *
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2004              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*                  FOR  WINDOWS95 & WINDOWS NT                     *
*                                                                  *
********************************************************************
*                                                                  *
*      ОПИСАНИЕ СТРУКТУРЫ ПАРАМЕТРОВ КОНВЕРТИРОВАНИЯ РАСТРА        *
*                                                                  *
*******************************************************************/

#ifndef PICEXPRM_H
#define PICEXPRM_H

#include "maptype.h"

#define WM_PCX2RST               0x590       // Конвертер  PCX в RST
#define WM_PCX2RST_PROGRESS      0x591
#define WM_PCX2RST_ERROR         0x592

#define WM_RSTCOMPRESS           0x593       // Сжатие растра
#define WM_RSTCOMPRESS_ERROR     0x594

// СТРУКТУРА ДЛЯ ПЕРЕДАЧИ ИНФОРМАЦИИ О ГРАФИЧЕСКИХ ФАЙЛАХ ДИАЛОГУ ОТ КОНВЕРТЕРА
typedef struct DATAINFORMATION
       {
       unsigned long int InputFileLength ;  // длина файла графического изображения
       long int Width           ;  // ширина изображения
       long int Height          ;  // высота изображения
       long int BitPerPixel     ;  // кол-во бит на пиксел
       double   Precision       ;  // разрешаюшая способность изображения (т\д)
       char PaletteType[4]      ;  // Устаревшее поле. Резерв      // 25/10/04

       long int BlockCount      ;  // кол-во блоков в растре
       double   RswFileLength   ;  // теоретическая длина файла *.rsw
       char CompressImage[16]   ;  // сжатие изображения  // 26/01/01
       }
          DATAINFORMATION;


// СТРУКТУРА ДЛЯ ПЕРЕДАЧИ ИНФОРМАЦИИ О GeoTIFF-ФАЙЛАХ
typedef struct GEOTIFFINFORMATION   // 20/03/03
       {
        unsigned long int StructSize;  // длина СТРУКТУРЫ
        unsigned long int TypeCS;      // Тип координатной системы

        unsigned long int Spheroid;    // Geographic CS
        unsigned long int Datum;       // Geographic CS
        unsigned long int PrimeMeridian;//Geographic CS

        unsigned long int Projection;  //
        unsigned long int Zone;        //

        unsigned long int Units;       // Единицы измерения (Метры, градусы ...)


        DFRAME FrameImage;             // Габариты изображения в районе
        double UnitsInElement_X;       // Размер элемента в единицах измерения по оси X
        double UnitsInElement_Y;       // Размер элемента в единицах измерения по оси Y
        double UnitsInElement_Z;       // Размер элемента в единицах измерения по оси Z
        char Information[256]  ;       // Информация о проекции, имени программы и т.д.
        double PrecisionInch;          // разрешаюшая способность изображения (т\д) // 01/11/05
        double PrecisionMet;           // разрешаюшая способность изображения (т\м) // 01/11/05
        char Reserv[248];
       }
          GEOTIFFINFORMATION;

 // СТРУКТУРА ДЛЯ ПЕРЕДАЧИ СООБЩЕНИЙ ОБ ОШИБКАХ ДИАЛОГУ ОТ КОНВЕРТЕРА
typedef struct ERRORINFORMATION
       {
       char * MessageR    ;  // Сообщение об ошибке на русском языке
       char * MessageE    ;  // Сообщение об ошибке на английском языке
       }
           ERRORINFORMATION;

// Тип платформы (0 - INTEL, 1 - MOTOROLA)    // 09/07/03
#define INTEL    0
#define MOTOROLA 1

// Структура изображения TIFF (0 - BLOCK, 1- STRIP, 2 - NONFRAG)
#define BLOCK    0
#define STRIP    1
#define NONFRAG  2


#endif  //PICEXPRM_H
