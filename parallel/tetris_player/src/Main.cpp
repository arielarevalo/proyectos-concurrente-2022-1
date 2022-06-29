// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#include "include/file/FileWatcher.hpp"
#include "include/concurrent/Manager.hpp"

/**
 * @brief Entry point.
 */
int main()
{
	try
	{
		//FileWatcher::start();
        Manager::processTetrisState();
	}
	catch (const std::exception& e)
	{
		Logger::error("FailWatcher has crashed.", e);
	}
	exit(1);
}
