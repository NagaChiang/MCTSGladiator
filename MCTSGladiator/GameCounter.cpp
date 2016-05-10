#include "GameCounter.h"

using namespace MCTSG;

// static members
GameCounter* GameCounter::_ptrInstance = NULL;
std::string GameCounter::_pathCtr = "gameCount.txt";

GameCounter::GameCounter()
	:_fptrCtr(NULL)
{

}

GameCounter::~GameCounter()
{
	// close
	if(_fptrCtr)
		fclose(_fptrCtr);
}

GameCounter* GameCounter::instance()
{
	if(!_ptrInstance)
	{
		_ptrInstance = new GameCounter();
		_ptrInstance->init(_pathCtr);
	}

	return _ptrInstance;
}

void GameCounter::init(const std::string &path)
{
	// if the log hasn't been initiated
	if(!_fptrCtr)
	{
		_fptrCtr = fopen(path.c_str(), "a+");

		// read in the number of current games
		if(_fptrCtr)
			readIn();
		else // failed to open
			BWAPI::Broodwar << "ERROR: Can't initiate GameCounter." << std::endl;
	}
}

void GameCounter::readIn()
{
	if(_fptrCtr)
	{
		// back to beginning
		rewind(_fptrCtr);
		
		// read in the number
		int num;
		if(fscanf(_fptrCtr, "%d", &num) != EOF)
			_gameCount = num;
		else // empty (new file cause' file not exist) -> new runs
			_gameCount = 0;
	}
}

// save current game count
void GameCounter::save()
{
	if(_fptrCtr)
	{
		// close first
		fclose(_fptrCtr);

		// re-open to clear the file
		_fptrCtr = fopen(_pathCtr.c_str(), "w");

		if(_fptrCtr)
		{
			// write to the file
			fprintf(_fptrCtr, "%d", _gameCount);

			// close the file
			fclose(_fptrCtr);
		}
	}
}

// called when the series of games is over
void GameCounter::end()
{
	if(_fptrCtr)
	{
		// close first
		fclose(_fptrCtr);

		// re-open to clear the file
		_fptrCtr = fopen(_pathCtr.c_str(), "w");

		// close it again
		if(_fptrCtr)
			fclose(_fptrCtr);
	}
}