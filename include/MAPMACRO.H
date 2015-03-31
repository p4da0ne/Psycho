/******  MAPMACRO.H  ***********  Belenkov O.V. ******  09/09/05 ***
*******  MAPMACRO.H  ***********  Dishlenko S.G.******  08/04/99 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2005              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*                  ОПИСАНИЕ МАКРОРАСШИРЕНИЙ                        *
*                                                                  *
*******************************************************************/

#if !defined(MAPMACRO_H)
#define MAPMACRO_H

#ifdef BUILD_TIM              // 29/10/01
 #define BUILD_DLL
#endif

#ifndef MAPSYST_H
 #include "mapsyst.h"
#endif

#define HUGER  // Устаревшее определение

// Идентификаторы сообщений процессов обработки карты

#define IMM_OBJECT 0x585
#define IMM_LIST   0x586
#define IMM_ERROR  0x587

#ifndef M_PI
  #define M_PI        3.14159265358979323846
  #define M_PI_2      1.57079632679489661923
  #define M_PI_4      0.785398163397448309616
#endif


// Определяется экспорт/импорт классов и функций (_DLLCLASS)
#if defined(BUILD_DLL)

    #if defined(__BORLANDC__) // Для Borland C++
      #define _DLLCLASS _export
      #define _MAPAPI   _export WINAPI
      #define HIDETRY                             // 19/09/01
      #define _MAPIMP
    #else                     // Для Visual C++
      #ifndef WINCE
        #if defined(LINUXAPI)
          #define _DLLCLASS
          #define _MAPAPI   WINAPI
          #define _MAPIMP
        #else
          #define _DLLCLASS __declspec( dllexport )
          #define _MAPAPI   WINAPI
          #define _MAPIMP   __declspec( dllexport )
        #endif
      #else
        #define _DLLCLASS
        #define _MAPAPI   __declspec( dllexport ) WINAPI
        #define _MAPIMP
      #endif
    #endif  //  __BORLANDC__


#ifndef BUILD_TIM              // 29/10/01
#ifdef  HIDENEW                // 23/07/04

    #if defined(__BORLANDC__) // Для Borland C++

    // Применение для универсальной DLL new и delete - опасно !

    inline void * operator new (size_t value)
     {
       return (void *) ::AllocateTheMemory((int) value);
     }

    inline void * operator new[] (size_t value)
     {
       return (void *) ::AllocateTheMemory((int) value);
     }

    inline void operator delete (void * point)
     {
       if (point)
        ::FreeTheMemory((char *)point);
     }

    inline void operator delete[] (void * point)
     {
       if (point)
        ::FreeTheMemory((char *)point);
     }

    #endif  //  __BORLANDC__

#endif // HIDENEW 
#endif // !BUILD_TIM

#else

    #if defined(__BORLANDC__) // Для Borland C++
      #define _DLLCLASS _import
      #define _MAPAPI   _import WINAPI
      #define _MAPIMP
    #else                     // Для Visual C++
      #ifndef WINCE
        #if defined(LINUXAPI)
          #define _DLLCLASS
          #define _MAPAPI   WINAPI
          #define _MAPIMP
        #else
          #define _DLLCLASS __declspec( dllimport )
          #define _MAPAPI   WINAPI
          #define _MAPIMP   __declspec(dllimport)
        #endif
      #else
        #define _DLLCLASS
        #define _MAPAPI   __declspec( dllimport ) WINAPI
        #define _MAPIMP
      #endif
    #endif


#ifdef GISOBJMEM                     // 09/09/05
#ifndef GISALLOC
#define GISALLOC

    // В универсальных библиотеках new/delete должны иметь единую реализацию

extern "C"
{
    char * _MAPAPI mapAllocate(long int value);
    void   _MAPAPI mapFree(char * memory);
}

    inline void * operator new[] (size_t value)
     {
       return (void *) ::AllocateTheMemory((int) value);
     }

    inline void operator delete[] (void * point)
     {
       if (point)
        ::FreeTheMemory((char *)point);
     }

    inline void * operator new (size_t value)
     {
       return (void *) ::mapAllocate((int) value);
     }

    inline void operator delete (void * point)
     {
       if (point)
        ::mapFree((char *)point);
     }
#endif
#endif

#endif // BUILD_DLL


#endif // MAPMACRO_H
