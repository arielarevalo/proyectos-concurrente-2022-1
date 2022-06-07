// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

class Logger
{
public:
	static void info(const std::string& message);

	static void error(const std::string& message);

	static u_int64_t duration();

	static void setStart();
private:
	static std::chrono::high_resolution_clock::time_point start;
};

std::chrono::high_resolution_clock::time_point Logger::start{
		std::chrono::high_resolution_clock::now()
};

void Logger::info(const std::string& message)
{
	std::cout << "[" << duration() << "ms]" << "[INFO]: "
			  << message << std::endl;
}

void Logger::error(const std::string& message)
{
	std::cerr << "[" << duration() << "ms]" << "[ERROR]: "
			  << message << std::endl;
}
u_int64_t Logger::duration()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::high_resolution_clock::now() - start).count();
}

void Logger::setStart()
{
	start = std::chrono::high_resolution_clock::now();
}
