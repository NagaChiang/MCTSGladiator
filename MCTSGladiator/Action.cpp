#include "Action.h"

using namespace MCTSG;

Action::Action()
{
	_unit = NULL;
	_actionType = Actions::Stop;
	_target = NULL;
	_time = -1;
}

Action::Action(const Unit &u, const Actions &aType, const Unit &uTarget, const int t)
{
	_unit = u;
	_actionType = aType;
	_target = uTarget;
	_time = t;
}