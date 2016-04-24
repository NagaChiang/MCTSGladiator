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
		int shields; // current shield
		int tAttack; // next time frame can attack
		int tMove; // next time frame can move

	public:

		UnitInterface();
		UnitInterface(const BWAPI::Unit &unit);

		void update(const BWAPI::Unit &unit);

		void attack(const std::shared_ptr<UnitInterface> &target);
		void move(const BWAPI::Position pos) { position = pos; };

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