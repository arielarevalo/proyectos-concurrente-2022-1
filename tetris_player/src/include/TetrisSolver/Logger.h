// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <ctime>
#include <iostream>
#include <string>

class Logger
{
public:
	static void log_info(const std::string& message);

	static void log_error(const std::string& message);
};

void Logger::log_info(const std::string& message)
{
	time_t now;
	std::time(&now);
	std::cout << std::ctime(&now) << "[INFO]: " << message << std::endl;
}

void Logger::log_error(const std::string& message)
{
	time_t now;
	std::time(&now);
	std::cerr << std::ctime(&now) << "[ERROR]: " << message << std::endl;
}
