#include "Action.h"

using namespace MCTSG;

Action::Action()
{
	_unitID = -1;
	_actionType = Actions::None;
	_targetID = -1;
	_time = -1;
}

Action::Action(const int &ID, const Actions &aType, const int &targetID, const int t)
{
	_unitID = ID;
	_actionType = aType;
	_targetID = targetID;
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