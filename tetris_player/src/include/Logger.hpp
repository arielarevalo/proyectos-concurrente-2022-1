// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

/**
 * @details Error handler Class
 */
class Logger
{
public:
	/**
	 * @brief INFO message handler type.
 	 * @details Handling the info messages of the application
	 * @param message: string
	 */
	static void info(const std::string& message);

	/**
	 * @brief ERROR message handler type.
 	 * @details Handling the error messages of the application
	 * @param message: string
	 */
	static void error(const std::string& message);

	/**
	 * @brief Determines the time it takes to process each method or action.
 	 * @details Method that records the processing time
	 * @return u_int64_t value
	 */
	static u_int64_t duration();

	/**
 	 * @details start the timer
	 */
	static void setStart();
private:
	static std::chrono::high_resolution_clock::time_point start;
};

std::chrono::high_resolution_clock::time_point Logger::start{
		std::chrono::high_resolution_clock::now()
};

/**
 * @brief INFO message handler type.
 * @details Handling the info messages of the application
 * @param message: string
 */
void Logger::info(const std::string& message)
{
	std::cout << "[" << duration() << "ms]" << "[INFO]: "
			  << message << std::endl;
}

/**
 * @brief ERROR message handler type.
 * @details Handling the error messages of the application
 * @param message: string
 */
void Logger::error(const std::string& message)
{
	std::cerr << "[" << duration() << "ms]" << "[ERROR]: "
			  << message << std::endl;
}

/**
 * @brief Determines the time it takes to process each method or action.
 * @details Method that records the processing time
 * @return u_int64_t value
 */
u_int64_t Logger::duration()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::high_resolution_clock::now() - start).count();
}

/**
 * @details start the timer
 */
void Logger::setStart()
{
	start = std::chrono::high_resolution_clock::now();
}
