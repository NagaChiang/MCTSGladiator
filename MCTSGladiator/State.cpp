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
	// get next interesting time frame (find the min)
	int minTimeFrame = getNextMinFrame();

	// make actions in the move
	for(const Action &action : move)
	{
		doAction(action);
	}

	// update frame time of this state
	_time = minTimeFrame;
}

void State::doAction(const Action &action)
{
	Unit unit = action.getUnit();
	BWAPI::Position position = unit->getPosition();
	int speed = unit->getSpeed();
	switch(action.getType())
	{
		case Actions::Stop:
			// stop/wait at the same position
			break;

		case Actions::Attack:
		{
			Unit target = action.getTarget();
			if(target)
				unit->attack(action.getTarget(), _time);

			break;
		}

		case Actions::North:
		{
			BWAPI::Position dirNorth = BWAPI::Position(0, 1);
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
			BWAPI::Position dirSouth = BWAPI::Position(0, -1);
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

	if(forAlly) // do not generate other moves for enemy
	{
		// generate the rest of moves
		bool hasAttack = true; // still has Move containing Actions::Attack
		bool hasMove = true; // still has Move containing Actions::Move
		Move lastMove = moveNOKAV; // keep its values
		for(int i = 0; i < amount - 1; i++)
		{
			// copy Move from last one
			Move move = Move(lastMove);

			// prepare random generator
			std::default_random_engine generator;
			std::uniform_int_distribution<int> distributionDir(1, 4); // for random direction

			// generate new Move with replacing
			if(hasAttack) // replace Attack with Move
			{
				// get actions of Actions::Attack
				Move moveAttack;
				for(Action &action : move)
				{
					if(action.getType() == Actions::Attack)
						moveAttack.push_back(action);
				}

				if(moveAttack.size() > 0)
				{
					// random select one
					std::uniform_int_distribution<int> distributionAtk(0, moveAttack.size() - 1);
					int index = distributionAtk(generator);

					// replace
					Action &action = moveAttack.at(index);
					int randomDir = distributionDir(generator);
					action.setType((Actions)randomDir);
				}
				else // no Attack anymore
					hasAttack = false;
			}

			if(!hasAttack && hasMove) // replace Move with Stop
			{
				// get actions of Actions::Move
				Move moveMove;
				for(Action &action : move)
				{
					Actions type = action.getType();
					if(type >= 1 && type <= 4)
						moveMove.push_back(action);
				}

				if(moveMove.size() > 0)
				{
					// random select one
					std::uniform_int_distribution<int> distributionMove(0, moveMove.size() - 1);
					int index = distributionMove(generator);

					// replace
					Action &action = moveMove.at(index);
					action.setType(Actions::Stop);
				}
				else // no Attack anymore
					hasMove = false;
			}

			if(!hasAttack && !hasMove) // only Actions::Stop remains
				break;

			// ready to be passed back
			moves.push_back(move);

			// keep this move's values for next
			lastMove = move;
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

		// find targets in range
		Unitset targetsInRange;
		for(auto &itrTarget : targetsClone)
		{
			Unit target = itrTarget.second;

			// any alive unit in range?
			if(unit->isTargetInRange(target) && target->isAlive())
				targetsInRange.addUnit(target);
		}

		// find the most valued target (alive; no-overkill)
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
		if(targetsInRange.size() > 0) // has targets in range
		{
			if(unit->canAttackAt(_time)) // can attack
			{
				// attack
				actionType = Actions::Attack;
				unit->attack(bestTarget, _time);
			}
			else // still cooldown
			{
				// stop
				actionType = Actions::Stop;
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
		Unit realUnit = units.getUnit(unit->getID());

		if(bestTarget)
		{
			Unit realTarget = targets.getUnit(bestTarget->getID());
			action = Action(realUnit, actionType, realTarget, 0);
		}
		else
		{
			action = Action(realUnit, actionType, NULL, 0);
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
		if(canAttackFrame < minTimeFrame)
			minTimeFrame = canAttackFrame;
		if(canMoveFrame < minTimeFrame)
			minTimeFrame = canMoveFrame;
	}

	// before current time frame
	if(minTimeFrame < _time)
		minTimeFrame = _time;

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

// debug
void State::kill()
{
	_allUnits[18]->killSelf();
}