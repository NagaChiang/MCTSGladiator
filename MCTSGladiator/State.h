#pragma once
#include <BWAPI.h>
#include <stdexcept>
#include "Unit.h"

namespace MCTSG
{
	class State
	{

		int time; // current frame count
		Unitset allUnits;

	public:

		State();
		State(const int t, const BWAPI::Unitset &units);
		~State();

		void clear();
		void set(const int t, const BWAPI::Unitset &units);
		void update(const int t, const BWAPI::Unitset &units);

		bool isEnd() const;

		// getters
		int getTimeCount() const { return time; };
		int getUnitsNum() const { return allUnits.size(); };
		int getAllyUnitsNum() const;
		int getEnemyUnitsNum() const;
		Unit getUnit(const int ID) const; // get unit (pointer) by ID
		Unitset getUnits() const { return allUnits; };
		Unitset getAllyUnits() const;
		Unitset getEnemyUnits() const;
	};
}