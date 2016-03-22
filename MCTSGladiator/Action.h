#pragma once
#include <BWAPI.h>
#include "Unit.h"

namespace MCTSG
{
	class Action
	{

	public:

		Unit *unit; // unit to perform the action
		BWAPI::UnitCommandType actionType; // type of action
		Unit *target; // target
		int t; // e.g. wait until time t 

		Action();
	};
}