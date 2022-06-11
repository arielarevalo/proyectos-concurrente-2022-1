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

class FileWatcher
{
public:
	static void start();
private:
	static void finalize(int sig);

	static const ssize_t EVENT_SIZE;
	static const ssize_t BUF_LEN;
	static const char INITIAL_PATH[7];
	static const char TARGET[17];
	static const char TARGET_PATH[23];
	static const char RETRY_ERROR[33];
};

const ssize_t FileWatcher::EVENT_SIZE{ sizeof(inotify_event) };
const ssize_t FileWatcher::BUF_LEN{ 1024 * (EVENT_SIZE + 16) };
const char FileWatcher::INITIAL_PATH[7]{ "./put/" };
const char FileWatcher::TARGET[17]{ "tetris_state.txt" };
const char FileWatcher::TARGET_PATH[23]{ "./put/tetris_state.txt" };
const char FileWatcher::RETRY_ERROR[33]{ "basic_ios::clear: iostream error" };

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

		char buffer[BUF_LEN];
		ssize_t length{ read(fd, buffer, BUF_LEN) };
		if (length < 0)
		{
			throw std::ios::failure("Could not read Inotify instance:" +
					std::string(std::strerror(errno)));
		}

		ssize_t i{ 0 };
		while (i < length)
		{
			const inotify_event* event
					= reinterpret_cast<inotify_event*>(&buffer[i]);

			Logger::info("Validating file event: " + std::string(event->name));

			if (event->len
					&& event->mask & IN_CREATE
					&& !(event->mask & IN_ISDIR)
					&& std::string(TARGET) == std::string(event->name))
			{
				Logger::setStart();
				Logger::info("Successfully found tetris game state file.");

				std::ifstream file;
				file.open(TARGET_PATH);
				file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
				try
				{
					TetrisSolverSerial::play(file);
				}
				catch (const std::exception& e)
				{
					/*if (Logger::deduce_exception_what(e)
							== std::string(RETRY_ERROR))
					{
						Logger::error("Failed to properly open tetris game "
									  "state file. Retrying.");
						file.close();
					} else {
						Logger::error("TetrisSolver has crashed.", e);
					}*/
					// TODO(aarevalo): Figure out how to retry with new
					//  ifstream object
					Logger::error("TetrisSolver has crashed.", e);
				}
				file.close();
			}
			else
			{
				Logger::info("File was not a tetris game state file.");
			}
			i += EVENT_SIZE + event->len;
		}
		(void)inotify_rm_watch(fd, wd);
		(void)close(fd);
	}
}

__attribute__((noreturn)) void FileWatcher::finalize(int sig)
{
	Logger::info("Received abort signal. Finalizing.");
	exit(0);
}

