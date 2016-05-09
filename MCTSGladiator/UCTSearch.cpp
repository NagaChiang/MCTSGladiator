	#include "UCTSearch.h"

using namespace MCTSG;
using namespace std::chrono;

UCTSearch::UCTSearch()
{
	// default
	_params.timeLimit = std::chrono::milliseconds(50);
	_params.maxChildren = 20;
	_params.explorationConst = 1.6;
	_params.evalMethod = EvaluationMethod::MostVisited;

	// reset
	reset();
}

UCTSearch::UCTSearch(const UCTSearchParams &UCTparams)
	:_params(UCTparams)
{
	// reset
	reset();
}

void UCTSearch::reset()
{
	// bestMove
	_bestMove = Move();

	// statistics
	_traversals = 0;
	_numNodeVisited = 0;
	_numNodeCreated = 0;
}

Move UCTSearch::search(const State &state)
{
	// reset statistics
	reset();

	// get start timestamp
	steady_clock::time_point startTime = steady_clock::now();

	// create a new node to traverse
	UCTNode root = UCTNode();

	// traverse until time out
	while(true)
	{
		// traverse
		State clone = State(state);
		traverse(root, clone);

		// check time duration
		steady_clock::time_point curTime = steady_clock::now();
		milliseconds duration = duration_cast<milliseconds>(curTime - startTime);
		if(duration.count() >= _params.timeLimit.count())
		{
			break; // time out!
		}
	}

	// return the best move (actions) of root
	double bestScore = -99999.0; // -Inf
	UCTNode *bestNode = NULL;
	std::vector<UCTNode*> children = root.getChildren();
	for(UCTNode *node : children)
	{
		// get score
		double score;
		if(_params.evalMethod == EvaluationMethod::WinRate)
			score = node->getWinRate();
		else if(_params.evalMethod == EvaluationMethod::MostVisited)
			score = node->getNumVisits();
		else // evaluation
			score = node->getScore();

		// compare to current best
		if(score > bestScore)
		{
			bestScore = score;
			bestNode = node;
		}
	}

	if(bestNode)
	{
		// keep the best Move
		_bestMove = bestNode->getMove();

		return bestNode->getMove();
	}
	else
		return Move();
}

double UCTSearch::traverse(UCTNode &node, State &state)
{
	_traversals++; // statistics

	double result = 0; // win = 1, lose = 0

	if(node.getNumVisits() == 0) // first time?
	{
		updateState(node, state, true); // leaf
		
		// only evaluate when needed
		if(!(_params.evalMethod == EvaluationMethod::WinRate
			|| _params.evalMethod == EvaluationMethod::MostVisited))
			result = evaluateState(state);

		_numNodeVisited++; // statistics
	}
	else
	{
		updateState(node, state, false);

		if(state.isEnd()) // end?
		{
			if(_params.evalMethod == EvaluationMethod::WinRate
				|| _params.evalMethod == EvaluationMethod::MostVisited)
				result = state.isWin() ? 1 : 0;
			else // evaluation
				result = evaluateState(state);
		}
		else // not end yet, expand nodes
		{
			if(!node.hasChildren())
				generateChildren(node, state);

			result = traverse(*selectNode(node), state);
		}
	}

	// result
	node.visit();
	if(_params.evalMethod == EvaluationMethod::WinRate
		|| _params.evalMethod == EvaluationMethod::MostVisited)
		node.updateWin((int)result);
	else // evaluation
		node.updateScore(result);

	return result;
}

void UCTSearch::generateChildren(UCTNode &node, const State &state)
{
	// decide the children nodes type (FIRST/SECOND/SOLO)
	UCTNodeTypes nodeTypeChild;
	UCTNodeTypes nodeType = node.getNodeType();

	bool isAllyMove; // who to move
	if(nodeType == UCTNodeTypes::FIRST)
	{
		nodeTypeChild = UCTNodeTypes::SECOND;
		isAllyMove = false;
	}
	else
	{
		// find out who can do
		int timeFrame = state.getTimeFrame();

		bool canAllyDo = false;
		bool canEnemyDo = false;

		Unitset unitset = state.getUnits();
		for(auto &itr : unitset)
		{
			Unit unit = itr.second;
			if(unit->canAttackAt(timeFrame) || unit->canMoveAt(timeFrame))
			{
				if(unit->isAlly())
					canAllyDo = true;
				else // enemy
					canEnemyDo = true;

				// already know both can do something
				if(canAllyDo && canEnemyDo)
					break;
			}
		}

		// decide
		if(canAllyDo && canEnemyDo) // FIRST
		{
			nodeTypeChild = UCTNodeTypes::FIRST;
			isAllyMove = true;
		}
		else if(canAllyDo) // SOLO, ally
		{
			nodeTypeChild = UCTNodeTypes::SOLO;
			isAllyMove = true;
		}
		else if(canEnemyDo) // SOLO, enemy
		{
			nodeTypeChild = UCTNodeTypes::SOLO;
			isAllyMove = false;
		}
	}

	// generate moves
	std::vector<Move> vecMoves = state.generateNextMoves(_params.maxChildren, isAllyMove);

	// add children to the node
	for(Move move : vecMoves)
	{
		UCTNode *child = new UCTNode(nodeTypeChild, move);
		node.addChild(child);

		// statistics
		_numNodeCreated++;
	}
}

UCTNode* UCTSearch::selectNode(const UCTNode &node) const
{
	UCTNode *bestNode = NULL;
	double bestScore = -99999.0; // -Inf
	std::vector<UCTNode*> children = node.getChildren();
	for(UCTNode *child : children)
	{
		// UCT
		if(child->getNumVisits() <= 0)
			return child;
		else
		{
			double score;

			if(_params.evalMethod == EvaluationMethod::WinRate
				|| _params.evalMethod == EvaluationMethod::MostVisited)
				score = (double)child->getNumWins() / child->getNumVisits()
					+ _params.explorationConst * sqrt(log((double)node.getNumVisits()) / child->getNumVisits());
			else // evaluation
				score = (double)child->getScore() / child->getNumVisits()
				+ _params.explorationConst * sqrt(log((double)node.getNumVisits()) / child->getNumVisits());

			// better one
			if(score > bestScore)
			{
				bestScore = score;
				bestNode = child;
			}
		}
	}

	return bestNode;
}

// only update when second/solo/leaf node appears
void UCTSearch::updateState(const UCTNode &node, State &state, bool isLeaf) const
{
	// make moves
	UCTNodeTypes nodeType = node.getNodeType();
	if(nodeType != UCTNodeTypes::FIRST || isLeaf)
	{
		// make move for its parent (FIRST) first
		if(nodeType == UCTNodeTypes::SECOND)
			state.makeMove(node.getParent()->getMove());

		state.makeMove(node.getMove());
	}

	// erase dead units from the unitset
	state.eraseDeadUnits();

	// update time frame
	int minTimeFrame = state.getNextMinFrame();
	state.setTimeFrame(minTimeFrame);
}

double UCTSearch::evaluateState(const State &state) const
{
	double score = 0.0;
	double allyTotal = 0;
	double enemyTotal = 0;
	Unitset allUnits = state.getUnits();

	switch(_params.evalMethod)
	{
		case EvaluationMethod::EvalHP:

			for(auto &itr : allUnits)
			{
				Unit unit = itr.second;
				double hp = unit->getHitPoints() + unit->getShields();

				if(unit->isAlly())
					allyTotal += hp;
				else // enemy
					enemyTotal += hp;
			}

			score = allyTotal - enemyTotal;
			break;

		case EvaluationMethod::EvalLTD:

			for(auto &itr : allUnits)
			{
				Unit unit = itr.second;
				double hp = unit->getHitPoints() + unit->getShields();
				double dpf = unit->getGroundWeaponDPF();

				if(unit->isAlly())
					allyTotal += hp * dpf;
				else // enemy
					enemyTotal += hp * dpf;
			}

			score = allyTotal - enemyTotal;
			break;

		case EvaluationMethod::EvalLTD2:

			for(auto &itr : allUnits)
			{
				Unit unit = itr.second;
				double hp = unit->getHitPoints() + unit->getShields();
				double dpf = unit->getGroundWeaponDPF();

				if(unit->isAlly())
				{
					if(hp > 0)
						allyTotal += sqrt(hp) * dpf;
				}
				else // enemy
				{
					if(hp > 0)
						enemyTotal += sqrt(hp) * dpf;
				}
			}

			score = allyTotal - enemyTotal;
			break;

		default:
			break;
	}

	return score;
}