#include "Action.h"

using namespace MCTSG;

Action::Action()
{
	_unit = NULL;
	_actionType = Actions::None;
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

std::string Action::toString() const
{
	switch(_actionType)
	{
		case Actions::None:
			return "None";
			break;

		case Actions::Stop:
			return "Stop";
			break;

		case Actions::North:
			return "North";
			break;

		case Actions::East:
			return "East";
			break;

		case Actions::West:
			return "West";
			break;

		case Actions::South:
			return "South";
			break;

		case Actions::Attack:
			return "Attack";
			break;

		default:
			return "Invalid";
			break;
	}
}