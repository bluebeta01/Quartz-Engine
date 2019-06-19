#pragma once
#include "framework/pch.h"

class LogBackend
{
public:
	template <class T>
	static void inline logDebug(const char* file, std::size_t line, T message)
	{
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console, 15);
		std::cout << "[DEBUG][" << file << ", " << line << "]: " << message << "\n";
	}
	template <class T>
	static void inline logError(const char* file, std::size_t line, T message)
	{
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console, 12);
		std::cout << "[ERROR][" << file << ", " << line << "]: " << message << "\n";
	}
	template <class T>
	static void inline logWarning(const char* file, std::size_t line, T message)
	{
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console, 14);
		std::cout << "[WARNING][" << file << ", " << line << "]: " << message << "\n";
	}
};

#define LOGDEBUG(message) LogBackend::logDebug(__FILE__, __LINE__, message)
#define LOGWARNING(message) LogBackend::logWarning(__FILE__, __LINE__, message)
#define LOGERROR(message) LogBackend::logError(__FILE__, __LINE__, message)