#include "UCTNode.h"

using namespace MCTSG;

UCTNode::UCTNode()
{
	// to be the root
	move.clear();
	visits = 0;
	wins = 0;
	parent = NULL;
}