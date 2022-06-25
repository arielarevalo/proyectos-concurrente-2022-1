// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <iostream>
#include "./prodcons/Consumer.hpp"
#include "./TetrisWorkUnit.hpp"

class TetrisConsumer : public Consumer<TetrisWorkUnit>
{
public:
	TetrisConsumer(Queue<TetrisWorkUnit>* consumingQueue,
			const TetrisWorkUnit& stopCondition,
			bool createOwnQueue)
			:Consumer(consumingQueue, stopCondition,
			createOwnQueue)
	{
	}

	void consume(TetrisWorkUnit tetrisWorkUnit) override;

	int run() override;
};

int TetrisConsumer::run()
{
	this->consumeForever();

	return EXIT_SUCCESS;
}

void TetrisConsumer::consume(TetrisWorkUnit tetrisWorkUnit)
{
	if (tetrisWorkUnit.number > 0)
	{
		*tetrisWorkUnit.result = 1;

		for (int i = 1; i <= tetrisWorkUnit.number; ++i)
		{
			*tetrisWorkUnit.result *= i;
		}
	}

	std::cout << "      Consumed" << tetrisWorkUnit.number << std::endl;
}