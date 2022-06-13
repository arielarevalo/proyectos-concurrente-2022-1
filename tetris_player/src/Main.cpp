// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#include "./include/FileWatcher.hpp"

/**
 * @brief system main method.
 *
 */
int main()
{
	try
	{
		FileWatcher::start();
	}
	catch (const std::exception& e)
	{
		Logger::error("FailWatcher has crashed.", e);
	}
	exit(1);
}
