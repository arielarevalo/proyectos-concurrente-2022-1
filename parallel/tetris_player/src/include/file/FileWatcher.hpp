// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <csignal>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "../Solver.hpp"
#include "../tetrisSolver/TetrisSolverSerial.hpp"
#include "../logger/Logger.hpp"
#include "./Filer.hpp"

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
     * @brief Builds a list of event pointers from points in a text buffer
     * containing the contents of the events.
     * @param eventBuffer Text buffer containing event contents.
     * @param length Length of the text buffer.
     * @return List of event pointers.
     */
    static std::vector<const inotify_event*>
    buildEvents(const char* eventBuffer, ssize_t length);

	/**
	 *
	 * @param event
	 * @param event_name
	 * @return
	 */
	static bool validateEvent(
			const inotify_event* event,
			std::basic_string<char> event_name);

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

    static const ssize_t EVENT_SIZE{ sizeof(inotify_event) };
    static const ssize_t BUF_LEN{ 1024 * (EVENT_SIZE + 16) };
    static constexpr char INITIAL_PATH[7]{ "./put/" };
    static constexpr char TARGET[17]{ "tetris_state.txt" };
    static constexpr char ENDTARGET[15]{ "tetris_end.txt" };
};

void FileWatcher::start()
{
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

        int wd{ inotify_add_watch(fd, INITIAL_PATH, IN_CLOSE_WRITE) };
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

std::vector<const inotify_event*>
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

        if (validateEvent(event, std::string(ENDTARGET))) {
            Logger::info("Successfully found stop listen events file. Finalizing.");

			exit(0);
        }

        if (validateEvent(event, std::string(TARGET)))
        {
            Logger::info("Successfully found tetris game state file.");

			processFile(path);

            std::filesystem::remove(path);
        }
        else
        {
            Logger::info("File was not a tetris game state file.");
        }
    }
}

bool FileWatcher::validateEvent(
        const inotify_event* event,
        std::basic_string<char> event_name) {

        return (event->mask & IN_CLOSE_WRITE
                && !(event->mask & IN_ISDIR)
                && event_name == std::string(event->name));
}

void FileWatcher::processFile(const std::string& path)
{
	std::ifstream file{ path };
	file.exceptions(
			std::ifstream::badbit | std::ifstream::failbit);

	std::unique_ptr<GameState> gameState;
	try
	{
		gameState = std::make_unique<GameState>(Filer::read(file));
	}
	catch (const std::exception& e)
	{
		Logger::error("Unable to process file: " + path, e);
	}

	try
	{
		Solver::processGameState(*gameState);
	}
	catch (const std::exception& e)
	{
		Logger::error("Solver has crashed.", e);
	}
}
