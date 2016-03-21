#pragma once
#include <BWAPI.h>
#include <vector>
#include "UnitState.h"

class State
{

public:

	int t; // current time frame
	std::vector<UnitState*> allyUnits;
	std::vector<UnitState*> enemyUnits;

	State();

};