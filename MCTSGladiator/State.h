#pragma once
#include <BWAPI.h>
#include <stdexcept>
#include "Unit.h"
#include "Unitset.h"
#include "UnitData.h"
#include "Action.h"

namespace MCTSG
{
	class State
	{

		int _time; // current frame count
		Unitset _allUnits;

	public:

		State();
		State(const int t, const BWAPI::Unitset &units);
		~State();

		void clear();
		void set(const int t, const BWAPI::Unitset &units);
		void update(const int t, const BWAPI::Unitset &units);

		void makeMove(const Move move);
		void doAction(const Action &action);
		void eraseDeadUnits();

		std::vector<Move> generateNextMoves(const int amount, const bool forAlly) const;

		// conditions
		bool isEnd() const;

		// getters
		int getNextMinFrame() const;

		int getTimeFrame() const { return _time; };
		int getUnitsNum() const { return _allUnits.size(); };
		int getAllyUnitsNum() const;
		int getEnemyUnitsNum() const;
		Unit getUnit(const int ID) const; // get unit (pointer) by ID
		Unitset getUnits() const { return _allUnits; };
		Unitset getAllyUnits() const;
		Unitset getEnemyUnits() const;

	private:

		// support functions of generateNextMoves()
		Move generateNOKAVMove(const bool forAlly) const;
		Actions getActionTypeFromTo(const BWAPI::Position &from, const BWAPI::Position &to) const;
	};
}