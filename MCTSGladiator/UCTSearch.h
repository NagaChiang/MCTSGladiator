#pragma once
#include <BWAPI.h>
#include "UCTNode.h"
#include "State.h"
#include "Action.h"

namespace MCTSG
{
	class UCTSearch
	{

		// params
		int timeLimit; // time limit for searching
		int maxChildren; // max number of children of a node
		int explorationConst; // exploration constant

	public:

		UCTSearch();

		Move search(const State &state) const; // return the UCT search result of the state

	private:

		// UCT search
		int traverse(const State &state, const UCTNode &node) const; // traverse from state with node (i.e. move, actions)
		UCTNode selectNode(const UCTNode &node) const; // select the best child of the node
	};
}