#pragma once
#include <BWAPI.h>
#include <vector>
#include "Unit.h"

namespace MCTSG
{
	enum Actions
	{
		Stop,
		North,
		East,
		West,
		South,
		Attack,
		NUM
	};

	class Action
	{

		Unit _unit; // unit to perform the action
		Actions _actionType; // type of action
		Unit _target; // target unit
		int _time; // e.g. wait until time frame

	public:

		Action();
		Action(const Unit &u, const Actions &aType, const Unit &uTarget, const int t);

		// getters
		Unit getUnit() const { return _unit; };
		Actions getType() const { return _actionType; };
		Unit getTarget() const { return _target; };
		int getEndFrame() const { return _time; };
	};

	typedef std::vector<Action> Move;
}