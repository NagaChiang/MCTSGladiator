#pragma once
#include <BWAPI.h>
#include "State.h"
#include "Unit.h"
#include "Logger.h"
#include "UCTSearch.h"

namespace MCTSG
{
	class CombatManager
	{

		State _currentState;
		const std::string _logPath;

		Logger _logger;

	public:

		CombatManager();
		CombatManager(const int t, const BWAPI::Unitset &units);

		void set(const int t, const BWAPI::Unitset &units); // set at first
		void update(const int t, const BWAPI::Unitset &units); // update every frame

	private:

		void issueCommands(const Move &move) const;

	};
}