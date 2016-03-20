#pragma once
#include <BWAPI.h>
#include "UnitState.h"

class Action
{

public:

	UnitState *unit; // unit to perform the action
	BWAPI::UnitCommandType actionType; // type of action
	UnitState *target; // target
	int t; // e.g. wait until time t 

	Action();
};