#pragma once
#include <BWAPI.h>

class UnitState
{

public:

	BWAPI::UnitType unitType; // type of this unit
	BWAPI::Position position; // current position
	int hitPoints; // current HP
	int tAttack; // next time frame can attack
	int tMove; // next time frame can move

	UnitState();
};