#pragma once
#include <BWAPI.h>
#include <map>
#include <memory>
#include "UnitData.h"

namespace MCTSG
{
	class UnitInterface
	{

		int ID;
		bool ally; // is it ally?
		BWAPI::UnitType unitType; // type of this unit
		BWAPI::Position position; // current position
		int hitPoints; // current HP
		int shields; // current shield
		int tAttack; // next time frame can attack
		int tMove; // next time frame can move

	public:

		UnitInterface();
		UnitInterface(const BWAPI::Unit &unit);

		void update(const BWAPI::Unit &unit);

		void attack(const std::shared_ptr<UnitInterface> &target, const int timeFrame);
		void move(const BWAPI::Position pos, const int timeFrame);

		bool isAlive() const { return hitPoints > 0 ? true : false; };
		bool isAlly() const { return ally; };

		// setters
		void setHitPoints(const int hp) { hitPoints = hp; };
		void setShields(const int sh) { shields = sh; };

		// getters
		int getArmor() const;
		int getSpeed() const { return (int)unitType.topSpeed(); };

		BWAPI::WeaponType getGroundWeapon() const { return unitType.groundWeapon(); };
		int getGroundWeaponDamage() const;
		BWAPI::DamageType getGroundWeaponDamageType() const { return unitType.groundWeapon().damageType(); };
		int getGroundWeaponCooldown() const { return unitType.groundWeapon().damageCooldown(); };
		const int getAttackAnimFrameDuration() const { return UnitData::getAttackAnimFrameDuration(unitType); };

		int getID() const { return ID; };
		BWAPI::UnitType getType() const { return unitType; };
		BWAPI::Position getPosition() const { return position; };
		int getHitPoints() const { return hitPoints; };
		int getShields() const { return shields; };
		int getNextCanAttackFrame() const { return tAttack; };
		int getNextCanMoveFrame() const { return tMove; };

	private:

		int calculateDamageTo(const int damage, const std::shared_ptr<UnitInterface> &defender) const;
	};

	typedef std::shared_ptr<UnitInterface> Unit;
	typedef std::map<unsigned int, Unit> Unitset;
}