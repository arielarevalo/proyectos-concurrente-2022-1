/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#pragma once

#include <mutex>
#include <queue>

#include "./jeisson/Common.hpp"
#include "./jeisson/Semaphore.hpp"

template<typename T>
class StatusQueue
{
	DISABLE_COPY(StatusQueue);

public:
	StatusQueue() = default;

	void push(const T& data);

	T pop();

	virtual bool done() const = 0;

	bool empty() const;

protected:
	virtual ~StatusQueue() = default;

private:
	std::mutex mutex;

	Semaphore canConsume{ 0 };

	std::queue<T> queue;
};

template<typename T>
void StatusQueue<T>::push(const T& data)
{
	std::scoped_lock lock{mutex};
	this->queue.push(data);
	this->canConsume.signal();
}

template<typename T>
T StatusQueue<T>::pop()
{
	this->canConsume.wait();
	std::scoped_lock lock{mutex};
	T result = this->queue.front();
	this->queue.pop();
	return result;
}

template<typename T>
bool StatusQueue<T>::empty() const
{
	return queue.empty();
}


