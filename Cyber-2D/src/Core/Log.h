#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Cyber {

	class Log
	{
	public:
		static void Init();

		static void Setlimits(int size, int capacity = -1) {
			s_Size = size;
			if (capacity < 1)
				s_Capacity = 4 * size;
			else
				s_Capacity = capacity;
		};
		static void Clear() { s_Contents.clear(); }
		static std::pair<std::string*, int> GetContents();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static int s_Size;
		static int s_Capacity;
		static std::vector<std::string> s_Contents;
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		friend class VectorSink;
	};

}

#ifdef CB_CONSOLE
// Core log macros
#define CB_CORE_TRACE(...)       ::Cyber::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CB_CORE_INFO(...)        ::Cyber::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CB_CORE_WARN(...)        ::Cyber::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CB_CORE_ERROR(...)       ::Cyber::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CB_CORE_CRITICAL(...)    ::Cyber::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CB_CORE_ASSERT(test,...) if(!(test)){::Cyber::Log::GetCoreLogger()->critical(__VA_ARGS__);__debugbreak();}
#else
// Core log macros
#define CB_CORE_TRACE(...)
#define CB_CORE_INFO(...)
#define CB_CORE_WARN(...)
#define CB_CORE_ERROR(...)
#define CB_CORE_CRITICAL(...)
#define CB_CORE_ASSERT(test,...)
#endif

// Client log macros
#define CB_TRACE(...)         ::Cyber::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CB_INFO(...)          ::Cyber::Log::GetClientLogger()->info(__VA_ARGS__)
#define CB_WARN(...)          ::Cyber::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CB_ERROR(...)         ::Cyber::Log::GetClientLogger()->error(__VA_ARGS__)
#define CB_CRITICAL(...)      ::Cyber::Log::GetClientLogger()->critical(__VA_ARGS__)
#define CB_ASSERT(test,...)   if(!(test)){::Cyber::Log::GetClientLogger()->critical(__VA_ARGS__);__debugbreak();}
