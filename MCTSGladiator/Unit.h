#pragma once
#include <BWAPI.h>

namespace MCTSG
{
	class Unit
	{

	public:

		int ID;
		BWAPI::UnitType unitType; // type of this unit
		BWAPI::Position position; // current position
		int hitPoints; // current HP
		int tAttack; // next time frame can attack
		int tMove; // next time frame can move

		Unit();
		Unit(const BWAPI::Unit &unit);

		void update(const BWAPI::Unit &unit);

		bool isAlive();
	};
}