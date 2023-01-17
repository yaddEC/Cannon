#include "Time.h"
#include "Time.h"


double Time::m_deltaTime = 0.0;

void Time::Init()
{
	m_previousTime = std::chrono::system_clock::now();
}

void Time::Update()
{
	auto currentTime = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = currentTime - m_previousTime;
	m_deltaTime = elapsed_seconds.count();
	m_previousTime = currentTime;
}

double Time::GetDeltaTime() 
{
	return m_deltaTime;
}
