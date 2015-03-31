/****** MAPGDI.H *************** Kruzhkov A.E.   ****** 10/05/07 ***
******* MAPGDI.H *************** Belenkov O.V.   ****** 21/03/08 ***
******* MAPGDI.H *************** Panteleeva N.A. ****** 06/03/08 ***
******* MAPGDI.H *************** Dishlenko S.G.  ****** 23/04/01 ***
******* MAPGDI.H *************** Kruzhkova E.A.  ****** 19/03/08 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2008              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*      ОПИСАНИЕ ПАРАМЕТРОВ ФУНКЦИЙ ВИЗУАЛИЗАЦИИ ПРОИЗВОЛЬНЫХ       *
*             ГРАФИЧЕСКИХ ОБ'ЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ               *
*                                                                  *
*******************************************************************/

#ifndef MAPGDI_H
#define MAPGDI_H

#ifndef MAPTYPE_H
 #include "maptype.h"
#endif


// Опции масштабируемости объекта (игнорируются при Image равном IMG_OBJECT)

#define IM_PRESS       0x00000000  // Сжимать при уменьшении карты (по умолчанию)
#define IM_DONTPRESS   0x04000000  // Не сжимать при уменьшении

#define IM_SCALE       0x01000000  // Масштабировать
#define IM_DONTSCALE   0x02000000  // Не масштабировать

#define IM_SCALEDONTPRESS  (IM_SCALE | IM_DONTPRESS)  // Только увеличивать

#define IM_CLEARSCALE 0x0F8FFFFFF  // Убрать флаги масштабирования


//-----------------------------------------------------------------
// ОПИСАНИЕ СПОСОБА ИЗОБРАЖЕНИЯ ГРАФИЧЕСКОГО ОБ'ЕКТА
//-----------------------------------------------------------------

typedef struct PAINTPARM
{
  long int Image;  // Вид функции отображения

  char *   Parm ;  // Параметры отображения для соответствующей функции.
                   // Игнорируется при Image равном IMG_OBJECT (параметры
                   // и номер функции устанавливается из классификатора)

  long int Mode ;  // Условия отображения и масштабирования
                   //   R2_COPYPEN   - отображение цветом
                   //   R2_NOT       - отображение инверсией фона (цвет игнорируется)

                   //   IM_SCALE, IM_DONTSCALE, ... - признаки масштабируемости
                   //    (игнорируются при Image равном IMG_OBJECT)

                   // Пример использования: Mode = R2_NOT | IM_SCALE
}
  PAINTPARM;


//-----------------------------------------------------------------
// ОПИСАНИЕ ВЫВОДИМОГО ТЕКСТА
//-----------------------------------------------------------------
// РЕАЛЬНАЯ ДЛИНА ТЕКСТА МОЖЕТ БЫТЬ МЕНЬШЕ ДЛИНЫ ЗАПИСИ.
// МИНИМАЛЬНАЯ ДЛИНА ЗАПИСИ - 5 байт: 4 байта - длина = 5 и 1 байт = 0
// ЕСЛИ В ТЕКСТЕ ВСТРЕЧАЕТСЯ '\n',
// ТО ВЫПОЛНЯЕТСЯ ПЕРЕХОД НА СЛЕДУЮЩУЮ СТРОКУ
//-----------------------------------------------------------------

typedef struct TEXTRECORD
{
  long Length ;                  // Длина записи, включая поле Length
  char Text[256];                // Текст, заканчивающийся двоичным 0
}                                // Текст всегда в кодировке WINDOWS
  TEXTRECORD;


//-----------------------------------------------------------------
// ОПИСАНИЕ КООРДИНАТ ГРАФИЧЕСКОГО ОБ'ЕКТА
//-----------------------------------------------------------------
// ЕДИНИЦА ИЗМЕРЕНИЯ И ВИД СИСТЕМЫ КООРДИНАТ
// ЗАДАЕТСЯ В ПАРАМЕТРАХ ФУНКЦИИ ВИЗУАЛИЗАЦИИ
//-----------------------------------------------------------------

typedef struct PLACEDATA         // КООРДИНАТЫ В ПРОИЗВОЛЬНОЙ СИСТЕМЕ
{
  DOUBLEPOINT * Points    ;      // Списки координат отдельных контуров
  long int *    PolyCounts;      // Списки количества точек контуров
  long int      Count     ;      // Количество контуров или структур TEXTRECORD
                                 // Для функций типа IMG_TEXT :
  TEXTRECORD *  PolyText  ;      // Область последовательно расположенных
                                 // записей типа TEXTRECORD, размеры записей
                                 // могут быть разными, но не меньше 5 байт,
                                 // число структур равно Count !
}
  PLACEDATA;


//-----------------------------------------------------------------
// ПАРАМЕТРЫ ОТОБРАЖЕНИЯ ОБРАЗЦА ВИДА ОБЪЕКТА
//-----------------------------------------------------------------

typedef struct PAINTEXAMPLE
{
  HDC       Hdc;       // Контекст устройства
  RECT*     Rect;      // Область отображения в пикселах

  long int  Func;      // Номер функции (IMG_LINE, IMG_DOT, ...)
  char*     Parm;      // Указатель на параметры функции

  POLYDATAEX* Data;    // Метрика
                       //   (если Data != 0, то text игнорируется)
                       //   (если Data == 0, то создается автоматически)

  char*     Text;      // Текст всегда в кодировке WINDOWS
                       //   (если Text == 0, то создается автоматически)

  long int  Local;     // Локализация (LOCAL_LINE, LOCAL_SQUARE, ...)
                       //   (используется при Data == 0)

  float     Factor;    // Коэффициент увеличения (Factor >= 0.0)
                       //  0.0 - рассчитывается автоматически

  long int  Colors;    // Число цветов палитры
  COLORREF* Palette;   // Указатель на палитру

  long int  VisualType;     // Тип визуализации (VT_SCREEN, VT_PRINT, ...)
                            //    0 - текущий

  long int  FillIntensity;  // Интенсивность заливки (0,100)
                            //    0 - текущая
                            //  100 - в реальных цветах
  char      Reserve[16];
}
  PAINTEXAMPLE;


//******************************************************************
// ВИДЫ ФУНКЦИЙ ВИЗУАЛИЗАЦИИ ПРОИЗВОЛЬНЫХ ГРАФИЧЕСКИХ ОБ'ЕКТОВ НА ЭК
//******************************************************************

#define IMG_EMPTY             127  // Пустая функция - нет параметров

#define IMG_LINE              128
#define IMG_DOT               129

#define IMG_SQUARE            135

#define IMG_CIRCLE            140

#define IMG_TEXT              142

#define IMG_MULTIMARK         143
#define IMG_MULTISQUAREMARK   144

#define IMG_DRAW              147

#define IMG_DOTSHIFT          148

#define IMG_VECTOREX          149

#define IMG_TEMPLATE          150

#define IMG_TRUETYPE          151

#define IMG_TRUETEXT          152

#define IMG_HATCHSQUARE       153

#define IMG_SQUAREGLASS       154

#define IMG_SQUAREVECTOR      155
#define IMG_VECTOREXTURN      156

#define IMG_DECORATE          157
#define IMG_DASH              158
#define IMG_LINESHIFT         159
#define IMG_VECTORTEXT        160
#define IMG_VECTORNODE        161       // Векторные в узлах (IMGVECTOREX)
#define IMG_THICKENLINE       162       // Утолщающаяся линия
#define IMG_MIDDLETHICKENLINE 163       // Утолщающаяся линия к середине от концов
#define IMG_SQUAREGLASSCOLOR  164
#define IMG_GRAPHICMARK       165       // Знак из файла (ВМР, Metafile )
#define IMG_NODE              167       // Знак по точкам

#define IMG_VECTORGRADIENT    199       // Служебная функция

#define IMG_PERCENT           249       // Структура типа IMG_LONG
#define IMG_LIBRARY           250

#define IMG_SEMANTIC          251       // Структура типа IMG_LONG
#define IMG_COLOR             252       // Структура типа IMG_LONG
#define IMG_DOUBLE            253
#define IMG_LONG              254

#define IMG_OBJECT            255       // Номер функции устанавливается
                                        // из классификатора

// Прозрачный цвет (для IMG_SQUAREMARK, IMG_MULTISQUAREMARK,
//                      IMG_SQUARECROSS, IMG_FONT, IMG_SQUAREGLASSCOLOR)
#define IMGC_TRANSPARENT  0x0FFFFFFFFL
#define IMGC_NOMARK       0x0FFFFFFFFL  // Не выделять (для IMG_OBJECT)

// Флаги для определения формата цвета (индекс|IMGC_INDEX, RGB)
#define IMGC_INDEX        0x0F0000000L  // Флаг цвета из палитры карты
#define IMGC_CLEARINDEX   0x0FF000000L  // Для очистки флага цвета (& IMGC_CLEARINDEX)
#define IMGC_CLEARCOLOR   0x000FFFFFFL  // Флаг очистки цвета      (& IMGC_CLEARCOLOR)

// Установить формат цвета 
// color - номер цвета в палитре RSC, начиная с 0 !!!
// Если флаг IMGC_INDEX не установлен, цвет в формате RGB
#define SETCOLORINDEX(color)   (((long int)(color))|IMGC_INDEX)

#define IMGC_SYSTEM       0x0F1000000L  // Флаг цвета из системной палитры (LINUX)  // 16/10/03
#define SETCOLORSYSTEM(color)  (((long int)(color))|IMGC_SYSTEM)

// Определить формат цвета (возвращает: 0 - RGB, 1 - индекс из палитры)
#define TESTCOLORINDEX(color)  ((((long int)(color))&IMGC_CLEARINDEX)==IMGC_INDEX)

// Определить формат цвета (возвращает: 0 - RGB, 1 - индекс из системной палитры)
#define TESTCOLORSYSTEM(color) ((((long int)(color))&IMGC_CLEARINDEX)==IMGC_SYSTEM)  // 16/10/03

// Тип сетки заполнения 4x4 (16 бит) (для IMG_SQUAREMARK, IMG_MULTISQUAREMARK)
//  1 - заполняемые элементы сетки
//  0 - незаполняемые
// Основные типоы сеток:
//  0000(0)  1111(F)  1010(A)  0101(5)
//  0000(0)  1111(F)  0101(5)  1010(A)
//  0000(0)  1111(F)  1010(A)  0101(5)
//  0000(0)  1111(F)  0101(5)  1010(A)
#define IMGK_REGULAR0     0x00000000L  // Регулярная (заполнить все)
#define IMGK_REGULAR      0x0000FFFFL  // Регулярная (заполнить все)
#define IMGK_ONECHESS     0x0000A5A5L  // Шахматная
#define IMGK_TWOCHESS     0x00005A5AL  // Шахматная смещенная

// Локализация объекта (для IMG_OBJECT)
#define IMGL_LINE      0  // Линейный
#define IMGL_SQUARE    1  // Площадной
#define IMGL_POINT     2  // Точечный
#define IMGL_TITLE     3  // Подпись
#define IMGL_VECTOR    4  // Векторный
#define IMGL_MIXED     5  // Сложная подпись (шаблон)

// Тип штpиховки (для IMG_SQUARECROSS)
#define SC_HORIZONTAL  1  // Гоpизонтальными линиями
#define SC_VERTICAL    2  // Веpтикaльными линиями
#define SC_QUADRATIC   3  // Кубикaми
#define SC_CROSS45     4  // Под углом 45 градусов
#define SC_CROSS135    5  // Под углом 135 градусов

// Типы векторных цепочек (для SCR_VECTOR)
#define VT_THICK2      2  // Линия толщины 2
#define VT_THICK3      3  // Линия толщины 3
#define VT_THICK4      4  // Линия толщины 4
#define VT_THICK5      5  // Линия толщины 5
#define VT_CIRCLE2     8  // Окружность толщины 2
#define VT_CIRCLE3     9  // Окружность толщины 3
#define VT_CIRCLE4    10  // Окружность толщины 4
#define VT_CIRCLE5    11  // Окружность толщины 5

// Типы векторных цепочек (IMG_VECTOREX)
#define VT_SQUARE      0  // Площадь
#define VT_LINE        1  // Линия
#define VT_ROUND       6  // Круг (площадь)
#define VT_CIRCLE      7  // Окружность
#define VT_FREE       12  // Цепочка не определена

// Типы векторных цепочек (IMG_VECTOREX)
#define VT_ELLIPSE    64  // Эллипс
#define VT_OVAL       65  // Эллипс (площадь)
#define VT_ARC        66  // Цепочка типа дуга
#define VT_PIE        67  // Сектор (площадь)
#define VT_TEXT       68  // Цепочка типа текст (постоянный)
#define VT_STEXT      69  // Цепочка типа текст (из семантики)
#define VT_ARC_REVERT 70  // Цепочка типа дуга

// Типы служебных цепочек для описания структуры блока
#define VT_OBJECT    101  // Начало блока
#define VT_SUBJECT   102  // Начало подобъекта
#define VT_END       103  // Конец блока

// Операции пересчета пикселов в микроны и обратно
#define MKMINPIX     250  //  1000 * 25.4 / 96 = 264.583333
#define PIX2MKM(pixel)  (((long int)(pixel))  * MKMINPIX)
#define MKM2PIX(metric) (((long int)(metric)) / MKMINPIX)

//------------------------------------------------------------------
// Для IMG_FONT
//------------------------------------------------------------------

// Прозрачный цвет (фона или тени)
#define UNIC_TRANSPARENT  255

// Относительная ширина символа
#define UNIW_NORMAL       0  // Нормальный
#define UNIW_NARROW       1  // Суженый
#define UNIW_WIDE         2  // Широкий

// Выравнивание по вертикали
#define UNIA_BASELINE     0  // По базовой линии
#define UNIA_TOP          1  // По верху
#define UNIA_BOTTOM       2  // По низу

// Выравнивание по горизонтали
#define UNIA_LEFT         0  // По левому краю
#define UNIA_CENTER       1  // По центру
#define UNIA_RIGHT        2  // По правому краю

// Минимально допустимая высота символа в пикселах
#define CharMin  5  // Подписи высотой < CharMin, не отображаются
                    // 5 - 25/10/99

// Относительная ширина символа (mapgds.h)
//  UNIW_NORMAL     - Нормальный
//  UNIW_NARROW     - Суженый
//  UNIW_WIDE       - Широкий

#ifdef LINUXAPI  // 23/04/01
// Вес шрифта, толщина контура (Windows-константы)
//  FW_THIN         - Тонкий
//  FW_ULTRALIGTH   - Четкий
//  FW_NORMAL       - Средний
//  FW_MEDIUM       - Утолщенный
//  FW_BOLD         - Толстый
#define FW_DONTCARE         0
#define FW_THIN             100
#define FW_EXTRALIGHT       200
#define FW_LIGHT            300
#define FW_NORMAL           400
#define FW_MEDIUM           500
#define FW_SEMIBOLD         600
#define FW_BOLD             700
#define FW_EXTRABOLD        800
#define FW_HEAVY            900

#define FW_ULTRALIGHT       FW_EXTRALIGHT
#define FW_REGULAR          FW_NORMAL
#define FW_DEMIBOLD         FW_SEMIBOLD
#define FW_ULTRABOLD        FW_EXTRABOLD
#define FW_BLACK            FW_HEAVY

#endif // LINUXAPI


// Выравнивание по горизонтали (Windows-константы)
#define FA_LEFT      0     // Слева = TA_LEFT       // 14/09/00
#define FA_RIGHT     2     // Справа = TA_RIGHT     // 14/09/00
#define FA_CENTER    6     // По центру = TA_CENTER // 14/09/00

// Выравнивание по вертикали (Windows-константы)
#define FA_TOP       0     // Сверху = TA_TOP       // 14/09/00
#define FA_BOTTOM    8     // Снизу = TA_BOTTOM     // 14/09/00
#define FA_BASELINE  24    // По базовой линии = TA_BASELINE  // 14/09/00
#define FA_MIDDLE    4120  // По средней линии = (TA_BASELINE|0x01000) // 14/09/00

// Маска выборки выравнивания
#define FA_MASK     (FA_CENTER | FA_MIDDLE)         // 23/04/03


// Коды выравнивания текста (внутренние, в метрике подписи)
#define CA_BASELINE_LEFT          20
#define CA_BASELINE_RIGHT         21
#define CA_BASELINE_CENTER        22

#define CA_MIDDLE_LEFT            23
#define CA_MIDDLE_RIGHT           24
#define CA_MIDDLE_CENTER          25

#define CA_TOP_LEFT               26
#define CA_TOP_RIGHT              27
#define CA_TOP_CENTER             28

#define CA_BOTTOM_LEFT            29
#define CA_BOTTOM_RIGHT           30
#define CA_BOTTOM_CENTER          31

// Диапазон кодов выравнивания текста
#define CA_MINCODE  CA_BASELINE_LEFT
#define CA_MAXCODE  CA_BOTTOM_CENTER

// Кодовая страница (Windows-константы)
//  RUSSIAN_CHARSET - для России (ANSI/WINDOWS)
//  OEM_CHARSET     - для России (OEM/866/DOS)
//------------------------------------------------------------------

//******************************************************************
//               СТРУКТУРЫ ПАРАМЕТРОВ ВИЗУАЛИЗАЦИИ                 *
//******************************************************************
//      ВСЕ РАЗМЕРЫ (ТОЛЩИНА, ДЛИНА, РАССТОЯНИЕ ...) В МИКРОНАХ    *
//                 (В ТЫСЯЧНЫХ ДОЛЯХ МИЛЛИМЕТРА)                   *
//******************************************************************
// Примечания:                                                     *
//  1. Цвет в формате COLORREF (0x00BBGGRR) или номер индекса      *
//     палитры карты в младшем байте, если старший байт равен 0x0F *
//  2. Для описания размеров в точках экрана (пикселах)            *
//     использовать операцию PIX2MKM(число пикселов):              *
//     Parm->Thick = PIX2MKM(2);  (толщина 2 пиксела)              *
//******************************************************************

typedef struct IMGLINE           // (128) ЛИНИЯ
{                                //      (size = 8)
  unsigned long Color;           // Цвет линии (COLORREF)
  unsigned long Thick;           // Толщина линии
}
  IMGLINE;

typedef struct IMGTHICKENLINE    // (162,163) УТОЛЩАЮЩАЯСЯ ЛИНИЯ
{                                //      (size = 12)
  unsigned long Color;           // Цвет линии (COLORREF)
  unsigned long Thick;           // Толщина минимальная
  unsigned long Thick2;          // Толщина максимальная (утолщения)
}
  IMGTHICKENLINE;

typedef struct IMGDOT            // (129) ПУНКТИРНАЯ ЛИНИЯ
{                                //      (size = 16)
  unsigned long Color;           // Цвет линии
  unsigned long Thick;           // Толщинa штpиха
  unsigned long Dash ;           // Длина штpиха
  unsigned long Blank;           // Длина пpобела
}
  IMGDOT;

typedef struct IMGSQUARE         // (135) ПЛОЩАДНОЙ ОБ'ЕКТ
{                                //      (size = 4)
  unsigned long Color;           // Цвет площади
}
  IMGSQUARE;



typedef struct IMGCIRCLE         // (140) ОКРУЖНОСТЬ
{                                //      (size = 12)
  unsigned long Color ;          // Цвет окружности
  unsigned long Thick ;          // Толщина линии
  unsigned long Radius;          // Радиус окружности
}
  IMGCIRCLE;



typedef struct IMGNODE           // (167) ЗНАК ПО ТОЧКАМ (ПОКА ОКРУЖНОСТЬ)
{                                //      (size = 28)
  unsigned long Color1;          // Цвет заполнения (IMGC_TRANSPARENT - прозрачный)
  unsigned long Color2;          // Цвет контура
  unsigned long Thick;           // Толщина линии
  unsigned long Radius1;         // Радиус описанной окружности
  unsigned long Radius2;         // Радиус вписанной окружности,
                                 // для выпуклых равноугольных фигур равен Radius1
  unsigned long VertexCount;     // Число внешних вершин (0 - окружность)
  unsigned long Angle;           // Начальный угол поворота знака относительно
                                 // вертикальной оси, по часовой стрелке, в градусах
}
  IMGNODE;


typedef struct IMGTEXT           // (142) ШРИФТ                   // 30/01/01
{                                //      (size = 32)
  unsigned long  Color       ;   // Цвет текста
  unsigned long  BkgndColor  ;   // Цвет фона (IMGC_TRANSPARENT - прозрачный)
  unsigned long  ShadowColor ;   // Цвет тени (IMGC_TRANSPARENT - прозрачный)
  unsigned long  Height      ;   // Высота (При растягивании по метрике
                                 // по умолчанию 1800 мкм)
  unsigned short Weight      ;   // Вес шрифта, толщина контура (FW_THIN, ...)
  unsigned char  Outline     ;   // Признак вывода тени в виде контура
  unsigned char  Reserve     ;   // Резерв
  unsigned short Align       ;   // Выравнивание (FA_LEFT|FA_BASELINE, ...)
  unsigned short Service     ;   // Равно 0
  unsigned char  Wide        ;   // Относительная ширина символа (UNIW_NORMAL, ...)
  unsigned char  Horizontal  ;   // Признак горизонтального расположения (0/1)
  unsigned char  Italic      ;   // Признак наклона символов (0/1)
  unsigned char  Underline   ;   // Признак подчеркивания    (0/1)
  unsigned char  StrikeOut   ;   // Признак перечеркивания   (0/1)
  unsigned char  Type        ;   // Тип шрифта (номер в таблице: 0,1,2,3,4)
  unsigned char  CharSet     ;   // Кодовая страница (RUSSIAN_CHARSET, ...)
  unsigned char  Flag        ;   // Флаги для текста :
                                 //  1 - Признак растягивания по метрике
}
  IMGTEXT;

#define IMGTEXT_SCALE  1
#define IMGTEXT_SHOWCODE 2                       // 21/07/06

typedef IMGTEXT IMGFONT;


typedef struct IMGMARKCHAIN      // ТОЧЕЧНЫЙ РАСТРОВЫЙ ОБ'ЕКТ ЦЕПОЧКИ
{                                //      (size = 4 + 128 = 132)
  unsigned long Color    ;       // Цвет знака
  unsigned char Bits[128];       // Растровое изображение 32x32 с верхнего угла
}
  IMGMARKCHAIN;


typedef struct IMGMULTIMARK      // (143) МНОГОЦВЕТНЫЙ ТОЧЕЧНЫЙ
{                                //      (size = 20 + 132*Count)
  unsigned long Length;          // Полная длина записи описания объекта
  unsigned long Count ;          // Число цветов в знаке
  unsigned long Size  ;          // Размер в микронах (для печати)
  unsigned long PosV  ;          // Точка привязки знака PosV,PosH  <  Size
  unsigned long PosH  ;          //  (для IMGMULTISQUAREMARK - шаг сетки >= Size)
                                 // затем: Count структур типа IMGMARKCHAIN
}
  IMGMULTIMARK;


typedef struct IMGMULTISQUAREMARK// (144) ПЛОЩАДНОЙ + МНОГОЦВЕТНЫЙ ТОЧЕЧНЫЙ
{                                //      (size = 4 + 20+132*Count )
  unsigned short Kind    ;       // Тип сетки заполнения 4x4 (16 бит)
                                 //  (IMGK_REGULAR, ...)
  unsigned char  FullMark;       // Флаг заполнения
                                 //  0 - по всей площади
                                 //  1 - только полными знаками
  unsigned char  Weight  ;       // Вес знака, толщина контура   // 27/04/00
                                 //  0 - тонкий
                                 //  1 - нормальный
  IMGMULTIMARK   Mark    ;       // (143) МНОГОЦВЕТНЫЙ ТОЧЕЧНЫЙ
                                 // затем: Count структур типа IMGMARKCHAIN
}
  IMGMULTISQUAREMARK;


typedef struct IMGDECOR          // ЗАГОЛОВОК ЧАСТИ ПАРАМЕТРОВ
{
  unsigned short Length;         // Длина параметров >= 4
                                 // (включая sizeof(IMGDECOR))
  unsigned short Number;         // Номер функции
}
  IMGDECOR;

// Флаги для набора примитивов   // 28/07/00
#define IMGD_SPLINE       0x0001 // Флаг рисования по сглаженной метрике

typedef struct IMGDRAW           // (147) НАБОР ПРИМИТИВОВ
{
  unsigned long Ident   ;        // Идентификатор записи 0x7FFF7FFE
  unsigned long Length  ;        // Полная длина записи описания объекта
  unsigned short Count  ;        // Количество примитивов
  unsigned short Flags  ;        // Должен быть 0
  IMGDECOR      Image   ;        // Заголовок первого примитива
  unsigned long Parm[1] ;        // Параметры примитива по типу
                                 // Далее заголовок и параметры
                                 // следующих примитивов
} IMGDRAW;

typedef IMGDRAW  SCRDRAW ;

typedef struct IMGDOTSHIFT       // (148) ПУНКТИР СМЕЩЕННЫЙ
{                                //
  IMGDOT    Dot   ;              // Пунктир
  long      Shift ;              // Направление и величина сдвига
                                 // 0 - пунктир лежит справа вплотную к оси
                                 // > 0 сдвигаемся вправо от оси,иначе влево
                                 // при сдвиге = - ширина пунктир лежит слева
                                 // вплотную к  оси
}
 IMGDOTSHIFT;

typedef IMGDOTSHIFT SCRDOTSHIFT;

typedef struct IMGVECTPOINT      // Для (149) описание точки объекта
{                                //      (size = 8)
  long Hor;                      // Координаты от точки привязки
  long Ver;                      // вправо и вниз в соответствии
                                 // с типом цепочки:
                                 //  Центр  Радиусы  Точки начала и окончания
                                 // Circle, Round  (h,v)  (dh,dh)
                                 // Ellipse, Oval  (h,v)  (dh,dv)
                                 // Arc,      Pie  (h,v)  (dh,dv) (h1,v1)(h2,v2)
}
  IMGVECTPOINT;


typedef struct IMGPOLYDESC       // Для (149) описание цепочки
{                                //      (size = 12 + 8*Count + Length)
  unsigned char  Type    ;       // Тип цепочки (VT_SQUARE, ...)
                                 // для выделения блока VT_OBJECT,
                                 // VT_SUBJECT, VT_END)
  unsigned char  Image   ;       // Тип параметров
                                 // (IMG_SQUARE, IMG_LINE, IMG_DOT,
                                 // IMG_VECTORTEXT)                 // 11/04/05
  unsigned short Length  ;       // Длина параметров (>= 4)
  unsigned long  Parm    ;       // Параметры цепочки по ее типу
  unsigned long  Count   ;       // Число точек в цепочке
  IMGVECTPOINT   Point[1];       // Координаты до 512 точек
}
  IMGPOLYDESC;

typedef struct IMGVECTOREX       // (149,156) ПАРАМЕТРЫ ВЕКТОРНОГО ОБ'ЕКТА
{                                //      (size = 20 + ...)
  long Length      ;             // Полная длина записи описания объекта
  long PosV        ;             // Точка привязки знака по вертикали
  long PosH        ;             //  и горизонтали (положение первой точки)
  long Base        ;             // Длина базовой осевой линии знака
  long VLine1      ;             // Начало отметки по вертикали  (0)
  long VLine2      ;             // Конец  отметки по вертикали  (VSize)
  long VSize       ;             // Требуемый размер отметки по вертикали
  long HLine1      ;             // Начало отметки по горизонтали (0)
  long HLine2      ;             // Конец отметки по горизонтали  (HSize)
  long HSize       ;             // Требуемый размер отметки по горизонтали
  char Static      ;             // 1 - горизонтальное расположение
                                 // 0 - по метрике
  char Mirror      ;             // Флаг зеркального отображения знака
                                 // в направлении второй точки метрики
  char NoPress     ;             // Признак запрета сжатия знака
  char Scale       ;             // Признак растягивания по метрике
  char Centre      ;             // Признак центрирования по метрике
  char Reserve[3]  ;             // Резерв
  long Border      ;             // Габариты знака от отсчета в мкм или 0
  long Count       ;             // Число фрагментов,описывающих знак
  IMGPOLYDESC  Desc;             // Описание фрагмента
}
  IMGVECTOREX;                   // то же для IMG_VECTOREXTURN (156)

typedef struct TABLETEMPLATE     // Постоянная часть векторного шаблона
{
  long Length  ;                 // длина структуры
  int Order[12];                 // Клетки с семантикой
                                 // 0-пустая клетка
                                 // -1 линия  -2 знак
                                 // >0  номер семантики подписи  (текст)
  long Origin  ;                 // Номер клетки привязки
  long Static  ;                 // 1 - горизонтальное расположение
                                 // 0 - по метрике
}
  TABLETEMPLATE;

typedef struct IMGTEMPLATE       // (150) ПАРАМЕТРЫ ВЕКТОРНОГО ШАБЛОНА
{                                //      (size = 20 + ...)
  long Length               ;    // Полная длина записи описания объекта
  TABLETEMPLATE Table       ;    // Постоянная часть векторного шаблона
  long          Count       ;    // Количество примитивов
  IMGDECOR      Text        ;    // Заголовок текста
  unsigned long Parmtext[1] ;    // Параметры текста
  IMGDECOR      Line        ;    // Заголовок линии
  unsigned long Parmline[1] ;    // Параметры линии
  IMGDECOR      Mark        ;    // Заголовок знака
  unsigned long Parmmark[1] ;    // Параметры знака
}
  IMGTEMPLATE;

typedef struct IMGTRUETYPE       // (151) ПАРАМЕТРЫ ЗНАКА из TRUETYPE ШРИФТА
{
  IMGTEXT Text     ;             // Параметры шрифта
  char FontName[32];             // Условное название шрифта
  long Number      ;             // Номер знака в шрифте
  long MinV        ;             // Габариты знака
  long MinH        ;             //  относительно точки привязки
  long MaxV        ;             // (0,0)
  long MaxH        ;             // (H,H/2) - начальные значения
  long PosV        ;             // Точка привязки знака по вертикали
  long PosH        ;             // и горизонтали (0,0)
}
  IMGTRUETYPE;

typedef struct IMGTRUETEXT       // (152) ШРИФТ
{                                //      (size = 64)
  IMGTEXT Text     ;             // Параметры шрифта (Поле Type - произвольно)
  char FontName[32];             // Условное название шрифта
}
  IMGTRUETEXT;


typedef struct IMGHATCHSQUARE    // (153) ШТРИХОВАННЫЙ ПЛОЩАДНОЙ
{
  long int Length;               // Полная длина записи описания объекта,
                                 // включая параметры штриховой линии
  long int Angle ;               // Угол наклона штриховки в градусах (0 - 180)
                                 // против часовой стрелки от горизонтали
  long int Step  ;               // Расстояние между осями штриховых линий
  long int Number;               // Номер функции штриховой линии (IMGLINE или IMGDOT)
                                 // затем: параметры для штриховой линии
} IMGHATCHSQUARE ;


typedef struct IMGSQUAREGLASS    // (154) ПЛОЩАДНОЙ ОБ'ЕКТ - СТЕКЛО
{                                //      (size = 8)
  signed long Bright;            // Процент изменения яркости (+/- 100)
  signed long Contrast;          // Процент изменения контраста (+/- 100)
}
  IMGSQUAREGLASS;

typedef struct IMGSQUAREVECTOR   // (155) ПЛОЩАДНОЙ + ВЕКТОРНЫЙ
{                                //
  long           Length  ;       // Длина структуры
  unsigned short Kind    ;       // Тип сетки заполнения 4x4 (16 бит)
                                 //  (IMGK_REGULAR, ...)
  unsigned char  FullMark;       // Флаг заполнения
                                 //  0 - по всей площади
                                 //  1 - только полными знаками
  unsigned char  Reserv  ;       // Должен быть 0
  long           StepHor ;       // Расстояние между знаками по горизонтали
  long           StepVer ;       // Расстояние между знаками по вертикали
  long           Angle   ;       // Угол поворота знака от вертикали по часовой
  IMGVECTOREX    Mark    ;       // Описание знака
}
  IMGSQUAREVECTOR;


#define DO_BASE      1           // базовый объект (линия либо площадь)
                                 // объекты заполняющие метрику
#define DO_FIRST     2           // на первой точке
#define DO_END       3           // на последней точке
#define DO_LEFT      4           // слева на каждой точке
#define DO_RIGHT     5           // справа на каждой точке
#define DO_MIDDLE1   6           // вдоль линии (1 - оформляющий)
#define DO_MIDDLE2   7           // вдоль линии (2 - заполняющий)


typedef struct IMGDECORHEAD      // ЗАГОЛОВОК ОФОРМЛЕННОГО ЛИНЕЙНОГО
{
  unsigned long Length      ;    // Полная длина записи описания объекта
           long Begin       ;    // Длина начала линии
           long End         ;    // Длина конца линии
           long Left        ;    // Длина начала отрезка
           long Right       ;    // Длина конца отрезка
           long First       ;    // Длина первого заполняющего знака
           long Second      ;    // Длина второго заполняющего знака
           char Equidistant ;    // Равномерно размещать по всей линии
           char Reserv[3]   ;    // Резерв - равен 0
}
  IMGDECORHEAD;

typedef struct IMGDECORATE        // (157) ОФОРМЛЕННЫЙ ЛИНЕЙНЫЙ
{
  IMGDECORHEAD   Head         ;   // Заголовок
  IMGDECOR       Base         ;   // Заголовок базовой функции
  unsigned long  BaseParm[1]  ;   // Параметры базовой по типу
  IMGDECOR       Begin        ;   // Заголовок начального знака линии
  unsigned long  BeginParm[1] ;   // Параметры по типу
  IMGDECOR       End          ;   // Заголовок последнего знака линии
  unsigned long  EndParm[1]   ;   // Параметры по типу
  IMGDECOR       Left         ;   // Заголовок начального знака отрезка
  unsigned long  LeftParm[1]  ;   // Параметры по типу
  IMGDECOR       Rigth        ;   // Заголовок последнего знака отрезка
  unsigned long  RigthParm[1] ;   // Параметры по типу
  IMGDECOR       First        ;   // Заголовок первого знака вдоль отрезка
  unsigned long  FirstParm[1] ;   // Параметры по типу
  IMGDECOR       Second       ;   // Заголовок второго знака вдоль отрезка
  unsigned long  SecondParm[1];   // Параметры по типу
}
  IMGDECORATE;

typedef struct IMGSECTION     // Описание отдельной секции для (158)
{
  unsigned long Length;       // Длина параметров (включая IMGSECTION)
  unsigned long Number;       // Номер функции(IMG_EMPTY, IMG_LINE, IMG_VECTOREX,
                              //               IMG_TRUETYPE)
  unsigned long Base;         // Длина отрезка, занимаемая секцией
  unsigned long Parm[1];      // Параметры
}
  IMGSECTION;

typedef struct IMGDASH           // (158) НАБОРНАЯ ШТРИХОВАЯ ЛИНИЯ
{
  long int    Length;            // Полная длина записи описания объекта,
  long int    Count;             // Число отдельных секций (штрихов, пробелов, знаков)
  long int    Last;              // Порядковый номер секции (от 0 до Count-1),
                                 // Завершающей контур объекта
  long int    Border;            // Максимальная толщина секций
  long int    Reserve;           // Резерв
  IMGSECTION  Section[1];        // Описание отдельной секции
}
  IMGDASH;

typedef struct IMGLINESHIFT      // (159) CМЕЩЕННАЯ ЛИНИЯ
{
  long int    Length;            // Полная длина записи описания объекта,
  long int    Shift;             // Направление и величина сдвига
                                 // = 0 - центр линии вдоль оси метрики
                                 // > 0 - сдвиг вправо от оси, иначе влево
  IMGDECOR    Head;              // Заголовок линии
  long int    Parm[1];           // Параметры линии по ее типу
}
  IMGLINESHIFT;

typedef struct IMGVECTORTEXT     // (160) ПАРАМЕТРЫ ЦЕПОЧКИ ВЕКТОРНОГО (VT_TEXT)
{                                // (size = 68)
  IMGTEXT  Text     ;            // Параметры шрифта
  long int Code     ;            // Код семантики объекта, для выбора переменного
                                 // текста или 0 для постоянного текста
  char     Title[32];            // Постоянный текст
}
  IMGVECTORTEXT;

typedef struct IMGSQUAREGLASSCOLOR // (164) ПЛОЩАДНОЙ ОБ'ЕКТ - ЦВЕТНОЕ СТЕКЛО
{                                  // (size = 12)
         long Color;               // Цвет тонирования (IMGC_TRANSPARENT - бесцветный)
  signed long Bright;              // Процент изменения яркости (+/- 100)
  signed long Contrast;            // Процент изменения контраста (+/- 100)
}
  IMGSQUAREGLASSCOLOR;

typedef struct IMGLIBRARY        // (250) ПАРАМЕТРЫ ОБ'ЕКТА(РИСУЕТ ПОЛЬЗОВАТЕЛЬ)
{
  long int Length  ;             // Полная длина записи описания объекта
  long int Code    ;             // Код DLL (IML)
  long int Index   ;             // Индекс DLL в списке библиотек
  long int Function;             // Код функции
  IMGDRAW  Draw    ;             // Параметры
}
  IMGLIBRARY;

typedef struct IMGDOUBLE         // (253) СВОБОДНЫЕ ПАРАМЕТРЫ ТИПА DOUBLE
{
  double Value;
}
  IMGDOUBLE;

typedef struct IMGLONG           // (254) СВОБОДНЫЕ ПАРАМЕТРЫ ТИПА LONG
{
  long Value;
}
  IMGLONG;

typedef struct IMGOBJECT         // (255) ОТОБРАЗИТЬ/ВЫДЕЛИТЬ ОБ'ЕКТ
{                                //  В УСЛОВНЫХ ЗНАКАХ КЛАССИФИКАТОРА
                                 //      (size = 16)
  unsigned long Color ;          // Цвет выделения объекта на карте
                                 //  (IMGC_NOMARK - не выделять)
  unsigned long Incode;          // Внутренний код объекта
                                 //  (0 - устанавливается по Excode и Local)
  unsigned long Excode;          // Внешний код объекта
  unsigned long Local ;          // Локализация объекта (IMGL_LINE, ...)

  // Для функций типа PaintMapObject(...) Incode устанавливается
  // автоматически. Если выделение цветом не нужно, то вместо
  // адреса структуры можно указывать ноль.
}
  IMGOBJECT;



typedef struct IMGVECTORGRADIENT    // ВСПОМОГАТЕЛЬНАЯ СТРУКТУРА
{
  unsigned long Color;        // Цвет градиентной заливки (COLORREF)
  unsigned long Left;         // Процент заполнения габаритов слева
  unsigned long Right;        // Процент заполнения габаритов справа
  unsigned long Bright;       // Процент изменения яркости заливки
}
  IMGVECTORGRADIENT;


//**********************************************************************
// УСТАРЕВШИЕ СТРУКТУРЫ ПАРАМЕТРОВ ВИЗУАЛИЗАЦИИ (заменены на IMGDRAW...)
//**********************************************************************

#define IMG_LINEDOT           130
#define IMG_DOUBLELINE        131
#define IMG_DOTDLINE          132
#define IMG_DOUBLEDOT         133
#define IMG_SHIFTLINEDOT      134
#define IMG_SQUARECROSS       136
#define IMG_MARK              137
#define IMG_SQUAREMARK        138
#define IMG_LEFTLINEDOT       145
#define IMG_DECORATELINE      146


typedef struct IMGLINEDOT        // (130) ПУНКТИР ПО ЛИНИИ
{                                //      (size = 24)
  IMGLINE Line;                  // Нижняя линия
  IMGDOT  Dot ;                  // Верхняя линия
}
  IMGLINEDOT;


typedef struct IMGDOUBLELINE     // (131) ЛИНИЯ ПО ЛИНИИ
{                                //      (size = 16)
  IMGLINE OneLine;               // Нижняя линия
  IMGLINE TwoLine;               // Верхняя линия
}
  IMGDOUBLELINE;


typedef struct IMGDOTDLINE       // (132) ДВОЙНАЯ ЛИНИЯ ПО ПУНКТИРУ
{                                //      (size = 32)
  IMGDOT        Dot   ;          // Нижняя линия
  IMGDOUBLELINE Double;          // Верхняя линия
}
  IMGDOTDLINE;


typedef struct IMGDOUBLEDOT      // (133) ДВОЙНОЙ ПУНКТИР
{                                //      (size = 32)
  IMGDOT OneDot;                 // Нижняя линия
  IMGDOT TwoDot;                 // Верхняя линия
}
  IMGDOUBLEDOT;


typedef struct IMGSHIFTLINEDOT   // (134) ПУНКТИР, СМЕЩЕННЫЙ ВПРАВО, ПО ЛИНИИ
{                                //      (size = 24)
  IMGLINE Line;                  // Нижняя линия
  IMGDOT  Dot ;                  // Верхняя линия-пунктир
}
  IMGSHIFTLINEDOT;


typedef struct IMGSQUARECROSS    // (136) ШТРИХОВАННАЯ ПЛОЩАДЬ
{                                //      (size = 16)
  unsigned long Color;           // Цвет штpиховки (IMGC_TRANSPARENT - прозрачный)
  unsigned long Kind ;           // Тип штpиховки (SC_HORIZONTAL, ...)
  unsigned long Thick;           // Толщинa штpихa
  unsigned long Blank;           // Толщинa пpобелa
}
  IMGSQUARECROSS;

typedef struct IMGMARK           // (137) ТОЧЕЧНЫЙ РАСТРОВЫЙ ОБ'ЕКТ
{                                //      (size = 16 + 128 = 144)
  unsigned long Color    ;       // Цвет знака
  unsigned long Size     ;       // Размер в микронах (для печати)
  unsigned long PosV     ;       // Точка привязки знака PosV,PosH < Size
  unsigned long PosH     ;       //  (для IMGSQUAREMARK - шаг сетки >= Size
  unsigned char Bits[128];       // Растровое изображение 32x32 с верхнего угла
}
  IMGMARK;

typedef struct IMGSQUAREMARK     // (138) ПЛОЩАДНОЙ + ТОЧЕЧНЫЙ
{                                //      (size = 8 + 144 = 152)
  unsigned long  Color   ;       // Цвет фона (IMGC_TRANSPARENT - прозрачный)
  unsigned short Kind    ;       // Тип сетки заполнения 4x4 (16 бит)
                                 //  (IMGK_REGULAR, ...)
  unsigned char  FullMark;       // Флаг заполнения
                                 //  0 - по всей площади
                                 //  1 - только полными знаками
  unsigned char  Weight  ;       // Вес знака, толщина контура   // 27/04/00
                                 //  0 - тонкий
                                 //  1 - нормальный
  IMGMARK        Mark    ;       // Описание знака
}
  IMGSQUAREMARK;



typedef struct IMGLEFTLINEDOT    // (145) ПУНКТИР, СМЕЩЕННЫЙ ВЛЕВО, ПО ЛИНИИ
{                                //      (size = 24)
  IMGLINE Line;                  // нижняя линия
  IMGDOT  Dot ;                  // верхняя линия-пунктир
}
  IMGLEFTLINEDOT;


typedef struct IMGDECHEAD        // ЗАГОЛОВОК ОФОРМЛЕННОГО ЛИНЕЙНОГО
{
  unsigned long Length   ;       // Полная длина записи описания объекта
           long Distance ;       // Расстояние между заполняющими векторн.
}
 IMGDECHEAD;
                                 // работа со структурой в классе
                                 //
typedef struct IMGDECORATELINE   // (146) ОФОРМЛЕННЫЙ ЛИНЕЙНЫЙ
{
  IMGDECHEAD     Head        ;   // Заголовок
  IMGDECOR       Base        ;   // Заголовок базовой функции
  unsigned long  BaseParm[1] ;   // Параметры базовой по типу
  IMGDECOR       First       ;   // Заголовок начального
  unsigned long  FirstParm[1];   // Параметры по типу
  IMGDECOR       End         ;   // Заголовок конечного
  unsigned long  EndParm[1]  ;   // Параметры по типу
  IMGDECOR       Left        ;   // Заголовок левого
  unsigned long  LeftParm[1] ;   // Параметры по типу
  IMGDECOR       Rigth       ;   // Заголовок правого
  unsigned long  RigthParm[1];   // Параметры по типу
  IMGDECOR       Middle1     ;   // Заголовок первого оформляющего
  unsigned long  Mid1Parm[1] ;   // Параметры по типу
  IMGDECOR       Middle2     ;   // Заголовок второго оформляющего
  unsigned long  Mid2Parm[1] ;   // Параметры по типу
}
  IMGDECORATELINE;

                                 // ( ) ОФОРМЛЕННЫЙ ЛИНЕЙНЫЙ
typedef IMGDECORATELINE SCRDECORATELINE;
                                 // Цвета векторных заданы индексами
                                 // Экранные параметры линий


#endif  // MAPGDI_H

