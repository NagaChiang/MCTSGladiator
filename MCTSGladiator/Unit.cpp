#include "Unit.h"

using namespace MCTSG;

UnitInterface::UnitInterface()
{
	ID = -1;
	ally = false;
	unitType = BWAPI::UnitTypes::None;
	position = BWAPI::Positions::None;
	hitPoints = 0;
	tAttack = 0;
	tMove = 0;
}

UnitInterface::UnitInterface(const BWAPI::Unit &unit)
{
	ID = unit->getID();
	ally = unit->getPlayer() == BWAPI::Broodwar->self() ? true : false;
	unitType = unit->getType();
	position = unit->getPosition();
	hitPoints = unit->getHitPoints() + unit->getShields();
	tAttack = 0; // with no prior information
	tMove = 0; // with no prior information
}

// update status from real unit (except for tAttack, tMove)
void UnitInterface::update(const BWAPI::Unit &unit)
{
	if(ID == unit->getID()) // double check 
	{
		position = unit->getPosition();
		hitPoints = unit->getHitPoints() + unit->getShields();
	}
}

void UnitInterface::command(const Action &action)
{

}

bool UnitInterface::isAlive() const
{
	if(hitPoints > 0)
		return true;
	else
		return false;
}