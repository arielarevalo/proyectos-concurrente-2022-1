// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <memory>

#include "./jeisson/Thread.hpp"
#include "./StatusQueue.hpp"

template<typename T>
class StatusProducer : public virtual Thread
{

	DISABLE_COPY(StatusProducer);

public:
	StatusProducer() = default;

	void setProducingQueue(std::shared_ptr<StatusQueue<T>> producingQueue);

	virtual void produce(const T& data);

private:
	std::shared_ptr<StatusQueue<T>> producingQueue;
};

template<typename T>
void StatusProducer<T>::setProducingQueue(
		std::shared_ptr<StatusQueue<T>> producingQueue)
{
	this->producingQueue = producingQueue;
}

template<typename T>
void StatusProducer<T>::produce(const T& data)
{
	assert(this->producingQueue);
	this->producingQueue->push(data);
}

