// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <ctime>
#include <iostream>
#include <string>

class Logger
{
public:
	static void info(const std::string& message) ;

	static void error(const std::string& message) ;
};

void Logger::info(const std::string& message)
{
	time_t now;
	std::time(&now);
	std::cout << std::ctime(&now) << "[INFO]: " << message << std::endl;
}

void Logger::error(const std::string& message)
{
	time_t now;
	std::time(&now);
	std::cerr << std::ctime(&now) << "[ERROR]: " << message << std::endl;
}
