// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

// TODO(aarevalo): No repetir los errores de nuestros antepasados (memoria estática)

#pragma once

#include <unistd.h>
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

#include "./concurrent/StatusAssembler.hpp"
#include "./concurrent/StatusQueue.hpp"
#include "./common/History.hpp"
#include "./logger/Logger.hpp"

class Solver {
public:
    static History processGameState(const GameState &gameState);

private:
    static std::unique_ptr<History> highScore;

    static void compare(History &current);

    class TAssembler : public StatusAssembler<History> {
    public:
        using StatusAssembler::StatusAssembler;

        /**
         * Task to run in thread. Consumes forever until stop condition.
         * @return Exit code.
         */
        int run() override;

        /**
         * Consumes each element in the queue. Handles each History object by
         * placing the latest permutation and either permuting or comparing
         * the resulting History object.
         * @param History History object to be consumed.
         */
        void consume(History History) override;

        /**
         * Produces each element in the queue. Handles each outgoing History
         * object by pushing the updated History object to the queue if it is
         * not done being assembled.
         * @param history
         */
        void produce(const History &history) override;
    };
};

History Solver::processGameState(const GameState &gameState) {
    long numAssemblers{sysconf(_SC_NPROCESSORS_ONLN)};

    highScore = std::make_unique<History>(gameState);

    History highScore{gameState};
    History stopCondition{gameState};
    std::shared_ptr<StatusQueue<History>> statusQueue;

    for (int a{0}; a < numAssemblers; ++a) {
        std::shared_ptr<TAssembler> assembler
                {std::make_shared<TAssembler>(stopCondition)};
        statusQueue = StatusQueue<History>::signUp(assembler);
    }

    History initialState{gameState};
    initialState.push(std::make_shared<PlayState>(gameState));

    statusQueue->startConsumers();

    Logger::info("Waiting for assemblers to finish.");

    statusQueue->waitForConsumers();

    if (!highScore.isEmpty()) {
        return highScore;
    } else {
        throw std::domain_error("No valid moves down to requested depth.");
    }
}

int Solver::TAssembler::run() {
    this->consumeForever();

    return EXIT_SUCCESS;
}

// Test THIS!!!!
void Solver::compare(History &current) {
    if (highScore->isEmpty()) {
        highScore = std::make_unique<History>(current);
    } else if (current.getLast() > highScore->getLast()) {
        highScore = std::make_unique<History>(current);
    }
}

void Solver::TAssembler::consume(History history) {
    if (history.place()) {

        if (history.reviewCurrentDepth()) {
            compare(history);
            history.setDone();
            StatusAssembler<History>::consume(history);
        } else {
            std::queue<History> histories{history.permutate()};

            while (!histories.empty()) {
                StatusAssembler<History>::consume(histories.front());
                histories.pop();
            }
        }
    } else {
        history.setDone();
        StatusAssembler<History>::consume(history);
    }
}

void Solver::TAssembler::produce(const History &history) {
    // * si !History.isDone()
    if (!history.isDone()) {
        Producer::produce(history);
    }
}