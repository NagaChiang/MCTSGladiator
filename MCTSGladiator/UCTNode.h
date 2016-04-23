#pragma once
#include <BWAPI.h>
#include <vector>
#include "State.h"
#include "Action.h"

namespace MCTSG
{
	// the type of the node in sequence
	enum UCTNodeTypes
	{
		FIRST, // wait until second node update
		SECOND // only update when second node appears; can also be a solo node
	};

	class UCTNode
	{

		Move move; // action vector; the move which generates this node from parent node
		UCTNodeTypes type; // the type of the node in sequence
		int visits, wins; // number of visits and wins
		std::vector<UCTNode*> vecChildren; // children nodes
		UCTNode *parent; // parent node

	public:

		UCTNode();

		// getters
		Move getMove() const { return move; };
		UCTNodeTypes getNodeType() const { return type; };
		int getNumVisits() const { return visits; };
		int getNumWins() const { return wins; };
		bool hasChildren() const { return vecChildren.empty() ? false : true; };
		std::vector<UCTNode*> getChildren() const { return vecChildren; };
		UCTNode* getParent() const { return parent; };
	};
}