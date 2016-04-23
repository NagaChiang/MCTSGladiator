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

		Unit unit; // unit to perform the action
		Actions actionType; // type of action
		Unit target; // target unit
		int time; // e.g. wait until time frame

	public:

		Action();
		Action(const Unit &u, const Actions &aType, const Unit &uTarget, const int t);

		// getters
		Unit getUnit() const { return unit; };
		Actions getType() const { return actionType; };
		Unit getTarget() const { return target; };
		int getEndFrame() const { return time; };
	};

	typedef std::vector<Action> Move;
}