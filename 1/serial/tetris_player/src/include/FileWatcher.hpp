// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include "./TetrisSolverSerial.hpp"
#include "./Logger.hpp"

using inotify_event = struct inotify_event;

/**
 * @brief Watches a directory and processes required files on event.
 */
class FileWatcher
{
public:
	/**
	 * @brief Starts watching the target directory.
	 * @details Watches a directory using the inotify library to continuously
	 * monitor a specific folder and, on given file system events, check
	 * for target file names and process the files.
	 */
	static void start();

private:
	/**
	 * @brief Signal handler that terminates FileWatcher process.
	 */
	static void finalize(int sig);

	/**
	 * @brief Builds a list of event pointers from points in a text buffer
	 * containing the contents of the events.
	 * @param eventBuffer Text buffer containing event contents.
	 * @param length Length of the text buffer.
	 * @return List of event pointers.
	 */
	static const std::vector<const inotify_event*>
	buildEvents(const char* eventBuffer, ssize_t length);

	/**
	 * @brief Checks if passed events include target filename, and processes
	 * it if found.
	 * @param events Events to process.
	 */
	static void processEvents(const std::vector<const inotify_event*>& events);

	/**
	 * @brief Opens file at a given path and solves it as a Tetris game state.
	 * @param path Path to file to solve.
	 */
	static void processFile(const std::string& path);

	/**
	 * @brief Decorates ::processFile with the set number of max retries.
	 * @param path Path to file to solve.
	 */
	static void processFileWithRetry(const std::string& path);

	static const ssize_t EVENT_SIZE{ sizeof(inotify_event) };
	static const ssize_t BUF_LEN{ 1024 * (EVENT_SIZE + 16) };
	static const ssize_t MAX_RETRIES{ 5 };
	static constexpr char INITIAL_PATH[7]{ "./put/" };
	static constexpr char TARGET[17]{ "tetris_state.txt" };
	static constexpr char RETRY_ERROR[33]{ "basic_ios::clear: iostream error" };
};

__attribute__((noreturn)) void FileWatcher::start()
{
	std::signal(SIGINT, finalize);

	std::filesystem::remove_all(INITIAL_PATH);
	std::filesystem::create_directory(INITIAL_PATH);

	Logger::info("Starting file watching loop.");
	while (true)
	{
		int fd{ inotify_init() };
		if (fd < 0)
		{
			throw std::ios::failure("Inotify could not create instance: " +
					std::string(std::strerror(errno)));
		}

		int wd{ inotify_add_watch(fd, INITIAL_PATH, IN_CREATE) };
		if (wd < 0)
		{
			throw std::ios::failure("Inotify could not add watch:" +
					std::string(std::strerror(errno)));
		}

		char eventBuffer[BUF_LEN];
		ssize_t length{ read(fd, eventBuffer, BUF_LEN) };
		if (length < 0)
		{
			throw std::ios::failure("Could not read Inotify instance:" +
					std::string(std::strerror(errno)));
		}

		const std::vector<const inotify_event*> events{
				buildEvents(eventBuffer, length)
		};

		processEvents(events);

		(void)inotify_rm_watch(fd, wd);
		(void)close(fd);
	}
}

const std::vector<const inotify_event*>
FileWatcher::buildEvents(const char* eventBuffer, ssize_t length)
{
	std::vector<const inotify_event*> events{};
	ssize_t i{ 0 };
	while (i < length)
	{
		auto event = reinterpret_cast<const inotify_event*>(&eventBuffer[i]);

		events.push_back(event);

		i += EVENT_SIZE + event->len;
	}
	return events;
}

void FileWatcher::processEvents(const std::vector<const inotify_event*>& events)
{
	for (const inotify_event* event : events)
	{
		Logger::info("Validating file event: " + std::string(event->name));

		std::string path{ INITIAL_PATH + std::string(event->name) };

		if (event->mask & IN_CREATE
				&& !(event->mask & IN_ISDIR)
				&& std::string(TARGET) == std::string(event->name))
		{
			Logger::info("Successfully found tetris game state file.");

			processFileWithRetry(path);

			std::filesystem::remove(path);
		}
		else
		{
			Logger::info("File was not a tetris game state file.");
		}
	}
}

void FileWatcher::processFile(const std::string& path)
{
	std::ifstream file{ path };
	file.exceptions(
			std::ifstream::badbit | std::ifstream::failbit);
	try
	{
		TetrisSolverSerial::solve(file);
	}
	catch (const std::exception& e)
	{
		if (Logger::deduce_exception_what(e)
				== std::string(RETRY_ERROR))
		{
			throw; // To retry handler
		}
		else
		{
			Logger::error("Unable to process file: " + path, e);
		}
	}
}

void FileWatcher::processFileWithRetry(const std::string& path)
{
	for (ssize_t i{ 0 }; i < MAX_RETRIES; ++i)
	{
		try
		{
			processFile(path);
			break;
		}
		catch (const std::exception&)
		{
			if (i < MAX_RETRIES - 1)
			{
				Logger::error("Failed to properly open tetris game "
							  "state file: " + path + " Attempt: "
						+ std::to_string(i + 1));
			}
			else
			{
				Logger::error(
						"Unable to process file: " + path + "\n Please retry.");
			}
		}
	}
}

__attribute__((noreturn)) void FileWatcher::finalize([[maybe_unused]]int sig)
{
	Logger::info("Received abort signal. Finalizing.");
	exit(0);
}

