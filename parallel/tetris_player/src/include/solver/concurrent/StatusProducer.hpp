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
class StatusProducer : public virtual Thread
{

	DISABLE_COPY(StatusProducer);

public:
	/**
	 * Constructs a StatusProducer instance.
	 * @param producingQueue Production queue to push to.
	 */
	explicit StatusProducer(std::shared_ptr<StatusQueue<T>> producingQueue)
			:producingQueue(producingQueue)
	{
	}

	/**
	 * @brief Pushes one element to production queue.
	 * @param data
	 */
	virtual void produce(const T& data);

private:
	std::shared_ptr<StatusQueue<T>> producingQueue;
};


template<typename T>
void StatusProducer<T>::produce(const T& data)
{
	assert(this->producingQueue);
	this->producingQueue->push(data);
}

