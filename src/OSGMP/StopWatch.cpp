#include "StopWatch.h"

StopWatch::StopWatch()
{
}

StopWatch::~StopWatch()
{
}

void StopWatch::Start()
{
	m_times.push_back(chrono::system_clock::now());
}

pair<float, float> StopWatch::Touch()
{
	m_times.push_back(chrono::system_clock::now());
	m_fromBegin = m_times[m_times.size() - 1] - m_times[0];
	m_fromLast = m_times[m_times.size() - 1] - m_times[m_times.size() - 2];
	return make_pair(float(std::chrono::duration_cast<std::chrono::milliseconds>(m_fromBegin).count()) / 1000, float(std::chrono::duration_cast<std::chrono::milliseconds>(m_fromLast).count()) / 1000);
}

pair<float, float> StopWatch::Stop()
{
	m_times.push_back(chrono::system_clock::now());
	m_fromBegin = m_times[m_times.size() - 1] - m_times[0];
	m_fromLast = m_times[m_times.size() - 1] - m_times[m_times.size() - 2];
	return make_pair(float(std::chrono::duration_cast<std::chrono::milliseconds>(m_fromBegin).count()) / 1000, float(std::chrono::duration_cast<std::chrono::milliseconds>(m_fromLast).count()) / 1000);
}

void StopWatch::Reset()
{
	m_times.clear();
}
