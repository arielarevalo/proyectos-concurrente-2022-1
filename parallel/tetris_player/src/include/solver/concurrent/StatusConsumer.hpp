/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#pragma once

#include <cassert>
#include <memory>

#include "./jeisson/Thread.hpp"
#include "./StatusQueue.hpp"

/**
 * @brief Consumes a consumption queues
 * @details until check is true, at which point it calls finalize.
 * @tparam T Data type for the consumption queue.
 */
template<typename T>
class StatusConsumer : public virtual Thread
{

	DISABLE_COPY(StatusConsumer);

public:
	/**
	 * Constructs a StatusConsumer instance.
	 * @param consumingQueue Consumption queue to pull from.
	 * @param stopCondition Data type object that denotes shutdown.
	 */
	explicit StatusConsumer(std::shared_ptr<StatusQueue<T>> consumingQueue,
			const T& stopCondition)
			:consumingQueue(consumingQueue), stopCondition(stopCondition)
	{
	}

	/**
	 * @brief Calls method from StatusQueue to confirm whether work is done.
	 */
	void check();

	/**
	 * @brief Method to be called when work is done, to induce shutdown.
	 */
	virtual void finalize() = 0;

	/**
	 * @brief Pulls one element from consumption queue and processes it.
	 * @param data
	 */
	virtual void consume(T data) = 0;

	/**
	 * @brief Continuously consumes and checks for finalization.
	 */
	virtual void consumeForever();

protected:
	const T stopCondition;

private:
	std::shared_ptr<StatusQueue<T>> consumingQueue;
};

template<typename T>
void StatusConsumer<T>::check()
{
	if (this->consumingQueue->done())
	{
		finalize();
	}
}

template<typename T>
void StatusConsumer<T>::consumeForever()
{
	assert(consumingQueue);
	while (true)
	{
		T data{ consumingQueue->pop() };

		if (data == stopCondition)
		{
			finalize();
			break;
		}

		consume(data);

		check();
	}
}
