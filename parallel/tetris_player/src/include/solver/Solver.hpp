// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <unistd.h>
#include <cstddef>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "./concurrent/StatusAssembler.hpp"
#include "./concurrent/StatusProducer.hpp"
#include "./concurrent/StatusQueue.hpp"
#include "./common/WorkState.hpp"
#include "./logger/Logger.hpp"

//TODO(aarevalo): Commnents.

using History = std::deque<PlayState>;

class Solver
{
public:
	static History solve(const GameState& gameState);

private:
	class TQueue : public StatusQueue<WorkState>
	{
		bool done() const override;
	};
	class TAssembler : public StatusAssembler<WorkState, WorkState>
	{
	public:
		using StatusAssembler::StatusAssembler;

		/**
		 * Consumes each element in the queue. Handles each WorkState object by
		 * placing the latest permutation and either permuting or comparing
		 * the resulting WorkState object.
		 * @param WorkState WorkState object to be consumed.
		 */
		void consume(WorkState current) override;
	};

	class TProducer : public StatusProducer<WorkState>
	{
	public:
		TProducer(std::shared_ptr<StatusQueue<WorkState>> producingQueue,
				const GameState& gameState)
				:StatusProducer<WorkState>(producingQueue), gameState(gameState)
		{
		}

		int run() override;

		const GameState& gameState;
	};

	static void startAssemblers(const GameState& gameState);

	static void waitForAssemblers();

	static void startProducer(const GameState& gameState);

	static void compare(const History& incoming);

	static std::vector<std::unique_ptr<TAssembler>> assemblers;

	static std::unique_ptr<TProducer> producer;

	static std::shared_ptr<TQueue> statusQueue;

	static std::unique_ptr<History> highScore;

	static std::mutex mutex;
};

std::vector<std::unique_ptr<Solver::TAssembler>> Solver::assemblers{};

std::unique_ptr<Solver::TProducer> Solver::producer{};

std::shared_ptr<Solver::TQueue> Solver::statusQueue{};

std::unique_ptr<History> Solver::highScore{};

std::mutex Solver::mutex{};

History Solver::solve(const GameState& gameState)
{
	statusQueue = std::make_unique<TQueue>();

	startProducer(gameState);

	producer->waitToFinish();

	startAssemblers(gameState);

	waitForAssemblers();

	if (highScore)
	{
		return *highScore;
	}
	else
	{
		throw std::domain_error("No valid moves down to requested depth.");
	}
}

bool Solver::TQueue::done() const
{
	return empty();
}

void Solver::TAssembler::consume(WorkState current)
{
	History candidate{ current.work() };
	if (!candidate.empty())
	{
		compare(candidate);
	}
}

int Solver::TProducer::run()
{
	Tetrimino::Figure firstTetrimino{ gameState.nextTetriminos[0] };
	size_t rotations{ Tetrimino::getTetriminoRotations(firstTetrimino) };
	size_t columns{ gameState.playArea.cols };

	for (size_t i{ 0 }; i < rotations; ++i)
	{
		for (size_t j{ 0 }; j < columns; ++j)
		{
			produce(WorkState{ gameState, Position{ i, j }});
		}
	}
	return EXIT_SUCCESS;
}

void Solver::startAssemblers(const GameState& gameState)
{
	long numAssemblers{ sysconf(_SC_NPROCESSORS_ONLN) };

	//impossible state
	WorkState stopCondition{ gameState, Position{ 0, gameState.playArea.cols }};

	for (size_t a{ 0 }; a < numAssemblers; ++a)
	{
		assemblers.push_back(
				std::make_unique<TAssembler>(statusQueue, statusQueue,
						stopCondition));
		assemblers.back()->startThread();
	}
}

void Solver::waitForAssemblers()
{
	while(!assemblers.empty()) {
		assemblers.back()->waitToFinish();
		assemblers.pop_back();
	}
}

void Solver::startProducer(const GameState& gameState)
{
	producer = std::make_unique<TProducer>(statusQueue, gameState);

	producer->startThread();
}

void Solver::compare(const History& incoming)
{
	std::scoped_lock lock{ mutex };
	if (!highScore || incoming.back() > highScore->back())
	{
		highScore = std::make_unique<History>(incoming);
	}
}
