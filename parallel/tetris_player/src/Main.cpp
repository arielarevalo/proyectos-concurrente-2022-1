// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#include "./include/TetrisPlayer.h"

/**
 * @brief Entry point.
 */
int main()
{
	try
	{
		TetrisPlayer::start();
	}
	catch (const std::exception& e)
	{
		Logger::error("TetrisPlayer has crashed.", e);
	}
	exit(1);
}
