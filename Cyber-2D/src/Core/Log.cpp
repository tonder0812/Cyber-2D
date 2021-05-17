#include "pch.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "spdlog/sinks/base_sink.h"

namespace Cyber {
	int Log::s_Size = 1000;
	int Log::s_Capacity = 4000;
	std::vector<std::string> Log::s_Contents;
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	class VectorSink : public spdlog::sinks::base_sink<std::mutex>
	{
	public:
		VectorSink(std::vector<std::string>& storage) :m_Storage(storage) {
			m_Storage.reserve(Log::s_Capacity);
		}
		~VectorSink() {}
		virtual void sink_it_(const spdlog::details::log_msg& msg) {
			spdlog::memory_buf_t formatted;
			formatter_->format(msg, formatted);
			m_Storage.emplace_back(formatted.data(), formatted.size());
			if (m_Storage.size() > Log::s_Capacity) {
				m_Storage.erase(m_Storage.begin(), m_Storage.end() - Log::s_Size);
			}
		}
		virtual void flush_() {}
	private:
		std::vector<std::string>& m_Storage;
	};

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Cyber.log", true));
		logSinks.emplace_back(std::make_shared<VectorSink>(s_Contents));

		logSinks[0]->set_pattern("[%T] [%l] %n: %v");
		logSinks[1]->set_pattern("%L[%T] %n: %v%$");
#ifdef CB_CONSOLE
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks[2]->set_pattern("%^[%T] %n: %v%$");
#endif

		s_CoreLogger = std::make_shared<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	}
	std::pair<std::string*, int> Log::GetContents() {
		if (s_Contents.size() == 0) return{ nullptr,0 };
		if (s_Contents.size() < s_Size) return { &s_Contents[0],s_Contents.size() };
		int start = s_Contents.size() - s_Size;
		if (start < 0) start = 0;
		return { &s_Contents[start],s_Contents.size() - start };
	}
}

