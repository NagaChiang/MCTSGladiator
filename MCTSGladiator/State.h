#pragma once
#include <BWAPI.h>
#include <vector>
#include "Unit.h"

namespace MCTSG
{
	class State
	{

		int time; // current frame count
		std::vector<Unit> allyUnits;
		std::vector<Unit> enemyUnits;

	public:

		State();

		void clear();
		void set(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies);
		void update(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies);

		bool isEnd() const;

		// getters
		int getTimeCount() const { return time; };
		int getAllyUnitsNum() const { return allyUnits.size(); };
		int getEnemyUnitsNum() const { return enemyUnits.size(); };
		std::vector<Unit> getAllyUnits() const { return allyUnits; };
		std::vector<Unit> getEnemyUnits() const { return enemyUnits; };

	private:

		// support functions
		void setUnits(std::vector<Unit> &vecUnits, const BWAPI::Unitset &unitset);
		void updateUnits(std::vector<Unit> &vecUnits, const BWAPI::Unitset &unitset);

	};
}