#include "pch.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Cyber {

	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define CB_CORE_TRACE(...)    ::Cyber::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CB_CORE_INFO(...)     ::Cyber::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CB_CORE_WARN(...)     ::Cyber::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CB_CORE_ERROR(...)    ::Cyber::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CB_CORE_CRITICAL(...) ::Cyber::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define CB_TRACE(...)         ::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CB_INFO(...)          ::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define CB_WARN(...)          ::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CB_ERROR(...)         ::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define CB_CRITICAL(...)      ::Hazel::Log::GetClientLogger()->critical(__VA_ARGS__)

