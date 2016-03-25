#include "UCTSearch.h"

using namespace MCTSG;

UCTSearch::UCTSearch()
{
	rootState = State();
}

UCTSearch::UCTSearch(const State &state)
{
	rootState = State(state);
}

void UCTSearch::setRootState(const State &state)
{
	rootState = State(state);
}