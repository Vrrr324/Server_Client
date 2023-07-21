#include "MsgMaker.h"

MsgMaker::MsgMaker()
{
	pkgmaker = new PkgMaker;
}

MsgMaker::~MsgMaker()
{
	delete pkgmaker;
}

void MsgMaker::MakeSimpleMsg(SimMsgType tasktype, char* pkg, size_t &len, int dst, int src)
{
	PkgHead pkghead;
	pkghead.type = simple;
	pkghead.tasktype = tasktype;
	pkghead.source = src;
	pkghead.target = dst;
	pkghead.len = 0;
	pkgmaker->Push((char*)&pkghead, sizeof(pkghead));
	pkgmaker->GetBuffer(pkg, len);
}

// 传入消息内容为int类型 通常表示客户端编号 因此不需要结束符
void MsgMaker::MakeSimplePlusMsg(SimPlusMsgType tasktype, char* pkg, size_t& len, int dst, int src)
{
	PkgHead pkghead;
	pkghead.type = simpleplus;
	pkghead.tasktype = tasktype;
	pkghead.source = src;
	pkghead.target = dst;
	pkghead.len = len ;
	pkgmaker->Push((char*)&pkghead, sizeof(pkghead));
	pkgmaker->Push(pkg, len);
	pkgmaker->GetBuffer(pkg, len);
}

void MsgMaker::MakeComplexMsg(char *pkg, size_t &len, int dst, int src)
{
	PkgHead pkghead;
	pkghead.type = complex;
	pkghead.tasktype = 0;
	pkghead.source = src;
	pkghead.target = dst;
	pkghead.len = len + 1;
	pkgmaker->Push((char*)&pkghead, sizeof(pkghead));
	pkgmaker->Push(pkg, len + 1);
	pkgmaker->GetBuffer(pkg, len);
}

void MsgMaker::AnalysisMsg(CircularQueue<char>& inpkg, char* outpkg, PkgHead& pkghead)
{
	char* tmpPkghead = new char[512];
	inpkg.pop(sizeof(pkghead), tmpPkghead);
	// 改
	pkgmaker->GetPkg(&tmpPkghead);
	pkgmaker->ReadData((char*)&pkghead, sizeof(pkghead));
	inpkg.pop(pkghead.len, tmpPkghead + sizeof(pkghead));
	pkgmaker->ReadData(outpkg, pkghead.len);
	delete[]tmpPkghead;
}

void MsgMaker::AnalysisMsg(char* inpkg, char* outpkg, PkgHead& pkghead)
{
	pkgmaker->GetPkg(&inpkg);
	pkgmaker->ReadData((char*)&pkghead, sizeof(pkghead));
	pkgmaker->ReadData(outpkg, pkghead.len);
}
