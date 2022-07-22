// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#pragma once

#include <mpi.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <filesystem>

/**
 * @brief Logs info and error messages, and unwinds nested exceptions.
 */
class Logger
{
public:
	/**
	 * @brief Logs info messages with timestamp.
	 * @param message Message to log.
	 */
	static void info(const std::string& message);

	/**
	 * @brief Logs error messages with timestamp.
	 * @param message Message to log.
	 */
	static void error(const std::string& message);

	/**
	 * @brief Logs error messages with timestamp and exception unwinding.
	 * @param message Message to log.
	 * @param e Exception to unwind.
	 */
	static void error(const std::string& message, const std::exception& e);

	/**
	 * @brief Unwinds a nested exception to return the descriptive message of
	 * the bottom level exception.
	 * @param e Exception to unwind and deduce.
	 * @return Descriptive message of bottom level exception.
	 */
	static std::string deduce_exception_what(const std::exception& e);

	/**
 	 * @brief Sets internal timer to zero.
	 */
	static void setStart();

	static void initialize();
	static void finalize();
private:
	static constexpr char LOG_DIR[8]{"./logs/"};

	static constexpr char LOG_PATH[12]{ "./logs/log_" };

	static constexpr char TXT[5]{".txt"};

	/**
	 * @brief Determines the time it takes to process each method or action.
 	 * @details Method that records the processing time
	 * @return u_int64_t value
	 */
	static u_int64_t duration();

	/**
	 * @brief Unwinds exception and prints each level.
	 * @param e Exception to unwind.
	 * @param level Current unwind depth level.
	 */
	static void print_exception(const std::exception& e, int level = 0);

	static MPI_File file;

	static std::chrono::high_resolution_clock::time_point start;
};

std::chrono::high_resolution_clock::time_point Logger::start{
		std::chrono::high_resolution_clock::now()
};

MPI_File Logger::file;

void Logger::info(const std::string& message)
{
	std::string output{};
	output = "[" + std::to_string(duration()) + " ms]" + "[INFO]: "
			  + message + "\n";
	MPI_File_write(file, output.data(), static_cast<int>(output.size()),
			MPI_CHAR, MPI_STATUS_IGNORE);
}

void Logger::error(const std::string& message)
{
	std::string output{};
	output = "[" + std::to_string(duration()) + " ms]" + "[ERROR]: "
			  + message + "\n";
	MPI_File_write(file, output.data(), static_cast<int>(output.size()),
			MPI_CHAR, MPI_STATUS_IGNORE);
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
	std::string output{};
	output = "Caused by: " + std::string(e.what()) + "\n";
	MPI_File_write(file, output.data(), static_cast<int>(output.size()),
			MPI_CHAR, MPI_STATUS_IGNORE);
	try
	{
		std::rethrow_if_nested(e);
	}
	catch (const std::exception& ne)
	{
		print_exception(ne, level + 1);
	}
}

std::string Logger::deduce_exception_what(const std::exception& e)
{
	try
	{
		std::rethrow_if_nested(e);
	}
	catch (const std::exception& ne)
	{
		return deduce_exception_what(ne);
	}
	return e.what();
}

void Logger::initialize()
{
	int rank{-1};
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank == 0) {
		std::filesystem::remove_all(LOG_DIR);
		std::filesystem::create_directory(LOG_DIR);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	std::string filename{LOG_PATH + std::to_string(rank) + TXT};
	MPI_File_open(MPI_COMM_WORLD, filename.data(), MPI_MODE_CREATE | MPI_MODE_WRONLY,
			MPI_INFO_NULL, &file);
}

void Logger::setStart()
{
	start = std::chrono::high_resolution_clock::now();
}

void Logger::finalize()
{
	MPI_File_close(&file);
}
