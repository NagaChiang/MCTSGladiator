#pragma once
#include <BWAPI.h>

namespace MCTSG
{
	class Unit
	{

		int ID;
		BWAPI::UnitType unitType; // type of this unit
		BWAPI::Position position; // current position
		int hitPoints; // current HP
		int tAttack; // next time frame can attack
		int tMove; // next time frame can move

	public:

		Unit();
		Unit(const BWAPI::Unit &unit);

		void update(const BWAPI::Unit &unit);

		bool isAlive() const;

		// getters
		int getID() const { return ID; };
		BWAPI::UnitType getType() const { return unitType; };
		BWAPI::Position getPosition() const { return position; };
		int getHitPoints() const { return hitPoints; };
		int getNextCanAttackFrame() const { return tAttack; };
		int getNextCanMoveFrame() const { return tMove; };
	};
}