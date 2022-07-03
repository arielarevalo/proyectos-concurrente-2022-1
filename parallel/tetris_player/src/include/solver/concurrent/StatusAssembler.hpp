// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <memory>

#include "./jeisson/Assembler.hpp"
#include "./StatusConsumer.hpp"
#include "./StatusProducer.hpp"
#include "./StatusQueue.hpp"

template<typename T>
class StatusQueue;

/**
 * Single queue
 * @tparam T
 */
template<typename T>
class StatusAssembler : public StatusConsumer<T>, public StatusProducer<T>
{
public:
	explicit StatusAssembler(const T& stopCondition)
			:StatusConsumer<T>(stopCondition),
			 StatusProducer<T>()
	{
	}

	void finalize() override;

	void
	setConsumingQueue(std::shared_ptr<StatusQueue<T>> consumingQueue) override;
};

template<typename T>
void StatusAssembler<T>::finalize()
{
	size_t numMembers{ this->consumingQueue->getMembersSize() };

	for (size_t w{ 0 }; w < numMembers; ++w)
	{
		this->produce(this->stopCondition);
	}
}

template<typename T>
void StatusAssembler<T>::setConsumingQueue(
		std::shared_ptr<StatusQueue<T>> consumingQueue)
{
	StatusProducer<T>::setProducingQueue(consumingQueue);
	StatusConsumer<T>::setConsumingQueue(consumingQueue);
}
