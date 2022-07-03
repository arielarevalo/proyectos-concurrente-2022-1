// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <unistd.h>
#include <cstddef>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <queue>

#include "./concurrent/StatusAssembler.hpp"
#include "./concurrent/StatusQueue.hpp"
#include "./common/History.hpp"
#include "./logger/Logger.hpp"

class Solver
{
public:
	static History solve(const GameState& gameState);

private:
	class TAssembler : public StatusAssembler<History>
	{
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
	};

	static void compare(History& current);

	static std::unique_ptr<History> highScore;

	static std::mutex mutex;
};

std::unique_ptr<History> Solver::highScore{};

std::mutex Solver::mutex{};

History Solver::solve(const GameState& gameState)
{
	long numAssemblers{ sysconf(_SC_NPROCESSORS_ONLN) };

	highScore = std::make_unique<History>(gameState);

	History stopCondition{ gameState, true };

	std::shared_ptr<StatusQueue<History>> statusQueue;

	for (int a{ 0 }; a < numAssemblers; ++a)
	{
		std::shared_ptr<TAssembler> assembler
				{ std::make_shared<TAssembler>(stopCondition) };
		statusQueue = StatusQueue<History>::signUp(assembler);
	}

	History initialState{ gameState };
	statusQueue->push(initialState);
	statusQueue->refreshSize();

	statusQueue->startConsumers();

	statusQueue->waitForConsumers();

	if (!highScore->isEmpty())
	{
		return *highScore;
	}
	else
	{
		throw std::domain_error("No valid moves down to requested depth.");
	}
}

int Solver::TAssembler::run()
{
	this->consumeForever();

	return EXIT_SUCCESS;
}

void Solver::TAssembler::consume(History history)
{
	if (history.place())
	{
		if (history.isMaxDepth())
		{
			compare(history);
		}
		else
		{
			std::queue<History> histories{ history.permutate() };
			while (!histories.empty())
			{
				produce(histories.front());
				histories.pop();
			}
		}
	}
}

void Solver::compare(History& current)
{
	std::scoped_lock<std::mutex> lock{ mutex };
	if (highScore->isEmpty() || *current.getLast() > *highScore->getLast())
	{
		highScore = std::make_unique<History>(current);
	}
}