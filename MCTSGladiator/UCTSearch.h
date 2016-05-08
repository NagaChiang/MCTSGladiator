#pragma once
#include <BWAPI.h>
#include <chrono>
#include <math.h>
#include "UCTNode.h"
#include "State.h"
#include "Unit.h"
#include "Action.h"
#include "Logger.h"

namespace MCTSG
{
	enum EvaluationMethod
	{
		MostVisited,
		WinRate,
		EvalHP, 
		EvalLTD, // Life-Time Damage, HP * dpf
		EvalLTD2 // sqrt(HP) * dpf
	};

	struct UCTSearchParams
	{
		std::chrono::milliseconds timeLimit; // time limit for searching
		int maxChildren; // max number of children of a node
		double explorationConst; // exploration constant
		EvaluationMethod evalMethod; // evaluation
	};

	class UCTSearch
	{

		// params
		UCTSearchParams _params;

		// Move
		Move _bestMove;

		// statistics
		int _traversals;
		int _numNodeVisited;
		int _numNodeCreated;

	public:

		UCTSearch();
		UCTSearch(const UCTSearchParams &UCTparams);

		Move search(const State &state); // return the UCT search result of the state

		// getters
		Move getBestMove() const { return _bestMove; };
		int getTraversals() const { return _traversals; };
		int getNumNodeVisited() const { return _numNodeVisited; };
		int getNumNodeCreated() const { return _numNodeCreated; };

	private:

		void reset();

		// UCT search
		double traverse(UCTNode &node, State &state); // traverse from state with node (i.e. move, actions)
		void generateChildren(UCTNode &node, const State &state); // generate new nodes from the node/state
		UCTNode* selectNode(const UCTNode &node) const; // select the best child of the node
		void updateState(const UCTNode &node, State &state, bool isLeaf) const; // update state with moves of both players
		double evaluateState(const State &state) const; // evaluate the state
	};
}