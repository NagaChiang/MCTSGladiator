#include "State.h"

using namespace MCTSG;

State::State()
{
	clear();
}

State::State(const int t, const BWAPI::Unitset &units)
{
	set(t, units);
}

// copy constructor
State::State(const State &other)
{
	_time = other._time;
	_allUnits = other._allUnits.deepCopy();
}

State::~State()
{
	clear();
}

void State::clear()
{
	_time = 0;
	_allUnits.clear();
}

void State::set(const int t, const BWAPI::Unitset &units)
{
	// reset
	clear();

	// time frame
	_time = t;

	// convert BWAPI units to MCTSG units
	for(auto &u : units)
	{
		if(u->exists()
			&& !u->getType().isWorker()
			&& !(u->getType() == BWAPI::UnitTypes::Special_Map_Revealer))
		{
			// convert and append
			Unit unit = Unit(new UnitInterface(u));
			_allUnits.addUnit(unit);
		}
	}
}

void State::update(const int t, const BWAPI::Unitset &units)
{
	// new time frame
	_time = t;

	// update units
	for(Unitset::iterator itr = _allUnits.begin(); itr != _allUnits.end(); itr++)
	{
		// search real units
		bool isFound = false;
		for(auto &u : units)
		{
			if(u->getID() == itr->second->getID())
			{
				itr->second->update(u); // update MCTSG unit
				isFound = true;
			}
		}

		if(!isFound) // not found -> dead in real game
		{
			_allUnits.erase(itr->second->getID()); // erase the unit
		}
	}
}

void State::makeMove(const Move &move)
{
	// make actions in the move
	for(const Action &action : move)
	{
		doAction(action);
	}
}

void State::doAction(const Action &action)
{
	Unit unit = getUnit(action.getUnitID());
	if(unit)
	{
		BWAPI::Position position = unit->getPosition();
		int speed = unit->getSpeed();
		switch(action.getType())
		{
			case Actions::Stop:
			{
				int endFrame = action.getEndFrame();
				unit->stop(endFrame);

				break;
			}

			case Actions::Attack:
			{
				Unit target = getUnit(action.getTargetID());
				unit->attack(target, _time);

				break;
			}

			case Actions::North:
			{
				BWAPI::Position dirNorth = BWAPI::Position(0, -1);
				BWAPI::Position posNew = position + (dirNorth * speed * UnitData::MOVE_DURATION);
				unit->move(posNew, _time);
				break;
			}

			case Actions::East:
			{
				BWAPI::Position dirEast = BWAPI::Position(1, 0);
				BWAPI::Position posNew = position + (dirEast * speed * UnitData::MOVE_DURATION);
				unit->move(posNew, _time);
				break;
			}

			case Actions::West:
			{
				BWAPI::Position dirWest = BWAPI::Position(-1, 0);
				BWAPI::Position posNew = position + (dirWest * speed * UnitData::MOVE_DURATION);
				unit->move(posNew, _time);
				break;
			}

			case Actions::South:
			{
				BWAPI::Position dirSouth = BWAPI::Position(0, 1);
				BWAPI::Position posNew = position + (dirSouth * speed * UnitData::MOVE_DURATION);
				unit->move(posNew, _time);
				break;
			}

			default:
				// undefined action type
				BWAPI::Broodwar << "ERROR: Undefined action type." << std::endl;
				break;
		}
	}
}

// erase dead units from the unitset
void State::eraseDeadUnits()
{
	for(auto &itr : _allUnits)
	{
		Unit unit = itr.second;

		if(!unit->isAlive())
			_allUnits.erase(unit->getID());
	}
}

std::vector<Move> State::generateNextMoves(const int amount, const bool forAlly) const
{
	// get NOKAV first
	std::vector<Move> moves;
	Move moveNOKAV = generateNOKAVMove(forAlly);
	moves.push_back(moveNOKAV);
	
	// generate the rest of moves
	if(forAlly) // do not generate other moves for enemy
	{
		// generate kiter move
		if(amount > 1)
		{
			Move moveKiter = Move(moveNOKAV);
			bool isChanged = false;
			for(Action &action : moveKiter)
			{
				// replace stop with kite
				if(action.getType() == Actions::Stop)
				{
					Unit unit = getUnit(action.getUnitID());
					Unit target = getUnit(action.getTargetID());
					Actions typeKite = getActionTypeFromToInv(unit->getPosition(), target->getPosition());
					action.setType(typeKite);

					isChanged = true;
				}
			}

			// push
			if(isChanged)
				moves.push_back(moveKiter);
		}

		// replace move/stop in NOKAV with random move
		// find all index of these types
		std::vector<int> indexStopNMove;
		for(unsigned int i = 0; i < moveNOKAV.size(); i++)
		{
			Action action = moveNOKAV.at(i);
			Actions type = action.getType();

			if(type == Actions::Stop
				|| type == Actions::North || type == Actions::East
				|| type == Actions::West || type == Actions::South)
			{
				indexStopNMove.push_back(i);
			}
		}

		if(indexStopNMove.size() > 0)
		{
			// prepare random generator
			std::default_random_engine generator;
			std::uniform_int_distribution<int> distributionIndex(0, indexStopNMove.size() - 1); // index
			std::uniform_int_distribution<int> distributionDir(2, 5); // direction

			// generate the rest of moves
			while(moves.size() < (unsigned int)amount)
			{
				Move cloneMove = Move(moveNOKAV);

				// pick action
				int index = distributionIndex(generator);
				Action &action = cloneMove.at(indexStopNMove.at(index));

				// replace it with random direction
				Actions typeNew = Actions::None;
				do
				{
					typeNew = (Actions)distributionDir(generator);
				} while(typeNew == action.getType()); // TODO: check all moves?

				// assign and push
				action.setType(typeNew);
				moves.push_back(cloneMove);
			}
		}
	}

	return moves;
}

// generate Move with NOK-AV (No-OverKill-Attack-Value)
Move State::generateNOKAVMove(const bool forAlly) const
{
	Move moveNOKAV;

	// get units
	Unitset units, targets;
	if(forAlly) // control ally
	{
		units = getAllyUnits();
		targets = getEnemyUnits();
	}
	else // control enemy
	{
		units = getEnemyUnits();
		targets = getAllyUnits();
	}

	// construct Actions for every unit
	Unitset unitsClone = units.deepCopy();
	Unitset targetsClone = targets.deepCopy();
	for(auto &itrUnits : unitsClone)
	{
		Unit unit = itrUnits.second;
		Unit bestTarget = NULL; // target to attack
		Actions actionType = Actions::None; // type of the action of this unit
		int endFrame = 0; // for stop

		// find targets in range
		Unitset targetsInRange;
		for(auto &itrTarget : targetsClone)
		{
			Unit target = itrTarget.second;

			// any alive unit in range?
			if(unit->isTargetInRange(target) && target->isAlive())
				targetsInRange.addUnit(target);
		}

		// find the most valued target in range (alive; no-overkill)
		double bestValue = 0;
		for(auto &itrTarget : targetsInRange)
		{
			Unit target = itrTarget.second;
			int targetHP = target->getHitPoints();
			int targetShield = target->getShields();
			double value = (double)target->getGroundWeaponDPF() / (targetHP + targetShield);

			if(value > bestValue)
			{
				bestValue = value;
				bestTarget = target;
			}
		}

		// assign Action type
		if(bestTarget) // has best target in range
		{
			// debug
			//BWAPI::Broodwar << unit->getNextCanAttackFrame() << "/" << _time << std::endl;

			if(unit->canAttackAt(_time)) // can attack
			{
				// attack
				actionType = Actions::Attack;
				unit->attack(bestTarget, _time); // attack in clone unitset to record distribution
			}
			else // still cooldown
			{
				// stop until cooldown finished
				actionType = Actions::Stop;
				endFrame = unit->getNextCanAttackFrame();
			}
		}
		else // no target in range
		{
			BWAPI::Position pos = unit->getPosition();

			// find closest target
			Unit unitClosest = NULL;
			int distMin = 999999;
			for(auto &itrTarget : targetsClone)
			{
				Unit target = itrTarget.second;
				BWAPI::Position posTarget = target->getPosition();
				int dist = pos.getApproxDistance(posTarget);
				if(dist < distMin)
				{
					distMin = dist;
					unitClosest = target;
				}
			}

			// select a move toward the closest target
			if(unitClosest)
				actionType = getActionTypeFromTo(pos, unitClosest->getPosition());
		}

		// construct an Action for this Unit
		Action action;
		if(bestTarget)
		{
			action = Action(unit->getID(), actionType, bestTarget->getID(), endFrame);
		}
		else
		{
			action = Action(unit->getID(), actionType, -1, endFrame);
		}
		
		moveNOKAV.push_back(action);
	}

	return moveNOKAV;
}

// get the actions type to that position (one of NEWS)
Actions State::getActionTypeFromTo(const BWAPI::Position &from, const BWAPI::Position &to) const
{
	BWAPI::Position direction = to - from;
	Actions actionType;

	int x = direction.x;
	int y = direction.y;
	if(abs(x) >= abs(y)) // magnitude of x is bigger
	{
		if(x >= 0)
			actionType = Actions::East;
		else // x < 0
			actionType = Actions::West;
	}
	else // magnitude of y is bigger
	{
		if(y >= 0)
			actionType = Actions::North;
		else // y < 0
			actionType = Actions::South;
	}

	return actionType;
}

Actions State::getActionTypeFromToInv(const BWAPI::Position &from, const BWAPI::Position &to) const
{
	BWAPI::Position direction = to - from;
	Actions actionType;

	int x = direction.x;
	int y = direction.y;
	if(abs(x) >= abs(y)) // magnitude of x is bigger
	{
		if(x >= 0)
			actionType = Actions::West;
		else // x < 0
			actionType = Actions::East;
	}
	else // magnitude of y is bigger
	{
		if(y >= 0)
			actionType = Actions::South;
		else // y < 0
			actionType = Actions::North;
	}

	return actionType;
}

// is game over? (in combat scenario)
bool State::isEnd() const
{
	if(getAllyUnitsNum() == 0 || getEnemyUnitsNum() == 0)
		return true;
	else
		return false;
}

// did ally win?
bool State::isWin() const
{
	if(isEnd() && getAllyUnitsNum() > 0)
		return true;
	else
		return false;
}

int State::getNextMinFrame() const
{
	// get next interesting time frame (find the min)
	int minTimeFrame = 9999999;
	for(auto &itr : _allUnits)
	{
		Unit unit = itr.second;
		int canAttackFrame = unit->getNextCanAttackFrame();
		int canMoveFrame = unit->getNextCanMoveFrame();

		if(canAttackFrame < minTimeFrame
			&& canAttackFrame >= _time)
			minTimeFrame = canAttackFrame;
		if(canMoveFrame < minTimeFrame
			&& canMoveFrame >= _time)
			minTimeFrame = canMoveFrame;
	}

	return minTimeFrame;
}

// get unit by ID; return null if not found
Unit State::getUnit(const int ID) const
{
	Unit unit = NULL;

	try
	{
		unit = _allUnits.at(ID);
	}
	catch(const std::out_of_range e)
	{
		// maybe log?
		//BWAPI::Broodwar << "ERROR: Unit ID not found!" << " (" << ID << ")" << std::endl;
	}

	return unit;
}

int State::getAllyUnitsNum() const
{
	int num = 0;
	for(const auto &itr : _allUnits)
	{
		if(itr.second->isAlly())
			num++;
	}

	return num;
}

int State::getEnemyUnitsNum() const
{
	int num = 0;
	for(const auto &itr : _allUnits)
	{
		if(!itr.second->isAlly())
			num++;
	}

	return num;
}

Unitset State::getAllyUnits() const
{
	Unitset unitset; // empty unitset
	for(Unitset::const_iterator itr = _allUnits.begin(); itr != _allUnits.end(); itr++)
	{
		if(itr->second->isAlly())
			unitset.insert(Unitset::value_type(itr->first, itr->second)); // add
	}

	return unitset;
}

Unitset State::getEnemyUnits() const
{
	Unitset unitset; // empty unitset
	for(Unitset::const_iterator itr = _allUnits.begin(); itr != _allUnits.end(); itr++)
	{
		if(!itr->second->isAlly())
			unitset.insert(Unitset::value_type(itr->first, itr->second)); // add
	}

	return unitset;
}