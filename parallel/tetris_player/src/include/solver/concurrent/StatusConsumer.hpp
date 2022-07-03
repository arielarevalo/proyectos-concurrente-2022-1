// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include "./StatusQueue.hpp"

template<typename T>
class StatusQueue;

template<typename T>
class StatusConsumer : public virtual Thread {

	DISABLE_COPY(StatusConsumer);

public:
	explicit StatusConsumer(const T& stopCondition)
	: stopCondition(stopCondition) {}

	void consumeForever();

	virtual void consume(T data) = 0;

	bool isBusy();

	void setConsumingQueue(StatusQueue<T>* consumingQueue);

protected:
	const T stopCondition;

	StatusQueue<T>* consumingQueue;

private:
	bool busy{ false };
};

template<typename T>
void StatusConsumer<T>::consumeForever()
{
	assert(this->consumingQueue);
	while (true)
	{
		T data = this->consumingQueue->pop();

		this->busy = true;

		if (data == this->stopCondition)
		{
			this->busy = false;

			break;
		}

		this->consume(data);

		this->busy = false;
	}
}

template<typename T>
void StatusConsumer<T>::setConsumingQueue(StatusQueue<T>* consumingQueue)
{
	this->consumingQueue = consumingQueue;
}

template<typename T>
bool StatusConsumer<T>::isBusy()
{
	return busy;
}
