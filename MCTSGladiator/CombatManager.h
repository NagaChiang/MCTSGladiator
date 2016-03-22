#pragma once
#include <BWAPI.h>
#include "State.h"
#include "Unit.h"
#include "Logger.h"

namespace MCTSG
{
	class CombatManager
	{

	public:

		State currentState;

		CombatManager();

		void init(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies); // set at first
		void update(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies); // update every frame

	private:

		const std::string logPath;
		Logger logger;
	};
}