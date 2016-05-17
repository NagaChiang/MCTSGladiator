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

	// UCT Search
	UCTSearchParams params = ConfigParser::instance()->getUCTParams();
	_uctSearch = UCTSearch(params);

	// display config
	BWAPI::Broodwar << "Max Children: " << params.maxChildren << std::endl;
	BWAPI::Broodwar << "Exploration Constant: " << params.explorationConst << std::endl;
	BWAPI::Broodwar << "Evaluation Method: " << (int)params.evalMethod << std::endl;

	// log
	/*
	Logger::instance()->log("CombatManager has been initiated!");
	Logger::instance()->log(params.maxChildren);
	Logger::instance()->log(params.explorationConst);
	Logger::instance()->log((int)params.evalMethod);
	*/
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
				BWAPI::Position dirNorth = BWAPI::Position(0, -1);
				BWAPI::Position posNew = position + (dirNorth * speed * UnitData::MOVE_DURATION);
				smartMove(unit, posNew);
				break;
			}

			case Actions::East:
			{
				BWAPI::Position dirEast = BWAPI::Position(1, 0);
				BWAPI::Position posNew = position + (dirEast * speed * UnitData::MOVE_DURATION);
				smartMove(unit, posNew);
				break;
			}

			case Actions::West:
			{
				BWAPI::Position dirWest = BWAPI::Position(-1, 0);
				BWAPI::Position posNew = position + (dirWest * speed * UnitData::MOVE_DURATION);
				smartMove(unit, posNew);
				break;
			}

			case Actions::South:
			{
				BWAPI::Position dirSouth = BWAPI::Position(0, 1);
				BWAPI::Position posNew = position + (dirSouth * speed * UnitData::MOVE_DURATION);
				smartMove(unit, posNew);
				break;
			}

			case Actions::Attack:
			{
				// get target
				BWAPI::Unit target = BWAPI::Broodwar->getUnit(action.getTargetID());

				// smart attack
				smartAttack(unit, target);

				break;
			}

			default:
				BWAPI::Broodwar << "[ERROR] Invalid action!" << std::endl;
				break;
		}
	}
}

void CombatManager::smartAttack(const BWAPI::Unit &unit, const BWAPI::Unit &target) const
{
	// check valid
	if(!unit || !target)
		return;

	// still attacking
	if(unit->isAttackFrame())
		return;

	// has already been told to attack that target
	BWAPI::UnitCommand currentCommand(unit->getLastCommand());
	if(currentCommand.getType() == BWAPI::UnitCommandTypes::Attack_Unit
		&& currentCommand.getTarget() == target)
		return;

	// attack
	unit->attack(target);

	// for virtual state's cooldown
	// TODO: should we do the same thing for movement?
	Unit unitFake = _currentState.getUnit(unit->getID());
	if(unitFake)
		unitFake->attack(NULL, _currentState.getTimeFrame()); // for cooldown
}

void CombatManager::smartMove(const BWAPI::Unit &unit, const BWAPI::Position &pos) const
{
	// check valid
	if(!unit || !pos.isValid())
		return;

	// has already been told to move to there
	/*
	BWAPI::UnitCommand currentCommand(unit->getLastCommand());
	if(currentCommand.getType() == BWAPI::UnitCommandTypes::Move
		&& currentCommand.getTargetPosition() == pos)
		return;
	*/

	// adjust destination to avoid collision
	BWAPI::Position posAdjusted = pos;
	/*
	Unit fakeUnit = _currentState.getUnit(unit->getID());
	if(fakeUnit)
	{
		int rad = fakeUnit->getRadius();
		BWAPI::Unitset unitsIn = BWAPI::Broodwar->getUnitsInRadius(pos, rad);

		// position already occupied
		if(unitsIn.size() > 0)
		{
			BWAPI::Position vec = pos - unit->getPosition();
			posAdjusted = unit->getPosition() + vec + vec;
		}
	}
	*/

	// move
	unit->move(posAdjusted);
}