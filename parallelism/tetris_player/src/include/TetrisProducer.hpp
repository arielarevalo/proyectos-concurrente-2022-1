// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <vector>
#include <iostream>
#include "./prodcons/Producer.hpp"
#include "./TetrisWorkUnit.hpp"

class TetrisProducer : public Producer<TetrisWorkUnit> {
 public:
    TetrisProducer
    (std::vector<TetrisWorkUnit*> tetrisWorkUnit, int numConsumers):
    products(tetrisWorkUnit), numConsumers(numConsumers) {}

    int run() override;

 private:
    std::vector<TetrisWorkUnit*> products;
    int numConsumers;
};

int TetrisProducer::run() {
    for (size_t i = 0; i < products.size(); ++i) {
        TetrisWorkUnit unit = *products[i];
        this->produce(unit);
        std::cout << "Produced: " << unit.number << std::endl;
    }

    for (int c = 0; c < numConsumers; ++c) {
        this->produce(TetrisWorkUnit());
    }

    return EXIT_SUCCESS;
}