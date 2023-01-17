#pragma once
#include <chrono>
#include <ctime>

class Time
{
private:
	std::chrono::system_clock::time_point m_previousTime;
	static double m_deltaTime ;
public:
	Time() {};
	void Init();
	void Update();
	static double GetDeltaTime();
};