#pragma once
#include <BWAPI.h>
#include <vector>
#include "Unit.h"

namespace MCTSG
{
	class State
	{

		int time; // current time frame
		std::vector<Unit> allyUnits;
		std::vector<Unit> enemyUnits;

	public:

		State();

		void clear();
		void set(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies);
		void update(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies);

		bool isEnd() const;

	private:

		// support functions
		void setUnits(std::vector<Unit> &vecUnits, const BWAPI::Unitset &unitset);
		void updateUnits(std::vector<Unit> &vecUnits, const BWAPI::Unitset &unitset);

	};
}