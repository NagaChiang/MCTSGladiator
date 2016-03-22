#include "CombatManager.h"

using namespace MCTSG;

CombatManager::CombatManager()
	:logPath("D:/Phate/Temp/Thesis/MCTSGladiator/Release/combatLog.txt")
{
	currentState = State();
	logger.init(logPath);
}

// assumption: no more units would appear later
void CombatManager::init(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies)
{
	// set new state
	currentState.set(t, allies, enemies);
}

// update the real state for manager; track on status of units, such as attack CD
void CombatManager::update(const int t, const BWAPI::Unitset &allies, const BWAPI::Unitset &enemies)
{
	
}