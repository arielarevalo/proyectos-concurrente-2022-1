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
	static const char initialPath[7];
	static const char target[17];
	static const char targetPath[23];
};

const ssize_t FileWatcher::EVENT_SIZE{ sizeof(inotify_event) };
const ssize_t FileWatcher::BUF_LEN{ 1024 * (EVENT_SIZE + 16) };
const char FileWatcher::initialPath[7]{ "./put/" };
const char FileWatcher::target[17]{ "tetris_state.txt" };
const char FileWatcher::targetPath[23]{ "./put/tetris_state.txt" };

__attribute__((noreturn)) void FileWatcher::start()
{
	std::signal(SIGINT, finalize);

	std::filesystem::remove_all(initialPath);
	std::filesystem::create_directory(initialPath);

	Logger::info("Starting file watching loop.");
	while (true)
	{
		int fd{ inotify_init() };
		if (fd < 0)
		{
			throw std::ios::failure("Inotify could not create instance: " +
					std::string(std::strerror(errno)));
		}

		int wd{ inotify_add_watch(fd, "./put/", IN_CREATE | IN_MODIFY) };
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
			if (event->len
					&& event->mask & IN_CREATE
					&& !(event->mask & IN_ISDIR)
					&& target == event->name)
			{
				Logger::setStart();
				Logger::info("Successfully found tetris game state file.");
				std::ifstream file{ targetPath };
				file.exceptions(std::ifstream::badbit);
				try
				{
					TetrisSolverSerial::play(file);
				}
				catch (const std::exception& e)
				{
					std::throw_with_nested(
							std::runtime_error("Tetris Solver has crashed.")
					);
				}
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

