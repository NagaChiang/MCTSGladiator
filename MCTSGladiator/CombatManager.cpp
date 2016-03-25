#include "CombatManager.h"

using namespace MCTSG;

CombatManager::CombatManager()
	:logPath("D:/Phate/Temp/Thesis/MCTSGladiator/Release/CombatManagerLog.txt")
{
	currentState = State();
	logger.init(logPath);
}

// assumption: no more units would appear later
void CombatManager::init(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies)
{
	// set new state
	currentState.set(t, allies, enemies);

	// log init state
	logger.log("CombatManager has been initiated!");
	logger.log(currentState);
}

// update the real state for manager; also track on status of units, such as attack CD
// perform UCT search and issue the commands
void CombatManager::update(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies)
{
	// update state
	currentState.update(t, allies, enemies);
}