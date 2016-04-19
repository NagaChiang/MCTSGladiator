#pragma once
#include <BWAPI.h>
#include "State.h"
#include "Action.h"

namespace MCTSG
{
	class UCTNode
	{

		Move move; // action vector
		int visits, wins; // number of visits and wins
		UCTNode *parent; // parent node

	public:

		UCTNode();

		// getters
		Move getMove() const { return move; };
		int getNumVisits() const { return visits; };
		int getNumWins() const { return wins; };
		UCTNode* getParent() const { return parent; };
	};
}