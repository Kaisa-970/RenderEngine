#pragma once
#include <memory>

//#ifdef _WIN32
//#ifdef _WIN64
//#define PK_PLATFORM_WINDOWS
//#endif
//#endif
#ifdef PK_PLATFORM_WINDOWS
#ifdef PK_DYNAMIC_LINK
	#ifdef PK_BUILD_DLL
		#define PKENGINE_API __declspec(dllexport)
	#else
		#define PKENGINE_API __declspec(dllimport)
	#endif
#else
	#define PKENGINE_API
#endif
#else
	#error PK only support windows!
#endif

#ifdef PK_ENABLE_ASSERTS
	#define PK_ASSERT(x,...) {if(!(x)) {PK_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}
	#define PK_CORE_ASSERT(x,...) {if(!(x)) {PK_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}
#else
	#define PK_ASSERT(x,...)
	#define PK_CORE_ASSERT(x,...)
#endif

#define BIT(x) (1<<x)

#define PK_BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)

namespace PKEngine {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T,typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}