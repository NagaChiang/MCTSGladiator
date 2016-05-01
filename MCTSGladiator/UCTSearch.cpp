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

	// traverse until time out
	while(true)
	{
		// traverse
		traverse(root, State(state));

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
	//if(bestNode)
		//Logger::instance()->log(bestNode->getWinRate());

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

	if(node.getNumVisits() == 0)
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
		}
		else
		{
			if(!node.hasChildren())
				generateChildren(node, state);

			result = traverse(*selectNode(node), state);
		}
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
		int nextMinFrame = state.getNextMinFrame();

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

	// statistics
	_numNodeCreated += isAllyMove ? _params.maxChildren : 1;

	// add children to the node
	for(Move move : vecMoves)
	{
		UCTNode *child = new UCTNode(nodeType, move);
		node.addChild(child);
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
}