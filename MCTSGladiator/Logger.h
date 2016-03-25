#pragma once
#include <time.h>
#include <string>
#include "State.h"

namespace MCTSG
{
	class Logger
	{

	public:

		Logger();
		Logger(const std::string &path);
		~Logger();

		void init(const std::string &path);

		void log(const std::string &str); // std::string
		void log(const State &state); // state

	private:

		FILE *fptrLog;
		std::string logPath;

		void timestamp();
	};
}