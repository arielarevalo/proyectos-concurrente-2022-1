// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <memory>

#include "./StatusQueue.hpp"
#include "./StatusConsumer.hpp"
#include "./StatusProducer.hpp"

/**
 * Single queue
 * @tparam T
 */
template<typename T, typename U>
class StatusAssembler : public StatusConsumer<T>, public StatusProducer<U>
{

	DISABLE_COPY(StatusAssembler);

public:
	StatusAssembler(std::shared_ptr<StatusQueue<T>> consumingQueue,
			std::shared_ptr<StatusQueue<U>> producingQueue,
			const T& stopCondition)
			:StatusConsumer<T>(consumingQueue, stopCondition),
			 StatusProducer<U>(producingQueue)
	{
	}

private:
	/**
	 * Task to run in thread. Consumes forever until stop condition.
	 * @return Exit code.
	 */
	int run() override;

	void finalize() override;
};

template<typename T, typename U>
int StatusAssembler<T, U>::run()
{
	this->consumeForever();

	return EXIT_SUCCESS;
}

template<typename T, typename U>
void StatusAssembler<T, U>::finalize()
{
	this->produce(this->stopCondition);
}