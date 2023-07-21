#pragma once

const int SERVERID	= 0;
const int SEC		= 1000;

#pragma pack(push,1)
struct PkgHead
{
	char		type;
	char		tasktype{ 0 };
	int 		source;				// 发送方（暂未使用）
	int 		target;				// 接收方（暂未使用）
	size_t		len{ 0 };
};
#pragma pop()

#pragma pack(push,1)
struct ClientInfo
{
	int			clientnum;
	SOCKET		clientsocket;
	time_t		heartime;
};
#pragma pop()

/*服务器工作状态*/
enum WorkState {
	working		=	0x01,
	closing		=	0x02,
	sleeping	=	0x04
};

/*消息类型*/
enum MsgType
{
	simple		=	0x01,
	complex		= 	0x02,
	simpleplus	=	0x04
};

/*简单消息*/
enum SimMsgType
{
	heart		=	0x01,
	over		=	0x02,
	cutone		=	0x04,
	cutall		=	0x08
};

enum SimPlusMsgType
{
	init		=	0x01
};

enum ServerSendType
{
	massmsg		=	0x01,
	singlemsg	=	0x02
};

#define TC(condtion) try \
{\
	condtion.detach();\
}\
catch (const std::system_error& e) {\
	std::cout << "Caught system_error with code " << e.code() \
		<< " meaning " << e.what() << '\n';\
}\
