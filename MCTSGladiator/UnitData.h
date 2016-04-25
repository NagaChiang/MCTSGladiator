#pragma once
#include <BWAPI.h>

namespace MCTSG
{
	class UnitData
	{

	public:

		static const int MOVE_DURATION = 8; // fixed duration for move (1/3 second)

		static const int getAttackAnimFrameDuration(const BWAPI::UnitType &unitType);
	};
}