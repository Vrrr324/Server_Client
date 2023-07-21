#include "Client.h"

Client::Client()
{
	WSADATA WSADATA;
	if (WSAStartup(MAKEWORD(2, 2), &WSADATA) != 0)
	{
		Log_Error("启动客户端失败");
		exit(1);
	}
	serversocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serversocket == INVALID_SOCKET)
	{
		Log_Error("启动服务器失败");
	}
	SetSockAddr();
	Connect();
	StartFunc();
}

Client::~Client()
{
	closesocket(serversocket);
	WSACleanup();
}

void Client::SetSockAddr()
{
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2023);
	server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
}

void Client::Connect()
{
	int ret;
	do
	{
		// connect 耗时过长
		ret = connect(serversocket, (sockaddr*)&server_addr, sizeof(server_addr));
		if (ret == 0)
		{
			Log_Info("连接成功");
		}
		else
		{
			Log_Error("连接失败");
			Sleep(5 * SEC);
		}
	} while (ret != 0);
}

void Client::StartFunc()
{
	thread receivethread(&Client::ReceiveMessages, this);
	while (clientID <= 0){}
	thread sendthread (&Client::SendMessages, this);
	thread heartthread(&Client::SendHeart, this);
	while (1) {}
}

void Client::ReceiveMessages()
{
	while (true) {
		char receivebuffer[512];
		int recvnum = recv(serversocket, receivebuffer, sizeof(receivebuffer), 0);
		if (recvnum > 0)
		{
			MsgMaker mm;
			PkgHead pkghead;
			char* outpkg = new char[512];
			mm.AnalysisMsg(receivebuffer, outpkg, pkghead);
			switch (pkghead.type)
			{
			case simple:
			{
				HandleSimMsg(pkghead.tasktype);
			}
				break;
			case complex:
			{
				printf("%s\n", outpkg);
			}
			case simpleplus:
				HandleSimPlusMsg(pkghead.tasktype, outpkg);
				break;
			}
			delete[] outpkg;
		}
		
	}
}

void Client::SendMessages()
{
	int a = 0;
	while (this->clientID > 0) {
	// while (a++ < 100){


// 		{
// 			MsgMaker mm;
// 			char* buffer = new char[256];
// 			//cin >> buffer;
// 			memcpy(buffer, "test1", sizeof("test1"));
// 			size_t len = strlen(buffer);
// 			Log_Cout(buffer);
// 			mm.MakeComplexMsg(buffer, len, SERVERID, clientID);
// 			if (send(serversocket, buffer, len, 0) == SOCKET_ERROR)
// 			{
// 				Log_Error("发送失败");
// 			}
// 			delete[] buffer;
// 			// Sleep(100 * SEC);
// 			Sleep(0.1 * SEC);
// 			
// 		}
 
		// 注意：以下代码没有delete（new之后）

		// 测试粘包代码 强行将两个包放在一起
		{
			MsgMaker mm1;
			char* buffer1 = new char[512];
			memcpy(buffer1, "test code by making two bags 2 one", sizeof("test code by making two bags 2 one"));
			char* buffer2 = new char[512];
			memcpy(buffer2, "test one over", sizeof("test one over"));
			size_t len1 = strlen(buffer1);
			size_t len2 = strlen(buffer2);
			mm1.MakeComplexMsg(buffer1, len1, SERVERID, clientID);
			MsgMaker mm2;
			mm2.MakeComplexMsg(buffer2, len2, SERVERID, clientID);
			memcpy(buffer1 + len1, buffer2, len2);
			send(serversocket, buffer1, len1 + len2, 0);
			Log_Test("buffer1");
			Sleep(0.1 * SEC);
			delete[]buffer1;
			delete[]buffer2;
		}

			// 测试粘包代码 强行将一个包分成两个
		{
			MsgMaker mm3;
			char* buffer3 = new char[512];
			memcpy(buffer3, "test code by spining one bag 2 two", sizeof("test code by spining one bag 2 two"));
			char* buffer4 = new char[512];
			memcpy(buffer4, "test two over", sizeof("test two over"));
			size_t len3 = strlen(buffer3);
			size_t len4 = strlen(buffer4);
			mm3.MakeComplexMsg(buffer3, len3, SERVERID, clientID);
			PkgHead pkghead;
			PkgMaker pkgmaker;
			pkghead.type = complex;
			pkghead.tasktype = 0;
			pkghead.source = 1;
			pkghead.target = 0;
			// len 14
			pkghead.len = len4 + 1;
			pkgmaker.Push((char*)&pkghead, sizeof(pkghead));
			memcpy(buffer3 + len3, (char*)&pkghead, sizeof(pkghead));
			// len 87
			send(serversocket, buffer3, len3 + sizeof(pkghead), 0);
			Log_Test("buffer3");
			// Sleep(0.1 * SEC);
			Sleep(10000 * SEC);
			// send(serversocket, buffer4, len4 + 1, 0);
			Log_Test("buffer4");
			delete[]buffer3;
			delete[]buffer4;
		}

	}
}

void Client::SendHeart()
{
	while (true)
	{
		// DELETE
		Sleep(50000 * SEC);
		MsgMaker mm;
		char* buffer = new char[256];
		size_t len = 0;
		mm.MakeSimpleMsg(heart, buffer, len, SERVERID, clientID);
		// Log_Test(fmt::format("{}->心跳包发送成功", clientID));
		if (send(serversocket, buffer, len, 0) == SOCKET_ERROR)
		{
			Log_Warning("心跳包发送失败");
		}
		delete[] buffer;
	}
}

void Client::HandleSimMsg(const char tasktype)
{
	switch (tasktype)
	{
	default:
		break;
	}
}

void Client::HandleSimPlusMsg(const char tasktype, const char* buffer)
{
	switch (tasktype)
	{
	case init:
	{
		clientID = *((int*)buffer);
		Log_Test(fmt::format("序号：{}",clientID));
	}
	break;
	default:
		break;
	}
}
