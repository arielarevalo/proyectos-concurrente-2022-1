/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#pragma once

#include <thread>

#include "./Common.hpp"

/**
 * @brief Base and wrapper class for managing execution threads
 */
class Thread
{
	DISABLE_COPY(Thread);

protected:
	/// Object that controls the thread
	std::thread* thread = nullptr;

public:
	/// Constructor
	Thread();
	/// Destructor
	virtual ~Thread();
	/// Create a new thread, and then, executes run()
	/// @return Error code, 0 for success
	int startThread();
	/// Stop execution of this server/daemon, called by Ctrl+C or signal
	/// @return Error code, 0 for success
	int waitToFinish();

protected:
	/// This is the first method to be called on the stack of the new thread
	/// @return Error code, 0 for success
	virtual int run() = 0;
};

Thread::Thread()
{
}

Thread::~Thread()
{
	delete this->thread;
}

int Thread::startThread()
{
	assert(this->thread == nullptr);
	this->thread = new std::thread(&Thread::run, this);
	return EXIT_SUCCESS;
}

int Thread::waitToFinish()
{
	assert(this->thread);
	this->thread->join();

	delete this->thread;
	this->thread = nullptr;

	return EXIT_SUCCESS;
}
