// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <memory>

#include "./StatusQueue.hpp"
#include "./StatusConsumer.hpp"
#include "./StatusProducer.hpp"

/**
 * @brief Assembles from production and consumption queues
 * @details until StatusConsumer check is true, at which point it calls
 * finalize. Finalize is defined here.
 * @tparam T Data type for the consumption queue.
 * @tparam U Data type for the production queue.
 */
template<typename T, typename U>
class StatusAssembler : public StatusConsumer<T>, public StatusProducer<U>
{

	DISABLE_COPY(StatusAssembler);

public:
	/**
	 * Constructs a StatusAssembler instance.
	 * @param consumingQueue Consumption queue to pull from.
	 * @param producingQueue Production queue to push to.
	 * @param stopCondition Data type object that denotes shutdown.
	 */
	StatusAssembler(std::shared_ptr<StatusQueue<T>> consumingQueue,
			std::shared_ptr<StatusQueue<U>> producingQueue,
			const T& stopCondition)
			:StatusConsumer<T>(consumingQueue, stopCondition),
			 StatusProducer<U>(producingQueue)
	{
	}

private:
	/**
	 * @brief Task to run in thread. Consumes forever until stop condition.
	 * @return Exit code.
	 */
	int run() override;

	/**
	 * @brief Pushes stopCondition onto appropriate queue.
	 */
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