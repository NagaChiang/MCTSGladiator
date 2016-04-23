#include "Unit.h"

using namespace MCTSG;

UnitInterface::UnitInterface()
{
	ID = -1;
	ally = false;
	unitType = BWAPI::UnitTypes::None;
	position = BWAPI::Positions::None;
	hitPoints = 0;
	shields = 0;
	tAttack = 0;
	tMove = 0;
}

UnitInterface::UnitInterface(const BWAPI::Unit &unit)
{
	ID = unit->getID();
	ally = unit->getPlayer() == BWAPI::Broodwar->self() ? true : false;
	unitType = unit->getType();
	position = unit->getPosition();
	hitPoints = unit->getHitPoints();
	shields = unit->getShields();
	tAttack = 0; // with no prior information
	tMove = 0; // with no prior information
}

// update status from real unit (except for tAttack, tMove)
void UnitInterface::update(const BWAPI::Unit &unit)
{
	if(ID == unit->getID()) // double check 
	{
		position = unit->getPosition();
		hitPoints = unit->getHitPoints();
		shields = unit->getShields();
	}
}

void UnitInterface::doAction(const Action &action)
{
	switch(action.getType())
	{
		case Actions::Stop:
			// stop/wait at the same position
			break;

		case Actions::Attack:
			break;

		case Actions::North:
			break;

		case Actions::East:
			break;

		case Actions::West:
			break;

		case Actions::South:
			break;

		default:
			// undefined action type
			break;
	}
}

// armor including upgrades
int UnitInterface::getArmor() const
{
	if(isAlly())
		return BWAPI::Broodwar->self()->armor(unitType);
	else // enemy
		return BWAPI::Broodwar->enemy()->armor(unitType);
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
int UnitInterface::calculateDamageTo(const int damage, const Unit &defender) const
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
			dmg = (int)dmg * 0.25;
		else if(sizeType == BWAPI::UnitSizeTypes::Medium)
			dmg = (int)dmg * 0.50;
		else if(sizeType == BWAPI::UnitSizeTypes::Small)
			dmg = (int)dmg * 1.00;
	}
	else if(damageType == BWAPI::DamageTypes::Explosive)
	{
		if(sizeType == BWAPI::UnitSizeTypes::Large)
			dmg = (int)dmg * 1.00;
		else if(sizeType == BWAPI::UnitSizeTypes::Medium)
			dmg = (int)dmg * 0.75;
		else if(sizeType == BWAPI::UnitSizeTypes::Small)
			dmg = (int)dmg * 0.50;
	}

	// least damage check
	dmg = dmg < 1 ? 1 : dmg;

	return dmg;
}