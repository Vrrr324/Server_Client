/*
* 1.ͨ������д�����ݣ���д������ݽ��д���
* 2.ͨ�������ȡ���ݣ�����ȡ�����ݽ��н���
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
	void		GetPos(int& pos, int& sp, int& rp, int& wp);	// ��ȡָ���С
	void		GetPkg(char** backbuffer);						// ��ȡд������
	bool		Push(char* obj, size_t cursize);				// ѹ����
	bool		Pop(size_t cursize);							// ������
	bool		AddSize(size_t addsize);						// ��Ӵ洢�������ڴ��С
	size_t		GetSize();
	bool		CpyPkg(char* obj, size_t cursize);				// ������������д��
	bool		ReadData(char* backdata, size_t cursize);		// ��ȡ����
	void 		GetBuffer(char* backdata, size_t& len);			// ��ȡ��������

private:
	char*		pkgbuffer;										// �����ڴ��С 
	int			pos;											// ��ǰָ�������ʼ��λ��
	int			sp;												// ��ʼ��
	int			rp;												// ��ָ�������ʼ��λ��
	int			wp;												// дָ�������ʼ��λ��
	size_t		maxsize;										// ���洢�����ڴ�ռ�
	size_t		usesize;										// ��ǰ����ʹ�ô�С

};

