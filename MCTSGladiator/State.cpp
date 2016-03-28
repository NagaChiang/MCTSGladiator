#include "State.h"

using namespace MCTSG;

State::State()
{
	clear();
}

void State::clear()
{
	time = 0;
	allyUnits.clear();
	enemyUnits.clear();
}

void State::set(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies)
{
	// reset
	clear();

	// time frame
	time = t;

	// convert BWAPI units to MCTSG units
	setUnits(allyUnits, allies);
	setUnits(enemyUnits, enemies);
}

void State::update(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies)
{
	// new time frame
	time = t;

	// update unitsets
	updateUnits(allyUnits, allies);
	updateUnits(enemyUnits, enemies);
}

// is game over? (in combat scenario)
bool State::isEnd() const
{
	if(allyUnits.size() == 0 || enemyUnits.size() == 0)
		return true;
	else
		return false;
}

// get unit by ID
Unit* State::getUnit(int ID)
{
	Unit *unit = NULL; // return null if not found
	bool isFound = false;
	for(Unit &u : allyUnits)
	{
		if(u.getID() == ID)
		{
			unit = &u;
			isFound = true;
			break;
		}
	}

	if(isFound) // if not in allies, keep searching
	{
		for(Unit &u : enemyUnits)
		{
			if(u.getID() == ID)
			{
				unit = &u;
				isFound = true;
				break;
			}
		}
	}

	return unit; // return null if not found
}

void State::setUnits(std::vector<Unit> &vecUnits, const BWAPI::Unitset &unitset)
{
	for(auto &u : unitset) // enemy
	{
		if(u->exists()
			&& !u->getType().isWorker()
			&& !(u->getType() == BWAPI::UnitTypes::Special_Map_Revealer))
		{
			// convert and append
			Unit unit = Unit(u);
			vecUnits.push_back(unit);
		}
	}
}

void State::updateUnits(std::vector<Unit> &vecUnits, const BWAPI::Unitset &unitset)
{
	// update new units status from real state (except for tAttack, tMove)
	for(std::vector<Unit>::iterator itr = vecUnits.begin();
		itr != vecUnits.end(); /* nothing */)
	{
		bool isFound = false;
		for(auto &real_u : unitset)
		{
			if(real_u->getID() == itr->getID())
			{
				itr->update(real_u);
				isFound = true;
				itr++;
				break;
			}
		}

		// if not found, this unit is dead
		if(!isFound)
		{
			// remove it from the vector
			itr = vecUnits.erase(itr); // next element after erased one
		}
	}
}