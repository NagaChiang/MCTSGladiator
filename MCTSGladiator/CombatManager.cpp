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
	//Logger::instance()->log(_currentState);

	// UCT Search
	UCTSearchParams params;
	params.timeLimit = std::chrono::milliseconds(50);
	params.maxChildren = 20;
	params.explorationConst = 1.6;
	params.evalMethod = EvaluationMethod::MostVisited;
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
		BWAPI::Unit unit = BWAPI::Broodwar->getUnit(action.getUnitID());

		// null unit
		if(!unit)
			continue;

		// unit is busy attacking
		if(unit->isAttackFrame())
			continue;

		// get properties
		BWAPI::Position position = unit->getPosition();
		Unit u = _currentState.getUnit(action.getUnitID());
		int speed = 1;
		if(u)
			speed = u->getSpeed();

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
				BWAPI::Unit target = BWAPI::Broodwar->getUnit(action.getTargetID());

				// has already been told to attack that target
				BWAPI::UnitCommand currentCommand(unit->getLastCommand());
				if(currentCommand.getType() == BWAPI::UnitCommandTypes::Attack_Unit
					&& currentCommand.getTarget() == target)
					continue;

				// attack
				if(target)
				{
					unit->attack(target);

					// in virtual state // TODO: should we do the same thing for movement?
					Unit unitFake = _currentState.getUnit(action.getUnitID());
					if(unitFake)
						unitFake->attack(NULL, _currentState.getTimeFrame()); // for cooldown
				}

				break;
			}

			default:
				BWAPI::Broodwar << "[ERROR] Invalid action!" << std::endl;
				break;
		}
	}
}