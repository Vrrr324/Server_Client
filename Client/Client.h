#pragma once
#include "stdafx.h"
#include "./Log.h"
#include "define.h"
#include "MsgMaker.h"

class Client
{
public:
	Client();
	~Client();
private:
	void				SetSockAddr();
	void				Connect();
	void				StartFunc();
	void				ReceiveMessages();
	void				SendMessages();
	void				SendHeart();
	void				HandleSimMsg(const char tasktype);
	void				HandleSimPlusMsg(const char tasktype, const char* buffer);
public:

private:
	int					clientID;
	SOCKET				serversocket;
	sockaddr_in			server_addr;

};

