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
	int								client_num = 0;								// 客户端连接数
	int								send_type = massmsg;						// 发送方式
	char							workstate;									// 服务器工作状态
	mutex							mtx;										// 锁
	ThreadPool						pool;										// 线程池
	CircularQueue<char>				msg_que;									// 消息队列
	unordered_map<int, ClientInfo>	clients_info;								// 客户端列表
};

