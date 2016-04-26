#include "State.h"

using namespace MCTSG;

State::State()
{
	clear();
}

State::State(const int t, const BWAPI::Unitset &units)
{
	set(t, units);
}

State::~State()
{
	clear();
}

void State::clear()
{
	_time = 0;
	_allUnits.clear();
}

void State::set(const int t, const BWAPI::Unitset &units)
{
	// reset
	clear();

	// time frame
	_time = t;

	// convert BWAPI units to MCTSG units
	for(auto &u : units)
	{
		if(u->exists()
			&& !u->getType().isWorker()
			&& !(u->getType() == BWAPI::UnitTypes::Special_Map_Revealer))
		{
			// convert and append
			Unit unit = Unit(new UnitInterface(u));
			_allUnits.insert(Unitset::value_type(u->getID(), unit));
		}
	}
}

void State::update(const int t, const BWAPI::Unitset &units)
{
	// new time frame
	_time = t;

	// update units
	for(Unitset::iterator itr = _allUnits.begin(); itr != _allUnits.end(); itr++)
	{
		// search real units
		bool isFound = false;
		for(auto &u : units)
		{
			if(u->getID() == itr->second->getID())
			{
				itr->second->update(u); // update MCTSG unit
				isFound = true;
			}
		}

		if(!isFound) // not found -> dead in real game
		{
			_allUnits.erase(itr->second->getID()); // erase the unit
		}
	}
}

void State::makeMove(const Move move)
{
	// get next interesting time frame (find the min)
	int minTimeFrame = 9999999;
	for(auto &itr : _allUnits)
	{
		Unit unit = itr.second;
		int canAttackFrame = unit->getNextCanAttackFrame();
		int canMoveFrame = unit->getNextCanMoveFrame();
		if(canAttackFrame < minTimeFrame)
			minTimeFrame = canAttackFrame;
		if(canMoveFrame < minTimeFrame)
			minTimeFrame = canMoveFrame;
	}

	// make actions in the move
	for(const Action &action : move)
	{
		doAction(action);
	}

	// update frame time of this state
	_time = minTimeFrame;
}

void State::doAction(const Action &action)
{
	Unit unit = action.getUnit();
	BWAPI::Position position = unit->getPosition();
	int speed = unit->getSpeed();
	switch(action.getType())
	{
		case Actions::Stop:
			// stop/wait at the same position
			break;

		case Actions::Attack:
			unit->attack(action.getTarget(), _time);
			break;

		case Actions::North:
		{
			BWAPI::Position dirNorth = BWAPI::Position(0, 1);
			BWAPI::Position posNew = position + (dirNorth * speed * UnitData::MOVE_DURATION);
			unit->move(posNew, _time);
			break;
		}

		case Actions::East:
		{
			BWAPI::Position dirEast = BWAPI::Position(1, 0);
			BWAPI::Position posNew = position + (dirEast * speed * UnitData::MOVE_DURATION);
			unit->move(posNew, _time);
			break;
		}

		case Actions::West:
		{
			BWAPI::Position dirWest = BWAPI::Position(-1, 0);
			BWAPI::Position posNew = position + (dirWest * speed * UnitData::MOVE_DURATION);
			unit->move(posNew, _time);
			break;
		}

		case Actions::South:
		{
			BWAPI::Position dirSouth = BWAPI::Position(0, -1);
			BWAPI::Position posNew = position + (dirSouth * speed * UnitData::MOVE_DURATION);
			unit->move(posNew, _time);
			break;
		}

		default:
			// undefined action type
			BWAPI::Broodwar << "ERROR: Undefined action type." << std::endl;
			break;
	}
}

// is game over? (in combat scenario)
bool State::isEnd() const
{
	if(getAllyUnitsNum() == 0 || getEnemyUnitsNum() == 0)
		return true;
	else
		return false;
}

// get unit by ID; return null if not found
Unit State::getUnit(const int ID) const
{
	Unit unit = NULL;

	try
	{
		unit = _allUnits.at(ID);
	}
	catch(const std::out_of_range e)
	{
		// maybe log?
		//BWAPI::Broodwar << "ERROR: Unit ID not found!" << " (" << ID << ")" << std::endl;
	}

	return unit;
}

int State::getAllyUnitsNum() const
{
	int num = 0;
	for(const auto &itr : _allUnits)
	{
		if(itr.second->isAlly())
			num++;
	}

	return num;
}

int State::getEnemyUnitsNum() const
{
	int num = 0;
	for(const auto &itr : _allUnits)
	{
		if(!itr.second->isAlly())
			num++;
	}

	return num;
}

Unitset State::getAllyUnits() const
{
	Unitset unitset; // empty unitset
	for(Unitset::const_iterator itr = _allUnits.begin(); itr != _allUnits.end(); itr++)
	{
		if(itr->second->isAlly())
			unitset.insert(Unitset::value_type(itr->first, itr->second)); // add
	}

	return unitset;
}

Unitset State::getEnemyUnits() const
{
	Unitset unitset; // empty unitset
	for(Unitset::const_iterator itr = _allUnits.begin(); itr != _allUnits.end(); itr++)
	{
		if(!itr->second->isAlly())
			unitset.insert(Unitset::value_type(itr->first, itr->second)); // add
	}

	return unitset;
}