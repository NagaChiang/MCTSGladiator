#include "CombatManager.h"

using namespace MCTSG;

const std::string LOG_PATH = "D:/Phate/Temp/Thesis/MCTSGladiator/Release/CombatManagerLog.txt";

CombatManager::CombatManager()
	:_logPath(LOG_PATH)
{
	_logger.init(_logPath);
}

CombatManager::CombatManager(const int t, const BWAPI::Unitset &units)
	:_logPath(LOG_PATH)
{
	set(t, units);
	_logger.init(_logPath);
}

// assumption: no more units would appear later
void CombatManager::set(const int t, const BWAPI::Unitset &units)
{
	// set new state
	_currentState.set(t, units);

	// log init state
	_logger.log("CombatManager has been initiated!");
	_logger.log(_currentState);
}

// update the real state for manager; also track on status of units, such as attack CD
// perform UCT search and issue the commands
void CombatManager::update(const int t, const BWAPI::Unitset &units)
{
	// update state
	_currentState.update(t, units);
	//logger.log(currentState); // debug

	// UCT Search
	UCTSearchParams params;
	params.timeLimit = std::chrono::milliseconds(40);
	params.maxChildren = 20;
	params.explorationConst = 1.6;
	UCTSearch UCT = UCTSearch(params);
	Move bestMove = UCT.search(_currentState);

	// issue commands
}