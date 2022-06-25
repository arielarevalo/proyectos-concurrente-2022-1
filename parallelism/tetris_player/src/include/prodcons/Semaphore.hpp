/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#pragma once

#include <unistd.h>
#include <semaphore.h>
#include <cassert>
#include <cstdlib>
#include <cstring>

#include "./common.hpp"

// macOS only uses named semaphores
#if __APPLE__ && __MACH__
#define USE_NAMED_SEMAPHORE 1
#endif

#ifdef USE_NAMED_SEMAPHORE
#define SEM_NAME_PATTERN "/tmp/semXXXXXX"
#define SEM_NAME_LEN 15  // strlen(SEM_NAME_PATTERN) + 1
#endif

/// Wrapper class for POSIX semaphores
class Semaphore
{
	DISABLE_COPY(Semaphore);

private:
#if USE_NAMED_SEMAPHORE
	/// The file name of the semaphore
	char name[SEM_NAME_LEN];
	/// Pointer to a thread-safe integer value
	sem_t* semaphore;
#else
	/// A thread-safe integer value
	sem_t semaphore;
#endif

public:
	/// Constructs a semaphore with the given value
	explicit Semaphore(unsigned value);
	/// Destructor
	~Semaphore();
	/// Increment the semaphore and wake a waiting thread if any
	void signal();
	/// Decrement the semaphore and block if the result is negative
	void wait();
};

Semaphore::Semaphore(unsigned value)
{
#if USE_NAMED_SEMAPHORE
	// Get an unique filename
  // Fill the patter with a unique name on the system
  std::strncpy(this->name, SEM_NAME_PATTERN, SEM_NAME_LEN);
  int fileDescriptor = ::mkstemp(this->name);
  assert(fileDescriptor >= 0);
  close(fileDescriptor);
  this->semaphore = ::sem_open(this->name, O_CREAT | O_EXCL, 0644, value);
  assert(this->semaphore != SEM_FAILED);
#else
	::sem_init(&this->semaphore, 0 /*pshared*/, value);
#endif
}

Semaphore::~Semaphore()
{
#if USE_NAMED_SEMAPHORE
	::sem_close(this->semaphore);
  ::sem_unlink(this->name);
#else
	::sem_destroy(&this->semaphore);
#endif
}

void Semaphore::signal()
{
#if USE_NAMED_SEMAPHORE
	::sem_post(this->semaphore);
#else
	::sem_post(&this->semaphore);
#endif
}

void Semaphore::wait()
{
#if USE_NAMED_SEMAPHORE
	::sem_wait(this->semaphore);
#else
	::sem_wait(&this->semaphore);
#endif
}

