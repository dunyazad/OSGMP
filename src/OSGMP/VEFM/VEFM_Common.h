#pragma once

#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <tuple>
#include <vector>
using namespace std;

//#include "Real.h"
#include "V3.h"

namespace VEFM
{
	template <typename T> class Vertex;
	template <typename T> class Edge;
	template <typename T> class Face;
	template <typename T> class Mesh;

	class VEFMElement
	{
	public:
		inline bool IsDeleteQueried() const { return m_bDeleteQueried; }
		inline void QueryDelete() { m_bDeleteQueried = true; OnQueryDelete(); }

		inline bool GetFlag(const string& flagName) { if (m_flags.count(flagName) != 0) return m_flags[flagName]; else return false; }
		inline void SetFlag(const string& flagName, bool flag) { m_flags[flagName] = flag; }

	private:
		bool m_bDeleteQueried = false;
		map<string, bool> m_flags;

		virtual void OnQueryDelete() = 0;
	};
}