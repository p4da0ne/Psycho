#if !defined(config_my_mapaccess_h)
#define config_my_mapaccess_h

#ifdef __USE_DLL_MY_MAPACCESS

	#define __EXPORT_MY_MAPACCESS __declspec(dllexport)
#else
	#define __EXPORT_MY_MAPACCESS __declspec(dllimport)
#endif
#else
	#define __EXPORT_MY_MAPACCESS
#endif
