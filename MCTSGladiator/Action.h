#pragma once
#include <BWAPI.h>
#include <vector>
#include "Unit.h"

namespace MCTSG
{
	class Action
	{

		int unitID; // ID of unit to perform the action
		BWAPI::UnitCommandType actionType; // type of action
		int targetID; // Id of target
		int time; // e.g. wait until time t 

	public:

		Action();
		Action(const int uID, const BWAPI::UnitCommandType &aType, const int tID, const int t);
	};

	typedef std::vector<Action> Move;
}