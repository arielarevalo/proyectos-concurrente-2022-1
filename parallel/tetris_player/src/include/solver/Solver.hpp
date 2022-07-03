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
		void consume(WorkState WorkState) override;
	};

	static void compare(WorkState& current);

	static std::unique_ptr<WorkState> highScore;

	static std::mutex mutex;
};

std::unique_ptr<WorkState> Solver::highScore{};

std::mutex Solver::mutex{};

std::queue<PlayState> Solver::solve(const GameState& gameState)
{
	long numAssemblers{ sysconf(_SC_NPROCESSORS_ONLN) };

	WorkState stopParent{ gameState };
	WorkState stopCondition{ stopParent };

	std::shared_ptr<StatusQueue<WorkState>> statusQueue;

	for (int a{ 0 }; a < numAssemblers; ++a)
	{
		std::shared_ptr<TAssembler> assembler
				{ std::make_shared<TAssembler>(stopCondition) };
		statusQueue = StatusQueue<WorkState>::signUp(assembler);
	}

	WorkState initialState{ gameState };
	statusQueue->push(initialState);

	statusQueue->refreshSize();

	statusQueue->startConsumers();

	statusQueue->waitForConsumers();

	if (highScore)
	{
		std::queue<PlayState> history{};
		// TODO(aarevalo): ir llenando esa mica con los playstates y cada parent
		return history;

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

// Recibo un estado de juego
// Intento hacer place en la ubicacion prescrita
// Rec: Si tengo exito, bajo un nivel, e intento en la primera ubicacion
// //	Si llego al último nivel, hago compare
// // //	Si el compare es exitoso, reemplazo el highScore
// // //	Si no, reviso si hay ubicaciones en este nivel
// // // //		Si hay, prescribo la proxima ubicacion (Produce)
// // // //		Si no, subo un nivel y prescribo la proxima ubicacion (Produce)
// //	Si no (no exito), reviso si hay ubicaciones en este nivel
// // //	Si hay, prescribo la proxima ubicacion (Produce)
// // //	Si no, subo un nivel y prescribo la proxima ubicacion (Produce)

void Solver::TAssembler::consume(WorkState workState)
{
	if (workState.place())
	{
		// Falta revisar si la profundidad ya
		WorkState child{ std::make_shared<WorkState>(workState) };
		consume(child);
	}
	else
	{
		// como hallo el proximo WorkState que voy a mandar a Produce?
	}

	/*if (workState.isMaxDepth())
	{
		compare(workState);
	}
	else
	{
		std::queue<WorkState> histories{ workState.permutate() };
		while (!histories.empty())
		{
			produce(histories.front());
			histories.pop();
		}
	}*/
}

/*
void Solver::compare(WorkState& current)
{
	mutex.lock();
	if (highScore->isEmpty() || *current.getLast() > *highScore->getLast())
	{
		highScore = std::make_unique<WorkState>(current);
	}
	mutex.unlock();
}*/
