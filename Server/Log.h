/*
		2023/06/29 - Vr
		功能：控制台输出日志内容，
		并且保存到本地的日志文件中，
		使用了
		饿汉单例模式
		涉及的知识点：
		1.数组为返回类型和传递参数
		2.单例模式
		3.在cpp文件中实现静态成员变量的定义
*/
#pragma once
#include "stdafx.h"
#define	    DEBUG_TEST 		1

class Log
{
private:
	static Log			Logger;
	ofstream			file;
	const string		filename = "../bin/CSMCs_Log.txt";
public:
	static Log&			GetInstance();
	static void			Log_Info(const string str);
	static void			Log_Cout(const string str);
	static void			Log_Printf(const string str);
	static void			Log_Warning(const string str);
	static void			Log_Error(const string str);
	static void			Log_Test(const string str);
private:
	Log();
	~Log();
	void				Log_InfoImpl(const string str);
	void				Log_CoutImpl(const string str);
	void				Log_PrintfImpl(const string str);
	void				Log_WarningImpl(const string std);
	void				Log_ErrorImpl(const string str);
	void				Log_TestImpl(const string str);
	void				GetCurTime(char* time, size_t size);
};

#define		Log_Info		Log::Log_Info
#define		Log_Cout		Log::Log_Cout
#define		Log_Printf		Log::Log_Printf
#define		Log_Warning		Log::Log_Warning
#define		Log_Error		Log::Log_Error

#if defined(DEBUG_TEST) && 1 == DEBUG_TEST      // if define
#define		Log_Test		Log::Log_Test
#else
#define		Log_Test(...)
#endif 

