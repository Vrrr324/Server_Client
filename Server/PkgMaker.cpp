#include "pkgmaker.h"

PkgMaker::PkgMaker()
{
	pos = 0;
	sp = 0;
	rp = 0;
	wp = 0;
	usesize = 512;
	maxsize = 512;
	pkgbuffer = new char[512];
	memset(pkgbuffer, 0, 512); // 将pkgbuffer指向的内存全部设置为0
}

PkgMaker::~PkgMaker()
{
	//delete[] pkgbuffer; // 释放动态分配的内存
}

void PkgMaker::GetPos(int& pos, int& sp, int& rp, int& wp)
{
	pos = this->pos;
	sp = this->sp;
	rp = this->rp;
	wp = this->wp;
}

void PkgMaker::GetPkg(char** backbuffer)
{
	pkgbuffer = *backbuffer;
}

bool PkgMaker::Push(char* obj, size_t cursize)
{
	if (usesize < cursize)
	{
		printf("错误：需要额外空间%d", (int)(cursize - usesize));
		return false;
	}
	memcpy(pkgbuffer + wp, obj, cursize);
	usesize -= cursize;
	wp += cursize;
	return true;
}

bool PkgMaker::Pop(size_t cursize)
{
	if (cursize > rp)
	{
		printf("错误：超出存储空间%d", (int)(cursize - rp));
		return false;
	}
	memset(pkgbuffer + wp - cursize, 0, cursize); // 将内存清零
	wp -= cursize;
	return true;
}

bool PkgMaker::AddSize(size_t addsize)
{
	if (addsize <= 0 || (addsize + maxsize) > std::numeric_limits<size_t>::max())
	{
		printf("错误：扩容大小错误");
		return false;
	}

	maxsize = addsize + maxsize;
	char* tmpbuffer = new char[maxsize];
	memcpy(tmpbuffer, pkgbuffer, usesize); // 将原有数据拷贝到新的缓冲区
	delete[] pkgbuffer; // 释放原有内存
	pkgbuffer = tmpbuffer;
	return true;
}

size_t PkgMaker::GetSize()
{
	return wp; // 返回实际使用的数据大小
}

bool PkgMaker::CpyPkg(char* obj, size_t cursize)
{
	if (cursize > maxsize)
	{
		printf("错误：写入数据过大 请先扩容%");
		return false;
	}
	memcpy(pkgbuffer, obj, cursize); // 将obj中的数据拷贝到pkgbuffer中
	usesize = cursize;
	return true;
}

bool PkgMaker::ReadData(char* backdata, size_t cursize)
{
	if ((rp + cursize) > maxsize)
	{
		printf("%s %d：读取内容过大 \n ", __FUNCTION__, __LINE__);
		return false;
	}
	memcpy(backdata, pkgbuffer + rp, cursize);
	rp += cursize;
	return true;
}

void PkgMaker::GetBuffer(char* backdata, size_t& len)
{
	len = wp;
	memcpy(backdata, this->pkgbuffer, wp); // 将pkgbuffer中的数据拷贝到backdata中
}
