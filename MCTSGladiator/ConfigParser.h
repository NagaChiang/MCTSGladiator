#pragma once

#include <BWAPI.h>
#include <string>
#include "UCTSearch.h"

namespace MCTSG
{
	class ConfigParser
	{

		// singleton
		static ConfigParser* _ptrInstance;
		static std::string _pathCfg;

		// config file
		FILE *_fptrCfg;

		// UCT params
		UCTSearchParams _params;

	public:

		ConfigParser();
		~ConfigParser();

		static ConfigParser* instance();

		UCTSearchParams getUCTParams() const { return _params; };

	private:

		void init(const std::string &path);
		void parseIn();
	};
}