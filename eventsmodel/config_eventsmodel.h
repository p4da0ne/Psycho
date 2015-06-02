#if !defined(__GUI_config_eventsmodel_h)
#define __GUI_config_eventsmodel_h
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
#ifdef __USE_DLL_EVENTSMODEL

	#define __EXPORT_EVENTSMODEL __declspec(dllexport)
#else
	#define __EXPORT_EVENTSMODEL __declspec(dllimport)
#endif
#else
	#define __EXPORT_EVENTSMODEL
#endif

#endif

