#pragma once
#include "stdafx.h"
#include "define.h"
#include "Log.h"
#include "MsgMaker.h"
#include "threadpool.h"
#include "CircularQueue.h"

class Server
{
public:
	Server();
	~Server();	

private:
	void							SetSockAddr();
	void							Bind();
	void							Listen();
	void							StartFunc();
	void							ReceiveMessages(const int target);
	void							AnalysisMessages();
	void							SendMessages();
	void							SetClient();
	void							SocketEnd();
	void							HandleSimMsg(const char tasktype, const int src, const int dst);
	void							HandleSimPlusMsg(const char tasktype, const char* buffer);
	void							HeartSub();
	void							InitClient();

private:
	SOCKET							serversocket;
	SOCKET							clientsocket;
	sockaddr_in						server_addr;
	sockaddr_in						client_addr;

private:
	int								client_num = 0;								// �ͻ���������
	int								send_type = massmsg;						// ���ͷ�ʽ
	char							workstate;									// ����������״̬
	mutex							mtx;										// ��
	ThreadPool						pool;										// �̳߳�
	CircularQueue<char>				msg_que;									// ��Ϣ����
	unordered_map<int, ClientInfo>	clients_info;								// �ͻ����б�
};

