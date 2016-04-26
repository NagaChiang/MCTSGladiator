#pragma once
#include <time.h>
#include <string>
#include "State.h"
#include "Unit.h"

namespace MCTSG
{
	class Logger
	{

	public:

		Logger();
		Logger(const std::string &path);
		~Logger();

		void init(const std::string &path);

		void log(const std::string &str) const; // std::string
		void log(const State &state) const; // state
		void log(const long long num) const; // big integer

	private:

		FILE *_fptrLog;
		std::string _logPath;

		void timestamp() const;

		// support function
		void Logger::logUnits(const Unitset &units) const;
	};
}