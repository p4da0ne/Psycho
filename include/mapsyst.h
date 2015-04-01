/******  MAPSYST.H  ************  Belenkov O.V.    ***  27/02/07 ***
*******  MAPSYST.H  ************  Dishlenko S.G.   ***  15/09/09 ***
*******  MAPSYST.H  ************  Derevyagina Zh.A.***  09/09/04 ***
*******  MAPSYST.H  ************  Shabakov  D.A.   ***  17/09/07 ***
*******  MAPSYST.H  ************  Kruzhkov  A.E.   ***  18/09/07 ***
*******  MAPSYST.H  ************  Shishkova L.A.   ***  12/05/03 ***
*******  MAPSYST.H  ************  Sokolova  T.O.   ***  13/12/03 ***
*******  MAPSYST.H  ************  Gorbunov  A.V.   ***  31/03/05 ***
*******  MAPSYST.H  ************  Kruzhkova E.A.   ***  06/03/08 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2009              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*      FOR WINDOWS, WINDOWS CE, LINUX, QNX, ОС-РВ, & ...           *
*                                                                  *
********************************************************************
*                                                                  *
*                ОПИСАНИЕ СИСТЕМНЫХ ФУНКЦИЙ                        *
*                                                                  *
*******************************************************************/
     
#if !defined(MAPSYST_H) 
#define MAPSYST_H

#define WIN32API        // Тип системного интерфейса Win32Api
//#define  LINUXAPI     // Тип системного интерфейса Linux  
//#define NUMBERTURN    // Процессор типа SPARC/MIPS (разворот байт)

//#define LINUXSERVER   // Тип сборки для Linux-сервера (увеличение размеров   // 06/03/08
                                          //  области памяти для отображения карты)

//#define WINCE           // ВАРИАНТ ДЛЯ WINDOWS CE 
//#undef  LINUXAPI 

#ifdef QNX
 #define  HIDETRY       // 02/09/00
 #define  LINUXAPI      // Тип системного интерфейса Linux
 #define  TINYDATA      // Тип данных для бортовых систем
 #define  WATCOM        // Тип компилятора WATCOM
#endif

#ifdef TINYDATA
  #define HIDEACC43
  #define HIDEMTL
  #define HIDETIN       // 31/03/05
  #define HIDEMTQ       // 30/07/01
  #define HIDEMETAFILE
  #define HIDEMTR3D
  #define MINIDATA      // IDSHORT2,IDDOUBLE2,IDDOUBLE3
  #define HIDEGDX       // 30/07/01
  #define HIDESET       // 30/07/01
  #define HIDEUNION     // 30/07/01
  #define HIDELOG
  #define HIDERSC3D     // 14/06/06
#endif

#ifdef WINCE
  #define _TEXT _T      // 22/04/03
  #define itoa(value, str, dec) sprintf(str, "%i", value)
  #define HIDEGDX       // 13/12/03
  #define HIDESECTION   // 06/07/06
#endif

#ifdef LINUXAPI

//  #define OC2000        // Для ОС2000 открыть
#ifdef NUMBERTURN       // 14/03/03
  #define HIDEACC43
#endif
  #define RISCCPU       // Выравнивание переменных и структур
#ifdef OC2000             // 11/10/02
  #define MINIDATA      // IDSHORT2,IDDOUBLE2,IDDOUBLE3
#endif
  #define HIDEMTL
  #define HIDETIN       // 31/03/05
//  #define HIDEMTQ       // 30/07/01     // 28/09/06
  #define HIDEMETAFILE
  #define HIDEMTR3D
  #define HIDE3D        // 08/07/04
  #define HIDEGDX       // 30/07/01
  #define HIDEIMAGEDIT  // 30/07/01
  #define HIDETIN       // 07/06/05
//  #define HIDEUNION     // 30/07/01
  #define WINAPI
  #define TURNRASTERTEXT    // 04/10/00
#ifndef __cplusplus         // 12/04/01
  #define HIDETRY
  #define HIDESET       // 10/10/02
//  #define HIDEIML         // 09/09/04
#endif
  #define HIDEX11    // Скрыть функции Xwindow  // 20/09/05
#ifdef HIDEX11
  #define HIDESCREENIMAGE
#endif
  #define CROSS_MIPS   // 20/09/05 // Поддержка кросскомпилятора под процессор MIPS
  #define HIDEPAINTSTEP  // 17/04/06 // Игнорировать шаг вывода DIB в окно

  #define MULTIPLATFORM  // 10/07/07 // Одновременная работа с данными на платформах SPARC и INTEL
  #define TRANSTELECOM   // 12/07/07
  #define ANSI           // 11/09/07 

  #include <sys/types.h>
  #include <sys/stat.h>
  #include <stdio.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <time.h>
  #include <stdlib.h>
  #include <ctype.h>
  #include <errno.h>
  #include <string.h>  // 14/04/00
#ifndef OC2000         // 04/05/01
  #include <dlfcn.h>
  #include <sys/vfs.h> // 02/06/03
#else
  #include <version.h> // 12/11/03
#endif
#if (!defined(WATCOM) && !defined(OC2000))    // 10/06/04
  #include <sys/vfs.h>
#endif  
#ifndef HIDESECTION                           // 28/09/06
  #include <pthread.h>
#endif
  #include <iconv.h>                          // 06/12/06
  
  typedef unsigned char BYTE;
  typedef struct PALETTEENTRY
  {
    BYTE peRed;
    BYTE peGreen;
    BYTE peBlue;
    BYTE peFlags;
  }
  PALETTEENTRY;

  typedef unsigned int HANDLE;
  typedef unsigned int UINT;
  typedef long int COLORREF;
#ifndef OC2000                      // 04/05/01
  typedef unsigned short int WORD;  // 28/02/01
  typedef unsigned long DWORD;
#else
#if (OS_VERSION > 206)              // 12/11/03
  typedef unsigned short int WORD;
  typedef unsigned long DWORD;
#endif
#endif
  typedef long int LONG;
  typedef HANDLE HBITMAP;
  typedef HANDLE HGLOBAL;
  typedef HANDLE HPALETTE;
  typedef HANDLE HDC;
  typedef HANDLE HPEN;
  typedef long int XYTYPE;            // 07/06/05
  typedef long int HWND;
  typedef long int HINSTANCE;
  typedef long int LPARAM;
  typedef long int WPARAM;            // 31/01/02
  typedef long int LRESULT;
  typedef long int HMENU;             // 05/03/02
  typedef void *   LPVOID;            // 10/12/04
  typedef char *   LPTSTR;
  typedef char     WCHAR;             // 12/12/06
#ifndef TCHAR
  typedef char TCHAR;
  #define _TEXT(x)   x                // 08/11/00
#endif

#if (!defined(WATCOM) && !defined(OC2000)) // 04/05/01
  #define strnicmp   strncasecmp
  #define stricmp    strcasecmp
  #define strcmpi    strcasecmp      // 25/09/00

//#if defined(CROSS_MIPS) && !defined(NUMBERTURN) // 06/03/07   // 20/09/05 // 07/10/05  
#if defined(CROSS_MIPS) // 20/06/07

  #define sinl sin
  #define cosl cos
#endif
#else
  #define strnicmp   strncmp
  #define stricmp    strcmp
  #define strcmpi    strcmp      // 25/09/00
  #define sinl       sin
  #define cosl       cos
#endif

#ifndef FALSE                        // 30/11/00
  #define FALSE      0
  #define TRUE       1
#endif

  #define PASCAL __stdcall
  #define WINAPI
  #define far        // 13/09/00
  #define _import    // 08/12/00
  #define near       // 13/09/00
  #define _export    // 13/09/00
  #define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))) // 13/09/00
  #define PALETTERGB(r,g,b)   (0x02000000 | RGB(r,g,b))  // 08/12/00
  #define PALETTEINDEX(i)     ((COLORREF)(0x01000000 | (DWORD)(WORD)(i)))
  #define GetRValue(rgb)   ((BYTE) (rgb))                 // 26/09/00
  #define GetGValue(rgb)   ((BYTE) (((WORD) (rgb)) >> 8)) // 26/09/00
  #define GetBValue(rgb)   ((BYTE) ((rgb) >> 16))         // 26/09/00

  #define ERROR_ALREADY_EXISTS 183L
  #define ERROR_FILE_NOT_FOUND 2L
  #define LF_FACESIZE 32

  #define FILE_SHARE_READ S_IRUSR|S_IRGRP|S_IROTH
  #define FILE_SHARE_WRITE S_IWUSR|S_IWGRP|S_IWOTH
//  #define GENERIC_READ O_RDONLY
  #define GENERIC_READ  0x100000              // 10/03/07
//  #define GENERIC_WRITE O_RDWR
  #define GENERIC_WRITE O_WRONLY              // 10/03/07
  #define OPEN_ALWAYS O_CREAT
  #define CREATE_ALWAYS O_CREAT|O_TRUNC
  #define OPEN_EXISTING O_EXCL
  #define CREAT_NEW O_CREAT|O_EXCL
  #define TRUNCATE_EXISTING O_CREAT|O_EXCL|O_TRUNC
  #define FILE_BEGIN SEEK_SET
  #define FILE_CURRENT SEEK_CUR
  #define FILE_END SEEK_END
  #define MAX_PATH 260                     // 18/03/02
  #define FILE_ATTRIBUTE_READONLY S_IRUSR|S_IRGRP|S_IROTH
  #define FILE_ATTRIBUTE_ARCHIVE S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR|S_IRGRP|S_IROTH
  #define FILE_ATTRIBUTE_HIDDEN S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR
  #define FILE_ATTRIBUTE_NORMAL S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR|S_IRGRP|S_IROTH
  #define FILE_ATTRIBUTE_OFFLINE S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR|S_IRGRP|S_IROTH
  #define FILE_ATTRIBUTE_SYSTEM S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR
  #define FILE_ATTRIBUTE_TEMPORARY S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR|S_IRGRP|S_IROTH

  #define OFN_FILEMUSTEXIST       0x00001000        //19/03/02
  #define OFN_PATHMUSTEXIST       0x00000800
  #define OFN_CREATEPROMPT        0x00002000
  #define OFN_OVERWRITEPROMPT     0x00000002
  #define OFN_ALLOWMULTISELECT    0x00000200        // 12/04/02
  #define OFN_HIDEREADONLY        0x00000004        // 19/05/02
  #define OFN_NOREADONLYRETURN    0x00008000
  // Маски клавиш (полное соотвествие WIN32)        // 14/05/02
  #define MK_LBUTTON                  0x0001
  #define MK_RBUTTON                  0x0002
  #define MK_SHIFT                    0x0004
  #define MK_CONTROL                  0x0008
  #define MK_MBUTTON                  0x0010
  // Коды клавиш (XWindow)
  #define VK_F10                      0xFFC7             // XK_F10
  #define VK_ESCAPE                   0xFF1B             // XK_Escape
  #define VK_TAB                      0xFF09             // XK_Tab
  #define VK_HOME                     0xFF50             // XK_Home
  #define VK_PRIOR                    0xFF55             // XK_Prior
  #define VK_NEXT                     0xFF56             // XK_Next
  #define VK_END                      0xFF57             // XK_End
  #define VK_LEFT                     0xFF51             // XK_Left
  #define VK_UP                       0xFF52             // XK_Up
  #define VK_RIGHT                    0xFF53             // XK_Right
  #define VK_DOWN                     0xFF54             // XK_Down
  #define VK_SHIFT                    0xFFE1             // XK_Shift_L      // 14/05/02
  #define VK_CONTROL                  0xFFE3             // XK_Control_L
  #define VK_RETURN                   0xFF0D             // XK_Return     // 05/03/02
  #define VK_F1                       0xFFBE             // XK_F1
  #define VK_LBUTTON                  0xFEE0             // XK_Pointer_Left // 14/05/02
  #define VK_RBUTTON                  0xFEE1             // XK_Pointer_Right
  #define VK_SPACE                    0x020              //  XK_space      // 10/03/02
  #define VK_MENU                     0xFF67             //  XK_Menu
  #define VK_BACK                     0xFF08             // XK_BackSpace // back space, back char   //19/03/02
  #define VK_INSERT                   0xFF63             // XK_Insert      Insert, insert here    WIN32 Ins 45
  #define VK_ADD                      0xFFAB             // XK_KP_Add
  #define VK_DELETE                   0xFFFF             // XK_Delete  Delete, rubout WIN32 Del  0x2e
                                                         // XK_KP_Delete 0xFF9F не добавлена
  #define VK_SUBTRACT                 0xFFAD             // XK_KP_Subtract   WIN32 109

  #define MB_OK                       0x00000000L // 15/04/99
  #define MB_ICONHAND                 0x00000010L
  #define MB_OKCANCEL                 0x00000001L
  #define MB_YESNOCANCEL              0x00000003L
  #define MB_YESNO                    0x00000004L
  #define MB_ABORTRETRYIGNORE         0x00000002L
  #define MB_RETRYCANCEL              0x00000005L

  #define MB_ICONQUESTION             0x00000020L
  #define MB_ICONEXCLAMATION          0x00000030L
  #define MB_ICONWARNING              MB_ICONEXCLAMATION
  #define MB_ICONERROR                MB_ICONHAND
  #define MB_ICONASTERISK             0x00000040L             // 27/09/00
  #define MB_ICONINFORMATION          MB_ICONASTERISK
  #define MB_ICONSTOP                 MB_ICONHAND
  #define MB_TASKMODAL                0x00002000L
  #define IDOK                        1
  #define IDCANCEL                    2
  #define IDABORT                     3
  #define IDRETRY                     4
  #define IDIGNORE                    5
  #define IDYES                       6
  #define IDNO                        7
  #define IDCLOSE                     8
  #define IDHELP                      9

  #define DT_PLOTTER          0   // Vector plotter
  #define DT_RASDISPLAY       1   // Raster display
  #define DT_RASPRINTER       2   // Raster printer

  #define R2_COPYPEN          0x3 // GXcopy   - src  для XWindow
//  #define R2_NOT              0x6 // GXxor      //0xa // GXinvert - (NOT dst) для XWindow // 04/04/02
  #define R2_NOT              0xa // GXinvert - (NOT dst) для XWindow // 19/04/02
  #define R2_NOTXORPEN        0x9 // GXequiv    - (NOT src) XOR dst  для XWindow   //11/04/02
  #define R2_XORPEN           0x6 // GXxor    - src XOR dst для XWindow            // 21/03/02

  #define OEM_CHARSET         255
  #define RUSSIAN_CHARSET     204 // 12/04/01
  #define BI_RGB              0L

  #define TA_LEFT             0     //19/03/02
  #define TA_RIGHT            2
  #define TA_CENTER           6

  #define TA_TOP              0
  #define TA_BOTTOM           8
  #define TA_BASELINE         24

  // Стандартный вид курсора            // Коды XWindow:                // 21/04/04
  #define IDC_ARROW    (char *)68L      // XC_left_ptr
  #define IDC_ICON     IDC_ARROW
  #define IDC_IBEAM    (char *)152L     // XC_xterm
  #define IDC_WAIT     (char *)150L     // XC_watch
  #define IDC_CROSS    (char *)34L      // XC_crosshair
  #define IDC_UPARROW  (char *)22L      // XC_center_ptr
  #define IDC_SIZEALL  (char *)52L      // XC_fleur
  #define IDC_SIZE     IDC_SIZEALL
  #define IDC_SIZENWSE (char *)32L      // XC_cross_reverse
  #define IDC_SIZENESW IDC_SIZENWSE
  #define IDC_SIZEWE   (char *)108L     // XC_sb_h_double_arrow
  #define IDC_SIZENS   (char *)116L     // XC_sb_v_double_arrow

  #define _fastcall

typedef struct _FILETIME
{
   DWORD dwLowDateTime;
   DWORD dwHighDateTime;
}
   FILETIME;

typedef struct tagSIZE
{
   LONG cx;
   LONG cy;
}
   SIZE;

typedef struct tagLOGFONT
{
   LONG lfHeight;
   LONG lfWidth;
   LONG lfEscapement;
   LONG lfOrientation;
   LONG lfWeight;
   BYTE lfItalic;
   BYTE lfUnderline;
   BYTE lfStrikeOut;
   BYTE lfCharSet;
   BYTE lfOutPrecision;
   BYTE lfClipPrecision;
   BYTE lfQuality;
   BYTE lfPitchAndFamily;
   char lfFaceName[LF_FACESIZE];
}
   LOGFONT;

typedef struct tagRGBQUAD
{
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
  }
    RGBQUAD;

typedef struct _RECT
{
    long left;
    long top;
    long right;
    long bottom;
}
    RECT;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
  }
    BITMAPINFOHEADER;

typedef struct tagBITMAPINFO
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
  }
    BITMAPINFO;

typedef struct tagPOINT
{
#ifdef __cplusplus                          // 04/03/02
    union { long int x; long int X; };
    union { long int y; long int Y; };
#else
    long int x;
    long int y;
#endif
}
    POINT;

typedef struct _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
}
    SYSTEMTIME;

typedef struct tagMSG    // 31/01/02
{
    HWND hwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD time;
    POINT pt;
}
    MSG;

typedef struct TMessage    //12/05/03
{
    WPARAM    WParam;
    LPARAM    LParam;
    long int  Result;
}
    TMessage;

// Прототипы переопределенных функций (XWindow.cpp)
char * _fastcall strlwr(char * string);
char * _fastcall itoa(int number, char * string,int index);
char * _fastcall ltoa(long number, char* string,int index);
void   _fastcall GetSystemTime(SYSTEMTIME * systemtime);
int    _fastcall MessageBox(HWND hwnd, const TCHAR * message,
                            const TCHAR * title, int flag);

// Прототипы функций, определенных только для Linux (xwindow.cpp)  // 17/09/02

// Конвертировать индекс палитры в цвет вида COLORREF
// index - индекс палитры XWindow
// возвращает COLORREF
COLORREF _fastcall ConvertIndexPaletteToCOLORREF(COLORREF index);

// Конвертировать цвет вида COLORREF в индекс палитры
// color - цвет вида COLORREF
// возвращает индекс палитры
COLORREF _fastcall ConvertCOLORREFToIndexPalette(COLORREF color);

typedef int(WINAPI *FARPROC) ();

#ifdef OC2000
 #define HIDELOG               // 08/11/01
#endif

#endif     // LINUX

#ifdef WIN32API       // Windows GDI
                       
typedef struct XDrawHandle        // 10/07/06
{
  unsigned long newhandle;
  unsigned long oldhandle;
}
  XDrawHandle;

typedef XDrawHandle  XHPEN;
typedef XDrawHandle  XHBRUSH;

#else

typedef long int   XHPEN;
typedef long int   XHBRUSH;

#endif

#ifdef BUILD_DLL
// Прототипы переопределенных функций (XWindow.cpp)
int _fastcall CheckDiskFreeSpaceEx(const char * name, double length);
#endif

#ifdef WIN32API
 #if !defined(STRICT)
    #define STRICT
 #endif
#include <windows.h>
 #include <stdlib.h>
 typedef long int XYTYPE;

#if defined(__BORLANDC__) // Для Borland C++
  #include <dir.h>        // 19/01/06
#endif
#endif

// MAPTRACER   - включение трассировки (TRUE/FALSE);
// TRACE_MEM   - трассировка работы с памятью;
// TRACE_MAP   - трассировка работы конструкторов,деструкторов,
//               доступа к району работ;
// TRACE_READ  - трассировка операций чтения;
// TRACE_WRITE - трассировка операций записи.

#ifndef MAPTRACER
 #define MAPTRACER 0
#endif
#ifndef TRACE_MAP
 #define TRACE_MAP TRUE
#endif
#ifndef TRACE_MEM
 #define TRACE_MEM TRUE
#endif
#ifndef TRACE_READ
 #define TRACE_READ FALSE
#endif
#ifndef TRACE_WRITE
 #define TRACE_WRITE FALSE
#endif

#ifdef WINCE
  #undef  LINUXAPI
//  #define HIDETRY
  #define HIDELOG
  #define HIDEACC43
  #define MINIDATA
//  #define HIDEDIB
  #define HIDECUT
  #define RISCCPU
  #define HIDEMTL
  #define HIDETIN       // 31/03/05
  #define HIDEMTQ
  #define HIDEMTR3D
  #define HIDEMETAFILE
  #define HIDERSC3D
  #define HIDE3D
  #define HIDESECTION    // 26/03/08
  #define HIDECHECKCLIENT

  #define stricmp    strcmp
  #define strcmpi    strcmp
  #define strnicmp   strncmp
  #define strncmpi   strncmp
  #define ltoa       _ltoa

  #define MB_TASKMODAL    0
  #define OEM_CHARSET     255
  #define FW_ULTRALIGHT   FW_EXTRALIGHT

  #define TA_LEFT         0
  #define TA_RIGHT        2
  #define TA_CENTER       6

  #define TA_TOP          0
  #define TA_BOTTOM       8
  #define TA_BASELINE    24

#else

// #undef UNICODE
// #undef _UNICODE

#ifndef __BORLANDC__          // Кроме Borland C++
 #define strncmpi strnicmp
#endif

#endif    // WINCE

#ifdef BUILD_DLL

#ifdef WIN32API    // 15/09/09
#define UNICODE
#endif

#ifndef UNICODE
#ifndef LINUXAPI   // 21/02/01
 #ifndef TCHAR
  typedef char TCHAR;
 #endif
#endif 

#ifndef _TEXT
#define _TEXT(x)  x
#define _tcscat   strcat
#define _tcsncat  strncat   
#define _tcscpy   strcpy

#ifndef GISWEB       // 20/03/07
#define _tcsncat  strncat
#define _tcsncpy  strncpy
#endif
#endif

#endif

#endif // BUILD_DLL

#ifndef MAPTRACE_H
  #include "maptrace.h"  // Трассировка вызовов функций
#endif

// --------------------------------------------------------------
// РЕАЛИЗАЦИЯ СИСТЕМНЫХ ФУНКЦИЙ В БИБЛИОТЕКАХ
//  MAPACCES.DLL (Borland C++) и MAPAXSVC.DLL (Visual C++)
// --------------------------------------------------------------
// ПРИ ОШИБКЕ ВСЕ ФУНКЦИИ ВОЗВРАЩАЮТ НОЛЬ !

// Преобразовать строку "char" в UNICODE
// ---------------------------------------
#ifdef __cplusplus        // 05/03/01
inline int StringToUnicode(const char * src,
                           WCHAR * dest,int bytes)
{
  if ((src == 0) || (dest == 0)) return 0;

#ifdef WIN32API       // UNICODE            // 11/12/06
  return MultiByteToWideChar(CP_ACP,0,
                             src,-1,
                             dest,bytes);   // 20/05/03
#else
#ifndef LINUXAPI                            // 06/12/06
  return (int)strncpy(dest,src,bytes);
#else
  iconv_t    mycodec;
#ifndef ANSI                                // 11/09/07
  const char *fromcode = "KOI8-R";
#else
  const char *fromcode = "CP1251";
#endif
  const char *tocode   = "UTF-8";
  size_t inbytes       = strlen(src);
  size_t outbytes      = (size_t)bytes;
  int    ret = 0;

  mycodec = iconv_open(tocode, fromcode);

  if ((int)mycodec > -1)
  {
    char **src1  = (char **)&src;     // 06/03/07
    char **dest1 = (char **)&dest;

    ret = iconv(mycodec, src1, &inbytes, dest1, &outbytes);
    if (ret == -1) ret = 0;
    iconv_close(mycodec);
  }

  return ret;
#endif
#endif

}
#else
 #define StringToUnicode(src,dest,bytes) strncpy(dest,src,bytes)
#endif

// Преобразовать символы UNICODE в строку "char"
// ---------------------------------------
#ifdef __cplusplus        // 05/03/01
inline int UnicodeToString(const WCHAR * src,              // 11/12/06
                           char * dest, int bytes)
{

  if ((src == 0) || (dest == 0)) return 0;

#ifdef WIN32API
  return WideCharToMultiByte(CP_ACP,0,
                             src,-1,
                             dest,bytes/sizeof(TCHAR),0,0);
#else
#ifndef LINUXAPI                            // 06/12/06
  return (int)strncpy(dest,src,bytes);
#else
  iconv_t    mycodec;
#ifndef ANSI                                // 11/09/07
  const char *tocode   = "KOI8-R";
#else
  const char *tocode   = "CP1251";
#endif
  const char *fromcode = "UTF-8";
  size_t inbytes       = strlen(src);
  size_t outbytes      = (size_t)bytes;
  int    ret = 0; 

  mycodec = iconv_open(tocode, fromcode);
  if ((int)mycodec > -1)
  {
    char **src1  = (char **)&src;      // 06/03/07
    char **dest1 = (char **)&dest;

    ret = iconv(mycodec, src1, &inbytes, dest1, &outbytes);
    if (ret == -1) ret = 0;
    iconv_close(mycodec);
  }

  return ret;
#endif
#endif

}
#else
 #define UnicodeToString(src,dest,bytes) strncpy(dest,src,bytes)
#endif

#ifdef LINUXAPI

#  define min(a,b) (((a) < (b)) ? (a) : (b))
#  define max(a,b) (((a) > (b)) ? (a) : (b))// 29/04/99

// Процедура strupr для LINUX
#ifdef __cplusplus        // 05/03/01
 inline char * strupr(char * string) {return string;} // 27/05/99
#else
 #define strupr(string) string
#endif

#endif

// РАБОТА С ПАМЯТЬЮ
// ----------------------
#ifdef __cplusplus        // 05/03/01
inline char * AllocateTheMemory(int size)
{
  void * Memory;

#ifdef WIN32API
  #ifdef WINCE                   // 16/05/03
    Memory = new char[size];  
  #else
    Memory = ::VirtualAlloc(NULL,(DWORD)size,
                            MEM_RESERVE | MEM_COMMIT,
                            PAGE_READWRITE);
  #endif
#else
 #ifdef LINUXAPI
    Memory = malloc(size);          // 20/02/01
 #else
    Memory = 0;
 #endif

#endif

#ifdef TRACE_MEM
  Tracer2("\n+++++ 0x%04X - Allocate memory block, size - 0x%04X\n",
          Memory,size);
#endif

  return (char *) Memory;
}
#else
 #define AllocateTheMemory(size) (char *) malloc(size)
#endif

#ifdef __cplusplus        // 05/03/01
inline int FreeTheMemory(char * address)
{
#ifdef TRACE_MEM
  Tracer1("\n----- 0x%04X - Free memory block\n",address);
#endif

#ifdef WIN32API
  if (address == 0) return 0;
 #ifdef WINCE                     // 16/05/03
   delete [] address;
   return 1;
 #else 
   return ::VirtualFree((LPVOID)address,0,MEM_RELEASE);
 #endif
#else

 #ifdef LINUXAPI
  free(address);
  return 1;
 #else
  return 0;
 #endif

#endif
}
#else
 #define FreeTheMemory(address) free(address)
#endif 

#ifdef __cplusplus        // 05/03/01
inline int GetTheLastError()
{

 int ret;

#ifdef WIN32API

  ret = ::GetLastError();

#else

  ret = 0;

#endif

  return ret;

}
#else
 #define GetTheLastError() 0
#endif 

// РАБОТА С ФАЙЛАМИ
// ----------------------

#ifdef __cplusplus   // 31/03/03
inline void SplitThePath(const char *path, char *drive, char *dir,
                         char *name, char *ext)
{

#if defined(WINCE) || defined(LINUXAPI)
  int first = 0;                // 20/02/01
  int size, begin;
  int end;
  int drivesize;                  //20/11/01

  if(!path) return ;

  size = strlen(path);

  // DRIVE
  if (drive)
  {
    drive[0] = 0;
    drivesize = strcspn(path, ":");                    //20/11/01
    if (drivesize !=  size)  // строка содержит ':'    //20/11/01
    {
      strncpy(drive, path, drivesize+1);
      drive[drivesize+1] = 0;
      first = drivesize+1;
    }
  }

  begin = first;
  end = size;

  if (dir)  dir[0] = 0;
#ifndef LINUXAPI
  while((path[begin] != '\\') &&  (begin <= end))
  {
      begin++;
  }
#else // 06/07/99
  while((path[begin] != '/') &&  (begin <= end))
  {
    begin++;
  }
#endif

#ifndef LINUXAPI
  while((path[end] != '\\') &&  (end > begin))
  {
    end--;
  }
#else  // 06/07/99
  while((path[end] != '/') &&  (end > begin))
  {
    end--;
  }
#endif
  if (begin < end)
  {
    end++;

    if (dir)   // 20/04/01
    {
      strncpy(dir, path+begin, end-begin);
      dir[end-begin] = 0;
    }
    begin = end;
  }
  else
  {
    begin = first;
  }

  if (name) name[0] = 0;
  if (ext)  ext[0]  = 0;

  if (begin < (end = size))
  {
    //EXT
    while((path[end] != '.') && (begin < end))
    {
      end--;
    }

    if (begin < end)
    {
      if (name)
      {
        strncpy(name, path+begin, end-begin);
        name[end-begin] = 0;
      }

      if(ext && (end < size))
      {
        strncpy(ext, path+end, size-end);
        ext[size-end] = 0;
      }
    }
    else
    {
      if (name)           // 18/06/02
      {
        strncpy(name, path + end, size - end);
        name[size - end] = 0;
      }
    }
  }

#else // if WIN32API

#if defined(__BORLANDC__) // Для Borland C++
  fnsplit(path,drive,dir,name,ext);           // 19/01/06
#else
  _splitpath(path,drive,dir,name,ext);
#endif

#endif

}
#else
void SplitThePath(const char *path, char *drive, char *dir,
                         char *name, char *ext); // xwindow.cpp
#endif

#ifdef __cplusplus   // 05/03/01
inline void MakeThePath(char *path, const char *drive, const char *dir,
                        const char *name, const char *ext)
{

#if defined(WINCE) || defined(LINUXAPI)

  if(!path) return ;
  path[0] = 0;              // 22/10/01
  if(drive)
  {
    strcat(path, drive);
  }

  if(dir)
  {
    strcat(path, dir);
  }

  if(name)
  {
    strcat(path, name);

    if(ext)
    {
      strcat(path, ext);
    }
  }

#else // if WIN32API

#if defined(__BORLANDC__) // Для Borland C++
  fnmerge(path,drive,dir,name,ext);         // 19/01/06
#else
  _makepath(path,drive,dir,name,ext);
#endif

#endif

}
#else
void MakeThePath(char *path, const char *drive, const char *dir,
                        const char *name, const char *ext); // xwindow.cpp
#endif


// Открыть файл
#ifdef __cplusplus   // 21/02/01
inline HANDLE OpenTheFile(const char * name,int access = 0,
                          int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                          int attribute = 0)
{
  HANDLE file;

  if (name == 0) name = "";

  if (attribute == 0) attribute = OPEN_ALWAYS;

#ifdef LINUXAPI
  if (attribute & O_CREAT)   // 18/06/02
    mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

  if (access == 0) access = O_RDWR;                      // 10/03/07
  else
  {
    if (access & GENERIC_READ)
    {
      // Уберем GENERIC_READ(это наш флаг)
      access = access & 0xFFEFFFFF;
      if (access & GENERIC_WRITE) 
      {
        // Уберем GENERIC_WRITE
        access = access & 0xFFFFFFFE;
	// Зададим аттрибуты файла на запись и чтение
        access = access | O_RDWR;
      }
    }
  }

  access=access|attribute;

  if (!(access & O_CREAT)) mode = 0; // 09/06/01 
#endif

#ifdef WIN32API

  if (access == 0) access = GENERIC_READ|GENERIC_WRITE;     // 12/11/03  // 10/03/07

#ifdef UNICODE
  TCHAR tname[MAX_PATH];
  StringToUnicode(name,tname,sizeof(tname));
#endif

  if ((file = ::CreateFile(
#ifndef UNICODE
                            name,
#else
                            tname,
#endif
                            access,mode,NULL,
                            attribute,FILE_ATTRIBUTE_NORMAL,
                            0)) == INVALID_HANDLE_VALUE)
    {
      file = 0;
    }
#else

  #ifdef LINUXAPI
   if ((int)(file = open(name,access,mode)) == -1) // 21/08/03
   {
     file = 0;
   }

   if ((access & O_CREAT) && file)                       // 31/03/03
   {
     char        dir[MAX_PATH];
     struct stat statv;
     int         flag = (S_IXUSR|S_IXGRP|S_IXOTH);     
  
     memset(dir, 0, MAX_PATH);
     SplitThePath(name, 0, dir, 0, 0);
     if (strlen(dir))
     {
       // Узнаем права каталога на запись и чтение 
       stat(dir, &statv);
     }
     else statv.st_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

     // Установим права созданного файла на запись и чтение 
     chmod(name, (statv.st_mode & ~flag));
   }     
  #else
    file = 0;
  #endif

#endif

#ifdef TRACE_MAP
  Tracer2("\n->>>> Open file : %s,"
          "\n      Handle    = 0x%04X\n", name,file);
#endif

  return file;
}
#else                          
HANDLE OpenTheFile(const char * name,int access ,
                          int mode ,
                          int attribute );
#endif


#ifdef __cplusplus   // 21/02/01
 inline HANDLE OpenTheShareFile(const char * name,int access = 0,
                               int mode = FILE_SHARE_READ|FILE_SHARE_WRITE,
                               int attribute = 0)
{
  HANDLE file;

  if (name == 0) name = "";

#ifdef LINUXAPI         // 18/06/02
  if (attribute & O_CREAT)  
  {
    mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
  }    
#endif

  if ((mode & FILE_SHARE_WRITE) == 0)
    {
      // Доступ на запись монопольный
      return OpenTheFile(name,access,mode,attribute);
    }

  if (attribute == 0) attribute = OPEN_ALWAYS;

#ifdef LINUXAPI

  if (access == 0) access = O_RDWR;                      // 10/03/07
  else
  {
    if (access & GENERIC_READ)
    {
      // Уберем GENERIC_READ(это наш флаг)
      access = access & 0xFFEFFFFF;
      if (access & GENERIC_WRITE) 
      {
        // Уберем GENERIC_WRITE
        access = access & 0xFFFFFFFE;
	// Зададим аттрибуты файла на запись и чтение
        access = access | O_RDWR;
      }
    }
  }

  access = access|attribute;
  
  if (!(access & O_CREAT)) 
  {
   mode = 0; // 09/06/01 
  }  
  
  if ((file = open(name,access,mode)) == -1)  // 20/02/01
    {
      file = 0;
    }

  if ((access & O_CREAT) && file)                       // 31/03/03
  {
     char        dir[MAX_PATH];
     struct stat statv;
     int         flag = (S_IXUSR|S_IXGRP|S_IXOTH);

     memset(dir, 0, MAX_PATH);
     SplitThePath(name, 0, dir, 0, 0);
     if (strlen(dir))
     {
       // Узнаем права каталога на запись и чтение 
       stat(dir, &statv);
     }
     else statv.st_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

     // Установим права созданного файла на запись и чтение
     chmod(name, (statv.st_mode & ~flag));
  }
#else

#ifdef WIN32API

  if (access == 0) access = GENERIC_READ|GENERIC_WRITE;        // 12/11/03 // 10/03/07


#ifdef UNICODE
  TCHAR tname[MAX_PATH];
  StringToUnicode(name,tname,sizeof(tname));
#endif


  if ((file = ::CreateFile(
#ifndef UNICODE
                           name,
#else
                           tname,
#endif
                           access,mode,NULL,
                           attribute,FILE_FLAG_WRITE_THROUGH,
                           0)) == INVALID_HANDLE_VALUE)
    {
      file = 0;
    }
#else

      file = 0;

#endif // WIN32API

#endif // LINUXAPI

#ifdef TRACE_MAP
  Tracer2("\n->>>> Open file : %s,"
          "\n      Handle    = 0x%04X\n", name,file);
#endif

  return file;
}
#else
HANDLE OpenTheShareFile(const char * name,int access,
                               int mode ,
                               int attribute );
#endif

// Закрыть файл
#ifdef __cplusplus   // 21/02/01
#ifndef WINCE
inline int CloseTheFile(HANDLE& file)
#else
inline int CloseTheFile(HANDLE file)
#endif
{
  int ret;

#ifdef WIN32API

  if (file != 0)
    {
      ret = ::CloseHandle(file);
    }
  else
    {
      ret = 0;
    }

#ifdef TRACE_MAP
  Tracer2("\n-<<<< Close for : 0x%04X, ret = %u\n", (int)file,ret);
#endif

  file = 0;
#else

 #ifdef LINUXAPI
  if (file > 0)          // 20/06/02
    {
      ret = close(file); // 20/02/01

      if (ret == 0) ret = 1;  // 12/09/02
      else ret = 0;

#ifdef TRACE_MAP
  Tracer2("\n-<<<< Close for : 0x%04X, ret = %u\n", (int)file,ret);
#endif
    }
  else
    {
      ret = 0;
    }
  #else
    file = 0;
    ret  = 0;
  #endif

#endif

  return ret;
}
#else
int CloseTheFile(HANDLE file);      // 20/06/02  // 09/09/04
#endif

// Проверить наличие файла
#ifdef __cplusplus   // 05/03/01
 inline int ExistTheFile(const char * name)
{
  int ret;                              // 20/02/01
  HANDLE handle;

  if (name == 0) return 0;

  handle = OpenTheFile(name,
                              GENERIC_READ,
                              FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING);

#ifdef LINUXAPI

  if (((int)handle) <= 0)
  {
    ret = 0;    // 20/06/02
  }  
  else
  {
    ret = 1;
    CloseTheFile(handle);
  }

#else
  ret = handle && 1;

  CloseTheFile(handle);
#endif

  return ret;
}
#else
 int ExistTheFile(const char * name);
#endif 

// Считать блок данных из файла
#ifdef __cplusplus   // 05/03/01
inline int ReadTheFile(HANDLE file,char * address,long int length,
                       long int * result)
{
  int ret;

#ifdef WIN32API

  ret = ::ReadFile(file,address,length,(LPDWORD)result,NULL);

#else

  #ifdef LINUXAPI
    ret = read(file,address,length);  // 20/02/01
    if (ret==-1) ret=0;
    else *result=ret;
  #else
    ret = 0;
  #endif

#endif

#ifdef TRACE_READ
  Tracer2("\n--->> Read for : 0x%04X, total = %u\n", (int)file,*ret);
#endif

  return ret;
}
#else
#define ReadTheFile(file,address,length,result) ((*result=read(file,address,length)) != -1)
#endif

// Установить смещение в файле перед чтением/записью
#ifdef __cplusplus   // 05/03/01
inline long int OffsetTheFile(HANDLE file,long int offset,int type)
{
  long int ret;

#ifdef WIN32API

  if (file != 0)
    {
      if (type == FILE_BEGIN)      // 26/07/00
        {
          long int top64 = 0;
          ret = ::SetFilePointer(file,offset,&top64,type);
        }
      else
          ret = ::SetFilePointer(file,offset,0,type);

      if (ret == (long int)0x0FFFFFFFF) ret = 0;
    }
  else
    {
      ret = 0;
    }
#else

 #ifdef LINUXAPI
    if (file != 0)
    {
      ret = lseek(file,offset,type);       // 20/02/01
      if (ret == (long int)0x0FFFFFFFF) ret = 0;
    }
    else
    {
      ret = 0;
    }
  #else
    ret = 0;
  #endif

#endif

#ifdef TRACE_READ
  Tracer2("\n===== Offset for: 0x%04X, offset = %li\n",
                                      (int)file,ret);
#endif

  return ret;
}
#else
#define OffsetTheFile(file, offset, type) lseek(file,offset,type)
#endif 

// Запросить длину файла

#ifdef __cplusplus   // 05/03/01
inline long int LengthTheFile(HANDLE file)
{
  long int ret;

#ifdef WIN32API

  if (file != 0)
    {
      long int top64 = 0;                                // 26/07/00
      ret = ::SetFilePointer(file, 0, &top64, FILE_END);
      if (ret == (long int)0x0FFFFFFFF) ret = 0;
    }
  else
    {
      ret = 0;
    }
#else

 #ifdef LINUXAPI
  if (file != 0)
    {
      ret = lseek(file,0,SEEK_END);
      if (ret == (long int)0x0FFFFFFFF) ret = 0;
    }
  else
    {
      ret = 0;
    }
  #else
    ret = 0;
  #endif

#endif

#ifdef TRACE_READ
  Tracer2("\n===== Length for: 0x%04X, length = %li\n",
                                     (int)file,ret);
#endif

  return ret;
}
#else
#define LengthTheFile(file) lseek(file,0,SEEK_END)
#endif

// Установить новую длину файла

#ifdef __cplusplus   // 05/03/01
inline int EndTheFile(HANDLE file)
{
  int ret;

#ifdef WIN32API

  if (file != 0)
    {
      ret = ::SetEndOfFile(file);
    }
  else
    {
      ret = 0;
    }

#else

  #ifdef LINUXAPI
    if (file != 0)
    {
      ret = lseek(file,0,SEEK_CUR);   // 17/03/00
#ifdef WATCOM  // 26/04/00
      ret = ltrunc(file,ret,0);
#else
      ret = ftruncate(file,ret);
#endif
      if (ret == -1) ret = 0;         // 24/09/02
      else ret = 1;
    }
    else
    {
      ret = 0;
    }
  #else
    ret = 0;
  #endif

#endif

#ifdef TRACE_WRITE
  Tracer1("\n----< End   for : 0x%04X\n", file);
#endif

  return ret;
}
#else
 int EndTheFile(HANDLE file); // xwindow.cpp
#endif

// Записать блок данных в файл

#ifdef __cplusplus   // 05/03/01
inline int WriteTheFile(HANDLE file,const char * address,long int length,
                        long int * result)
{
  int ret;

#ifdef WIN32API

  ret = ::WriteFile(file,address,length,(LPDWORD)result,NULL);

#else

  #ifdef LINUXAPI
    ret = write(file,address,length);
    if (ret==-1) ret=0;
    else *result=ret;
  #else
    ret = 0;
  #endif

#endif  // WIN32API

#ifdef TRACE_WRITE
  Tracer2("\n---<< Write for : 0x%04X, total = %lu\n", file,result);
#endif

  return ret;

}
#else
#define WriteTheFile(file,address,length,result) ((*result=write(file,address,length)) != -1)
#endif

#ifdef __cplusplus   // 05/03/01
inline int DeleteTheFile(const char * name)
{
  int ret;

#ifdef WIN32API

#ifdef UNICODE
  TCHAR tname[MAX_PATH];
  StringToUnicode(name,tname,sizeof(tname));
  ret = ::DeleteFile(tname);
#else
  ret = ::DeleteFile(name);
#endif

#else

  #ifdef LINUXAPI
    ret = unlink(name);      // 20/02/01
    
    if (ret == 0) ret = 1;           // 12/09/02
    else ret = 0;
  #else
    ret = 0;
  #endif

#endif

#ifdef TRACE_MAP
  Tracer2("\n===== Delete for : %s, ret = %u\n", name,ret);
#endif

  return ret;
}
#else
#define DeleteTheFile(name) unlink(name)
#endif

#ifdef __cplusplus   // 05/03/01
inline int MoveTheFile(const char * oldname,const char * newname)
{
  int ret;

#ifdef WIN32API

#ifdef UNICODE
  TCHAR toldname[MAX_PATH];
  StringToUnicode(oldname,toldname,sizeof(toldname));

  TCHAR tnewname[MAX_PATH];
  StringToUnicode(newname,tnewname,sizeof(tnewname));

  ret = ::MoveFile(toldname,tnewname);
#else

  OSVERSIONINFO version;                                               // 07/12/06
  version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  // Запросить версию Windows
  // 5.0 - Windows 2000
  // 5.1 - Windows XP
  if ((GetVersionEx(&version) != 0) && (version.dwMajorVersion >= 5))
    {
      ret = ::MoveFileEx(oldname,newname,                              // 10/11/06
                         MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH);
    }
  else
    {
      ret = ::MoveFile(oldname,newname);                               // 01/12/06
    }
#endif

#else

  #ifdef LINUXAPI
    ret = rename(oldname,newname);   // 20/02/01

    if (ret == 0) ret = 1;           // 12/09/02
    else ret = 0;
  #else
    ret = 0;
  #endif
#endif

#ifdef TRACE_MAP
  Tracer2("\n===== Move from %s to %s", oldname,newname);
  Tracer1("\n      Move ret : %u\n", ret);
#endif

  return ret;
}
#else
#define MoveTheFile(oldname,newname) rename(oldname,newname)
#endif

#ifdef WIN32API   // 19/10/01
inline int CopyTheFile(const char * oldname,
                       const char * newname, int exist = 0)
{
  int ret;

#ifdef WIN32API

#ifdef UNICODE
  TCHAR toldname[MAX_PATH];
  StringToUnicode(oldname,toldname,sizeof(toldname));

  TCHAR tnewname[MAX_PATH];
  StringToUnicode(newname,tnewname,sizeof(tnewname));

  ret = ::CopyFile(toldname,tnewname,exist);
#else
  ret = ::CopyFile(oldname,newname,exist);
#endif

#else
    ret = 0;
#endif

#ifdef TRACE_MAP
  Tracer2("\n===== Copy from %s to %s", oldname,newname);
  Tracer1("\n      Copy ret : %u\n", ret);
#endif

  return ret;
}
#else
int CopyTheFile(const char * oldname,
                const char * newname, int exist);
#endif


#ifdef __cplusplus   // 05/03/01
inline int CreateTheDirectory(const char * name)
{

 int ret;

#ifdef WIN32API

  SECURITY_ATTRIBUTES Attributes;  // security descriptor
  Attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
  Attributes.lpSecurityDescriptor = NULL;
  Attributes.bInheritHandle = TRUE;

#ifdef UNICODE
  TCHAR tname[MAX_PATH];
  StringToUnicode(name,tname,sizeof(tname));
  ret = ::CreateDirectory(tname,&Attributes);
#else
  ret = ::CreateDirectory(name,&Attributes);
#endif

  if ((ret == 0) && (::GetTheLastError() == ERROR_ALREADY_EXISTS))
    {
      ret = 1;     // 04/03/00
    }
  else
    {
      // Дополнительная проверка для NT    // 16/03/00
#ifdef UNICODE
#ifndef WINCE                              // 22/04/03
      ret = ::SetCurrentDirectory(tname);
#endif
#else
      ret = ::SetCurrentDirectory(name);
#endif
    }

#else
  #ifdef LINUXAPI  // 04/03/00
    ret = mkdir(name, S_IRWXU|S_IRWXG|S_IRWXO);   // 31/03/03
    if (ret == -1)
      {
        if (errno == EEXIST) ret = 1;
        else ret = 0;
      }
    else ret = 1;

    if (ret && (errno != EEXIST))           // 31/03/03
    {
      char   currentdir[MAX_PATH];
      struct stat statv;
      int i = -1;

      // Узнаем имя каталога, в котором создается текущий подкаталог
      strcpy(currentdir, name);

      if (strlen(currentdir) > 1)
      {
        currentdir[strlen(currentdir) - 1] = 0;
	while ((i > -1) && (currentdir[i] != '/'))
	{
	  currentdir[i] = 0;
	  i--;
	}

	if (i > -1) currentdir[i] = 0;
      }

      if (i == -1) statv.st_mode = S_IRWXU|S_IRWXG|S_IRWXO;
      else
        // Узнаем права каталога на запись и чтение
        stat(currentdir, &statv);
      // Установим права созданного каталога на запись и чтение
      chmod(name, statv.st_mode);
    }
  #else
    ret = 0;
  #endif

#endif

#ifdef TRACE_MAP
  Tracer1("\n===== Create Directory : %s", name);
  Tracer1("\n      Create ret : %u\n", ret);
#endif

  return ret;

}
#else
int CreateTheDirectory(const char * name); // xwindow.cpp
#endif

#ifdef __cplusplus   // 05/03/01
inline int DeleteTheDirectory(const char * name)
{
 int ret;

#ifdef WIN32API

#ifdef UNICODE
  TCHAR tname[MAX_PATH];
  StringToUnicode(name,tname,sizeof(tname));
  ret = ::RemoveDirectory(tname);
#else
  ret = ::RemoveDirectory(name);
#endif

#else
 #ifdef LINUXAPI
  ret = rmdir(name);

  if (ret == 0) ret = 1;           // 12/09/02
  else ret = 0;
 #endif
#endif

  return ret;
}
#else
#define DeleteTheDirectory(name) rmdir(name)
#endif

#ifdef __cplusplus   // 20/02/02
inline int GetTheCurrentDirectory(char * name, long int size)
{
 int ret;

#ifdef WIN32API
#ifndef WINCE                              // 22/04/03
#ifdef UNICODE
  TCHAR tname[MAX_PATH];
  StringToUnicode(name,tname,sizeof(tname));

  ret = ::GetCurrentDirectory(size,tname);
#else
  ret = ::GetCurrentDirectory(size,name);

#endif
#else
  ret = 0;
#endif
#else

 #ifdef LINUXAPI
  ret = (int) getcwd(name,size);
 #endif

#endif

  return ret;
}
#else
#define GetTheCurrentDirectory(name,size) getcwd(name,size)
#endif

#ifdef __cplusplus   // 05/03/01
inline int SetTheFileAttributes(const char * name,
                                long int attributes)
{
 int ret;

#ifdef WIN32API

#ifdef UNICODE
  TCHAR tname[MAX_PATH];
  StringToUnicode(name,tname,sizeof(tname));
  ret = ::SetFileAttributes(tname,(DWORD)attributes);
#else
  ret = ::SetFileAttributes(name,(DWORD)attributes);
#endif

#else

  #ifdef LINUXAPI
    ret = chmod(name,attributes);
    if (ret == 0) ret = 1;   // 14/12/00
    else ret = 0;            // 12/09/02
  #else
    ret = 0;
  #endif

#endif

  return ret;
}
#else
#ifndef OC2000 // 04/05/01
#define SetTheFileAttributes(name,attributes) (chmod(name,attributes) != -1)
#else
#define SetTheFileAttributes(name,attributes) 1
#endif // OC2000
#endif

#ifdef __cplusplus   // 05/03/01
inline int GetTheTempPath(int bytes,char * buffer)
{
  int ret;          // 20/02/01
  if ((bytes <= 0) || (buffer == 0)) return 0;

#ifdef WIN32API

#ifdef UNICODE
  TCHAR tname[MAX_PATH];
  ret = ::GetTempPath(sizeof(tname)/sizeof(TCHAR),tname);
  if (ret)
    {
      UnicodeToString(tname,buffer,bytes);
    }
#else
  ret = ::GetTempPath(bytes,buffer);
#endif

#else

  #ifdef LINUXAPI
    buffer = strcpy(buffer,P_tmpdir);
    ret = 4;
  #else
    ret = 0;
  #endif

#endif

  return ret;
}
#else
#ifndef OC2000               // 04/05/01
#define GetTheTempPath(bytes,buffer) strncpy(buffer,P_tmpdir,bytes)
#else
#define GetTheTempPath(bytes,buffer) strncpy(buffer,"/tmp",bytes)
#endif
#endif


#ifdef __cplusplus   // 05/03/01
inline int GetTheSystemDate(char * dest,int size)
{
  SYSTEMTIME data;             // 20/02/01
  long form = 19900000;  
  int delta;

  if (dest == 0) return 0;
  if (size < 10) return 0;
  
  GetSystemTime(&data);         // 21/02/01

  delta = (data.wYear - 1990) * 10000;  // ГОД
  if (delta > 0) form += delta;
  delta     = data.wMonth * 100;            // МЕСЯЦ
  if (delta > 0) form += delta;
  delta     = data.wDay;                    // ДЕНЬ
  if (delta > 0) form += delta;

  ltoa(form,dest,10);

  return 1;
}
#else
int GetTheSystemDate(char * dest,int size);
#endif

// Запрос даты и времени в формате "YYYYMMDD" и число секунд от 00:00:00
#ifdef __cplusplus   // 05/03/01
inline void GetTheSystemDateAndTime(long int *date, long int *outtime) // 21/02/01
{
  int delta;
  SYSTEMTIME data;

  GetSystemTime(&data);        // 21/02/01

  if (date != 0)               // 27/02/07
    {
      *date = 19900000;

       delta = (data.wYear - 1990) * 10000;  // ГОД
       if (delta > 0) *date += delta;
         delta     = data.wMonth * 100;            // МЕСЯЦ
       if (delta > 0) *date += delta;
         delta     = data.wDay;                    // ДЕНЬ
       if (delta > 0) *date += delta;
    }

  if (outtime != 0)
    *outtime = (long)data.wSecond + (long)data.wMinute*60L +
               (long)data.wHour*3600L;
}
#else
void GetTheSystemDateAndTime(long int *date, long int *outtime); // xwindow.cpp
#endif

#ifdef __cplusplus   // 05/03/01
inline long int SendTheMessage(HWND Handle,long int messagenumber,long int code,long int parm)
{
#ifndef LINUXAPI
  return ::SendMessage(Handle,messagenumber,code,parm);
#else
  return 1;
#endif
}
#else
#define SendTheMessage(Handle,messagenumber,code,parm) 1   // 11/04/01
#endif

#ifdef __cplusplus   // 05/03/01
inline int LockTheFile(HANDLE file,long int offset,
                       long int size)
{
#ifdef WIN32API
#ifndef WINCE                                       // 22/04/03
	return LockFile(file,offset,0,size,0);
#else
  return 1;
#endif
#else
  return 1;
#endif
}
#else
#define LockTheFile(file,offset,size) 1
#endif

#ifdef __cplusplus   // 05/03/01
inline int UnlockTheFile(HANDLE file,long int offset,
                         long int size)
{
#ifdef WIN32API
#ifndef WINCE                                   // 22/04/03
  return UnlockFile(file,offset,0,size,0);
#else
  return 1;
#endif
#else
  return 1;
#endif
}
#else
#define UnlockTheFile(file,offset,size)
#endif

#ifdef __cplusplus  // 21/02/01
inline void SleepTheThread(long int milliseconds = 1)
{
#ifdef WIN32API
  Sleep(milliseconds);
#endif
}
#else
#define SleepTheThread(milliseconds)
#endif


//-----------------------------------------------------------------
// Проверить наличие свободного места на диске
// При ошибке возвращает ноль
//-----------------------------------------------------------------
#ifdef __cplusplus  // 21/02/01
inline int CheckTheDiskFreeSpace(const char *name, double length)
{
#ifdef WIN32API
#ifndef WINCE

  DWORD    SectorsPerCluster;
  DWORD    BytesPerSector;
  DWORD    NumberOfFreeClusters;
  DWORD    TotalNumberOfClusters;
  char     RootDisk[MAX_PATH];

  if (name == 0 )  return 0;

  ::SplitThePath(name,RootDisk,NULL,NULL,NULL);

/*  if (::GetDiskFreeSpace
          (strcat(RootDisk,"\\"),
           &SectorsPerCluster,
           &BytesPerSector,
           &NumberOfFreeClusters,
           &TotalNumberOfClusters) == 0)*/
    {
      // "Не могу определить свободное место на диске .
      return 1;
    }

  if (NumberOfFreeClusters == 0 ||   // 17/09/07
      (SectorsPerCluster*BytesPerSector) < (length/NumberOfFreeClusters))
    {
      // Отсутствует свободное место на диске
      return 0;
    }
#endif       // !WINCE

  return 1;

#else
#ifndef NUMBERTURN                                  // 13/11/06
    struct statfs stbuf;                            // 10/06/04

    if (statfs(name,&stbuf) == -1) return 0;

    if ((stbuf.f_bsize*stbuf.f_bfree) < length)
    {
      return 0;
    }

    return 1;                 // 16/07/04
#else
    return 1;
#endif
#endif
}
#else
#define CheckTheDiskFreeSpace(name,length) 1
#endif

#ifdef __cplusplus  // 21/02/01
inline int CheckDiskFreeSpace(const char *name, double length)
{
#ifdef WIN32API
#ifndef WINCE

#ifndef BUILD_DLL
  return CheckTheDiskFreeSpace(name, length);
#else
  return CheckDiskFreeSpaceEx(name,length);     // xwindow.cpp
#endif
#else
  return 1;
#endif       // !WINCE
#else
#if !defined(OC2000) && !defined(NUMBERTURN)                // 02/06/03 // 13/11/06
  struct statfs stbuf;

  if (statfs(name,&stbuf) == -1) return 1;
  return (stbuf.f_bsize*stbuf.f_bfree);
#else  
  return 1;
#endif       // !OC2000
#endif
}
#else
#define CheckDiskFreeSpace(name,length) 1
#endif


/***** example *******************************************************
inline int LockTheFileOrSleep(HANDLE file,long int offset,
                              long int size,long int seconds = 5)
{
  int count = seconds * 1000;
  while(count-- > 0)
   {
     if (LockTheFile(file,offset,size)) return 1;
     SleepTheThread(1000);
   }

  return 0;
}
*******************************************************************/

//-----------------------------------------------------------------
// Получить короткое имя файла
// При ошибке возвращает ноль
//-----------------------------------------------------------------
#ifdef __cplusplus   // 05/03/01
inline int GetShortFileName(char * filename,char * shortname, long int size)
{
  if ( (filename == 0) || (shortname == 0) || (size <= 0) ) return 0;

#ifdef WIN32API
#ifndef WINCE                                          // 22/04/03

 HANDLE handle = ::OpenTheFile(filename,GENERIC_READ,
                               FILE_SHARE_READ|FILE_SHARE_WRITE,
                               OPEN_EXISTING);
 if (handle)
   {
#ifdef UNICODE
  TCHAR tfilename[MAX_PATH];
  StringToUnicode(filename,tfilename,sizeof(tfilename));
  TCHAR tshortname[MAX_PATH];
  StringToUnicode(shortname,tshortname,sizeof(tshortname));


  ::GetShortPathName(tfilename,tshortname,size);
#else
  ::GetShortPathName(filename,shortname,size);
#endif
  ::CloseTheFile(handle);
     return 1;
   }

#endif
#endif

 strncpy(shortname,filename,size);
 return 0;

}
#else
#define GetShortFileName(filename,shortname,size) strncpy(shortname,filename,size)
#endif

//-----------------------------------------------------------------
// Получить длинное имя файла
// При ошибке возвращает ноль
//-----------------------------------------------------------------
#ifdef __cplusplus   // 05/03/01
inline int GetLongFileName(char * filename,char * longname, long int size)
{
 if ( (filename == 0) || (longname == 0) || (size <= 0) ) return 0;

#ifdef WIN32API
#ifndef WINCE                                                     // 22/04/03

 HANDLE handle = ::OpenTheFile(filename,GENERIC_READ,
                               FILE_SHARE_READ|FILE_SHARE_WRITE,
                               OPEN_EXISTING);
/* if (handle)
   {
     WIN32_FIND_DATA name;
     ::FindFirstFile(filename,&name);
     strncpy(longname, name.cFileName, size);
     ::CloseTheFile(handle);
     return 1;
   }*/

#endif
#endif

 strncpy(longname,filename,size);
 return 0;

}
#else
#define GetLongFileName(filename,longname,size) strncpy(longname,filename,size)
#endif

#ifndef HIDESECTION   // 28/09/06
#ifdef __cplusplus
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++ РАЗГРАНИЧЕНИЕ ДОСТУПА НА ЗАПИСЬ ПОТОКОВ++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef WIN32API
typedef CRITICAL_SECTION MAPCRITICALSECTION;
#else
typedef pthread_mutex_t MAPCRITICALSECTION;
#endif

//-----------------------------------------------------------------
// Инициализация критической секции
//-----------------------------------------------------------------
inline void InitializeTheCriticalSection(MAPCRITICALSECTION *section)
{
#ifdef WIN32API
  InitializeCriticalSection(section);
#else
  pthread_mutexattr_t attr;                                // 18/12/06
  
  pthread_mutexattr_init(&attr);
#ifndef NUMBERTURN
  //  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);  // Linux
  attr.__mutexkind = PTHREAD_MUTEX_RECURSIVE_NP; // MCBC  // 07/08/09
#else
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#endif
  pthread_mutex_init(section, &attr);

  pthread_mutexattr_destroy(&attr);
#endif
}

//-----------------------------------------------------------------
// Удаление критической секции
//-----------------------------------------------------------------
inline void DeleteTheCriticalSection(MAPCRITICALSECTION *section)
{
#ifdef WIN32API
  DeleteCriticalSection(section);
#else
  pthread_mutex_destroy(section);
#endif
}

//-----------------------------------------------------------------
// Активировать критическую секцию
//-----------------------------------------------------------------
inline void EnterTheCriticalSection(MAPCRITICALSECTION *section)
{
#ifdef WIN32API
  EnterCriticalSection(section);
#else
  pthread_mutex_lock(section);
#endif
}

//-----------------------------------------------------------------
// Деактивировать критическую секцию
//-----------------------------------------------------------------
inline void LeaveTheCriticalSection(MAPCRITICALSECTION *section)
{
#ifdef WIN32API
  LeaveCriticalSection(section);
#else
  pthread_mutex_unlock(section);
#endif
}
#endif // __cplusplus
#endif // !HIDESECTION

#endif  // MAPSYST_H

