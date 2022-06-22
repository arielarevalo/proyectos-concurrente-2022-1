// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#include "./include/FileWatcher.hpp"
#include "./include/ConcurrencyManager.hpp"

/**
 * @brief Entry point.
 */
int main()
{
	try
	{
		//FileWatcher::start();
        ConcurrencyManager::processTetrisState();
	}
	catch (const std::exception& e)
	{
		Logger::error("FailWatcher has crashed.", e);
	}
	exit(1);
}
