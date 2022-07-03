// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <memory>

#include "./jeisson/Thread.hpp"
#include "./StatusQueue.hpp"

template<typename T>
class StatusQueue;

template<typename T>
class StatusConsumer : public virtual Thread
{

	DISABLE_COPY(StatusConsumer);

public:
	explicit StatusConsumer(const T& stopCondition)
			:stopCondition(stopCondition)
	{
	}

	void check();

	virtual void finalize() = 0;

	virtual void consume(T data) = 0;

	virtual void consumeForever();

	virtual void setConsumingQueue(
			std::shared_ptr<StatusQueue<T>> consumingQueue);

	bool isBusy() const;

protected:
	const T stopCondition;

	std::shared_ptr<StatusQueue<T>> consumingQueue;

private:
	bool busy{ false };
};

template<typename T>
void StatusConsumer<T>::check()
{
	if (this->consumingQueue->isDone())
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

		busy = true;

		consumingQueue->refreshSize();

		if (data == stopCondition)
		{
			busy = false;
			break;
		}

		consume(data);

		consumingQueue->refreshSize();

		busy = false;

		check();
	}
}

template<typename T>
void StatusConsumer<T>::setConsumingQueue(
		std::shared_ptr<StatusQueue<T>> consumingQueue)
{
	this->consumingQueue = consumingQueue;
}

template<typename T>
bool StatusConsumer<T>::isBusy() const
{
	return busy;
}
