#pragma once
// Core headers
#include "Core/Manager/Manager.h"
#include "Core/Utilities/SafeQueue.h"
#include "Core/Utilities/Utilities.h"

enum class ELogType
{
	Info = 0,
	Warning,
	Error
};

void gLog(const char* fmt...);
void gLog(ELogType inLogType, const char* fmt...);

class LogManager;

extern LogManager gLogManager;

struct LogQueueItem
{
	ELogType logType;
	std::string msg;
};

class LogManager : public Manager
{
	RTTI_CLASS(LogManager, Manager, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	LogManager(const ConstructParameters& inParameters = {}) : Base(inParameters) {}

	virtual void Init() override;
	virtual void Shutdown() override;

	void Log(ELogType inLogType, const char* fmt...);
	void Log(ELogType inLogType, const char* fmt, va_list args);

	void CleanLogQueue(bool inErrorOnly = false);

	Mutex& GetLogMutex() { return mLogMutex; }

	const std::string& GetLogString() const { return mOutputLog; }

	struct LogEntry
	{
		ELogType mType;
		String mMessage;
	};

	const MutexReadProtectedPtr<Array<LogEntry>> GetLogArray();
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
