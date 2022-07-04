/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#pragma once

#include <cassert>
#include <memory>

#include "./jeisson/Thread.hpp"
#include "./StatusQueue.hpp"

template<typename T>
class StatusConsumer : public virtual Thread
{

	DISABLE_COPY(StatusConsumer);

public:
	explicit StatusConsumer(std::shared_ptr<StatusQueue<T>> consumingQueue,
			const T& stopCondition)
			:consumingQueue(consumingQueue), stopCondition(stopCondition)
	{
	}

	void check();

	virtual void finalize() = 0;

	virtual void consume(T data) = 0;

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
