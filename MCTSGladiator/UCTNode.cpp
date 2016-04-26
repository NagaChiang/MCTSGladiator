#include "UCTNode.h"

using namespace MCTSG;

UCTNode::UCTNode()
	:_type(UCTNodeTypes::FIRST),
	_visits(0),
	_wins(0),
	_parent(NULL)
{
	// to be the root
	_move.clear();
}