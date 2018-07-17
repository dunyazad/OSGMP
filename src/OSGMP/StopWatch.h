#pragma once

//#include "../EuclideanSpaceCommon.h"
#include <chrono>
using namespace std::chrono;

#include <map>
#include <vector>
using namespace std;

class StopWatch
{
public:
	StopWatch();
	~StopWatch();

	void Start();
	pair<float, float> Touch();
	pair<float, float> Stop();
	void Reset();

private:
	vector<chrono::system_clock::time_point> m_times;
	std::chrono::duration<double> m_fromBegin;
	std::chrono::duration<double> m_fromLast;
};
