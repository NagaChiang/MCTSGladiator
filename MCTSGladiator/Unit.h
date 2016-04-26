#pragma once
#include <BWAPI.h>
#include <map>
#include <memory>
#include "UnitData.h"

namespace MCTSG
{
	class UnitInterface
	{

		int _ID;
		bool _ally; // is it ally?
		BWAPI::UnitType _unitType; // type of this unit
		BWAPI::Position _position; // current position
		int _hitPoints; // current HP
		int _shields; // current shield
		int _tAttack; // next time frame can attack
		int _tMove; // next time frame can move

	public:

		UnitInterface();
		UnitInterface(const BWAPI::Unit &unit);

		void update(const BWAPI::Unit &unit);

		void attack(const std::shared_ptr<UnitInterface> &target, const int timeFrame);
		void move(const BWAPI::Position pos, const int timeFrame);

		bool isAlive() const { return _hitPoints > 0 ? true : false; };
		bool isAlly() const { return _ally; };

		// setters
		void setHitPoints(const int hp) { _hitPoints = hp; };
		void setShields(const int sh) { _shields = sh; };

		// getters
		int getArmor() const;
		int getSpeed() const { return (int)_unitType.topSpeed(); };

		BWAPI::WeaponType getGroundWeapon() const { return _unitType.groundWeapon(); };
		int getGroundWeaponDamage() const;
		BWAPI::DamageType getGroundWeaponDamageType() const { return _unitType.groundWeapon().damageType(); };
		int getGroundWeaponCooldown() const { return _unitType.groundWeapon().damageCooldown(); };
		const int getAttackAnimFrameDuration() const { return UnitData::getAttackAnimFrameDuration(_unitType); };

		int getID() const { return _ID; };
		BWAPI::UnitType getType() const { return _unitType; };
		BWAPI::Position getPosition() const { return _position; };
		int getHitPoints() const { return _hitPoints; };
		int getShields() const { return _shields; };
		int getNextCanAttackFrame() const { return _tAttack; };
		int getNextCanMoveFrame() const { return _tMove; };

	private:

		int calculateDamageTo(const int damage, const std::shared_ptr<UnitInterface> &defender) const;
	};

	typedef std::shared_ptr<UnitInterface> Unit;
	typedef std::map<unsigned int, Unit> Unitset;
}