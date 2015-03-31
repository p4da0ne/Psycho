/****  PICEXTRS.H    **********  Gustinovich N.A.*****  23/04/03 ***
*****  PICEXTRS.H    **********  Shabakov D.A.   *****  28/09/05 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2005              *
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
#if !defined(PICEXTRS_H)
#define PICEXTRS_H

#ifndef MACPICEX_H
  #include "macpicex.h"
#endif


#define WM_RSTROTATE        0x500       // Поворот растра
#define RSTTRANS_DIAL       0x501       // вызов диалога трансф-я растра
#define RSTTRANS_NEWPARM    0x502       // обновление параметров вых.растра
#define RSTTRANS_END        0x503       // завершение задачи

#ifdef WIN32API                  // 18/06/02
   #define HMESSAGE HWND
#else
   #define HMESSAGE MSGHANDLER
#endif


extern "C"
{
 typedef long int  HTFCR;    // ИДЕНТИФИКАТОР ПРОЦЕССА СОЗДАНИЯ TIFF

 // *********************************************************
 // Схема запуска ПРОЦЕССА СОЗДАНИЯ TIFF:
 // =============
 // HTFCR hTiffCreate = picexCreateTiffFile(handle,tiffname,
 //                               width, height,
 //                               nbits,
 //                               palette, colorcount,
 //                               precision,
 //                               flagCompress,
 //                               platform,
 //                               flagCMYK};
 // if (hTiffCreate)
 //    {
 //     long int stripCount         = picexGetTiffStripCount(hTiffCreate);
 //     long int stringCountByStrip = picexGetTiffstringCountByStrip(hTiffCreate);
 //     for (int i = 0; i < stripCount, i++)
 //        {
 //         if (picexPutTiffStrip(hTiffCreate, numberStrip,
 //                              bits, sizeBits) == 0)
 //            break;
 //        }
 //     picexSaveTiffFile(hTiffCreate);
 //     picexFreeTFCRProcess(hTiffCreate);
 //    }
 // **********************************************************


 //    Сохранить графические данные в файле формата TIFF  (Конструктор)
 //    handle     - диалог визуального сопровождения процесса обработки.
 //    tiffname   - имя TIFF-файла;
 //    width      - ширина изображения в пикселях
 //    height     - высота изображения в пикселях
 //    nbits      - размер пикселя (бит на пиксель)
 //    palette    - адрес устанавливаемой палитры
 //                 если palette == 0, а colorcount != 0, то возможно установить
 //                 палитру позже функцией Load_SetTiffPalette()
 //    colorcount - число элементов в новой палитре
 //    precision  - разрешение изображения (точек на метр)
 //    platform - Тип платформы (0 - INTEL, 1 - MOTOROLA)             (рекомендуемое значение - 0)
 //    compressMethod - Флаг сжатия изображения (0- не применять сжатие, 1 - сжатие PackBit) (рекомендуемое значение - 0)
 //    flagCMYK - выбор цветовой модели:
 //                     0 - цветовая модель RGB 24 бит на пиксел
 //                     1 - цветовая модель CMYK 32 бит на пиксел
 //                     Режим поддерживается только для  растров 24,32 бит на пиксел.
 //    flagIntergraphTIFF - Флаг записи матрицы трансформирования для однобитного TIFF для использования в Intergraph // 28/09/05
 //                     Режим поддерживается только для  растров 1 бит на пиксел.
 //    dframe           - габариты изображения в районе в метрах для записи матрицы трансформирования для однобитного TIFF  // 28/09/05
 //                     Режим поддерживается только для  растров 1 бит на пиксел.
 //    Возвращает ИДЕНТИФИКАТОР ПРОЦЕССА СОЗДАНИЯ TIFF
 //    При ошибке возвращает 0
 //    Диалогу визуального сопровождения процесса обработки посылаются
 //    сообщения:
 //    -  (WM_ERROR) Извещение об ошибке
 //       LPARAM - указатель на структуру ERRORINFORMATION
 //       Структура ERRORINFORMATION описана в picexprm.h, WM_ERROR - в maptype.h
 HTFCR WINAPI _export picexCreateTiffFile(HMESSAGE handle,
                                          const char * tiffname,
                                          long int width, long int height,
                                          long int nbits,
                                          COLORREF* palette, long int colorcount,
                                          double precision,
                                          long int compressMethod,
                                          long int platform,
                                          int flagCMYK);

 HTFCR WINAPI _export picexCreateTiffFileEx(HMESSAGE handle,
                                          const char * tiffname,
                                          long int width, long int height,
                                          long int nbits,
                                          COLORREF* palette, long int colorcount,
                                          double precision,
                                          long int compressMethod,
                                          long int platform,
                                          long int flagCMYK,
                                          long int flagIntergraphTIFF,  // 28/09/05
                                          DFRAME * dframe);             // 28/09/05



 //    Запросить количество стрипов (полос изображения)
 //    При ошибке возвращает 0
 long int WINAPI _export picexGetTiffStripCount(HTFCR hTiffCreate);

 //    Запросить количество строк изображения в стрипе
 //    При ошибке возвращает 0
 long int WINAPI _export picexGetTiffStringCountByStrip(HTFCR hTiffCreate);

 //    Записать стрип с номером numberStrip в файл
 //    Все стрипы имеют одинаковый размер, за исключением последнего
 //    Нумерация и запись стрипов сверху-вниз
 //    Если flagCompress == 1, стрип записывается сжатым по методу PackBit
 //    При ошибке возвращает 0
 long int WINAPI _export picexPutTiffStrip(HTFCR hTiffCreate, long int numberStrip,
                                           char*bits, long int sizeBits);

 //    Записать стрип с номером numberStrip в файл   // 06/02/04
 //    Цветовая модель изображения - CMYK(flagCMYK = 1),  32 бит на пиксел (nbits = 32)
 //    Цвета хранятся последовательно(не по плоскостям).
 //    Все стрипы имеют одинаковый размер, за исключением последнего
 //    Нумерация и запись стрипов сверху-вниз
 //    Если flagCompress == 1, стрип записывается сжатым по методу PackBit
 //    numberStrip - начиная с нуля
 //    При ошибке возвращает 0
 long int WINAPI _export picexPutTiffStrip_CMYK(HTFCR hTiffCreate, long int numberStrip,
                                                 char*bitsCMYK, long int sizeBitsCMYK);

 // Записать палитру в файл
 // Для изображений с 4 и 8 бит на пиксел
 //    При ошибке возвращает 0
 long int WINAPI _export picexSetTiffPalette(HTFCR hTiffCreate, COLORREF* palette,
                                                             long int colorCount);

 // Записать заголовок и измененные тэги в файл
 // При ошибке возвращает 0
 long int WINAPI _export picexSaveTiffFile(HTFCR hTiffCreate);


 // Освобождение памяти (Деструктор)
 // При ошибке возвращает 0
 long int WINAPI _export picexFreeTFCRProcess(HTFCR hTiffCreate);

 // Определение размеров изображения с учетом рамки растра  // 10/07/03
 // rect - по адресу rect записываются габариты рамки растра в элементах растра
 // При ошибке возвращает ноль
 long int WINAPI _export picexSizeImageDefinition(HMAP hMap, long int rstNumber, RECT* rect);

 // Вывести состав Тэгов TIFF-файла в файл *.tls   // 25/05/05
 //    Map - карта,содержащая векторные данные;
 //    TifName - имя TIF-файла;
 //    FileName - имя файла *.tls;
 long int WINAPI _export picexSaveTiffTagsIntoFile(HMESSAGE Handle, char * TiffName, char * FileName);



//========================================================================
//    Трансформирование растра
// (вычисление коэффициентов пересчета координат методом наименьших квадратов)
//
//    map        - карта,содержащая векторные данные;
//    RstParm    - параметры трансформирования растра
//    handle     - диалог визуального сопровождения процесса обработки.
//========================================================================
/*long int WINAPI _export RstTransform(HMAP map,
                        RSTPARM * RstParm,
                        HMESSAGE handle);


//========================================================================
//    Трансформирование растра по рамке с точками прогиба
//                 (получение одного растра)
// (вычисление коэффициентов пересчета координат методом наименьших квадратов)
//
//    map        - карта,содержащая векторные данные;
//    RstParm    - параметры трансформирования растра
//    handle     - диалог визуального сопровождения процесса обработки.
//========================================================================
long int WINAPI _export RstSomeTransform(HMAP map,
                            RSTPARM * RstParm,
                            HMESSAGE handle);

//========================================================================
//    Трансформирование растра
// (вычисление коэффициентов пересчета координат по методу Делоне)
//
//    map        - карта,содержащая векторные данные;
//    RstParm    - параметры трансформирования растра
//    handle     - диалог визуального сопровождения процесса обработки.
//========================================================================
long int WINAPI _export RstTriangle(HMAP map,
                            RSTPARM * RstParm,
                            HMESSAGE handle);
//========================================================================
//    Трансформирование растра по элементам внешнего ориентирования
//
//    map        - карта,содержащая векторные данные;
//    RstParm    - параметры трансформирования растра
//    handle     - диалог визуального сопровождения процесса обработки.
//========================================================================
long int WINAPI _export RstTransEvo(HMAP map,
                       RSTPARM * RstParm,
                       HMESSAGE handle);
*/

}       // extern "C"


#endif  // PICEXTRS_H
