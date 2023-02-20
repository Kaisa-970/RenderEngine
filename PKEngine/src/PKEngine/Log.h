#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace PKEngine {

	class PKENGINE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define PK_CORE_ERROR(...)	PKEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PK_CORE_WARN(...)	PKEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PK_CORE_INFO(...)	PKEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PK_CORE_TRACE(...)	PKEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PK_CORE_FATAL(...)	PKEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define PK_ERROR(...)	PKEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define PK_WARN(...)		PKEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PK_INFO(...)		PKEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define PK_TRACE(...)	PKEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PK_FATAL(...)	PKEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)

