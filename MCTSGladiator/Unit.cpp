#include "Unit.h"

using namespace MCTSG;

Unit::Unit()
{
	ID = -1;
	unitType = BWAPI::UnitTypes::None;
	position = BWAPI::Positions::None;
	hitPoints = 0;
	tAttack = 0;
	tMove = 0;
}

Unit::Unit(const BWAPI::Unit &unit)
{
	ID = unit->getID();
	unitType = unit->getType();
	position = unit->getPosition();
	hitPoints = unit->getHitPoints() + unit->getShields();
	tAttack = 0; // with no prior information
	tMove = 0; // with no prior information
}

// update status from real unit (except for tAttack, tMove)
void Unit::update(const BWAPI::Unit &unit)
{
	if(ID == unit->getID())
	{
		position = unit->getPosition();
		hitPoints = unit->getHitPoints() + unit->getShields();
	}
}

bool Unit::isAlive()
{
	if(hitPoints > 0)
		return true;
	else
		return false;
}