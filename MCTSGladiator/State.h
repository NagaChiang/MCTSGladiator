#pragma once
#include <BWAPI.h>
#include <stdexcept>
#include "Unit.h"
#include "Action.h"

namespace MCTSG
{
	class State
	{

		int time; // current frame count
		Unitset allUnits;

		static const int MOVE_DURATION = 8; // fixed duration for move (1/3 second)

	public:

		State();
		State(const int t, const BWAPI::Unitset &units);
		~State();

		void clear();
		void set(const int t, const BWAPI::Unitset &units);
		void update(const int t, const BWAPI::Unitset &units);
		void makeMove(const Move move);
		void doAction(const Action &action);

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