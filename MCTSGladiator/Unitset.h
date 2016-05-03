#pragma once
#include <BWAPI.h>
#include <map>
#include "Unit.h"

namespace MCTSG
{
	class Unitset : public std::map<unsigned int, Unit>
	{

	public:

		void addUnit(const Unit &unit);
		Unit getUnit(const int ID) const;

		Unitset deepCopy() const;
		BWAPI::Position getCenter() const;
	};
}
