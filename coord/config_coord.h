#if !defined(__GUI_config_coord_h)
#define __GUI_config_coord_h
/* ==================================================================== */
/*      We will use WIN32 as a standard windows define.                 */
/* ==================================================================== */
#if defined(_WIN32) && !defined(WIN32)
    #define WIN32
#endif

#if defined(_WINDOWS) && !defined(WIN32)
    #define WIN32
#endif


#ifdef WIN32

#include <windows.h>
#pragma warning (disable:4251)
#pragma warning (disable:4530)
#ifdef __USE_DLL_COORD

	#define __EXPORT_COORD __declspec(dllexport)
#else
	#define __EXPORT_COORD __declspec(dllimport)
#endif
#else
	#define __EXPORT_COORD
#endif

#endif
