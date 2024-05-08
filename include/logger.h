
#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <format>

//#if _DEBUG
//#define LOG(level, source, message) glacier::Logger::add(level, source "(" __FILE__ ")", message)
//#else
#define LOG(level, source, message) glacier::Logger::add(level, source, message)
//#define LOG(level, source, message) glacier::Logger::add(level, __func__, message)
//#endif

namespace glacier {
	
	struct Log {
		enum Level {
			Debug,
			Info,
			Warning,
			Error
		} level;

		std::chrono::system_clock::time_point time;
		std::string source;
		std::string message;

		std::string ToString() {
			std::stringstream ss;
			ss << std::format("{:%d.%m.%Y %H:%M:%S}", time);
			ss << " | " << source << " | ";
			switch (level) {
			case Debug:
				ss << "DEBUG";
				break;
			case Info:
				ss << "INFO";
				break;
			case Warning:
				ss << "WARNING";
				break;
			case Error:
				ss << "ERROR";
				break;
			}
			ss << " | " << message;
			return ss.str();
		}
	};

	class Logger {
	public:
		static void add(Log::Level level, std::string source, std::string message);

		static Log::Level minimumLevel;

	protected:
		static std::vector<Log> m_logs;
	};

}
