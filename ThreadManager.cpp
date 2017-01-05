#include "ThreadManager.h"

#include <SDL_timer.h>
#include <iostream>

std::vector<Thread> ThreadManager::m_threads;
std::unordered_map<int, ThreadTask> ThreadManager::m_tasks;
bool ThreadManager::m_running;

void ThreadManager::startThreadPool(unsigned int numThreads)
{
	m_running = true;
	for (unsigned int i = 0; i < numThreads; i++) {
		SDL_Thread* thread = SDL_CreateThread(threadPoolFunction, std::string("Thread " + i).c_str(), (void*)i);
		m_threads.push_back(Thread(thread));
	}
}

void ThreadManager::stopThreadPool()
{
	m_running = false;
	for (unsigned int i = 0; i < m_threads.size(); i++) {
		int status;
		SDL_WaitThread(m_threads[i].getSDLThread(), &status);
	}
	m_threads.clear();
}

void ThreadManager::addTask(const ThreadTask& task)
{
	while (true) {
		for (int i = 0; i < (int)m_threads.size(); i++) {
			if (!m_tasks[i].isValid()) {
				m_tasks[i] = task;
				return;
			}
		}
		SDL_Delay(1);
	}
}

void ThreadManager::tryAddTask(const ThreadTask& task)
{
	for (int i = 0; i < (int)m_threads.size(); i++) {
		if (!m_tasks[i].isValid()) {
			m_tasks[i] = task;
			break;
		}
	}
}

bool ThreadManager::saturated()
{
	for (unsigned int i = 0; i < m_threads.size(); i++) {
		if (!m_tasks[i].isValid()) return false;
	}
	return true;
}

int threadPoolFunction(void* parameters)
{
	int index = (int)parameters;
	ThreadManager::getTasks()[index] = ThreadTask();
	while (ThreadManager::isRunning()) {
		if (ThreadManager::getTasks()[index].isValid()) {
			ThreadTask& task = ThreadManager::getTasks()[index];
			task.execute();
			task.setValid(false);
		}
	}
	return 0;
}
