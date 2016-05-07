#include "UnitData.h"

using namespace MCTSG;

// return the attack animation frame duration of this unit
const int UnitData::getAttackAnimFrameDuration(const BWAPI::UnitType &unitType)
{
	if(unitType == BWAPI::UnitTypes::Terran_Marine)
		return 8;
	else if(unitType == BWAPI::UnitTypes::Protoss_Zealot)
		return 7;
	else if(unitType == BWAPI::UnitTypes::Protoss_Dragoon)
		return 7;
	else if(unitType == BWAPI::UnitTypes::Zerg_Zergling)
		return 4;
	else
		return 0;
}
