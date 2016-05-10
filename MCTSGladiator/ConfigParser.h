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

		// configs
		UCTSearchParams _params; // UCT params
		int _numGames; // number of games to run
		bool _isLogWinMode;
		bool _isNoGUIMode;

	public:

		ConfigParser();
		~ConfigParser();

		static ConfigParser* instance();

		// getters
		UCTSearchParams getUCTParams() const { return _params; };
		int getNumGames() const { return _numGames; };
		bool isLogWinMode() const { return _isLogWinMode; };
		bool isNoGUIMode() const { return _isNoGUIMode; };

	private:

		void init(const std::string &path);
		void parseIn();
	};
}