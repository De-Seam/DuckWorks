#pragma once
// Core headers
#include "Core/Utilities/SafeQueue.h"
#include "Core/Utilities/Utilities.h"

enum class LogType
{
	Info = 0,
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
	LogManager();

	void Init();
	void Shutdown();

	void Log(LogType inLogType, const char* fmt...);
	void Log(LogType inLogType, const char* fmt, va_list args);

	void CleanLogQueue(bool inErrorOnly = false);

	Mutex& GetLogMutex() { return mLogMutex; }

	const std::string& GetLogString() const { return mOutputLog; }

	struct LogEntry
	{
		LogType mType;
		String mMessage;
	};

	const Array<LogEntry>& GetLogArray() const;
	void SetLogFilePath(const String& inFilePath);
	void SetLogFileName(const String& inFileName);

	void WriteLogToFile();

private:
	void LogThreadLoop();

	bool mThreadRunning = false;
	std::thread mLogThread = {};
	SafeQueue<LogQueueItem> mLogQueue = {};

	uint64 mMaxOutputLogSize = 81920;

	Mutex mLogMutex;
	Array<LogEntry> mLogEntries = {};
	String mOutputLog;
	String mLogFilePath = "Debug/Logs/";
	String mLogFileName = "Log";
	String mLogFileExtension = ".Log";
};
