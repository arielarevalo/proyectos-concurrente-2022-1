// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

#include <algorithm>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

#include "./jeisson/Common.hpp"
#include "./jeisson/Queue.hpp"
#include "./jeisson/Semaphore.hpp"
#include "./StatusConsumer.hpp"

template<typename T>
class StatusConsumer;

template<typename T>
class StatusQueue
{
	DISABLE_COPY(StatusQueue);

public:
	// sa not required
	static std::shared_ptr<StatusQueue<T>>
	signUp(std::shared_ptr<StatusConsumer<T>> sa);

	void startConsumers() const;

	void waitForConsumers() const;

	std::shared_ptr<StatusConsumer<T>> getMember(size_t i) const;

	size_t getMembersSize();

	void push(const T& data);

	T pop();

	void refreshSize();

	bool isDone() const;

	bool isEmpty() const;

	size_t getSize() const;

private:
	static std::shared_ptr<StatusQueue<T>> statusQueue;

	StatusQueue() = default;

	std::vector<std::shared_ptr<StatusConsumer<T>>> members{};

	std::mutex mutex;

	Semaphore canConsume{ 0 };

	std::queue<T> queue;

	size_t size{ 0 };
};

template<typename T>
std::shared_ptr<StatusQueue<T>> StatusQueue<T>::statusQueue{};

template<typename T>
void StatusQueue<T>::startConsumers() const
{
	for(std::shared_ptr<StatusConsumer<T>> member : members) {
		member->startThread();
	}
}

template<typename T>
void StatusQueue<T>::waitForConsumers() const
{
	for(std::shared_ptr<StatusConsumer<T>> member : members) {
		member->waitToFinish();
	}
}

template<typename T>
std::shared_ptr<StatusQueue<T>>
StatusQueue<T>::signUp(std::shared_ptr<StatusConsumer<T>> sa)
{
	if (sa)
	{
		if (!statusQueue)
		{
			// make_shared tries to access private constructor
			std::shared_ptr<StatusQueue<T>> newStatusQueue
					{ new StatusQueue<T>{}};

			statusQueue = newStatusQueue;
		}
		statusQueue->members.emplace_back(sa);
	}
	return statusQueue;
}

template<typename T>
void StatusQueue<T>::push(const T& data)
{
	this->mutex.lock();
	this->queue.push(data);
	this->mutex.unlock();
	this->canConsume.signal();
}

template<typename T>
T StatusQueue<T>::pop()
{
	this->canConsume.wait();
	this->mutex.lock();
	T result = this->queue.front();
	this->queue.pop();
	this->mutex.unlock();
	return result;
}

template<typename T>
bool StatusQueue<T>::isDone() const
{
	bool anyBusy{ false };
	for (std::shared_ptr<StatusConsumer<T>> member : members)
	{
		if (member->isBusy())
		{
			anyBusy = true;
			break;
		}
	}
	return isEmpty() && !anyBusy;
}

template<typename T>
bool StatusQueue<T>::isEmpty() const
{
	return !size;
}

template<typename T>
size_t StatusQueue<T>::getSize() const
{
	return size;
}

template<typename T>
void StatusQueue<T>::refreshSize()
{
	size = this->queue.size();
}

template<typename T>
std::shared_ptr<StatusConsumer<T>> StatusQueue<T>::getMember(size_t i) const
{
	return members[i];
}

template<typename T>
size_t StatusQueue<T>::getMembersSize()
{
	return members.size();
}


