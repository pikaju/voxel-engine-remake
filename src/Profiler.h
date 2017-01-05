#pragma once

#include <unordered_map>
#include <chrono>

class Profiler
{
public:
	static void start(std::string name);
	static void stop();
	static void print();
private:
	static std::unordered_map<std::string, float> m_times;
	static std::string m_currentName;
	static std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	static unsigned int counter;
};

