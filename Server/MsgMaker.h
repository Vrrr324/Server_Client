/*
	2023/06/20 - Vr
	װ������ࣺʹ��PkgMaker��װ�����
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
	void			MakeSimpleMsg(SimMsgType tasktype, char* pkg, size_t& len, int dst, int src);			// ����������Ϣ��
	void			MakeSimplePlusMsg(SimPlusMsgType tasktype, char* pkg, size_t& len, int dst, int src);	// ���������ݵļ�����Ϣ��
	void			MakeComplexMsg(char* pkg, size_t& len, int dst, int src);								// ����������Ϣ��
	void			AnalysisMsg(CircularQueue<char>& inpkg, char* outpkg, PkgHead& pkghead);				// �������������
	void			AnalysisMsg(char* inpkg, char* outpkg, PkgHead& pkghead);
private:
};

