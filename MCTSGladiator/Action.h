#pragma once
#include <BWAPI.h>
#include <vector>
#include "Unit.h"

namespace MCTSG
{
	class Action
	{

		Unit unit; // unit to perform the action
		BWAPI::UnitCommandType actionType; // type of action
		Unit target; // target unit
		int time; // e.g. wait until time frame

	public:

		Action();
		Action(const Unit &u, const BWAPI::UnitCommandType &aType, const Unit &uTarget, const int t);

		// getters
		Unit getUnit() const { return unit; };
		BWAPI::UnitCommandType getType() const { return actionType; };
		Unit getTarget() const { return target; };
		int getEndFrame() const { return time; };
	};

	typedef std::vector<Action> Move;
}