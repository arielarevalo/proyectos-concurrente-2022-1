// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <ios>
#include <vector>

using inotify_event = struct inotify_event;

/**
 * @brief Watches a directory and processes required files on event.
 */
class FileWatcher
{
public:
	/**
	 *
	 * @param initialPath
	 * @param target
	 * @param endTarget
	 */
	FileWatcher(const std::string& initialPath,
			std::string_view target,
			std::string_view endTarget)
			: initialPath(initialPath),
			target(target),
			endTarget(endTarget) {}

	/**
	 *
	 * @return
	 */
	std::string watch();

private:
	/**
	 * @brief Builds a list of event pointers from points in a text buffer
	 * containing the contents of the events.
	 * @param eventBuffer Text buffer containing event contents.
	 * @param length Length of the text buffer.
	 * @return List of event pointers.
	 */
	std::vector<const inotify_event*>
	buildEvents(const char* eventBuffer, ssize_t length);

	/**
	 *
	 * @param event
	 * @param event_name
	 * @return
	 */
	bool validateEvent(const inotify_event* event) const;

	/**
	 * @brief Checks if passed events include target filename, and processes
	 * it if found.
	 * @param events Events to process.
	 */
	std::string processEvents(
			const std::vector<const inotify_event*>& events
	);

	const ssize_t EVENT_SIZE{ sizeof(inotify_event) };
	const ssize_t BUF_LEN{ 1024 * (EVENT_SIZE + 16) };

	const std::string initialPath;
	const std::string target;
	const std::string endTarget;
};

std::string FileWatcher::watch()
{
	int fd{ inotify_init() };
	if (fd < 0)
	{
		throw std::ios::failure("Inotify could not create instance: " +
				std::string(std::strerror(errno)));
	}

	int wd{ inotify_add_watch(fd, initialPath.c_str(), IN_CLOSE_WRITE) };
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

	std::string path{ processEvents(events) };

	(void)inotify_rm_watch(fd, wd);
	(void)close(fd);

	return path;
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

std::string FileWatcher::processEvents(
		const std::vector<const inotify_event*>& events)
{
	for (const inotify_event* event : events)
	{
		if (validateEvent(event))
		{
			return initialPath + std::string(event->name);
		}
	}
	return "";
}

bool FileWatcher::validateEvent(const inotify_event* event) const
{
	std::string eventName{ event->name };

	return event->mask & IN_CLOSE_WRITE
			&& !(event->mask & IN_ISDIR)
			&& (target == eventName || endTarget == eventName);
}

