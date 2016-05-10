#pragma once

#include <BWAPI.h>
#include <string>

namespace MCTSG
{
	class GameCounter
	{
		// singleton
		static GameCounter* _ptrInstance;
		static std::string _pathCtr;

		// config file
		FILE *_fptrCtr;

		int _gameCount;

	public:

		GameCounter();
		~GameCounter();

		static GameCounter* instance();

		void count() { _gameCount++; };
		void save();
		void end();

		// getters
		int getGameCount() const { return _gameCount; };

	private:

		void init(const std::string &path);
		void readIn();
	};
}
