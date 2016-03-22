#include "State.h"

using namespace MCTSG;

State::State()
{
	clear();
}

void State::set(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies)
{
	// reset
	clear();

	// new time frame
	time = t;

	// convert BWAPI units to MCTSG units
	for(auto &u : allies) // ally
	{
		if(u->exists()
			&& !u->getType().isWorker()
			&& !(u->getType() == BWAPI::UnitTypes::Special_Map_Revealer))
		{
			// convert and append
			Unit unit = Unit(u);
			allyUnits.push_back(unit);
		}
	}

	for(auto &u : enemies) // enemy
	{
		if(u->exists()
			&& !u->getType().isWorker()
			&& !(u->getType() == BWAPI::UnitTypes::Special_Map_Revealer))
		{
			// convert and append
			Unit unit = Unit(u);
			enemyUnits.push_back(unit);
		}
	}
}

void State::clear()
{
	time = 0;
	allyUnits.clear();
	enemyUnits.clear();
}