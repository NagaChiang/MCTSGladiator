#pragma once
#include <BWAPI.h>
#include <map>
#include <memory>

namespace MCTSG
{
	class UnitInterface
	{

		int ID;
		bool ally; // is it ally?
		BWAPI::UnitType unitType; // type of this unit
		BWAPI::Position position; // current position
		int hitPoints; // current HP
		int tAttack; // next time frame can attack
		int tMove; // next time frame can move

	public:

		UnitInterface();
		UnitInterface(const BWAPI::Unit &unit);

		void update(const BWAPI::Unit &unit);

		bool isAlive() const;
		bool isAlly() const { return ally; };

		// getters
		int getID() const { return ID; };
		BWAPI::UnitType getType() const { return unitType; };
		BWAPI::Position getPosition() const { return position; };
		int getHitPoints() const { return hitPoints; };
		int getNextCanAttackFrame() const { return tAttack; };
		int getNextCanMoveFrame() const { return tMove; };
	};

	typedef std::shared_ptr<UnitInterface> Unit;
	typedef std::map<unsigned int, Unit> Unitset;
}