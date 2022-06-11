// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#include "./include/FileWatcher.hpp"

int main(int argc, char* argv[])
{
	try
	{
		FileWatcher::start();
	}
	catch (const std::exception& e)
	{
		Logger::error("FailWatcher has crashed.", e);
	}
	catch (...) {
		Logger::error("Non-Exception thrown.");
	}
	exit(1);
}
