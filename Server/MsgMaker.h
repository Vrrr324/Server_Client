/*
	2023/06/20 - Vr
	装包拆包类：使用PkgMaker类装包拆包
*/
#pragma once
#include "PkgMaker.h"
#include "CircularQueue.h"
#include "stdafx.h"
#include "define.h"

class MsgMaker
{
public:
	MsgMaker();
	~MsgMaker();
private:
	PkgMaker*		pkgmaker;
public:
	void			MakeSimpleMsg(SimMsgType tasktype, char* pkg, size_t& len, int dst, int src);			// 制作简易消息包
	void			MakeSimplePlusMsg(SimPlusMsgType tasktype, char* pkg, size_t& len, int dst, int src);	// 制作带数据的简易消息包
	void			MakeComplexMsg(char* pkg, size_t& len, int dst, int src);								// 制作复杂消息报
	void			AnalysisMsg(CircularQueue<char>& inpkg, char* outpkg, PkgHead& pkghead);				// 解析包（拆包）
	void			AnalysisMsg(char* inpkg, char* outpkg, PkgHead& pkghead);
private:
};

