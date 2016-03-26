#include "Action.h"

using namespace MCTSG;

Action::Action()
{
	unitID = -1;
	actionType = BWAPI::UnitCommandTypes::None;
	targetID = -1;
	time = -1;
}

Action::Action(const int uID, const BWAPI::UnitCommandType &aType, const int tID = -1, const int t = -1)
{
	unitID = uID;
	actionType = aType;
	targetID = tID;
	time = t;
}