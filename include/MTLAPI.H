/****** MTLAPI.H *************** Gorbunov A.V.   ****** 09/09/05 ***
******* MTLAPI.H *************** Kruzhkov A.E.   ****** 24/10/00 ***
******* MTLAPI.H *************** Shabakov D.A.   ****** 08/12/03 ***
******* MTLAPI.H *************** Belenkov O.V.   ****** 08/04/01 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2005              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*                  FOR  WINDOWS95 & WINDOWS NT                     *
*                                                                  *
********************************************************************
*                                                                  *
*            ОПИСАНИЕ ФУНКЦИЙ ДОСТУПА К МАТРИЦЕ СЛОЕВ              *
*                                                                  *
*   СТРУКТУРЫ :                                                    *
*                                                                  *
*   ПАРАМЕТРЫ СОЗДАНИЯ МАТРИЦЫ СЛОЕВ (MTLBUILDPARM, BUILDMTL)      *
*   ОПИСАНИЕ ОТКРЫТОЙ МАТРИЦЫ СЛОЕВ (MTLDESCRIBE)                  *
*                                                                  *
*                                                                  *
********************************************************************
*                                                                  *
*                 ИМПОРТИРОВАНИЕ ФУНКЦИЙ :                         *
*                                                                  *
*                                                                  *
*    // Загрузка библиотеки                                        *
*                                                                  *
*    HINSTANCE libInst = ::LoadLibrary("gisacces.dll");            *
*                                                                  *
*    // Вызов функции                                              *
*                                                                  *
*    HMAP (WINAPI * lpfn_OpenMap)(const char*, long int);          *
*    (FARPROC)lpfn_OpenMap = GetProcAddress(libInst, "OpenMap");   *
*    HMAP hMap = (*lpfn_OpenMap)("Noginsk.map",0);                 *
*                                                                  *
*    // Выгрузка библиотеки                                        *
*                                                                  *
*    ::FreeLibrary(libInst);                                       *
*                                                                  *
*******************************************************************/

#ifndef MTLAPI_H
#define MTLAPI_H

#ifndef MAPTYPE_H
  #include "maptype.h"   // Описание структур интерфейса MAPAPI
#endif

#ifndef MAPCREAT_H
  #include "mapcreat.h" 
#endif

//------------------------------------------------------------------
//  ПАРАМЕТРЫ СОЗДАНИЯ МАТРИЦЫ СЛОЕВ
//------------------------------------------------------------------
typedef struct BUILDMTL     // РАЗМЕР СТРУКТУРЫ 160 БАЙТ
{
  unsigned
  long int StructSize;     // Размер данной структуры :
                           //  sizeof (BUILDMTL)
  long int Free;           // Должен быть ноль

  double   BeginX;         // Прямоугольные координаты начала
  double   BeginY;         // (юго-западного угла) матрицы в метрах

  double   EndX;           // Прямоугольные координаты окончания
  double   EndY;           // (северо - восточного угла) матрицы в метрах

  double   ElemSizeMeters; // Размер стороны элементарного участка
                           // в метрах на местности

  long int LayerCount;     // Количество слоев

  long int LayerForm;      // Форма представления инф-ции о слоях
                           // допустимые значения :
                           //  0 - мощности слоев
                           //  1 - абсолютые высоты

  long int HeightSizeBytes; // Размер высоты рельефа в байтах
                            // допустимые значения :
                            //  0 - рельеф отсутствует,
                            //  2 - высоты "short int"
                            //  4 - высоты "long int"

  long int LayerSizeBytes;  // Размер мощности слоя в байтах
                            // допустимые значения :
                            //  1 - мощности "unsigned char"
                            //  2 - мощности "unsigned short int"
                            //  4 - мощности "unsigned long int"

  long int HeightMeasure;  // Eдиница измерения высоты
                           // допустимые значения :
                           //  0 - метры
                           //  1 - дециметры
                           //  2 - сантиметры
                           //  3 - миллиметры

  long int LayerMeasure;   // Eдиница измерения мощности слоя
                           // допустимые значения
                           // - те же,что для HeightMeasure

  long int UserType;       // Произвольное число, связываемое
                           // с создаваемой матрицей

  long int Scale;          // Масштаб

  char     Reserve[80];    // Должны быть нули
}
  BUILDMTL;


//------------------------------------------------------------------
//  ПАРАМЕТРЫ СОЗДАНИЯ МАТРИЦЫ СЛОЕВ (устаревшая структура)
//------------------------------------------------------------------
typedef struct MTLBUILDPARM     // РАЗМЕР СТРУКТУРЫ 128 БАЙТ
{
  unsigned
  long int StructSize;     // Размер данной структуры :
                           //  sizeof (MTRLAYERBUILDPARM)

  double   BeginX;         // Прямоугольные координаты начала
  double   BeginY;         // (юго-западного угла) матрицы в метрах

  double   EndX;           // Прямоугольные координаты окончания
  double   EndY;           // (северо - восточного угла) матрицы в метрах

  double   ElemSizeMeters; // Размер стороны элементарного участка
                           // в метрах на местности

  long int LayerCount;     // Количество слоев

  long int LayerForm;      // Форма представления инф-ции о слоях
                           // допустимые значения :
                           //  0 - мощности слоев
                           //  1 - абсолютые высоты

  long int HeightSizeBytes; // Размер высоты рельефа в байтах
                            // допустимые значения :
                            //  0 - рельеф отсутствует,
                            //  2 - высоты "short int"
                            //  4 - высоты "long int"

  long int LayerSizeBytes;  // Размер мощности слоя в байтах
                            // допустимые значения :
                            //  1 - мощности "unsigned char"
                            //  2 - мощности "unsigned short int"
                            //  4 - мощности "unsigned long int"

  long int HeightMeasure;  // Eдиница измерения высоты
                           // допустимые значения :
                           //  0 - метры
                           //  1 - дециметры
                           //  2 - сантиметры
                           //  3 - миллиметры

  long int LayerMeasure;   // Eдиница измерения мощности слоя
                           // допустимые значения
                           // - те же,что для HeightMeasure

  long int UserType;       // Произвольное число, связываемое
                           // с создаваемой матрицей

  long int Scale;          // Масштаб

  char     Reserve[52];     // Должны быть нули
}
  MTLBUILDPARM;


//------------------------------------------------------------------
//  ОПИСАНИЕ ОТКРЫТОЙ МАТРИЦЫ СЛОЕВ (размер структуры 1024 байта)
//------------------------------------------------------------------
typedef struct MTLDESCRIBE
{
  char  Name[260];              // ИМЯ ФАЙЛА МАТРИЦЫ
  char  MaterialFileName[260];  // ИМЯ ФАЙЛА ОПИСАНИЯ МАТЕРИАЛОВ

  long  LayerCount;      // КОЛ-ВО СЛОЕВ
  long  MaterialCount;   // КОЛ-ВО МАТЕРИАЛОВ

  double ElementInPlane; // РАЗМЕР ЭЛЕМЕНТА В МЕТРАХ НА МЕСТНОСТИ

  DFRAME FrameMeters;    // ГАБАРИТЫ МАТРИЦЫ В ПЛАНЕ (МЕТРЫ)

  double MinHeightValue; // МИНИМАЛЬНОЕ ЗНАЧЕНИЕ ВЫСОТЫ ЭЛЕМЕНТА В МАТРИЦЕ (МЕТРЫ)
  double MaxHeightValue; // МАКСИМАЛЬНОЕ ЗНАЧЕНИЕ ВЫСОТЫ ЭЛЕМЕНТА В МАТРИЦЕ (МЕТРЫ)

  double BotLevelHeight; // ВЫСОТА ЭЛЕМЕНТА МАТРИЦЫ НИЖНЕГО УРОВНЯ (МЕТРЫ)  // 24/10/00

  long   UserType;       // ТИП ПОЛЬЗОВАТЕЛЬСКИХ ДАННЫХ
  long   View;           // ПРИЗНАК ОТОБРАЖАЕМОСТИ
  long   UserLabel;      // ПОЛЬЗОВАТЕЛЬСКИЙ ИДЕНТИФИКАТОР
  long   ReliefPresence; // ПРИЗНАК НАЛИЧИЯ РЕЛЬЕФА  17/01/01

  double MaxSummaryPower; // МАКСИМАЛЬНАЯ СУММАРНАЯ МОЩНОСТЬ
                          // СЛОЕВ  (МЕТРЫ)  // 17/05/01

  char   Reserve[408];   // РЕЗЕРВ

}
  MTLDESCRIBE;

// МОЩНОСТЬ - РАССТОЯНИЕ МЕЖДУ ВЕРХНЕЙ И НИЖНЕЙ ТОЧКАМИ,
//            ОГРАНИЧИВАЮЩИМИ ОБ'ЕМНОЕ ТЕЛО ПО ОСИ,
//            ПЕРПЕНДИКУЛЯРНОЙ ГОРИЗОНТАЛЬНОЙ ПЛОСКОСТИ
//            (БЕЗЗНАКОВОЕ ЦЕЛОЕ ЧИСЛО).

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++ ОПИСАНИЕ ФУНКЦИЙ ДОСТУПА К МАТРИЦАМ СЛОЕВ +++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

extern "C"
{

  // Открыть матричные данные
  // Возвращает идентификатор открытой матричной карты (TMapAccess*)
  // При ошибке возвращает ноль

  HMAP _MAPAPI mapOpenMtl(const char * mtrname,
                          long int mode = 0);

  // Открыть матричные данные в заданном районе работ
  // (добавить в цепочку матриц)
  // Возвращает номер файла в цепочке матриц
  // При ошибке возвращает ноль

  long int _MAPAPI mapOpenMtlForMap(HMAP hMap,
                                    const char * mtrname,
                                    long int mode);

  // Закрыть матричные данные
  // number - номер закрываемой матрицы
  // если number == 0, закрываются все матрицы в окне

  void _MAPAPI mapCloseMtl(HMAP hMap,long int number = 0);

  // Закрыть матричные данные в заданном районе работ
  // number - номер матричного файла в цепочке
  // Если number == 0, закрываются все матричные данные
  // При ошибке возвращает ноль

  long int _MAPAPI mapCloseMtlForMap(HMAP hMap,
                                     long int number);

  // Построение матрицы на заданный участок района работ
  // При ошибке возвращает ноль
  // hMap    - исходная карта для построения матрицы,
  // mtrname - полное имя файла создаваемой матрицы,
  // ininame - полное имя файла легенды создаваемой матрицы,
  // mtrparm - параметры создаваемой матрицы,
  // hselect - идентификатор контекста отбора объектов карты,
  // handle   - идентификатор окна диалога, которому посылаются
  // сообщения о ходе процесса :
  //  0x0378 - сообщение о проценте выполненных работ (в WPARAM),
  //  если процесс должен быть принудительно завершен, в ответ
  //  должно вернуться значение 0x0378.
  // Если handle равно нулю - сообщения не посылаются.

  long int _MAPAPI mapBuildMtlEx(HMAP hMap,
                                 const char * mtrname,
                                 const char * ininame,
                                 BUILDMTL * mtrparm,
                                 HSELECT  hselect,
                                 HWND     handle);

  // Построение матрицы на заданный участок района работ (устаревшая)
  // При ошибке возвращает ноль
  // hMap    - исходная карта для построения матрицы,
  // mtrname - полное имя файла создаваемой матрицы,
  // ininame - полное имя файла легенды создаваемой матрицы,
  // mtrparm - параметры создаваемой матрицы,
  // hselect - идентификатор контекста отбора объектов карты,
  // handle  - идентификатор окна диалога, которому посылаются
  // сообщения о ходе процесса :
  //  0x0378 - сообщение о проценте выполненных работ (в WPARAM),
  //  если процесс должен быть принудительно завершен, в ответ
  //  должно вернуться значение 0x0378.
  // Если handle равно нулю - сообщения не посылаются.

  long int _MAPAPI mapBuildMtl(HMAP hMap,
                               const char * mtrname,
                               const char * ininame,
                               MTLBUILDPARM * mtrparm,
                               HSELECT  hselect,
                               HWND     handle);


  /*********************************************************
  *                                                        *
  *           ЗАПРОС ДАННЫХ ИЗ МАТРИЦЫ СЛОЕВ               *
  *                                                        *
  *********************************************************/

  // Запросить описание файла матричных данных
  // number - номер файла в цепочке
  // describe - адрес структуры, в которой будет размещено
  // описание матрицы
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetMtlDescribe(HMAP hMap,long int number,
                                     MTLDESCRIBE * describe);

  // Запросить имя файла матричных данных
  // number - номер файла в цепочке
  // При ошибке возвращает ноль

  const char * _MAPAPI mapGetMtlName(HMAP hMap,long int number);

  // Запросить число открытых файлов матричных данных
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetMtlCount(HMAP hMap);

  // Запросить максимальное количество слоев всех матриц MTL-цепочки
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetMaxLayerCount(HMAP hMap);

  // Запросить количество слоев матрицы с номером number в цепочке.
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetLayerCountOfMtl(HMAP hMap,long int number);

  // Запросить минимальную высоту нижнего уровня
  // При ошибке возвращает ERRORHEIGHT

  double _MAPAPI mapGetMinBotLevelHeight(HMAP hMap);    // 24/10/00

  // Запросить максимальную суммарную мощность слоев
  // При ошибке возвращает ERRORPOWER

  double _MAPAPI mapGetMaxSummaryPower(HMAP hMap);    // 17/05/01

  // Выбор значения абсолютной высоты в заданной точке.
  // Координаты точки (x,y) задаются в метрах в системе координат
  // векторной карты. Возвращает значение высоты в метрах.
  // В случае ошибки при выборе высоты и в случае необеспеченности
  // заданной точки матричными данными возвращает ERRORHEIGHT.

  double _MAPAPI mapGetElementHeight(HMAP hMap,double x,double y);

  // Выбор значения абсолютной высоты в заданной точке из матрицы
  // с номером number в цепочке.
  // number - номер матрицы в цепочке.
  // Координаты точки (x,y) задаются в метрах в системе координат
  // векторной карты. Возвращает значение высоты в метрах.
  // В случае ошибки при выборе высоты и в случае необеспеченности
  // заданной точки матричными данными возвращает ERRORHEIGHT.

  double _MAPAPI mapGetElementHeightOfMtl(HMAP hMap,long int number,double x,double y);

  // Выбор значения мощности слоя в заданной точке.
  // layernumber - номер слоя.
  // Координаты точки (x,y) задаются в метрах в системе координат
  // векторной карты. Возвращает значение мощности слоя в метрах.
  // В случае ошибки и в случае необеспеченности заданной
  // точки матричными данными возвращает ERRORPOWER.

  double _MAPAPI mapGetElementPower(HMAP hMap,double x,double y,long layernumber);

  // Выбор значения мощности слоя в заданной точке из матрицы
  // с номером number в цепочке.
  // layernumber - номер слоя.
  // Координаты точки (x,y) задаются в метрах в системе координат
  // векторной карты. Возвращает значение мощности слоя в метрах.
  // В случае ошибки и в случае необеспеченности заданной
  // точки матричными данными возвращает ERRORPOWER.

  double _MAPAPI mapGetElementPowerOfMtl(HMAP hMap,long int number,
                                         double x,double y,long layernumber);

  // Вычисление значений мощностей слоев в заданной точке
  // методом треугольников по матрице с номером number в цепочке.
  // Координаты точки задаются в метрах в системе координат
  // векторной карты.
  // powers - адрес массива для записи вычисленных значений
  //          мощностей (в метрах)
  // count - размер массива, должен быть не менее TMtl::LayerCount().
  // Возвращает количество заполненных элементов массива powers.
  // При ошибке возвращает ноль.

  long int _MAPAPI mapGetElementPowersTriangleOfMtl
                      (HMAP hMap,long int number,
                       double x,double y,
                       double* powers,long int count);

  // Вычисление значения мощности слоя layernumber в заданной точке
  // методом треугольников по матрице с номером number в цепочке.
  // Координаты точки задаются в метрах в системе координат
  // векторной карты.
  // Возвращает значение мощности слоя в метрах.
  // При ошибке возвращает ERRORPOWER.

  double _MAPAPI mapGetElementPowerTriangleOfMtl  
                   (HMAP hMap,long int number,
                    double x,double y,long int layernumber);

  // Занесение значения абсолютной высоты в элемент матрицы,
  // соответствующий заданной точке.
  // number - номер матрицы в цепочке.
  // Координаты точки (x,y) и значение высоты (h) задаются в метрах
  // в системе координат векторной карты.
  // В случае ошибки возвращает ноль.

  long int _MAPAPI mapPutElementHeight(HMAP hMap,long int number,
                                       double x, double y, double h);


  // Занесение значения мощности слоя layernumber в элемент,
  // соответствующий заданной точке.
  // number - номер матрицы в цепочке.
  // Координаты точки (x,y) и значение мощности (power) задаются
  // в метрах в системе координат векторной карты.
  // В случае ошибки возвращает 0.

  long int _MAPAPI mapPutElementPower(HMAP hMap,long int number,
                                      double x,double y,double power,
                                      long int layernumber);

  // Запросить номер в цепочке для матрицы, расположенной
  // в заданной точке
  // number - порядковый номер, найденной матрицы в точке
  // (1 - первая в данной точке, 2 - вторая ...)
  // При ошибке возвращается ноль

  long int _MAPAPI mapGetMtlNumberInPoint(HMAP hMap,double x, double y,
                                          long int number);
                                          
  // Запросить номер в цепочке последней открытой матрицы
  // с установленным (равным 1) признаком видимости.
  // При ошибке возвращается ноль

  long int _MAPAPI mapGetMtlNumberLastVisible(HMAP hMap);

  // Запросить размер полного блока матрицы в байтах
  // number - номер файла в цепочке
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetMtlBlockSize(HMAP hMap,long int number);

  // Запросить адрес блока матрицы по номеру строки и столбца
  // Блоки последнего ряда могут иметь усеченный размер
  // number - номер файла в цепочке
  // При запросе следующего блока может вернуть прежний адрес
  // При ошибке возвращает ноль, иначе - адрес считанного блока

  char *  _MAPAPI mapGetMtlBlockAddress(HMAP hMap,long int number,
                                                  long int row,
                                                  long int column);

  // Запросить число строк блоков матрицы
  // number - номер файла в цепочке
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetMtlBlockRow(HMAP hMap,long int number);

  // Запросить число столбцов блоков матрицы
  // number - номер файла в цепочке
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetMtlBlockColumn(HMAP hMap,long int number);

  // Запросить число строк елементов в матрице
  // number - номер файла в цепочке
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetMtlElementRow(HMAP hMap,long int number);

  // Запросить число столбцов элементов в матрице
  // number - номер файла в цепочке
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetMtlElementColumn(HMAP hMap,long int number);

  // Выбор массива значений абсолютных высот, соответствующих
  // логическим элементам, лежащим на заданном отрезке.
  // Координаты точек, задающих начало и конец отрезка
  // (FirstPoint,SecondPoint) задаются в метрах в системе
  // координат векторной карты.
  // Размер массива высот, заданного адресом HeightArray,
  // должен соответствовать запрашиваемому количеству высот
  // (HeightCount), в противном случае возможны ошибки работы
  // с памятью.
  // В случае необеспеченности логического элемента матричными
  // данными его значение равно ERRORHEIGHT (-111111.0 м)
  // В случае ошибки при выборе высот возвращает ноль.

  long int _MAPAPI mapGetHeightArrayFromMtl(HMAP   hMap,
                                            double*   HeightArray,
                                            long int  HeightCount,
                                            DOUBLEPOINT*  FirstPoint,
                                            DOUBLEPOINT*  SecondPoint);


  // Выбор значения цвета слоя layernumber.
  // number - номер матрицы в цепочке
  // В случае ошибки возвращает ноль.

  COLORREF _MAPAPI mapGetLayerColor(HMAP hMap,long int number,long int layernumber);

  // Выбор короткого имени (ключ семантики) слоя layernumber.
  // number - номер матрицы в цепочке
  // В случае ошибки возвращает ноль.

  char* _MAPAPI mapGetLayerShortName(HMAP hMap,long int number,long int layernumber);

  // Выбор имени (название семантики) слоя layernumber.
  // number - номер матрицы в цепочке
  // В случае ошибки возвращает ноль.

  char* _MAPAPI mapGetLayerLongName(HMAP hMap,long int number,long int layernumber);

  // Выбор максимальной мощности слоя layernumber в метрах.
  // number - номер матрицы в цепочке
  // В случае ошибки возвращает ноль.

  long int _MAPAPI mapGetMaxLayerHeight(HMAP hMap,long int number,long int layernumber);

  // Установка максимальной мощности слоя layernumber в метрах.
  // number - номер матрицы в цепочке
  // В случае ошибки возвращает ноль.

  long int _MAPAPI mapSetMaxLayerHeight(HMAP hMap,long int number,long int layernumber,long int maxlayerheight);

  // Установка значения цвета слоя layernumber.
  // number - номер матрицы в цепочке
  // В случае ошибки возвращает ноль.

  long int _MAPAPI mapSetLayerColor(HMAP hMap,long int number,
                                    long int layernumber,
                                    COLORREF layercolor);

  // Установка короткого имени (ключ семантики) слоя layernumber.
  // number - номер матрицы в цепочке
  // В случае ошибки возвращает ноль.

  char* _MAPAPI mapSetLayerShortName(HMAP hMap,long int number,
                                     long int layernumber,
                                     char* layername);

  // Установка имени (название семантики) слоя layernumber.
  // number - номер матрицы в цепочке
  // В случае ошибки возвращает ноль.

  char* _MAPAPI mapSetLayerLongName(HMAP hMap,long int number,
                                    long int layernumber,
                                    char* layername);

  // Запросить данные о проекции матричных данных
  // number - номер файла в цепочке
  // projectiondata - адрес структуры, в которой будут размещены
  // данные о проекции
  // Структурa MTRPROJECTIONDATA описанa в maptype.h
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetMtlProjectionData
                     (HMAP hMap,long int number,
                      MTRPROJECTIONDATA * projectiondata);

  /*********************************************************
  *                                                        *
  *     ОПИСАНИЕ ФУНКЦИЙ ДЛЯ СОЗДАНИЯ МАТРИЦ СЛОЕВ         *
  *                                                        *
  *********************************************************/

  // Создать матричную карту
  // Возвращает идентификатор открытой матричной карты (TMapAccess*)
  // Структурa BUILDMTL описанa в mtlapi.h
  // При ошибке возвращает ноль

  HMAP _MAPAPI mapCreateMtlEx(const char * mtrname,
                              BUILDMTL * mtrparm);

  // Создать файл матрицы
  // hmap -  идентификатор открытой векторной карты
  // mtrname - полное имя файла матрицы
  // Структурa BUILDMTL описанa в mtlapi.h
  // Структурa MTRPROJECTIONDATA описанa в maptype.h
  // Возвращает  номер файла в цепочке матриц
  // При ошибке возвращает ноль

  long int _MAPAPI mapCreateAndAppendMtlEx(HMAP hMap,
                                           const char * mtrname,
                                           BUILDMTL * mtrparm,
                                           MTRPROJECTIONDATA * mtrprojectiondata);

  // Создать матричную карту (устаревшая)
  // Возвращает идентификатор открытой матричной карты (TMapAccess*)
  // Структурa MTLBUILDPARM описанa в mtlapi.h
  // При ошибке возвращает ноль

  HMAP _MAPAPI mapCreateMtl(const char * mtrname,
                            MTLBUILDPARM * mtrparm);

  // Создать файл матрицы (устаревшая)
  // hmap -  идентификатор открытой векторной карты
  // mtrname - полное имя файла матрицы
  // Структурa MTLBUILDPARM описанa в mtlapi.h
  // Структурa MTRPROJECTIONDATA описанa в maptype.h
  // Возвращает  номер файла в цепочке матриц
  // При ошибке возвращает ноль

  long int _MAPAPI mapCreateAndAppendMtl(HMAP hMap,
                                         const char * mtrname,
                                         MTLBUILDPARM * mtrparm,
                                         MTRPROJECTIONDATA * mtrprojectiondata);

                                           
  // Записать изменения матрицы в файл
  // number - номер файла в цепочке
  // При ошибке возвращает ноль

  long int _MAPAPI mapSaveMtl(HMAP hMap,long int number);

  // Установить диапазон отображаемых элементов матрицы
  // number   - номер иатрицы в цепочке
  // minvalue - минимальное значение отображаемого элемента
  //            в единицах матрицы
  // maxvalue - максимальное значение отображаемого элемента
  //            в единицах матрицы
  // При ошибке возвращает 0

  long int _MAPAPI mapSetMtlShowRange(HMAP hMap,
                                      long int number,
                                      double minvalue,
                                      double maxvalue);

  // Установить нижний уровень слоев матрицы
  // number - номер матрицы в цепочке
  // botlevel - нижний уровень слоев в метрах
  // При ошибке возвращает 0

  long int _MAPAPI mapSetMtlBotLevel(HMAP hMap,
                                     long int number,
                                     double botlevel);

  // Установить максимальную суммарную мощность слоев матрицы
  // maxsummarypower - максимальная суммарная мощность в метрах
  // При ошибке возвращает 0

  long int _MAPAPI mapSetMtlMaxSummaryPower(HMAP hMap,
                                            long int number,
                                            double maxsummarypower);

  // Установить данные о проекции матричных данных
  // number - номер файла в цепочке
  // mapregister - адрес структуры, содержащей данные о проекции
  // Структурa MAPREGISTEREX описанa в mapcreat.h
  // При ошибке возвращает ноль

  long int _MAPAPI mapSetMtlProjectionData(HMAP hMap,      // 09/09/05
                                           long int number,
                                           MAPREGISTEREX* mapregister);

                                            
  /*********************************************************
  *                                                        *
  *   ОПИСАНИЕ ФУНКЦИЙ ДЛЯ РАБОТЫ С РАМКОЙ МАТРИЦЫ СЛОЕВ   *
  *                                                        *
  *********************************************************/

  // Установить рамку матрицы по метрике замкнутого объекта
  // Замкнутый объект должен иметь не менее 4-х точек
  // number     - номер файла в цепочке
  // info       - замкнутый объект карты
  // После выполнения функции отображение матрицы ограничится заданной областью
  // При ошибке возвращает 0

  long int _MAPAPI mapSetMtlBorder(HMAP hMap,long int number,HOBJ info);

  // Запросить объект рамки матрицы               // 15/03/01
  // number     - номер файла в цепочке
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetMtlBorder(HMAP hMap,long int number,HOBJ info);

  // Удалить рамку матрицы
  // number     - номер файла в цепочке
  // После выполнения функции отображение матрицы будет полным
  // При ошибке возвращает 0

  long int _MAPAPI mapDeleteMtlBorder(HMAP hMap,long int number);

  // Определение существования рамки матрицы
  // Если рамка матрицы существует возвращает 1, иначе возвращает 0

  long int _MAPAPI mapCheckExistenceMtlBorder(HMAP hMap,long int number);

  // Определение способа отображения матрицы(относительно рамки)    // 02/10/03
  // Возвращает 1 - при отображении матрицы по рамке
  //            0 - при отображении матрицы без учета рамки
  // При ошибке возвращает -1

  long int _MAPAPI mapCheckShowMtlByBorder(HMAP hMap,long int number);

  // Определить координаты и порядковый номер точки рамки, которая // 02/20/03
  // входит в прямоугольник Габариты растра(матрицы) и
  // имеет наименьшее удаление от точки pointIn (координаты в метрах).
  // По адресу pointOut записываются координаты найденной точки в метрах
  // При ошибке или отсутствии рамки возвращает 0.

  long int _MAPAPI mapGetImmediatePointOfMtlBorder(HMAP hMap, long int number,
                                           DOUBLEPOINT* pointIn,
                                           DOUBLEPOINT* pointOut);

  // Запросить фактические габариты отображаемой матрицы в метрах в районе работ   // 14/05/03
  // При отображение матрицы по рамке возвращаются габариты рамки
  // number - номер файла в цепочке
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetActualMtlFrame(HMAP hMap,DFRAME *frame, long int number);

  // Запросить масштаб матрицы
  // number - номер файла в цепочке
  // При ошибке возвращает ноль

  long int _MAPAPI mapGetMtlScale(HMAP hMap,long int number);

  // Запросить значения масштаба нижней и верхней границ видимости матрицы
  // number     - номер файла в цепочке
  // По адресу bottomScale записывается знаменатель масштаба нижней границы видимости матрицы
  // По адресу topScale записывается знаменатель масштаба верхней границы видимости матрицы
  // При ошибке возвращает 0
  long int _MAPAPI mapGetMtlRangeScaleVisible(HMAP hMap,long int number, long int *bottomScale, long int *topScale);  // 08/12/03

  // Установить значения масштаба нижней и верхней границ видимости матрицы
  // number     - номер файла в цепочке
  // bottomScale   - знаменатель масштаба нижней границы видимости матрицы
  // topScale   - знаменатель масштаба верхней границы видимости матрицы
  //              bottomScale <= topScale, иначе возвращает 0
  // При ошибке возвращает 0
  long int _MAPAPI mapSetMtlRangeScaleVisible(HMAP hMap,long int number, long int bottomScale, long int topScale);   // 08/12/03


  /*********************************************************
  *                                                        *
  *          УПРАВЛЕНИЕ ОТОБРАЖЕНИЕМ МАТРИЦЫ               *
  *                                                        *
  *********************************************************/

  // Запросить/Установить степень видимости матрицы
  //  number - номер файла в цепочке
  //  view = 0 - не виден
  //  view = 1 - полная
  //  view = 2 - насыщенная
  //  view = 3 - полупрозрачная
  //  view = 4 - средняя
  //  view = 5 - прозрачная

  long int _MAPAPI mapGetMtlView(HMAP hMap,long int number);
  long int _MAPAPI mapSetMtlView(HMAP hMap,long int number,
                                           long int view);


}

#endif // MTLAPI_H
