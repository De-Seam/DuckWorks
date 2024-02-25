#include "Precomp.h"
#include "Core/Log/Log.h"

#include <cstdarg>
#include <iostream>
#include <WTypesbase.h>

void gLog(const char* fmt ...)
{
	va_list args;
	va_start(args, fmt);
	LogManager::Log(LogType::Info, fmt, args);
	va_end(args);
}

void gLog(LogType inLogType, const char* fmt ...)
{
	va_list args;
	va_start(args, fmt);
	LogManager::Log(inLogType, fmt, args);
	va_end(args);
}

bool LogManager::mThreadRunning = false;
std::thread LogManager::mLogThread = {};
SafeQueue<LogQueueItem> LogManager::mLogQueue = {};

String LogManager::mOutputLog;
String LogManager::mLogFilePath;
String LogManager::mLogFileName;

void LogManager::Init()
{
	mOutputLog.reserve(8192);

	mLogThread = std::thread(LogThreadLoop);
}

void LogManager::Shutdown()
{
	// We have to log something here so the thread which is waiting for a log message can exit.
	Log(LogType::Info, "LogManager shutting down.");
	mThreadRunning = false;
	mLogThread.join();
}

void LogManager::Log(LogType logType, const char* fmt ...)
{
	va_list args;
	va_start(args, fmt);
	Log(logType, fmt, args);
	va_end(args);
}

void LogManager::Log(LogType inLogType, const char* fmt, va_list args)
{
	String msg;
	msg.reserve(128);

	switch (inLogType)
	{
	case LogType::Info:
		msg = "[I] ";
		break;
	case LogType::Warning:
		msg = "[W] ";
		break;
	case LogType::Error:
		msg = "[E] ";
		break;
	}

	while (*fmt != '\0')
	{
		if (*fmt == '%')
		{
			++fmt;
			if (*fmt == 'i' || *fmt == 'I')
			{
				int i = va_arg(args, int);
				msg += std::to_string(i);
			}
			else if (*fmt == 'u' || *fmt == 'U')
			{
				uint32 i = va_arg(args, uint32);
				msg += std::to_string(i);
			}
			else if (*fmt == 'f' || *fmt == 'F')
			{
				double f = va_arg(args, double);
				msg += std::to_string(f);
			}
			else if (*fmt == 'c' || *fmt == 'C')
			{
				const char c = va_arg(args, char);
				msg += c;
			}
			else if (*fmt == 's' || *fmt == 'S')
			{
				std::string s = va_arg(args, const char*);
				msg += s;
			}
			else if (*fmt == 'v' || *fmt == 'V')
			{
				++fmt;
				if (*fmt == '2')
				{
					fm::vec2 v2 = va_arg(args, fm::vec2);
					msg += std::to_string(v2.x) + ", " + std::to_string(v2.y);
				}
				else if (*fmt == '3')
				{
					fm::vec3 v3 = va_arg(args, fm::vec3);
					msg += std::to_string(v3.x) + ", " + std::to_string(v3.y) + ", " + std::to_string(v3.z);
				}
			}
		}
		else
		{
			msg += *fmt;
		}
		++fmt;
	}

	msg += '\n';

	LogQueueItem logQueueItem = {inLogType, msg};
	mLogQueue.enqueue(logQueueItem);
}

void LogManager::SetConsoleColor(int32 inColor)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, static_cast<WORD>(inColor));
}

void LogManager::SetLogFilePath(const String& inFilePath)
{
	mLogFilePath = inFilePath;
}

void LogManager::SetLogFileName(const String& inFileName)
{
	mLogFileName = inFileName;
}

void LogManager::LogThreadLoop()
{
	mThreadRunning = true;
	while (mThreadRunning)
	{
		LogQueueItem queueItem = mLogQueue.dequeue();

		switch (queueItem.logType)
		{
		case LogType::Info:
			SetConsoleColor(static_cast<int32>(ConsoleColor::White));
			break;
		case LogType::Warning:
			SetConsoleColor(static_cast<int32>(ConsoleColor::Yellow));
			break;
		case LogType::Error:
			SetConsoleColor(static_cast<int32>(ConsoleColor::Red));
			break;
		}

		std::cout << queueItem.msg;
		mOutputLog += queueItem.msg;

		SetConsoleColor(static_cast<int32>(ConsoleColor::White));
	}
}
