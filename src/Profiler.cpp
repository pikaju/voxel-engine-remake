#include "Profiler.h"

#include <iostream>
#include <string>

std::unordered_map<std::string, float> Profiler::m_times;
std::string Profiler::m_currentName;
std::chrono::time_point<std::chrono::high_resolution_clock> Profiler::m_start;
unsigned int Profiler::counter = 0;

void Profiler::start(std::string name)
{
	m_currentName = name;
	m_start = std::chrono::high_resolution_clock::now();
}

void Profiler::stop()
{
	std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_start;
	m_times[m_currentName] += std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	counter++;
}

void Profiler::print()
{
	auto it = m_times.begin();
	float total = 0.0f;
	for (; it != m_times.end(); it++) {
		std::cout << it->first.c_str() << ": " << std::to_string(it->second / (float)counter).c_str() << std::endl;
		total += it->second / (float)counter;
		it->second = 0.0f;
	}
	std::cout << "Total: " << total << std::endl;
	counter = 0;
}