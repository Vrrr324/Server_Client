﻿#include "Log.h"
#include "Server.h"

/*
	可在其他项目中使用的类：日志类（Log.h），装包拆包类（MsgMaker.h）
	1.心跳包：
		服务端连接到客户端时，服务端使用两个map容器，
		一个map容器存放客户端的序号和最新确认连接时间,
		另一个map容器存放客户端序号和客户端SOCKET；
		客户端一个线程用于发送心跳包，
		服务端接收线程接收到简单消息-心跳包，更新最新确认连接时间，
		服务端一个线程用于遍历map容器，
		并判断每个客户端最新确认连接时间与当前的时间的差值大小，
		若差值大小大于某值 则视为与该客户端断开连接
	2.装包拆包
		使用一个MsgMaker类用来组装消息（简单命令消息、复杂命令消息、复杂消息）
		其中，包含一个辅助组装的PkgMaker类（包含压入、弹出、读取、复制、获取消息等）
		接收消息到的消息存放到循环队列
		从循环队列中取出相应的消息 
		并且使用条件变量 当队列消息长度大于包长度时 唤醒解析线程
		*目前的问题：
		1.可能存在一个不完整的包后面是另一个包的内容（两个包来源不同的客户端）
		2.唤醒解析线程处可能有错
		3.解析时可能发生队列count为0的情况
		注意：
		1.循环队列初始大小不能太小（太小可能会使扩容2倍也无法存储一个较大的包）
		2.消息发送频率不能太大
		*
	3.日志
		控制台输出日志内容，
		并且保存到本地的日志文件中，
		使用了饿汉单例模式
		涉及的知识点：
		1.数组为返回类型和传递参数
		2.单例模式
		3.在cpp文件中实现静态成员变量的定义
	4.多客户端连接，单发、群发：
		使用map关联客户端信息
	
	
*/

mutex _m;
int main()
{
	Server serer;
}

