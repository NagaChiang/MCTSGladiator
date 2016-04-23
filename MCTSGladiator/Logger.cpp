#include "Logger.h"

using namespace MCTSG;

Logger::Logger()
{
	// not initiated
	fptrLog = NULL;
	logPath = "";
}

Logger::Logger(const std::string &path)
{
	// initiate the logger
	init(path);
}

Logger::~Logger()
{
	// close the log
	if(fptrLog)
		fclose(fptrLog);
}

// initiate the log if hasn't been
void Logger::init(const std::string &path)
{
	// if the log hasn't been initiated
	if(!fptrLog)
	{
		logPath = path;
		fptrLog = fopen(path.c_str(), "a");

		// failed to open
		if(!fptrLog)
		{
			BWAPI::Broodwar << "ERROR: Can't initiate the logger." << std::endl;
		}
	}
	else
	{
		// error message
		log("Logger has already been initiated!");
	}
		
}

// log a string with timestamp and newline
void Logger::log(const std::string &str) const
{
	// if not initiated
	if(!fptrLog)
		return;

	// timestamp
	timestamp();

	// log
	std::string strOut = str + "\n";
	fprintf(fptrLog, strOut.c_str());
}

// log a state
void Logger::log(const State &state) const
{
	// if not initiated
	if(!fptrLog)
		return;

	// timestamp
	timestamp();
	fprintf(fptrLog, "\n");
	fprintf(fptrLog, "\t<State>\n");
	fprintf(fptrLog, "\n");
	
	// time frame
	fprintf(fptrLog, "\tTime Frame = %d\n", state.getTimeCount());
	fprintf(fptrLog, "\n");

	// ally units
	fprintf(fptrLog, "\tAlly Units (%d):\n", state.getAllyUnitsNum()); // title
	Unitset allyUnits = state.getAllyUnits();
	logUnits(allyUnits);

	// enemy units
	fprintf(fptrLog, "\tEnemy Units (%d):\n", state.getEnemyUnitsNum()); // title
	Unitset enemyUnits = state.getEnemyUnits();
	logUnits(enemyUnits);
}

// log an integer
void Logger::log(const long long num) const
{
	timestamp();
	fprintf(fptrLog, "%lld\n", num);
}

// support function of log state
void Logger::logUnits(const Unitset &units) const
{
	for(Unitset::const_iterator itr = units.begin(); itr != units.end(); itr++)
	{
		Unit unit = itr->second;
		fprintf(fptrLog, "\t\t%2d %-20s (%4d,%4d) %4d %4d %5d %5d\n",
			unit->getID(),
			unit->getType().toString().c_str(),
			unit->getPosition().x, unit->getPosition().y,
			unit->getHitPoints(),
			unit->getShields(),
			unit->getNextCanAttackFrame(),
			unit->getNextCanMoveFrame());
	}
	fprintf(fptrLog, "\n");
}

// prefix for every log message
void Logger::timestamp() const
{
	time_t now = time(0); // get current time
	tm *timeStruct = localtime(&now); // convert to local time struct

	// print out the timestamp prefix
	fprintf(fptrLog, "[%04d/%02d/%02d %02d:%02d:%02d] ",
			timeStruct->tm_year + 1900,
			timeStruct->tm_mon + 1,
			timeStruct->tm_mday,
			timeStruct->tm_hour,
			timeStruct->tm_min,
			timeStruct->tm_sec);
}