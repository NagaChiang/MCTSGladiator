#include "Action.h"

using namespace MCTSG;

Action::Action()
{
	unit = NULL;
	actionType = BWAPI::UnitCommandTypes::None;
	target = NULL;
	time = -1;
}

Action::Action(const Unit &u, const BWAPI::UnitCommandType &aType, const Unit &uTarget = NULL, const int t = -1)
{
	unit = u;
	actionType = aType;
	target = uTarget;
	time = t;
}