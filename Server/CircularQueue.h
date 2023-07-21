/*
	2023/07/17 by V.r
	循环队列缓冲区（含迭代器）
*/
#pragma once
#include "stdafx.h"

const int BUFFER_MAX = 4096;
// const int BUFFER_MAX = 1024;
// const int BUFFER_MAX = 128;

template<class T>
class CircularQueue {
private:
	T* data;
	mutex mtx;
	int capacity;
	int frontIdx;
	int rearIdx;
	int count;
public:
	class Iterator {
	private:
		CircularQueue<T>* queue;
		int curIdx;
	public:
		Iterator(CircularQueue<T>* q, int idx) :queue(q), curIdx(idx) {}
		Iterator& operator++()
		{
			curIdx = (curIdx + 1) % queue->capacity;
			return *this;
		}

		bool operator!=(const Iterator& right)const
		{
			return curIdx != right.curIdx;
		}

		T& operator*()
		{
			return queue->data[curIdx];
		}
	};

	CircularQueue() : capacity(BUFFER_MAX), frontIdx(0), rearIdx(0), count(0)
	{
		data = new T[capacity];
	}

	~CircularQueue()
	{
		delete[] data;
	}

	void enqueue(const T& element)
	{
		if (full())
		{
			expand();
		}
		if (count < capacity)
		{
			data[rearIdx] = element;
			rearIdx = (rearIdx + 1) % capacity;
			count++;
		}
	}

	void dequeue()
	{
		if (!empty())
		{
			frontIdx = (frontIdx + 1) % capacity;
			count--;
		}
	}

	T& front()
	{
		if (!empty())
		{
			return data[frontIdx];
		}
	}

	bool empty()
	{
		return count == 0;
	}

	bool full()
	{
		return count == capacity;
	}

	bool enough(size_t len)
	{
		return (count + len) <= capacity;
	}

	bool expand()
	{
		if (capacity <= (BUFFER_MAX * 16))
		{
			int newCapacity = capacity * 2;
			T* tmp = new T[newCapacity];
			int i = 0;
			for (auto it = begin(); it != end(); ++it) {
				tmp[i++] = *it;
			}
			frontIdx = 0;
			rearIdx = count;
			delete[] data;
			data = tmp;
			capacity = newCapacity;
		}
		else
		{
			// Log_Test(fmt::format("{} -> {} 消息队列队满", __FUNCTION__, __LINE__));
			printf("%s -> %d 消息队列队满\n", __FUNCTION__, __LINE__);
			Sleep(100 * 1000);
			return false;
		}
		return true;
	}

	Iterator begin()
	{
		return Iterator(this, frontIdx);
	}

	Iterator end()
	{
		return Iterator(this, rearIdx);
	}

	void push(const T* receivebuffer, size_t len)
	{
		unique_lock<mutex> lock(mtx);
		if (!enough(len))
		{
			expand();
		}
		for (int idx = 0; idx < len; ++idx)
		{
			this->enqueue(receivebuffer[idx]);
		}
	}

	void pop(size_t len)
	{
		unique_lock<mutex> lock(mtx);
		for (int idx = 0; idx < len; ++idx)
		{
			this->dequeue();
		}
	}

	void pop(size_t len, T* buffer)
	{
		unique_lock<mutex> lock(mtx);
		for (int idx = 0; idx < len; ++idx)
		{
			buffer[idx] = front();
			dequeue();
		}
	}

	void get(size_t len, T* buffer)
	{
		unique_lock<mutex> lock(mtx);
		Iterator it = this->begin();
		for (int idx = 0; idx < len; ++idx)
		{
			buffer[idx] = *it;
			++it;
		}
	}

	int GetCount()
	{
		unique_lock<mutex> lock(mtx);
		return count;
	}
};
