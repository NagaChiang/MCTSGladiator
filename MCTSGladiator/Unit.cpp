#include "Unit.h"

using namespace MCTSG;

Unit::Unit()
{
	unitType = BWAPI::UnitTypes::None;
	position = BWAPI::Positions::None;
	hitPoints = 0;
	tAttack = 0;
	tMove = 0;
}

Unit::Unit(const BWAPI::Unit &unit)
{
	unitType = unit->getType();
	position = unit->getPosition();
	hitPoints = unit->getHitPoints() + unit->getShields();
	tAttack = 0; // with no prior information
	tMove = 0; // with no prior information
}