#pragma once
// Core headers
#include "Core/Utilities/SafeQueue.h"
#include "Core/Utilities/Utilities.h"

enum class LogType
{
	Info,
	Warning,
	Error
};

void gLog(const char* fmt...);
void gLog(LogType inLogType, const char* fmt...);

class LogManager;

extern LogManager gLogManager;

struct LogQueueItem
{
	LogType logType;
	std::string msg;
};

class LogManager
{
public:
	static void Init();
	static void Shutdown();

	static void Log(LogType inLogType, const char* fmt...);
	static void Log(LogType inLogType, const char* fmt, va_list args);

	enum class ConsoleColor : int32
	{
		White = 15,
		Yellow = 14,
		Red = 4,
		RedWhite = 0x0040 + 15
	};

	static void SetConsoleColor(int32 inColor);

	static void CleanLogQueue(bool inErrorOnly = false);

	static const std::string& GetLog() { return mOutputLog; }
	static void SetLogFilePath(const String& inFilePath);
	static void SetLogFileName(const String& inFileName);

	static void WriteLogToFile();

private:
	static void LogThreadLoop();

	static bool mThreadRunning;
	static std::thread mLogThread;
	static SafeQueue<LogQueueItem> mLogQueue;

	static uint64 mMaxOutputLogSize;

	static String mOutputLog;
	static String mLogFilePath;
	static String mLogFileName;
	static String mLogFileExtension;
};
