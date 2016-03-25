#pragma once
#include <BWAPI.h>
#include "State.h"

namespace MCTSG
{
	class UCTSearch
	{

		State rootState;

	public:

		UCTSearch();
		UCTSearch(const State &state);

		void setRootState(const State &state);
	};
}