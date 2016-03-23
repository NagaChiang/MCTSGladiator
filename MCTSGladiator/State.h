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

		void clear();
		void set(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies);
		void setUnits(std::vector<Unit> &vecUnits, const BWAPI::Unitset &unitset);
		void update(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies);
		void updateUnits(std::vector<Unit> &vecUnits, const BWAPI::Unitset &unitset);

	};
}