#if !defined(__GUI_config_textedit_h)
#define __GUI_config_textedit_h
/* ==================================================================== */
/*      We will use WIN32 as a standard windows define.                 */
/* ==================================================================== */
#if defined(_WIN32) && !defined(WIN32)
    #define WIN32
#endif

#if defined(_WINDOWS) && !defined(WIN32)
    #define WIN32
#endif



#include <windows.h>
#pragma warning (disable:4251)
#pragma warning (disable:4530)
#ifdef __USE_DLL_TEXTEDIT

	#define __EXPORT_TEXTEDIT __declspec(dllexport)
#else
	#define __EXPORT_TEXTEDIT __declspec(dllimport)
#endif
#else
	#define __EXPORT_TEXTEDIT
#endif

#include <math.h>
#include <stdio.h>



