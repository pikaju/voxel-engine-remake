#pragma once

#include <vector>
#include <unordered_map>
#include <SDL_thread.h>
#include <string>

class ThreadTask
{
public:
	ThreadTask() : m_name(), m_function(), m_parameters(), m_valid(false) {  }
	ThreadTask(std::string name, int(*function)(void*), void* parameters) : m_name(name), m_function(function), m_parameters(parameters), m_valid(true) {  }
	~ThreadTask() {}

	inline void execute() { m_function(m_parameters); }
	inline bool isValid() { return m_valid; }
	inline void setValid(bool valid) { m_valid = valid; }
private:
	std::string m_name;
	int(*m_function)(void*);
	void* m_parameters;
	bool m_valid;
};

class Thread
{
public:
	Thread(SDL_Thread* thread) : m_thread(thread) {  }
	inline SDL_Thread* getSDLThread() { return m_thread; }
private:
	SDL_Thread* m_thread;
};

class ThreadManager
{
public:
	static void startThreadPool(unsigned int numThreads);
	static void stopThreadPool();
	
	static void addTask(const ThreadTask& task);
	static void tryAddTask(const ThreadTask& task);
	
	static bool saturated();

	static inline Thread& getThread(int index) { return m_threads[index]; }
	static inline std::unordered_map<int, ThreadTask>& getTasks() { return m_tasks; }
	
	static inline bool isRunning() { return m_running; }

private:
	static std::vector<Thread> m_threads;
	static std::unordered_map<int, ThreadTask> m_tasks;
	static bool m_running;
};

extern int threadPoolFunction(void* parameters);