#pragma once

#ifdef PK_PLATFORM_WINDOWS
	#ifdef PK_BUILD_DLL
		#define PKENGINE_API __declspec(dllexport)
	#else
		#define PKENGINE_API __declspec(dllimport)
	#endif
#else
	#error PK only support windows!
#endif