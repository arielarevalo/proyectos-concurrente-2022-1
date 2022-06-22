// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <unistd.h>
#include <iostream>
#include <vector>

#include "TetrisWorkUnit.hpp"
#include "TetrisConsumer.hpp"
#include "TetrisProducer.hpp"
#include "prodcons/Queue.hpp"

#define NUM_UNITS 10

class ConcurrencyManager {
 public:
    static void processTetrisState();
};

void ConcurrencyManager::processTetrisState()
{
    int numConsumers = sysconf(_SC_NPROCESSORS_ONLN);
    int numUnits = NUM_UNITS;

    std::cout << "Starting simulation ..." << std::endl;

    std::vector<TetrisWorkUnit*> units;
    double results[NUM_UNITS];

    for (int unit = 0; unit < numUnits; ++unit) {
        results[unit] = -1.0;
        units.push_back(new TetrisWorkUnit(unit, &results[unit]));
    }

    Queue<TetrisWorkUnit>* queue = new Queue<TetrisWorkUnit>();

    TetrisProducer* producer = new TetrisProducer(units, numConsumers);
    producer->setProducingQueue(queue);

    std::vector<TetrisConsumer*> consumers;

    for (int con = 0; con < numConsumers; ++con) {
        TetrisConsumer* consumer = new TetrisConsumer();
        consumer->setConsumingQueue(queue);
        consumers.push_back(consumer);
        consumer->startThread();
    }

    producer->startThread();

    std::cout << "Waiting producer to finish" << std::endl;

    producer->waitToFinish();

    std::cout << "Waiting consumers to finish" << std::endl;
    for (int con = 0; con < numConsumers; ++con) {
        consumers[con]->waitToFinish();
        delete consumers[con];
    }

    std::cout << "\nResult: " << std::endl;
    for (int unit = 0; unit < numUnits; ++unit) {
        std::cout << "Fact of " << units[unit]->number << " is "
                  << *units[unit]->result << std::endl;
        delete units[unit];
    }
    delete queue;
    delete producer;
}