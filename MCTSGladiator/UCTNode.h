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

		Move _move; // action vector; the move which generates this node from parent node
		UCTNodeTypes _type; // the type of the node in sequence
		int _visits, _wins; // number of visits and wins
		std::vector<UCTNode*> _vecChildren; // children nodes
		UCTNode *_parent; // parent node

	public:

		UCTNode();

		// getters
		Move getMove() const { return _move; };
		UCTNodeTypes getNodeType() const { return _type; };
		int getNumVisits() const { return _visits; };
		int getNumWins() const { return _wins; };
		bool hasChildren() const { return _vecChildren.empty() ? false : true; };
		std::vector<UCTNode*> getChildren() const { return _vecChildren; };
		UCTNode* getParent() const { return _parent; };
	};
}