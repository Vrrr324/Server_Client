#pragma once
#include "stdafx.h"
#include "Log.h"
using namespace std;

//�������
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

//�̳߳�
class ThreadPool {
private:
	//�����߶���
	class ThreadWorker {
	public:
		ThreadPool* pool; // ָ���̳߳ص�ָ��
		ThreadWorker(ThreadPool* pool) : pool{ pool } {} // ���캯��
		void operator ()() { // ���غ������ò��������߳�������ʱ��ִ�иú���
			while (!pool->shutdown) {
				{
					// �ȴ��̳߳�����������̳߳��Ѿ��ر�
					// ���������������������̳߳عر�/�̳߳�������
					unique_lock<mutex> lock(pool->m_mutex);
					pool->cv.wait(lock, [this]() {
						return this->pool->shutdown ||
							!this->pool->que.empty();
						});
				}
				// �����������ȡ��һ������
				function<void()>func;
				bool flag = pool->que.pop(func);
				if (flag) { // ����ɹ�ȡ����һ������
					func(); // ִ�и�����
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
	// n Ϊ�̳߳��еĹ����߳���
	ThreadPool(int n) : threads(n), shutdown{ false } {
		// ���� n �������̣߳����� ThreadWorker ���󴫵ݸ�ÿ���߳�
		for (auto& t : threads) {
			t = thread{ ThreadWorker(this) };
		}
	}
	// ��ֹ�������ƶ�
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	//F���� Args����
	template <typename F, typename... Args>
	//&&��ֵ���� �˴�������ֵ���� Args�ɱ������ future�첽�������
	//decltype����ʵ�ʱ��ʽ���ݳ��������ʱ���������
	//����һ��future���͵Ķ��󣬶���ĺ��������Զ��Ƶ��ó�
	auto submit(F&& f, Args &&...args) -> std::future<decltype(f(args...))> {
		//����һ��lambda���� ��װ��һ��function������
		Log_Cout("�̳߳��߳�ִ��");
		function<decltype(f(args...))()> func = [&f, args...]() {return f(args...); };
		//����һ������ָ��
		auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
		//����һ��lambda��������װ��һ��funtion�����У�������ִ�з�װ��task_ptr��ָ��
		std::function<void()> warpper_func = [task_ptr]() {
			(*task_ptr)();
		};
		que.push(warpper_func);
		cv.notify_one();
		//����ֵ������future����
		return task_ptr->get_future();
	}
	~ThreadPool() {
		//�ȴ���������ִ�����
		auto f = submit([]() {});
		f.get();

		shutdown = true;
		cv.notify_all(); // ֪ͨ���������й����߳�
		// �ȴ����й����߳��˳�
		for (auto& t : threads) {
			if (t.joinable()) t.join();
		}
	}
};
