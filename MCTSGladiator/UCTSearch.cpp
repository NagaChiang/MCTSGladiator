#include "UCTSearch.h"

using namespace MCTSG;
using namespace std::chrono;

const std::string LOG_PATH = "D:/Phate/Temp/Thesis/MCTSGladiator/Release/UCTSearchLog.txt";

UCTSearch::UCTSearch()
	:logPath(LOG_PATH)
{
	// params
	params.timeLimit = milliseconds(0);
	params.maxChildren = 0;
	params.explorationConst = 0;

	// log
	logger.init(logPath);
}

UCTSearch::UCTSearch(const UCTSearchParams &UCTparams)
	:logPath(LOG_PATH)
{
	// params
	params.timeLimit = milliseconds(UCTparams.timeLimit);
	params.maxChildren = UCTparams.maxChildren;
	params.explorationConst = UCTparams.explorationConst;

	// log
	logger.init(logPath);
}

Move UCTSearch::search(const State &state) const
{
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
		if(duration.count() >= params.timeLimit.count())
		{
			break; // time out!
		}
	}

	// return the best move (actions) of root
	return Move();
}

int UCTSearch::traverse(const UCTNode &node, const State &state) const
{
	return 0;
}

void UCTSearch::generateChildren(const UCTNode &node, const State &state) const
{

}

UCTNode UCTSearch::selectNode(const UCTNode &node) const
{
	return UCTNode();
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
}