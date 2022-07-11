/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#pragma once

#include <mutex>
#include <queue>

#include "./jeisson/Common.hpp"
#include "./jeisson/Semaphore.hpp"

/**
 * @brief Holds work units and is able to broadcast whether work is done.
 * @tparam T Data type the queue will hold.
 */
template<typename T>
class StatusQueue
{
	DISABLE_COPY(StatusQueue);

public:
	/**
	 * @brief Constructs an empty StatusQueue.
	 */
	StatusQueue() = default;

	/**
	 * @brief Thread safe adds data onto queue.
	 * @param data Data to add.
	 */
	void push(const T& data);

	/**
	 * @brief Thread safe removes data from queue.
	 * @param data Data to remove.
	 */
	T pop();

	/**
	 * @brief Signals whether work at this queue has completed.
	 * @return Whether work has completed.
	 */
	virtual bool done() const = 0;

	/**
	 * @brief Returns whether queue is empty.
	 * @return Whether queue is empty.
	 */
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


