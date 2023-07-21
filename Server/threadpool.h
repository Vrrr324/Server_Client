#pragma once
#include "stdafx.h"
#include "Log.h"
using namespace std;

//任务队列
template<typename T>
struct Task_Queue {
	queue<T>que;
	shared_mutex m_mutex;
	bool empty() {
		shared_lock<shared_mutex>lock(m_mutex);
		return que.empty();
	}
	auto size() {
		shared_lock<shared_mutex>lock(m_mutex);
		return que.size();
	}
	void push(T& t) {
		unique_lock<shared_mutex>lock(m_mutex);
		que.push(t);
	}
	bool pop(T& t) {
		unique_lock<shared_mutex>lock(m_mutex);
		if (que.empty())return false;
		t = move(que.front());
		que.pop();
		return true;
	}
};

//线程池
class ThreadPool {
private:
	//工作者队列
	class ThreadWorker {
	public:
		ThreadPool* pool; // 指向线程池的指针
		ThreadWorker(ThreadPool* pool) : pool{ pool } {} // 构造函数
		void operator ()() { // 重载函数调用操作符，线程在运行时会执行该函数
			while (!pool->shutdown) {
				{
					// 等待线程池中有任务或线程池已经关闭
					// 条件变量，唤醒条件：线程池关闭/线程池有任务
					unique_lock<mutex> lock(pool->m_mutex);
					pool->cv.wait(lock, [this]() {
						return this->pool->shutdown ||
							!this->pool->que.empty();
						});
				}
				// 从任务队列中取出一个任务
				function<void()>func;
				bool flag = pool->que.pop(func);
				if (flag) { // 如果成功取出了一个任务
					func(); // 执行该任务
				}
			}
		}
	};
public:
	bool shutdown;
	Task_Queue<std::function<void()>> que;
	vector<std::thread> threads;
	mutex m_mutex;
	condition_variable cv;
	// n 为线程池中的工作线程数
	ThreadPool(int n) : threads(n), shutdown{ false } {
		// 创建 n 个工作线程，并将 ThreadWorker 对象传递给每个线程
		for (auto& t : threads) {
			t = thread{ ThreadWorker(this) };
		}
	}
	// 禁止拷贝和移动
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	//F函数 Args参数
	template <typename F, typename... Args>
	//&&右值引用 此处不是右值引用 Args可变参数包 future异步操作结果
	//decltype根据实际表达式推演出定义变量时所需的类型
	//返回一个future类型的对象，对象的函数参数自动推导得出
	auto submit(F&& f, Args &&...args) -> std::future<decltype(f(args...))> {
		//创建一个lambda函数 封装在一个function对象中
		Log_Cout("线程池线程执行");
		function<decltype(f(args...))()> func = [&f, args...]() {return f(args...); };
		//创建一个智能指针
		auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
		//创建一个lambda函数，封装在一个funtion对象中，作用是执行封装在task_ptr的指针
		std::function<void()> warpper_func = [task_ptr]() {
			(*task_ptr)();
		};
		que.push(warpper_func);
		cv.notify_one();
		//返回值必须是future类型
		return task_ptr->get_future();
	}
	~ThreadPool() {
		//等待其他任务执行完成
		auto f = submit([]() {});
		f.get();

		shutdown = true;
		cv.notify_all(); // 通知，唤醒所有工作线程
		// 等待所有工作线程退出
		for (auto& t : threads) {
			if (t.joinable()) t.join();
		}
	}
};
