#pragma once
#include <time.h>
#include <string>
#include "UCTNode.h"
#include "State.h"
#include "Action.h"
#include "Unit.h"

namespace MCTSG
{
	class Logger
	{

		// singleton
		static Logger* _ptrInstance;

		FILE *_fptrLog;
		std::string _logPath;

	public:

		Logger();
		~Logger();

		static Logger* instance();

		void log(const std::string &str) const; // std::string
		void log(const State &state) const; // state
		void log(const Unit &unit) const; // Unit
		void log(const Move &Move) const; // Move
		void log(const int num) const; // integer
		void log(const long long num) const; // big integer
		void log(const double num) const; // double

		void logChildrenMoves(const UCTNode &node) const;

	private:

		void init(const std::string &path);
		void timestamp() const;

		// support function
		void logUnits(const Unitset &units) const;
	};
}