#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

/*ϵͳͷ�ļ�*/
#include <iostream>
#include <fstream>
#include <time.h>
#include <thread>
#include <limits>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <condition_variable>
#include <WinSock2.h>
#include <shared_mutex>
#include <functional>
#include <utility>
#include <chrono>
#include <future>
#include <vector>
#include <string>
#include <queue>


/*��������ͷ�ļ�*/
#include "fmt/color.h"

/*ϵͳ��*/
#pragma comment(lib,"ws2_32.lib")

using namespace std;
