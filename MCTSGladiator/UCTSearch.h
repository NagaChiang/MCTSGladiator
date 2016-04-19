#pragma once
#include <BWAPI.h>
#include <chrono>
#include "UCTNode.h"
#include "State.h"
#include "Action.h"
#include "Logger.h"

namespace MCTSG
{
	struct UCTSearchParams
	{
		std::chrono::milliseconds timeLimit; // time limit for searching
		int maxChildren; // max number of children of a node
		double explorationConst; // exploration constant
	};

	class UCTSearch
	{

		// params
		UCTSearchParams params;

		// log
		std::string logPath;
		Logger logger;

	public:

		UCTSearch();
		UCTSearch(const UCTSearchParams &UCTparams);

		Move search(const State &state) const; // return the UCT search result of the state

	private:

		// UCT search
		int traverse(const UCTNode &node, const State &state) const; // traverse from state with node (i.e. move, actions)
		UCTNode selectNode(const UCTNode &node) const; // select the best child of the node
		void updateState(const UCTNode &node, State &state, bool isLeaf) const; // update state with moves of both players
	};
}