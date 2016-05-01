#include "CombatManager.h"

using namespace MCTSG;


CombatManager::CombatManager()
{
	
}

CombatManager::CombatManager(const int t, const BWAPI::Unitset &units)
{
	set(t, units);
}

// assumption: no more units would appear later
void CombatManager::set(const int t, const BWAPI::Unitset &units)
{
	// set new state
	_currentState.set(t, units);

	// log init state
	Logger::instance()->log("CombatManager has been initiated!");
	Logger::instance()->log(_currentState);

	// UCT Search
	UCTSearchParams params;
	params.timeLimit = std::chrono::milliseconds(40);
	params.maxChildren = 20;
	params.explorationConst = 1.6;
	_uctSearch = UCTSearch(params);
}

// update the real state for manager; also track on status of units, such as attack CD
// perform UCT search and issue the commands
void CombatManager::update(const int t, const BWAPI::Unitset &units)
{
	// update state
	_currentState.update(t, units);

	// UCT Search
	Move bestMove = _uctSearch.search(_currentState);

	// issue commands
	issueCommands(bestMove);
}

void CombatManager::issueCommands(const Move &move) const
{
	for(Action action : move)
	{
		// get real unit
		BWAPI::Unit unit = BWAPI::Broodwar->getUnit(action.getUnit()->getID());

		// null unit
		if(!unit)
			continue;

		// unit is busy attacking
		if(unit->isAttackFrame())
			continue;

		// get properties
		BWAPI::Position position = unit->getPosition();
		int speed = action.getUnit()->getSpeed();

		// do action
		switch(action.getType())
		{
			case Actions::None:
				// do nothing
				break;

			case Actions::Stop:
				unit->stop();
				break;

			case Actions::North:
			{
				BWAPI::Position dirNorth = BWAPI::Position(0, 1);
				BWAPI::Position posNew = position + (dirNorth * speed * UnitData::MOVE_DURATION);
				unit->move(posNew);
				break;
			}

			case Actions::East:
			{
				BWAPI::Position dirEast = BWAPI::Position(1, 0);
				BWAPI::Position posNew = position + (dirEast * speed * UnitData::MOVE_DURATION);
				unit->move(posNew);
				break;
			}

			case Actions::West:
			{
				BWAPI::Position dirWest = BWAPI::Position(-1, 0);
				BWAPI::Position posNew = position + (dirWest * speed * UnitData::MOVE_DURATION);
				unit->move(posNew);
				break;
			}

			case Actions::South:
			{
				BWAPI::Position dirSouth = BWAPI::Position(0, -1);
				BWAPI::Position posNew = position + (dirSouth * speed * UnitData::MOVE_DURATION);
				unit->move(posNew);
				break;
			}

			case Actions::Attack:
			{
				// get target
				BWAPI::Unit target = BWAPI::Broodwar->getUnit(action.getTarget()->getID());

				// has already been told to attack that target
				BWAPI::UnitCommand currentCommand(unit->getLastCommand());
				if(currentCommand.getType() == BWAPI::UnitCommandTypes::Attack_Unit
					&& currentCommand.getTarget() == target)
					continue;

				// attack
				unit->attack(target);
				action.getUnit()->attack(action.getTarget(), _currentState.getTimeFrame()); // for cooldown

				break;
			}

			default:
				BWAPI::Broodwar << "[ERROR] Invalid action!" << std::endl;
				break;
		}
	}
}