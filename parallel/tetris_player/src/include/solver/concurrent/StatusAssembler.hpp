// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include "./jeisson/Assembler.hpp"
#include "./StatusQueue.hpp"

template<typename T>
class StatusQueue;

/**
 * Single queue
 * @tparam T
 */
template<typename T>
class StatusAssembler : public StatusConsumer<T>, public Producer<T>
{
public:
	explicit StatusAssembler(const T& stopCondition)
	: StatusConsumer<T>(stopCondition) {}

	virtual void consume(T data) = 0;

	void setStatusQueue(StatusQueue<T> statusQueue);
};

template<typename T>
void StatusAssembler<T>::consume(T data)
{
	if(this->consumingQueue->isDone()) {
		size_t numMembers{ this->consumingQueue->getMembersSize()};

		for(size_t w{0}; w < numMembers; ++w) {
			this->produce(this->stopCondition);
		}
	} else {
		this->produce(data);
	}
}

template<typename T>
void StatusAssembler<T>::setStatusQueue(StatusQueue<T> statusQueue)
{
	this->setProducingQueue(statusQueue);
	this->setConsumingQueue(statusQueue);
}


