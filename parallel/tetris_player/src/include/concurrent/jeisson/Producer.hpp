/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#pragma once

#include <cassert>

#include "Queue.hpp"
#include "Thread.hpp"

/**
 * @brief A template that generates abstract base classes for Producers
 * Producers are execution threads. They create elements that are pushed
 * to a queue. These elements will be popped by a consumer thread.
 */
template<typename DataType>
class Producer : public virtual Thread
{
	/// Objects of this class cannot be copied
	DISABLE_COPY(Producer);

public:
	/// Constructor
	explicit Producer(Queue<DataType>* producingQueue = nullptr)
			: producingQueue(producingQueue)
	{
	}

	/// Get access to the queue where this thread will produce
	Queue<DataType>* getProducingQueue();

	/// Set the queue where this thread will produce elements
	void setProducingQueue(Queue<DataType>* producingQueue);

	/// Add to the queue the produced data unit
	virtual void produce(const DataType& data);

private:
	/// This thread will produce for this queue
	Queue<DataType>* producingQueue;
};
template<typename DataType>
Queue<DataType>* Producer<DataType>::getProducingQueue()
{
	return this->producingQueue;
}

template<typename DataType>
void Producer<DataType>::setProducingQueue(Queue<DataType>* producingQueue)
{
	this->producingQueue = producingQueue;
}

template<typename DataType>
void Producer<DataType>::produce(const DataType& data)
{
	assert(this->producingQueue);
	this->producingQueue->push(data);
}

