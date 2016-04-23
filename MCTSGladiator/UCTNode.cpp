#include "UCTNode.h"

using namespace MCTSG;

UCTNode::UCTNode()
{
	// to be the root
	move.clear();
	type = UCTNodeTypes::FIRST;
	visits = 0;
	wins = 0;
	parent = NULL;
}