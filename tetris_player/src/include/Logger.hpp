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

	static void error(const std::string& message, const std::exception& e);

	static std::string deduce_exception_what(const std::exception& e);

	static void setStart();
private:
	static std::chrono::high_resolution_clock::time_point start;

	static u_int64_t duration();

	static void print_exception(const std::exception& e, int level = 0);
};

std::chrono::high_resolution_clock::time_point Logger::start{
		std::chrono::high_resolution_clock::now()
};

void Logger::info(const std::string& message)
{
	std::cout << "[" << duration() << " ms]" << "[INFO]: "
			  << message << std::endl;
}

void Logger::error(const std::string& message)
{
	std::cerr << "[" << duration() << " ms]" << "[ERROR]: "
			  << message << std::endl;
}

void Logger::error(const std::string& message, const std::exception& e)
{
	error(message);
	print_exception(e);
}

u_int64_t Logger::duration()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::high_resolution_clock::now() - start).count();
}

void Logger::print_exception(const std::exception& e, int level)
{
	std::cerr << "Caused by: " << e.what() << std::endl;
	try
	{
		std::rethrow_if_nested(e);
	}
	catch (const std::exception& nestedException)
	{
		print_exception(nestedException, level + 1);
	}
	catch (...)
	{
		std::cerr << "Non-Exception thrown nested." << std::endl;
	}
}

std::string Logger::deduce_exception_what(const std::exception& e)
{
	try {
		std::rethrow_if_nested(e);
	} catch(const std::exception& n) {
		return deduce_exception_what(n);
	}
	return e.what();
}

void Logger::setStart()
{
	start = std::chrono::high_resolution_clock::now();
}
