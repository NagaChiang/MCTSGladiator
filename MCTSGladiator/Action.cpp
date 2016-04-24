#include "Action.h"

using namespace MCTSG;

Action::Action()
{
	unit = NULL;
	actionType = Actions::Stop;
	target = NULL;
	time = -1;
}

Action::Action(const Unit &u, const Actions &aType, const Unit &uTarget, const int t)
{
	unit = u;
	actionType = aType;
	target = uTarget;
	time = t;
}