#include "Server.h"

Server::Server():pool(5)
{
	WSADATA WSADATA;
	if (WSAStartup(MAKEWORD(2, 2), &WSADATA) != 0)
	{
		Log_Error("启动服务器失败");
		exit(1);
	}
	serversocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serversocket == INVALID_SOCKET)
	{
		Log_Error("启动服务器失败");
		WSACleanup();
		exit(1);
	}
	SetSockAddr();
	workstate = working;
	Bind();
	Listen();
	thread setclient(&Server::SetClient, this);
	StartFunc();
	setclient.join();
}

Server::~Server()
{
	SocketEnd();
	Log_Error("服务器关闭");
}

void Server::SetSockAddr()
{
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(2023);
}

void Server::Bind()
{
	while (WIN32::bind(serversocket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		Log_Error("地址绑定失败");
		Sleep(5 * SEC);
	}
	Log_Info("地址绑定成功");
}

void Server::Listen()
{
	if (listen(serversocket, 5) == SOCKET_ERROR)
	{
		Log_Error("监听失败");
		SocketEnd();
		exit(1);
	}
	Log_Info("开始监听");
}

void Server::StartFunc()
{
	thread analysisthread(&Server::AnalysisMessages, this);
	thread sendthread(&Server::SendMessages, this);
	thread hearthread(&Server::HeartSub, this);
	analysisthread.join();
	hearthread.join();
	sendthread.join();
}

void Server::ReceiveMessages(const int target)
{
	char receivebuffer[256];
	SOCKET curclientsocket = clients_info[target].clientsocket;
	int recvnum = recv(curclientsocket, receivebuffer, sizeof(receivebuffer), 0);
	while(recvnum >= 0)
	{
		msg_que.push(receivebuffer, recvnum);
		recvnum = recv(curclientsocket, receivebuffer, sizeof(receivebuffer), 0);
	}
	Log_Test(fmt::format("{}号客户端接收线程结束", target));
}

void Server::AnalysisMessages()
{
	while ((workstate & working) == working)
	{
		MsgMaker mm;
		PkgHead pkghead;
		char* receivebuffer = nullptr;

		// 优化
		if (msg_que.GetCount() < sizeof(PkgHead))
		{
			continue;
		}
		msg_que.get(sizeof(PkgHead), (char*)&pkghead);
		if (msg_que.GetCount() < pkghead.len + sizeof(PkgHead))
		{
			continue;
		}

		char* outpkg = new char[256];
		mm.AnalysisMsg(msg_que, outpkg, pkghead);
		switch (pkghead.type)
		{
		case simple:
		{
			HandleSimMsg(pkghead.tasktype, pkghead.source, pkghead.target);
		}
		break;
		case complex:
		{
			Log_Printf(fmt::format("{} -> {}", pkghead.source, outpkg));
		}
		break;
		}
		delete[] outpkg;
	}
}

void Server::SendMessages()
{
	// 编码问题：发送 汉字+' 出现错误
	while ((workstate & working) == working)
	{
		// 群发
		if ((send_type & massmsg) == massmsg) 
		{
			MsgMaker mm;
			char* buffer = new char[256];
			cin >> buffer;
			size_t len = strlen(buffer);
			Log_Cout(buffer);
			for (auto& it : clients_info)
			{
				mm.MakeComplexMsg(buffer, len, it.second.clientnum, SERVERID);
				if (send(it.second.clientsocket, buffer, len, 0) == SOCKET_ERROR)
				{
					Log_Error(fmt::format("{}号客户端发送失败"));
				}
			}
			delete[] buffer;
		}
		else if ((send_type & singlemsg) == singlemsg) 
		{
			int sendtarget;
			cin >> sendtarget;
			if (clients_info.count(sendtarget) <= 0)
			{
				if (cin.rdstate())
				{
					Log_Warning("输入非法编号");
					// MARK -> auto a = numeric_limits<streamsize>::max();
					cin.clear();
					cin.ignore((numeric_limits<streamsize>::max)(), '\n');
				}
				else
				{
					Log_Warning("目标客户端不存在");
				}
				continue;
			}
			MsgMaker mm;
			char* buffer = new char[256];
			cin >> buffer;
			size_t len = strlen(buffer);
			Log_Cout(buffer);
			mm.MakeComplexMsg(buffer, len, sendtarget, SERVERID);
			if (send(clients_info[sendtarget].clientsocket, buffer, len, 0) == SOCKET_ERROR)
			{
				Log_Error("发送失败");
			}
			delete[] buffer;
		}
	}
}

void Server::SetClient()
{
	while((workstate & working) == working){
		int addrlen = sizeof(client_addr);
		clientsocket = accept(serversocket, (SOCKADDR*)&client_addr, &addrlen);
		if (clientsocket == INVALID_SOCKET)
		{
			Log_Error("接收失败");
		}
		ClientInfo client_info{ ++client_num, clientsocket, time(nullptr) };
		clients_info.insert(pair<int, ClientInfo>(client_num, client_info));
		Log_Info(fmt::format("连接到地址：{} => 客户端序号{}",
			inet_ntoa((client_addr.sin_addr)), client_num));
		InitClient();
// 		thread receivethread(&Server::ReceiveMessages, this, client_num);
// 		receivethread.detach();
		pool.submit([this]() {
			ReceiveMessages(client_num); // 在 lambda 函数内部调用类成员函数
			});
	}
}

void Server::SocketEnd()
{
	closesocket(serversocket);
	WSACleanup();
}

void Server::HandleSimMsg(const char tasktype, const int src, const int dst)
{
	switch (tasktype)
	{
	case heart:
	{
		Log_Test(fmt::format("{}心跳包", src));
		clients_info[src].heartime = time(nullptr);
	}
		break;
	default:
		break;
	}
}

void Server::HandleSimPlusMsg(const char tasktype, const char* buffer)
{
	switch (tasktype)
	{
	default:
		break;
	}
}

void Server::HeartSub()
{
	while((workstate & working) == working)
	{
		Sleep(50000 * SEC);
		time_t curtime = time(nullptr);
		vector<int> deleteclient;
		for (auto &it : clients_info)
		{
			// 最长时效不一定是大于的时间
			// 建议大于时间为睡眠时间的倍数
			// DELETE
			if ((curtime - it.second.heartime) > 15)
			{
				deleteclient.push_back(it.first);
				closesocket(it.second.clientsocket);
				Log_Warning(fmt::format("{}号客户端断开连接", it.first));
			}
		}
		for (auto it : deleteclient)
		{
			clients_info.erase(it);
		}
	}
}

void Server::InitClient()
{
	MsgMaker mm;
	char* buffer = new char[256];
	memcpy(buffer, &client_num, sizeof(client_num));
	size_t len = sizeof(client_num);
	mm.MakeSimplePlusMsg(init, buffer, len, client_num, SERVERID);
	if (send(clientsocket, buffer, len, 0) == SOCKET_ERROR)
	{
		Log_Error("初始化失败");
	}
	delete[] buffer;
}
