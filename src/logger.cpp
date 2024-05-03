
#include "../include/logger.h"

#include <iostream>

#if _DEBUG
glacier::Log::Level glacier::Logger::minimumLevel = glacier::Log::Level::Debug;
#else
glacier::Log::Level glacier::Logger::minimumLevel = glacier::Log::Level::Info;
#endif

std::vector<glacier::Log> glacier::Logger::m_logs = {};

void glacier::Logger::add(glacier::Log::Level level, std::string source, std::string message) {
	Log log;
	log.time = std::chrono::system_clock::now();
	log.source = source;
	log.message = message;
	log.level = level;

	if (level >= minimumLevel) {
		std::cout << log.ToString() << std::endl;
	}
	
	m_logs.push_back(log);
}
