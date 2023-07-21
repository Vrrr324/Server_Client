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
	memset(pkgbuffer, 0, 512); // ��pkgbufferָ����ڴ�ȫ������Ϊ0
}

PkgMaker::~PkgMaker()
{
	//delete[] pkgbuffer; // �ͷŶ�̬������ڴ�
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
		printf("������Ҫ����ռ�%d", (int)(cursize - usesize));
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
		printf("���󣺳����洢�ռ�%d", (int)(cursize - rp));
		return false;
	}
	memset(pkgbuffer + wp - cursize, 0, cursize); // ���ڴ�����
	wp -= cursize;
	return true;
}

bool PkgMaker::AddSize(size_t addsize)
{
	if (addsize <= 0 || (addsize + maxsize) > std::numeric_limits<size_t>::max())
	{
		printf("�������ݴ�С����");
		return false;
	}

	maxsize = addsize + maxsize;
	char* tmpbuffer = new char[maxsize];
	memcpy(tmpbuffer, pkgbuffer, usesize); // ��ԭ�����ݿ������µĻ�����
	delete[] pkgbuffer; // �ͷ�ԭ���ڴ�
	pkgbuffer = tmpbuffer;
	return true;
}

size_t PkgMaker::GetSize()
{
	return wp; // ����ʵ��ʹ�õ����ݴ�С
}

bool PkgMaker::CpyPkg(char* obj, size_t cursize)
{
	if (cursize > maxsize)
	{
		printf("����д�����ݹ��� ��������%");
		return false;
	}
	memcpy(pkgbuffer, obj, cursize); // ��obj�е����ݿ�����pkgbuffer��
	usesize = cursize;
	return true;
}

bool PkgMaker::ReadData(char* backdata, size_t cursize)
{
	if ((rp + cursize) > maxsize)
	{
		printf("%s %d����ȡ���ݹ��� \n ", __FUNCTION__, __LINE__);
		return false;
	}
	memcpy(backdata, pkgbuffer + rp, cursize);
	rp += cursize;
	return true;
}

void PkgMaker::GetBuffer(char* backdata, size_t& len)
{
	len = wp;
	memcpy(backdata, this->pkgbuffer, wp); // ��pkgbuffer�е����ݿ�����backdata��
}
