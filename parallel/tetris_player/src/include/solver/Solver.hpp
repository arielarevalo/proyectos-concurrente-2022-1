// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

// TODO(aarevalo): No repetir los errores de nuestros antepasados (memoria estática)

#pragma once

#include <unistd.h>
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

#include "./concurrent/StatusAssembler.hpp"
#include "./concurrent/StatusQueue.hpp"
#include "./common/History.hpp"
#include "./logger/Logger.hpp"

class Solver
{
public:
	static History processGameState(const GameState& gameState);
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

		/**
		 * Produces each element in the queue. Handles each outgoing History
		 * object by pushing the updated History object to the queue if it is
		 * not done being assembled.
		 * @param history
		 */
		void produce(const History& history) override;
	};
};

History Solver::processGameState(const GameState& gameState)
{
	long numAssemblers{ sysconf(_SC_NPROCESSORS_ONLN) };

	History highScore{ gameState };
	History stopCondition{ gameState };
	std::shared_ptr<StatusQueue<History>> statusQueue;

	for (int a{ 0 }; a < numAssemblers; ++a)
	{
		std::shared_ptr<TAssembler> assembler
				{ std::make_shared<TAssembler>(stopCondition) };
		statusQueue = StatusQueue<History>::signUp(assembler);
	}

	History initialState{ gameState };
	initialState.push(std::make_shared<PlayState>(gameState));

	statusQueue->startConsumers();

	Logger::info("Waiting for assemblers to finish.");

	statusQueue->waitForConsumers();

	if (!highScore.isEmpty())
	{
		Logger::info("Successfully found best moves for game state.");
		return highScore;
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
//	 * * history.getLast() conseguir el ultimo elemento
//	 * * hacer place (sobreescribir) sobre el último con los datos aux.
//	 * * si el place es exitoso
//	 * 		* si el getSize de history es igual a prof máx + 1
//	 * 				* Compare
//	 * 				* History.setDone()
					StatusAssembler<History>::consume(history);
//	 * 		* si no
//	 * 				* "Permutar" el último
//	 *				* for loop (each queue<history>)
					StatusAssembler<History>::consume(history);
//	 *				* END FOR
//	 *
//	 * * si no
//	 * 		* History.setDone()
			StatusAssembler<History>::consume(history);
}

void Solver::TAssembler::produce(const History& history)
{
	// * si !History.isDone()
	Producer::produce(history);
}
