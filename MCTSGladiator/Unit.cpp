#include "Unit.h"

using namespace MCTSG;

UnitInterface::UnitInterface()
{
	_ID = -1;
	_ally = false;
	_unitType = BWAPI::UnitTypes::None;
	_position = BWAPI::Positions::None;
	_hitPoints = 0;
	_shields = 0;
	_tAttack = 0;
	_tMove = 0;
}

UnitInterface::UnitInterface(const BWAPI::Unit &unit)
{
	_ID = unit->getID();
	_ally = unit->getPlayer() == BWAPI::Broodwar->self() ? true : false;
	_unitType = unit->getType();
	_position = unit->getPosition();
	_hitPoints = unit->getHitPoints();
	_shields = unit->getShields();
	_tAttack = 0; // with no prior information
	_tMove = 0; // with no prior information
}

// update status from real unit (except for tAttack, tMove)
void UnitInterface::update(const BWAPI::Unit &unit)
{
	if(_ID == unit->getID()) // double check 
	{
		_position = unit->getPosition();
		_hitPoints = unit->getHitPoints();
		_shields = unit->getShields();
	}
}

void UnitInterface::attack(const std::shared_ptr<UnitInterface> &target, const int timeFrame)
{
	// TODO: can attack?

	// total damage by this unit
	int totalDamage = getGroundWeaponDamage();
	int remainDamage = totalDamage;

	// shields take full damage
	int targetShields = target->getShields();
	if(targetShields < totalDamage) // not enough shields
	{
		targetShields = 0;
		remainDamage = totalDamage - targetShields;
	}
	else // enough shields
	{
		targetShields -= totalDamage;
		remainDamage = 0;
	}
	target->setShields(targetShields);

	// remaining damage to hitpoints
	if(remainDamage > 0)
	{
		int calculatedDamage = calculateDamageTo(remainDamage, target);
		int targetHP = target->getHitPoints();
		targetHP -= calculatedDamage;

		target->setHitPoints(targetHP);
	}

	// update next available time frame
	_tMove = timeFrame + getAttackAnimFrameDuration();
	_tAttack = timeFrame + getGroundWeaponCooldown();
}

void UnitInterface::move(const BWAPI::Position pos, const int timeFrame)
{
	// move instantly
	_position = pos;

	// update next available time frame
	_tMove = timeFrame + UnitData::MOVE_DURATION;
}

// armor including upgrades
int UnitInterface::getArmor() const
{
	if(isAlly())
		return BWAPI::Broodwar->self()->armor(_unitType);
	else // enemy
		return BWAPI::Broodwar->enemy()->armor(_unitType);
}

// weapon damage including upgrades
int UnitInterface::getGroundWeaponDamage() const
{
	if(isAlly())
		return BWAPI::Broodwar->self()->damage(getGroundWeapon());
	else // enemy
		return BWAPI::Broodwar->enemy()->damage(getGroundWeapon());
}

// calculate damage depending on armor, damage type and unit size
int UnitInterface::calculateDamageTo(const int damage, const std::shared_ptr<UnitInterface> &defender) const
{
	int dmg = damage;

	// armor
	dmg -= defender->getArmor();

	// damage type and unit size
	BWAPI::DamageType damageType = getGroundWeaponDamageType();
	BWAPI::UnitSizeType sizeType = defender->getType().size();

	if(damageType == BWAPI::DamageTypes::Concussive)
	{
		if(sizeType == BWAPI::UnitSizeTypes::Large)
			dmg = (int)(dmg * 0.25);
		else if(sizeType == BWAPI::UnitSizeTypes::Medium)
			dmg = (int)(dmg * 0.50);
		else if(sizeType == BWAPI::UnitSizeTypes::Small)
			dmg = (int)(dmg * 1.00);
	}
	else if(damageType == BWAPI::DamageTypes::Explosive)
	{
		if(sizeType == BWAPI::UnitSizeTypes::Large)
			dmg = (int)(dmg * 1.00);
		else if(sizeType == BWAPI::UnitSizeTypes::Medium)
			dmg = (int)(dmg * 0.75);
		else if(sizeType == BWAPI::UnitSizeTypes::Small)
			dmg = (int)(dmg * 0.50);
	}

	// least damage check
	dmg = dmg < 1 ? 1 : dmg;

	return dmg;
}