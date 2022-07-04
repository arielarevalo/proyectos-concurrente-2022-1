// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <unistd.h>
#include <cstddef>
#include <iostream>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>
#include <queue>

#include "./concurrent/StatusAssembler.hpp"
#include "./concurrent/StatusQueue.hpp"
#include "./common/WorkState.hpp"
#include "./logger/Logger.hpp"

class Solver
{
public:
	static std::queue<PlayState> solve(const GameState& gameState);

private:
	class TAssembler : public StatusAssembler<WorkState>
	{
	public:
		using StatusAssembler::StatusAssembler;

		/**
		 * Task to run in thread. Consumes forever until stop condition.
		 * @return Exit code.
		 */
		int run() override;

		/**
		 * Consumes each element in the queue. Handles each WorkState object by
		 * placing the latest permutation and either permuting or comparing
		 * the resulting WorkState object.
		 * @param WorkState WorkState object to be consumed.
		 */
		void consume(WorkState current) override;

		/**
		 *
		 * @param current
		 */
		void produceNext(const WorkState& current);
	};

	static void initializeQueue(std::shared_ptr<StatusQueue<WorkState>>& queue,
			const GameState& gameState);

	static void compare(const WorkState& current);

	static std::unique_ptr<WorkState> highScore;

	static std::mutex mutex;
};

std::unique_ptr<WorkState> Solver::highScore{};

std::mutex Solver::mutex{};

std::queue<PlayState> Solver::solve(const GameState& gameState)
{
	std::shared_ptr<StatusQueue<WorkState>> statusQueue;

	initializeQueue(statusQueue, gameState);

	statusQueue->refreshSize();

	statusQueue->startConsumers();

	statusQueue->waitForConsumers();

	if (highScore)
	{
		return highScore->getPlayStates();
	}
	else
	{
		throw std::domain_error("No valid moves down to requested depth.");
	}
}

void Solver::initializeQueue(std::shared_ptr<StatusQueue<WorkState>>& queue,
		const GameState& gameState)
{
	long numAssemblers{ sysconf(_SC_NPROCESSORS_ONLN) };

	for (int a{ 0 }; a < numAssemblers; ++a)
	{
		std::shared_ptr<TAssembler> assembler
				{ std::make_shared<TAssembler>(WorkState::stopCondition) };
		queue = StatusQueue<WorkState>::signUp(assembler);
	}

	Tetrimino::Figure firstTetrimino{ gameState.nextTetriminos[0] };
	size_t rotations{ Tetrimino::getTetriminoRotations(firstTetrimino) };
	size_t columns{ gameState.playArea.cols };

	for (size_t i{ 0 }; i < rotations; ++i)
	{
		for (size_t j{ 0 }; j < columns; ++j)
		{
			queue->push(WorkState{ gameState, Position{ i, j }});
		}
	}
	queue->refreshSize();
}

void Solver::compare(const WorkState& current)
{
	std::scoped_lock lock{ mutex };
	if (!highScore || current > *highScore)
	{
		highScore = std::make_unique<WorkState>(current);
	}
}

int Solver::TAssembler::run()
{
	this->consumeForever();

	return EXIT_SUCCESS;
}

void Solver::TAssembler::consume(WorkState current)
{
	if (current.place())
	{
		if (!current.isMaxDepth())
		{
			WorkState child{ std::make_shared<WorkState>(current) };
			consume(child);
		}
		else
		{
			compare(current);
			produceNext(current);
		}
	}
	else
	{
		produceNext(current);
	}
}

void Solver::TAssembler::produceNext(const WorkState& current)
{
	WorkState next{ WorkState::getNext(current) };
	if (!(WorkState::stopCondition == std::as_const(next)))
	{
		produce(next);
	}
}
