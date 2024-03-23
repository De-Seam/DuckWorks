#include "Precomp.h"
#include "Core/Log/Log.h"

#include <cstdarg>
#include <fstream>

void gLog(const char* fmt ...)
{
	va_list args;
	va_start(args, fmt);
	gLogManager.Log(LogType::Info, fmt, args);
	va_end(args);
}

void gLog(LogType inLogType, const char* fmt ...)
{
	va_list args;
	va_start(args, fmt);
	gLogManager.Log(inLogType, fmt, args);
	va_end(args);
}

LogManager gLogManager = {};

LogManager::LogManager()
{
	gAssert(this == &gLogManager, "Use gLogManager!");
}

void LogManager::Init()
{
	gLog(LogType::Info, "Initializing LogManager");

	mOutputLog.reserve(8192);

	mLogThread = std::thread(&LogManager::LogThreadLoop, this);

	gLog(LogType::Info, "LogManager Initialized");
}

void LogManager::Shutdown()
{
	// We have to log something here so the thread which is waiting for a log message can exit.
	Log(LogType::Info, "LogManager shutting down.");
	mThreadRunning = false;

	WriteLogToFile();

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
	PROFILE_SCOPE(LogManager::Log)
	static THREADLOCAL String msg;
	msg.clear();

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

	LogQueueItem logQueueItem = {inLogType, msg};
	mLogQueue.enqueue(logQueueItem);
}

void LogManager::CleanLogQueue(bool inErrorOnly)
{
	PROFILE_SCOPE(LogManager::CleanLogQueue)

	std::vector<LogQueueItem> items_to_requeue;

	while (!mLogQueue.empty())
	{
		// Anything that's not info is important enough that it needs to be re-queued
		LogQueueItem queue_item = mLogQueue.dequeue();

		if (inErrorOnly)
		{
			if (queue_item.logType == LogType::Error)
				items_to_requeue.push_back(queue_item);
		}
		else if (queue_item.logType != LogType::Info)
			items_to_requeue.push_back(queue_item);
	}

	// Requeue the items to requeue
	for (LogQueueItem& item : items_to_requeue)
		mLogQueue.enqueue(item);

	Log(LogType::Warning, "Log queue had to be cleaned.");
}

const Array<LogManager::LogEntry>& LogManager::GetLogArray() const
{
	return mLogEntries;
}

void LogManager::SetLogFilePath(const String& inFilePath)
{
	mLogFilePath = inFilePath;
}

void LogManager::SetLogFileName(const String& inFileName)
{
	mLogFileName = inFileName;
}

std::string GetCurrentDateTime()
{
	// Get current time as time_point
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	// Convert to tm struct for formatting
	std::tm bt;
	errno_t error = localtime_s(&bt, &in_time_t);
	if (error)
	{
		gAssert(!error, "Error getting current time.");
		return "Error";
	}

	// Format the time into a string with format YYYY-MM-DD_HH-MM-SS
	std::ostringstream ss;
	ss << std::put_time(&bt, "%y-%m-%d_%H-%M-%S");
	return ss.str();
}

void LogManager::WriteLogToFile()
{
	PROFILE_SCOPE(LogManager::WriteLogToFile)

	// Construct the full path
	std::filesystem::path dir(mLogFilePath);
	std::filesystem::path file = dir / (mLogFileName + GetCurrentDateTime() + mLogFileExtension);

	// Check if folder exists, if not create it
	if (!exists(dir))
	{
		if (!create_directories(dir))
		{
			// Create the directory.
			gLog(LogType::Error, "Error: Unable to create directory: %s", mLogFilePath.c_str());
			return;
		}
	}

	std::ofstream log_file;
	log_file.open(file, std::ios::out);
	if (log_file.is_open())
	{
		log_file << mOutputLog;
		log_file.close();
		mOutputLog.clear();
	}
	else
	{
		Log(LogType::Error, "Failed to open log file for writing.");
	}
}

void LogManager::LogThreadLoop()
{
	OPTICK_THREAD("LogManagerThread")
	mThreadRunning = true;
	while (mThreadRunning)
	{
		if (mLogQueue.size() > 128)
			CleanLogQueue();
		if (mLogQueue.size() > 512)
			CleanLogQueue(true);
		if (mLogQueue.size() > 1024)
			mLogQueue.clear();

		gAssert(mLogQueue.size() < 2048, "Log queue is getting too big. This is a sign that the log thread is not keeping up with the log messages.");

		LogQueueItem queueItem = mLogQueue.dequeue();

		PROFILE_SCOPE(LogManager::LogThreadLoop)

		mLogMutex.WriteLock();

		LogEntry log_entry = {queueItem.logType, queueItem.msg};
		mLogEntries.emplace_back(log_entry);

		queueItem.msg += "\n";
		mOutputLog += queueItem.msg;
		mLogMutex.WriteUnlock();

		if (mOutputLog.size() > mMaxOutputLogSize)
			WriteLogToFile();
	}
}
