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

	// traverse until time is up
	while(true)
	{

		// check time duration
		steady_clock::time_point curTime = steady_clock::now();
		milliseconds duration = duration_cast<milliseconds>(curTime - startTime);
		if(duration.count() >= params.timeLimit.count())
		{
			break;
		}
	}

	// return the best move (actions) of root
	return Move();
}

int UCTSearch::traverse(const State &state, const UCTNode &node) const
{

}

UCTNode UCTSearch::selectNode(const UCTNode &node) const
{

}