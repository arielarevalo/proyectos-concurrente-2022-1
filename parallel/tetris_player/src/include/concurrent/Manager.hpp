// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

// TODO(aarevalo): No repetir los errores de nuestros antepasados (memoria estática)

#pragma once

#include <unistd.h>
#include <iostream>
#include <vector>

#include "../common/PlayState.hpp"
#include "./jeisson/Assembler.hpp"
#include "./jeisson/Producer.hpp"
#include "./jeisson/Queue.hpp"

#define NUM_UNITS 10

class Manager
{
public:
	static void processTetrisState(const GameState gameState);
private:
    class Producer : public Producer<PlayState> {
    public:
        Producer
                (std::vector<PlayState*> PlayState, int numConsumers):
                products(PlayState), numConsumers(numConsumers) {}

        int run() override;

    private:
        std::vector<PlayState*> products;
        int numConsumers;
    };

    class Consumer : public Consumer<PlayState>
    {
    public:
        Consumer(Queue<PlayState>* consumingQueue,
                       const PlayState& stopCondition,
                       bool createOwnQueue)
                :Consumer<PlayState>(consumingQueue, stopCondition,
                          createOwnQueue)
        {
        }

        void consume(PlayState PlayState) override;

        int run() override;
    };

	static const PlayState stopCondition;
};

const PlayState Manager::stopCondition;

void Manager::processTetrisState(const GameState gameState)
{
	int numConsumers = sysconf(_SC_NPROCESSORS_ONLN);
	int numUnits = NUM_UNITS;

	std::cout << "Starting simulation ..." << std::endl;

	std::vector<PlayState*> units;
	double results[NUM_UNITS];

	for (int unit = 0; unit < numUnits; ++unit)
	{
		results[unit] = -1.0;
		units.push_back(new PlayState(unit, &results[unit]));
	}

	auto queue = new Queue<PlayState>();

	auto producer = new Producer(units, numConsumers);
	producer->setProducingQueue(queue);

	std::vector<Consumer*> consumers;

	for (int con = 0; con < numConsumers; ++con)
	{
		auto consumer = new Consumer(queue, stopCondition,
				false);
		consumers.push_back(consumer);
		consumer->startThread();
	}

	producer->startThread();

	std::cout << "Waiting producer to finish" << std::endl;

	producer->waitToFinish();

	std::cout << "Waiting consumers to finish" << std::endl;
	for (int con = 0; con < numConsumers; ++con)
	{
		consumers[con]->waitToFinish();
		delete consumers[con];
	}

	std::cout << "\nResult: " << std::endl;
	for (int unit = 0; unit < numUnits; ++unit)
	{
		std::cout << "Fact of " << units[unit]->number << " is "
				  << *units[unit]->result << std::endl;
		delete units[unit];
	}
	delete queue;
	delete producer;
}

int Manager::Producer::run() {
    for (size_t i = 0; i < products.size(); ++i) {
        PlayState unit = *products[i];
        this->produce(unit);
        std::cout << "Produced: " << unit.number << std::endl;
    }

    for (int c = 0; c < numConsumers; ++c) {
        this->produce(PlayState());
    }

    return EXIT_SUCCESS;
}

int Manager::Consumer::run()
{
    this->consumeForever();

    return EXIT_SUCCESS;
}

void Manager::Consumer::consume(PlayState PlayState)
{
    if (PlayState.number > 0)
    {
        *PlayState.result = 1;

        for (int i = 1; i <= PlayState.number; ++i)
        {
            *PlayState.result *= i;
        }
    }

    std::cout << "      Consumed" << PlayState.number << std::endl;
}