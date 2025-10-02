/******  MAPCREAT.H  ***********  Belenkov  O.V. *****  26/12/06 ***
/******  MAPCREAT.H  ***********  Shishkova L.A. *****  14/06/06 ***
/******  MAPCREAT.H  ***********  Sokolova  T.O. *****  14/07/03 ***
/******  MAPCREAT.H  ***********  Savochkin V.N. *****  16/09/03 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2004              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*                 ФОРМУЛЯР ЭЛЕКТРОННОЙ КАРТЫ                       *
*                                                                  *
*******************************************************************/

// ИЗМЕНЕНИЯ СТРУКТУР MAPREGISTER И LISTREGISTER НЕ ДОПУСКАЮТСЯ !

#if !defined(MAPCREAT_H)
#define MAPCREAT_H

#ifndef MAPTYPE_H
  #include "maptype.h"
#endif

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++  ТАБЛИЦЫ КОДОВ ЭЛЕКТРОННОЙ КАРТЫ  +++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // Типы флага "Проекция исходного материала"
  typedef enum MAPPROJECTION
  {
// UNDEFINED          = -1, // Не установлено
   GAUSSCONFORMAL      = 1, // Равноугольная Гаусса-Крюгера
   CONICALORTHOMORPHIC = 2, // Коническая равноугольная
   CYLINDRICALSPECIAL  = 3, // Цилиндрическая специальная
   LAMBERT             = 4, // Азимутальная поперечная Ламберта
   STEREOGRAPHIC       = 5, // Стереографическая
   POSTEL              = 6, // (Нормальная) равнопромежуточная
                            // азимутальная проекция Постеля
   AZIMUTHALOBLIQUE    = 7, // Азимутальная равнопромежуточная косая
   MERCATORMAP         = 8, // Цилиндрическая прямая равноугольная Меркатора
   URMAEV              = 9, // Цилиндрическая произвольная(проф.Урмаева)
   POLYCONICAL         =10, // Поликоническая проекция ЦНИИГАиК
   SIMPLEPOLYCONICAL   =11, // Простая видоизмененная поликоническая
   PSEUDOCONICAL       =12, // Псевдоконическая произвольная
   STEREOGRAPHICPOLAR  =13, // Стереографическая полярная
   CHEBISHEV           =14, // Равноугольная Чебышева
   GNOMONIC            =15, // Гномоническая
   CYLINDRICALSPECIALBLANK =16, // Цилиндрическая специальная
                                // для бланковой карты
   UTM                 =17,  // UTM
                             // 28/06/01 --------------------------------
   KAVRAJSKY           =18,  // Псевдоцилиндрическая равновеликая
                             // синусоидальная проекция Каврайского
   MOLLWEIDE           =19,  // Псевдоцилиндрическая равновеликая
                             // эллиптическая проекция Мольвейде
   CONICALEQUIDISTANT  =20,  // (Прямая) равнопромежуточная коническая проекция
   CONICALEQUALAREA    =21,  // (Прямая) равновеликая коническая проекция
   CONICALDIRECTORTHOMORPHIC   =22, // (Прямая) равноугольная коническая проекция
   AZIMUTHALORTHOMORPHICPOLAR  =23, // Полярная равноугольная азимутальная
                                    // (стереографическая) проекция
   LAMBERTAZIMUTHALEQUALAREA   =24, // (Нормальная) равновеликая
                                    // азимутальная проекция Ламберта
   URMAEVSINUSOIDAL            =25, // Псевдоцилиндрическая синусоидальная
                                    // проекция Урмаева для карт
                                    // океанов(Тихого и Индийского)
   AITOFF                      =26, // Производная равновеликая
                                    // проекция Аитова-Гамера
   CYLINDRICALEQUALSPACED      =27, // Равнопромежуточная цилиндрическая проекция
   LAMBERTCYLINDRICALEQUALAREA =28, // Равновеликая цилиндрическая
                                    // проекция Ламберта
   MODIFIEDPOLYCONICAL         =29, // Видоизмененная простая поликоническая
                                    // проекция (международная)
   LAMBERTOBLIQUEAZIMUTHAL     =30, // Косая равновеликая азимутальная
                                    // проекция Ламберта
   TRANSVERSECYLINDRICAL       =31, // Равноугольная поперечно-цилиндрическая
                                    // проекция       //28/01/03
   GAUSSCONFORMAL_SYSTEM_63    =32  // Система координат 63 года  //22/08/03
  }
    MAPPROJECTION;

  // Типы флага "Вид эллипсоида"
  typedef enum ELLIPSOIDKIND
  {
   UNDEFINED   = -1,       // Не установлено
   KRASOVSKY42 =  1,       // Красовского 1942г.
   WGS_76      =  2,       // Международный 1976г.
   HEFORD      =  3,       // Хейфорда 1909г.
   CLARKE_80   =  4,       // Кларка 1880г.
   CLARKE_66   =  5,       // Кларка 1866г.
   EVEREST_57  =  6,       // Эвереста 1857г.
   BESSEL      =  7,       // Бесселя 1841г.
   AIRY        =  8,       // Эри 1830г.
   WGS_84      =  9        // Международный 1984г.
  }
   ELLIPSOIDKIND;

  // Типы флага "Система высот"
  typedef enum HEIGHTSYSTEM
  {
// UNDEFINED   = -1,       // Не установлено
   BALTIC      =  1,       // Балтийская система высот
   LAMANSH     =  6,       // Средний уровень моря в проливе Ламанш
   NORMALNULL  = 16,       // Средний уровень Cеверного мор
   MARSEL      = 22,       // Средний уровень Средиземного моря в Марселе
   USAKANADA   = 24,       // Средний уровень морей и океанов,омывающих США и Канаду
   BALTIC77    = 25,       // Балтийская система 1977 г.
   OKHOTSK     = 26,       // Средний уровень Охотского моря и Тихого океана
   PEACEOCEAN  = 27        // Средний уровень мирового океана
  }
   HEIGHTSYSTEM;

  // Типы флага "Система координат"
  typedef enum COORDINATESYSTEM
  {
// UNDEFINED          = -1, // Не установлено
   ORTHOGONAL          = 1, // Система координат 42 года
   UNIVERSALMERCATOR   = 2, // Система проекции Меркатора
   NATIONALGRID        = 3, // Национальная прямоугольная сетка Великобритании
   AREAORTHOGONAL      = 4, // Прямоугольная местная система координат
   SYSTEM_63           = 5, // Система координат 63 года
   CONDITION           = 6, // Прямоугольная условная для обзорных карт
   GEOCOORDINATE       = 7, // Геодезические координаты в радианах
   GEOCOORDINATEGRADUS = 8  // Геодезические координаты в градусах
  }
   COORDINATESYSTEM;

  // Типы флага "Тип карты"
  typedef enum MAPTYPE
  {
// UNDEFINED   = -1,      // Не установлено
   TOPOGRAPHIC  = 1,      // Топографическая
   GEOGRAPHIC   = 2,      // Обзорно-географическая
   GLOBE        = 3,      // Космонавигационная (ГЛОБУС)
   CITYPLAN     = 4,      // Топографический план города
   LARGESCALE   = 5,      // Крупномасштабный план местности
   AERONAUTIC   = 6,      // Аэронавигационная
   SEANAUTIC    = 7,      // Морская навигационная
   AVIATION     = 8,      // Авиационная
   BLANK        = 9,      // Бланковка
   UTMNAD27     = 10,     // UTM на North American Datum 1927
//18/09/02
   UTMWGS84     = 11,
   UTMTYPE      = 12,      // UTM   04/11/02
   CK_63         = 13       // Система координат 63 года  // 09/09/03
  }
   MAPTYPE;

  // Типы флага "Вид рамки"
  typedef enum FRAMEKIND
  {
// UNDEFINED    = -1,     // Не установлено
   TRAPEZE      = 1,      // Трапециевидная без точек излома
   TRAPEZECURVE = 2,      // Трапециевидная с точками излома
   RECTANGULAR  = 3,      // Прямоугольная
   CIRCLE       = 4       // Круговая
  }
   FRAMEKIND;

typedef struct COORDINATES
{
  double Coord[4];
}
  COORDINATES;

// Число элементов в таблице масштабов
enum COUNTSCALE
{
  COUNTTOPO      = 9,        // Топографическая     02/10/02
  COUNTGEOG      = 5,        // Обзорно-географическая
  COUNTAERO      = 2,        // Аэронавигационная
  COUNTCITYPLAN  = 3,        // Топографический план города     // 14/06/06
  COUNTLARGE     = 5,        // КРупномасштабный план
  COUNTSCALENONE = 16        // Тип карт не установлен          // 14/06/06
};

// Коды подсистем для обзорно-географических карт
enum GEOGRAPHICCODE     // 02/10/09
{
// UNDEFINED    = -1,     // Не установлено
  EUROPE           = 1,    // блок Европа
  ASIA             = 2,    // блок Азия
  PACIFICOCEAN     = 3,    // Тихий океан
  AMERICANORTH     = 4,    // Северная Америка
  ATLANTICOCEAN    = 5,    // Атлантический океан
  NORTHPOLARSYSTEM = 6,    // Северная полярная подсистема
  SOUTHPOLARSYSTEM = 7,    // Южная полярная подсистема
  EQUATORIALSYSTEM = 8     // Экваториальная подсистема
};

#ifdef __cplusplus

typedef struct REGISTERTYPES
{
  // Типы флага соответствия проекции
  enum PROJECTION
  {
	  NOADEQUACY = 0,   // Данные не соответствуют проекции
   ADEQUACY   = 1    // Данные соответствуют проекции
  };

  // Типы флага "Проекция исходного материала"
  enum MAPPROJECTION
  {
// UNDEFINED          = -1, // Не установлено
   GAUSSCONFORMAL      = 1, // Равноугольная Гаусса-Крюгера
   CONICALORTHOMORPHIC = 2, // Коническая равноугольная
   CYLINDRICALSPECIAL  = 3, // Цилиндрическая специальная
   LAMBERT             = 4, // Азимутальная поперечная Ламберта
   STEREOGRAPHIC       = 5, // Стереографическая
   POSTEL              = 6, // (Нормальная) равнопромежуточная
                            // азимутальная проекция Постеля
   AZIMUTHALOBLIQUE    = 7, // Азимутальная равнопромежуточная косая
   MERCATORMAP         = 8, // Цилиндрическая прямая равноугольная Меркатора
   URMAEV              = 9, // Цилиндрическая произвольная(проф.Урмаева)
   POLYCONICAL         =10, // Поликоническая проекция ЦНИИГАиК
   SIMPLEPOLYCONICAL   =11, // Простая видоизмененная поликоническая
   PSEUDOCONICAL       =12, // Псевдоконическая произвольная
   STEREOGRAPHICPOLAR  =13, // Стереографическая полярная
   CHEBISHEV           =14, // Равноугольная Чебышева
   GNOMONIC            =15, // Гномоническая
   CYLINDRICALSPECIALBLANK =16, // Цилиндрическая специальная
                                // для бланковой карты
   UTM                 =17,     // UTM
                             // 28/06/01 --------------------------------
   KAVRAJSKY           =18,  // Псевдоцилиндрическая равновеликая
                             // синусоидальная проекция Каврайского
   MOLLWEIDE           =19,  // Псевдоцилиндрическая равновеликая
                             // эллиптическая проекция Мольвейде
   CONICALEQUIDISTANT  =20,  // (Прямая) равнопромежуточная коническая проекция
   CONICALEQUALAREA    =21,  // (Прямая) равновеликая коническая проекция
   CONICALDIRECTORTHOMORPHIC   =22, // (Прямая) равноугольная коническая проекция
   AZIMUTHALORTHOMORPHICPOLAR  =23, // Полярная равноугольная азимутальная
                                    // (стереографическая) проекция
   LAMBERTAZIMUTHALEQUALAREA   =24, // (Нормальная) Равновеликая
                                    // азимутальная проекция Ламберта
   URMAEVSINUSOIDAL            =25, // Псевдоцилиндрическая синусоидальная
                                    // проекция Урмаева для карт
                                    // океанов(Тихого и Индийского)
   AITOFF                      =26, // Производная равновеликая
                                    // проекция Аитова-Гамера
   CYLINDRICALEQUALSPACED      =27, // Равнопромежуточная цилиндрическая проекция
   LAMBERTCYLINDRICALEQUALAREA =28, // Равновеликая цилиндрическая
                                    // проекция Ламберта
   MODIFIEDPOLYCONICAL         =29, // Видоизмененная простая поликоническая
                                    // проекция (международная)
   LAMBERTOBLIQUEAZIMUTHAL     =30, // Косая равновеликая азимутальная
                                    // проекция Ламберта
   TRANSVERSECYLINDRICAL       =31, // Равноугольная поперечно-цилиндрическая   //04/09/03
                                    // проекция       //28/01/03
   GAUSSCONFORMAL_SYSTEM_63    =32  // Система координат 63 года  //22/08/03
  };

  // Типы флага "Вид эллипсоида"
  enum ELLIPSOIDKIND
  {
   UNDEFINED   = -1,       // Не установлено
   KRASOVSKY42 =  1,       // Красовского 1942г.
   WGS_76      =  2,       // Международный 1976г.
   HEFORD      =  3,       // Хейфорда 1909г.
   CLARKE_80   =  4,       // Кларка 1880г.
   CLARKE_66   =  5,       // Кларка 1866г.
   EVEREST_57  =  6,       // Эвереста 1857г.
   BESSEL      =  7,       // Бесселя 1841г.
   AIRY        =  8,       // Эри 1830г.
   WGS_84      =  9        // Международный 1984г.
  };


  // Типы флага "Система высот"
  enum HEIGHTSYSTEM
  {
// UNDEFINED   = -1,       // Не установлено
   BALTIC      =  1,       // Балтийская система высот
   LAMANSH     =  6,       // Средний уровень моря в проливе Ламанш
   NORMALNULL  = 16,       // Средний уровень Cеверного мор
   MARSEL      = 22,       // Средний уровень Средиземного моря в Марселе
   USAKANADA   = 24,       // Средний уровень морей и океанов,омывающих США и Канаду
   BALTIC77    = 25,       // Балтийская система 1977 г.
   OKHOTSK     = 26,       // Средний уровень Охотского моря и Тихого океана
   PEACEOCEAN  = 27        // Средний уровень мирового океана
  };

  // Типы флага "Система координат"
  enum COORDINATESYSTEM
  {
// UNDEFINED          = -1, // Не установлено
   ORTHOGONAL          = 1, // Система координат 42 года
   UNIVERSALMERCATOR   = 2, // Система проекции Меркатора
   NATIONALGRID        = 3, // Национальная прямоугольная сетка Великобритании
   AREAORTHOGONAL      = 4, // Прямоугольная местная система координат
   SYSTEM_63           = 5, // Система координат 63 года
   CONDITION           = 6, // Прямоугольная условная для обзорных карт
   GEOCOORDINATE       = 7, // Геодезические координаты в радианах
   GEOCOORDINATEGRADUS = 8  // Геодезические координаты в градусах
  };

  // Типы флага "Единицы измерения"
  enum UNIT
  {
// UNDEFINED     = -1,   // Не установлено
   KILOMETRE     = 5,    // Километры
   METRE05       = 4,    // 0.5 м
   METRE         = 0,    // Метры
   DECIMETRE     = 1,    // Дециметры
   CENTIMETRE    = 2,    // Сантиметры
   MILLIMETRE    = 3,    // Миллиметры
   SECOND01      = 66,   // 0.1 сек
   RADIAN        = 64,   // Радианы
   RADIAN8       = 67,   // 10e-8 рад
   DEGREE        = 65,   // Градусы
   FOOT          = 16    // Футы
  };


  // Типы флага "Вид рамки"
  enum FRAMEKIND
  {
// UNDEFINED    = -1,     // Не установлено
   TRAPEZE      = 1,      // Трапециевидная без точек излома
   TRAPEZECURVE = 2,      // Трапециевидная с точками излома
   RECTANGULAR  = 3,      // Прямоугольная
   CIRCLE       = 4       // Круговая
  };

  // Типы флага "Тип карты"
  enum MAPTYPE
  {
// UNDEFINED   = -1,      // Не установлено
   TOPOGRAPHIC  = 1,      // Топографическая
   GEOGRAPHIC   = 2,      // Обзорно-географическая
   GLOBE        = 3,      // Космонавигационная (ГЛОБУС)
   CITYPLAN     = 4,      // Топографический план города
   LARGESCALE   = 5,      // Крупномасштабный план местности
   AERONAUTIC   = 6,      // Аэронавигационная
   SEANAUTIC    = 7,      // Морская навигационная
   AVIATION     = 8,      // Авиационная
   BLANK        = 9,      // Бланковка
   UTMNAD27     = 10,     // UTM на North American Datum 1927  //18/09/02
//18/09/02
   UTMWGS84     = 11,
   UTMTYPE      = 12,      // UTM  04/11/02
   CK_63        = 13       // Система координат 63 года  // 09/09/03
  };

  // Типы исходного материала
  // Вид источника информации
  enum MAPINITKIND
  {
    MAP     = 1,           // Картографический
    PHOTO   = 2,           // Фотоплан
    IMAGE   = 3,           // Фотоснимок
    GRAM    = 4            // Фотограмметрический
  };

  // Вид исходного карт/фотоматериала
  enum MAPINITTYPE
  {
   MAPRUN     = 1,        // Тиражный оттиск
   FINAL      = 2,        // Издательский оригинал
   MANUSCRIPT = 3,        // Составительский оригинал
   UPDATE     = 4,        // Оригинал изменений
   SPECIAL    = 5,        // Специальный оригинал
   CONSTANT   = 6,        // Диапозитив постоянного хранения
  };

  // Вид исходного фотоматериала
  enum PHOTOINITKIND
  {
   FGM        = 7,        // Фотограмметрический материал
   FGMMAPRUN  = 8,        // ФГМ и тиражный оттиск
   FGMMANUSCRIPT = 9,     // ФГМ и составительский оригинал
   FGMFINAL      = 10,    // ФГМ и издательский оригинал
   FGMCONSTANT   = 11,    // ФГМ и диапозитив постоянного хранения
   FGMSPECIAL    = 12,    // ФГМ и специальный оригинал
   SPACE         = 64,    // Космические
   AERO          = 65,    // Аэроснимки
   PHOTOGRAM     = 66     // Фототеодолитные снимки
  };


  // Типы масштабов для Равноугольной проекции Гаусса-Крюгера
  enum SCALEGAUSS
  {
   FGKSCALEMLN = 1,
   FGKSCALE500 = 2,
   FGKSCALE200 = 3,
   FGKSCALE100 = 4,
   FGKSCALE50  = 5,
   FGKSCALE25  = 6,
   FGKSCALE10  = 7,
   FGKSCALE5   = 8,
   FGKSCALE2   = 9
  };

  // Типы масштабов для обзорно-географических карт
  enum SCALEGEOG
  {
   FGEOSCALE10MLN = 1,
   FGEOSCALE5MLN  = 2,
   FGEOSCALE2MLN  = 3,
   FGEOSCALEMLN   = 4,
   FGEOSCALE500   = 5
  };

  // Типы масштабов аэронавигационных карт
  enum SCALEAERO
  {
   FARSCALE4MLN = 1,
   FARSCALE2MLN = 2
  };

  // Типы масштабов для карт топографический план города
  enum SCALECITYPLAN
  {
   FCPSCALE25 = 1,
   FCPSCALE10 = 2
  };

  // Количество типов для таблиц соответствий
  enum TABLEVALUETYPE
  {
   COUNTMAPTYPE = 14,             // тип карты   //16/09/03
   COUNTUNIT    = 12,             // единицы измерения
   COUNTUNITHEIGHT = 8,           // единицы измерения по высоте
   COUNTELL     = 10,             // вид эллипсоида
   COUNTHEIGHT  =  9,             // система высот
   COUNTCOORD   =  8,             // система координат
   COUNTPROJ    = 33,             // проекция    // 14/07/03 //16/09/03
   COUNTMAPINITKIND = 5,          // вид исходного материала
   COUNTMAPINITTYPE = 7,          // тип ИКМ
   COUNTPHOTO   = 10,             // тип снимков
   COUNTFRAME   = 5,              // вид рамки
   COUNTGEOGRAPHICCODE = 9        // коды для обзорно-географических карт
  };

}
  REGISTERTYPES;

#endif __cplusplus

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++  ПАРАМЕТРЫ ПРОЕКЦИИ ЭЛЕКТРОННОЙ КАРТЫ  ++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct CONSTPROJ
{
  double FirstMainParal    ; // Первая главная параллель
  double SecondMainParal   ; // Вторая главная параллель
  double AxisMeridian      ; // Осевой меридиан
  double MainPointParallel ; // Параллель главной точки
  double PoleLatitude      ; // Широта полюса проекции
  double PoleLongitude     ; // Долгота полюса проекции
}
  CONSTPROJ;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++  ФОРМУЛЯР ЛИСТА ЭЛЕКТРОННОЙ КАРТЫ  ++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct LISTREGISTER
{
long        Length                 ;  // Размер данной структуры

char        Nomenclature[32]       ;  // Номенклатура листа
char        ListName[32]           ;  // Название листа

char        FileName[260]          ;  // Имя файла по которому в районе будет
                                      // создан лист с данным именем
                                      // и расширениями HDR, DAT, SEM, DRW

// Прямоугольные координаты листа в метрах
double      XSouthWest             ;  // X ю-з
double      YSouthWest             ;  // Y ю-з
double      XNorthWest             ;  // X с-з
double      YNorthWest             ;  // Y с-з
double      XNorthEast             ;  // X с-в
double      YNorthEast             ;  // Y с-в
double      XSouthEast             ;  // X ю-в
double      YSouthEast             ;  // Y ю-в

// Геодезические координаты листа в радианах
double      BSouthWestCoordinate   ;  // B ю-з
double      LSouthWestCoordinate   ;  // L ю-з
double      BNorthWestCoordinate   ;  // B с-з
double      LNorthWestCoordinate   ;  // L с-з
double      BNorthEastCoordinate   ;  // B с-в
double      LNorthEastCoordinate   ;  // L с-в
double      BSouthEastCoordinate   ;  // B ю-в
double      LSouthEastCoordinate   ;  // L ю-в

long        MaterialKind           ;  // Вид исходного материала
long        MaterialType           ;  // Тип ИКМ
long        ReliefHeight           ;  // Высота сечения рельефа в дециметрах
char        Date[12]               ;  // Дата съемки "ГГГГММДД"

double      MagneticAngle          ;  // Магнитное склонение
double      YearMagneticAngle      ;  // Годовое магнитное склонение
double      MeridianAngle          ;  // Среднее сближение меридианов
char        DateAngle[12]          ;  // Дата склонения "ГГГГММДД"

long        Reserv2[3]             ;  // = 0

DOUBLEPOINT BorderSW               ;  // Служебное поле (ВСЕГДА 0 !)
}
  LISTREGISTER;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++ "ФОРМУЛЯР РАЙОНА РАБОТ" ++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct MAPREGISTER
{
long        Length                 ; // Размер данной структуры

char        Name[32]               ; // Имя района
long        Scale                  ; // Знаменатель масштаба
long        ProjectionFlag         ; // Флаг соответствия проекции
long        EllipsoideKind         ; // Вид эллипсоида
long        HeightSystem           ; // Система высот
long        MaterialProjection     ; // Проекция исх. материала
long        CoordinateSystem       ; // Система координат
long        PlaneUnit              ; // Единица измерения в плане
long        HeightUnit             ; // Единица измерения по высоте
long        FrameKind              ; // Вид рамки
long        MapType                ; // Обобщенный тип карты
long        DeviceCapability       ; // Разрешающая способность прибора
long        DataProjection         ; // Наличие данных о проекции
long        OrderViewSheetFlag     ; // Порядок отображения листов
long        FlagRealPlace          ; // Признак реальных координат
long        ZoneNumber             ; // Заполняется системой при запросе
                                     // формуляра - номер зоны топокарты

                                     // В радианах
double      FirstMainParallel      ; // Первая главная параллель
double      SecondMainParallel     ; // Вторая главная параллель
double      AxisMeridian           ; // Осевой меридиан
double      MainPointParallel      ; // Параллель главной точки
}
  MAPREGISTER;


typedef struct MAPREGISTEREX
{
long        Length                 ; // Размер данной структуры

char        Name[32]               ; // Имя района
long        Scale                  ; // Знаменатель масштаба
long        ProjectionFlag         ; // Флаг соответствия проекции
long        EllipsoideKind         ; // Вид эллипсоида
long        HeightSystem           ; // Система высот
long        MaterialProjection     ; // Проекция исх. материала
long        CoordinateSystem       ; // Система координат
long        PlaneUnit              ; // Единица измерения в плане
long        HeightUnit             ; // Единица измерения по высоте
long        FrameKind              ; // Вид рамки
long        MapType                ; // Обобщенный тип карты
long        DeviceCapability       ; // Разрешающая способность прибора
                                     // Обычно равна 20 000
                                     // Для карт повышенной точности:  // 26/12/06
                                     // -1 - максимальная точность
                                     // -2 - хранить координаты в сантиметрах
                                     // -3 - хранить координаты в миллиметрах
long        DataProjection         ; // Наличие данных о проекции
long        OrderViewSheetFlag     ; // Порядок отображения листов
long        FlagRealPlace          ; // Признак реальных координат
long        ZoneNumber             ; // Заполняется системой при запросе
                                     // формуляра - номер зоны топокарты

                                     // В радианах
double      FirstMainParallel      ; // Первая главная параллель
double      SecondMainParallel     ; // Вторая главная параллель
double      AxisMeridian           ; // Осевой меридиан
double      MainPointParallel      ; // Параллель главной точки
double      PoleLatitude           ; // Широта полюса проекции
double      PoleLongitude          ; // Долгота полюса проекции
double      Reserv2[8]             ; // Резерв = 0

}
  MAPREGISTEREX;

#endif

