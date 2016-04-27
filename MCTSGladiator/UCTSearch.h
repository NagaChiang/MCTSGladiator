#pragma once
#include <BWAPI.h>
#include <chrono>
#include "UCTNode.h"
#include "State.h"
#include "Unit.h"
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
		UCTSearchParams _params;

		// log
		std::string _logPath;
		Logger _logger;

	public:

		UCTSearch(const UCTSearchParams &UCTparams);

		Move search(const State &state) const; // return the UCT search result of the state

	private:

		// UCT search
		int traverse(UCTNode &node, State &state) const; // traverse from state with node (i.e. move, actions)
		void generateChildren(UCTNode &node, const State &state) const; // generate new nodes from the node/state
		UCTNode selectNode(const UCTNode &node) const; // select the best child of the node
		void updateState(const UCTNode &node, State &state, bool isLeaf) const; // update state with moves of both players
	};
}