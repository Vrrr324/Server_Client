#include "Log.h"

Log Log::Logger;

Log::Log()
{
	file.open(filename);
	if (!file.is_open())
	{
		Log_Error("无法打开日志文件");
	}
	Log_Info("日志文件启动成功");
}

Log::~Log()
{
	if (file.is_open())
	{
		Log_Warning("日志文件已关闭");
		file.close();
	}
}

Log& Log::GetInstance()
{
	return Logger;
}

void Log::Log_Info(const string str)
{
	GetInstance().Log_InfoImpl(str);
}

void Log::Log_Cout(const string str)
{
	GetInstance().Log_CoutImpl(str);
}

void Log::Log_Warning(const string str)
{
	GetInstance().Log_WarningImpl(str);
}

void Log::Log_Error(const string str)
{
	GetInstance().Log_ErrorImpl(str);
}

void Log::Log_Test(const string str)
{
	GetInstance().Log_TestImpl(str);
}

void Log::Log_InfoImpl(const string str)
{
	char curtime[20];
	GetCurTime(curtime, sizeof(curtime));
	fmt::print(fg(fmt::color::green), "{} -> {}\n", curtime, str);
	file << curtime << " -> " << str << std::endl;
}

void Log::Log_CoutImpl(const string str)
{
	char curtime[20];
	GetCurTime(curtime, sizeof(curtime));
	fmt::print(fg(fmt::color::blue), "{} -> {}\n", curtime, str);
	file << curtime << " -> " << str << std::endl;
}

void Log::Log_WarningImpl(const string str)
{
	char curtime[20];
	GetCurTime(curtime, sizeof(curtime));
	fmt::print(fg(fmt::color::purple), "{} -> {}\n", curtime, str);
	file << curtime << " -> " << str << std::endl;
}

void Log::Log_ErrorImpl(const string str)
{
	char curtime[20];
	GetCurTime(curtime, sizeof(curtime));
	fmt::print(fg(fmt::color::red), "{} -> {}\n", curtime, str);
	file << curtime << " -> " << str << std::endl;
}

void Log::Log_TestImpl(const string str)
{
	char curtime[20];
	GetCurTime(curtime, sizeof(curtime));
	fmt::print(fg(fmt::color::light_yellow), "{} -> {}\n", curtime, str);
	file << curtime << " -> " << str << std::endl;
}

void Log::GetCurTime(char* curtime, size_t size)
{
	time_t sectime = time(NULL);
	strftime(curtime, size, "%Y-%m-%d %H:%M:%S", localtime(&sectime));
}
