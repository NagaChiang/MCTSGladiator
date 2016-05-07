#include "Logger.h"

using namespace MCTSG;

Logger* Logger::_ptrInstance = NULL; // static

//const std::string LOG_PATH = "D:/Phate/Temp/Thesis/MCTSGladiator/Release/log.txt";
const std::string LOG_PATH = "log.txt";

Logger::Logger()
{
	_fptrLog = NULL;
}

Logger::~Logger()
{
	// close the log
	if(_fptrLog)
		fclose(_fptrLog);
}

Logger* Logger::instance()
{
	if(!_ptrInstance)
	{
		_ptrInstance = new Logger();
		_ptrInstance->init(LOG_PATH);
	}

	return _ptrInstance;
}

// initiate the log if hasn't been
void Logger::init(const std::string &path)
{
	// if the log hasn't been initiated
	if(!_fptrLog)
	{
		_logPath = path;
		_fptrLog = fopen(path.c_str(), "a");

		// failed to open
		if(!_fptrLog)
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
	if(!_fptrLog)
		return;

	// timestamp
	timestamp();

	// log
	std::string strOut = str + "\n";
	fprintf(_fptrLog, strOut.c_str());
}

// log a state
void Logger::log(const State &state) const
{
	// if not initiated
	if(!_fptrLog)
		return;

	// timestamp
	timestamp();

	// title
	fprintf(_fptrLog, "\n");
	fprintf(_fptrLog, "\t<State>\n");
	fprintf(_fptrLog, "\n");
	
	// time frame
	fprintf(_fptrLog, "\tTime Frame = %d\n", state.getTimeFrame());
	fprintf(_fptrLog, "\n");

	// ally units
	fprintf(_fptrLog, "\tAlly Units (%d):\n", state.getAllyUnitsNum()); // title
	Unitset allyUnits = state.getAllyUnits();
	logUnits(allyUnits);

	// enemy units
	fprintf(_fptrLog, "\tEnemy Units (%d):\n", state.getEnemyUnitsNum()); // title
	Unitset enemyUnits = state.getEnemyUnits();
	logUnits(enemyUnits);
}

// unit
void Logger::log(const Unit &unit) const
{
	// if not initiated
	if(!_fptrLog)
		return;

	// timestamp
	timestamp();
	
	// unit
	fprintf(_fptrLog, "%2d %-20s (%4d,%4d) %4d %4d %5d %5d\n",
		unit->getID(),
		unit->getType().toString().c_str(),
		unit->getPosition().x, unit->getPosition().y,
		unit->getHitPoints(),
		unit->getShields(),
		unit->getNextCanAttackFrame(),
		unit->getNextCanMoveFrame());
}

// Move
void Logger::log(const Move &Move) const
{
	// if not initiated
	if(!_fptrLog)
		return;

	// timestamp
	timestamp();

	// title
	fprintf(_fptrLog, "\n");
	fprintf(_fptrLog, "\t<Move>\n");
	fprintf(_fptrLog, "\n");

	// actions in this Move
	for(Action action : Move)
	{
		int unitID = action.getUnitID();
		int targetID = action.getTargetID();
		int t = action.getEndFrame();

		fprintf(_fptrLog, "\t");

		// unit
		fprintf(_fptrLog, "%2d ", unitID);

		// type
		fprintf(_fptrLog, "%-7s", action.toString().c_str());

		// target
		fprintf(_fptrLog, "%2d ", targetID);

		// time
		fprintf(_fptrLog, "%5d", t);

		fprintf(_fptrLog, "\n");
	}

	fprintf(_fptrLog, "\n");
}

void Logger::log(const int num) const
{
	// if not initiated
	if(!_fptrLog)
		return;

	timestamp();
	fprintf(_fptrLog, "%d\n", num);
}

// log an big integer
void Logger::log(const long long num) const
{
	// if not initiated
	if(!_fptrLog)
		return;

	timestamp();
	fprintf(_fptrLog, "%lld\n", num);
}

void Logger::log(const double num) const
{
	// if not initiated
	if(!_fptrLog)
		return;

	timestamp();
	fprintf(_fptrLog, "%lf\n", num);
}

// log all children moves of this node
void Logger::logChildrenMoves(const UCTNode &node) const
{
	// if not initiated
	if(!_fptrLog)
		return;

	// timestamp
	timestamp();

	// title
	fprintf(_fptrLog, "\n");
	fprintf(_fptrLog, "\t<All Children Moves>\n");
	fprintf(_fptrLog, "\n");

	// log all children moves
	std::vector<UCTNode*> children = node.getChildren();
	for(UCTNode *child : children)
	{
		if(!child)
			continue;

		Move move = child->getMove();

		// prefix
		fprintf(_fptrLog, "\t\t");

		// log actions in this node
		for(Action action : move)
		{
			Actions type = action.getType();
			char token;

			switch(type)
			{
				case Actions::None:
					token = 'N';
					break;

				case Actions::Stop:
					token = '0';
					break;

				case Actions::North:
					token = '1';
					break;

				case Actions::East:
					token = '2';
					break;

				case Actions::West:
					token = '3';
					break;

				case Actions::South:
					token = '4';
					break;

				case Actions::Attack:
					token = 'A';
					break;

				default:
					token = '?';
					break;
			}

			// log
			fprintf(_fptrLog, "%c", token);
		}

		fprintf(_fptrLog, "\n");
	}

	fprintf(_fptrLog, "\n");
}

// support function of log state
void Logger::logUnits(const Unitset &units) const
{
	for(Unitset::const_iterator itr = units.begin(); itr != units.end(); itr++)
	{
		Unit unit = itr->second;
		fprintf(_fptrLog, "\t\t%2d %-20s (%4d,%4d) %4d %4d %5d %5d\n",
			unit->getID(),
			unit->getType().toString().c_str(),
			unit->getPosition().x, unit->getPosition().y,
			unit->getHitPoints(),
			unit->getShields(),
			unit->getNextCanAttackFrame(),
			unit->getNextCanMoveFrame());
	}
	fprintf(_fptrLog, "\n");
}

// prefix for every log message
void Logger::timestamp() const
{
	time_t now = time(0); // get current time
	tm *timeStruct = localtime(&now); // convert to local time struct

	// print out the timestamp prefix
	fprintf(_fptrLog, "[%04d/%02d/%02d %02d:%02d:%02d] ",
			timeStruct->tm_year + 1900,
			timeStruct->tm_mon + 1,
			timeStruct->tm_mday,
			timeStruct->tm_hour,
			timeStruct->tm_min,
			timeStruct->tm_sec);
}