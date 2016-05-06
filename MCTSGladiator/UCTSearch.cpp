	#include "UCTSearch.h"

using namespace MCTSG;
using namespace std::chrono;

UCTSearch::UCTSearch()
{
	// default
	_params.timeLimit = std::chrono::milliseconds(40);
	_params.maxChildren = 20;
	_params.explorationConst = 1.6;

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

	// debug
	/*
	State clone1 = State(state);
	State clone2 = State(state);
	Action action1 = Action(clone1.getUnit(1), Actions::Attack, clone1.getUnit(3), 0);
	//Action action2 = Action(clone2.getUnit(0), Actions::West, NULL, 0);
	Move move1, move2;
	move1.push_back(action1);
	//move2.push_back(action2);
	UCTNode node1 = UCTNode(UCTNodeTypes::SOLO, move1);
	//UCTNode node2 = UCTNode(UCTNodeTypes::SOLO, move2);
	updateState(node1, clone1, false);
	//updateState(node2, clone2, false);
	
	Logger::instance()->log(state);
	Logger::instance()->log(clone1);
	//Logger::instance()->log(clone1);
	Logger::instance()->log("------");
	*/
	

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
	double bestWinRate = -9999.0; // -Inf
	UCTNode *bestNode = NULL;
	std::vector<UCTNode*> children = root.getChildren();
	for(UCTNode *node : children)
	{
		double winRate = node->getWinRate();

		if(winRate > bestWinRate)
		{
			bestWinRate = winRate;
			bestNode = node;
		}
	}

	// debug
	if(bestNode && false)
	{
		Logger::instance()->log(bestNode->getNumVisits());
		Logger::instance()->log(bestNode->getNumWins());
		Logger::instance()->log("------");
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

int UCTSearch::traverse(UCTNode &node, State &state)
{
	_traversals++; // statistics

	int result = 0; // win = 1, lose = 0

	if(node.getNumVisits() == -1)
	{
		updateState(node, state, true); // leaf
		// score <- s.eval()

		_numNodeVisited++; // statistics
	}
	else
	{
		updateState(node, state, false);

		if(state.isEnd())
		{
			result = state.isWin() ? 1 : 0;

			// debug
			if(result == 1 && false)
			{
				Logger::instance()->log(state);
				Logger::instance()->log("------------");
			}
		}
		else
		{
			if(!node.hasChildren())
				generateChildren(node, state);

			result = traverse(*selectNode(node), state);
		}
	}

	// debug
	if(result == 1 && false)
	{
		Logger::instance()->log(state);
		Logger::instance()->log("------------");

		/*Logger::instance()->log(node.getNumWins());
		Logger::instance()->log(node.getNumVisits());*/
	}

	node.visit();
	node.updateWins(result);
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
		if(child->getNumVisits() == 0)
			return child;
		else
		{
			double score = (double)child->getNumWins() / child->getNumVisits()
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
	// debug
	State origin = State(state);

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

	// debug
	if(false)
	{
		Logger::instance()->log(node.toString());
		if(node.getParent())
			Logger::instance()->log(node.getParent()->getMove());
		Logger::instance()->log(node.getMove());
		Logger::instance()->log(origin);
		Logger::instance()->log(state);
		Logger::instance()->log("---------");
	}
}