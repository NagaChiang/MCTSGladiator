#pragma once
#include <BWAPI.h>
#include "State.h"
#include "Unit.h"
#include "Logger.h"
#include "UCTSearch.h"
#include "ConfigParser.h"

namespace MCTSG
{
	class CombatManager
	{

		State _currentState;
		UCTSearch _uctSearch;

	public:

		CombatManager();
		CombatManager(const int t, const BWAPI::Unitset &units);

		void set(const int t, const BWAPI::Unitset &units); // set at first
		void update(const int t, const BWAPI::Unitset &units); // update every frame

		// getters
		UCTSearch getUCTSearch() const { return _uctSearch; };

	private:

		void issueCommands(const Move &move) const;
		void smartAttack(const BWAPI::Unit &unit, const BWAPI::Unit &target) const;
		void smartMove(const BWAPI::Unit &unit, const BWAPI::Position &pos) const;

	};
}