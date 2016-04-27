#include "Unitset.h"

using namespace MCTSG;

void Unitset::addUnit(const Unit &unit)
{
	insert(Unitset::value_type(unit->getID(), unit));
}

Unitset Unitset::deepCopy() const
{
	Unitset cloneSet;
	for(auto &itr : *this)
	{
		Unit unit = itr.second;
		Unit clone = Unit(new UnitInterface(unit));
		cloneSet.addUnit(clone);
	}
}

BWAPI::Position Unitset::getCenter() const
{
	// sum of all positions
	BWAPI::Position posSum = BWAPI::Position(0, 0);
	for(auto &itr : *this)
	{
		Unit unit = itr.second;
		posSum += unit->getPosition();
	}

	// calculate center
	BWAPI::Position posCenter = posSum / size();

	return posCenter;
}