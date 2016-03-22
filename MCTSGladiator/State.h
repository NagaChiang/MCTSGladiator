#pragma once
#include <BWAPI.h>
#include <vector>
#include "Unit.h"

namespace MCTSG
{
	class State
	{

	public:

		int time; // current time frame
		std::vector<Unit> allyUnits;
		std::vector<Unit> enemyUnits;

		State();

		void set(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies);
		void clear();

	};
}