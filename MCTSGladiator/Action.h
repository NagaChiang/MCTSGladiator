#pragma once
#include <BWAPI.h>
#include <vector>
#include "Unit.h"

namespace MCTSG
{
	enum Actions
	{
		None,
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

		int _unitID; // index of unit to perform the action
		Actions _actionType; // type of action
		int _targetID; // index of target unit
		int _time; // e.g. wait until time frame

	public:

		Action();
		Action(const int &ID, const Actions &aType, const int &targetID, const int t);

		std::string toString() const;

		// getters
		int getUnitID() const { return _unitID; };
		Actions getType() const { return _actionType; };
		int getTargetID() const { return _targetID; };
		int getEndFrame() const { return _time; };

		// setters
		void setType(const Actions &type) { _actionType = type; };
	};

	typedef std::vector<Action> Move;
}