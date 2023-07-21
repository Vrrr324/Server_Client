/*
		2023/06/29 - Vr
		���ܣ�����̨�����־���ݣ�
		���ұ��浽���ص���־�ļ��У�
		ʹ����
		��������ģʽ
		�漰��֪ʶ�㣺
		1.����Ϊ�������ͺʹ��ݲ���
		2.����ģʽ
		3.��cpp�ļ���ʵ�־�̬��Ա�����Ķ���
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

