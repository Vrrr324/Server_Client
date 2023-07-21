/*
* 1.通过该类写入数据，将写入的数据进行传输
* 2.通过该类读取数据，将读取的数据进行解析
*/
#pragma once
#include <iostream>
#
#include <limits>
class PkgMaker
{

public:
	PkgMaker();
	~PkgMaker();
	void		GetPos(int& pos, int& sp, int& rp, int& wp);	// 获取指针大小
	void		GetPkg(char** backbuffer);						// 获取写入数据
	bool		Push(char* obj, size_t cursize);				// 压数据
	bool		Pop(size_t cursize);							// 出数据
	bool		AddSize(size_t addsize);						// 添加存储的数据内存大小
	size_t		GetSize();
	bool		CpyPkg(char* obj, size_t cursize);				// 将待解析数据写入
	bool		ReadData(char* backdata, size_t cursize);		// 读取数据
	void 		GetBuffer(char* backdata, size_t& len);			// 获取生成数据

private:
	char*		pkgbuffer;										// 数据内存大小 
	int			pos;											// 当前指针相对起始点位置
	int			sp;												// 起始点
	int			rp;												// 读指针相对起始点位置
	int			wp;												// 写指针相对起始点位置
	size_t		maxsize;										// 最大存储数据内存空间
	size_t		usesize;										// 当前可以使用大小

};

