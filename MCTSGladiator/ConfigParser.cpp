#include "ConfigParser.h"

using namespace MCTSG;

// static members
ConfigParser* ConfigParser::_ptrInstance = NULL;
std::string ConfigParser::_pathCfg = "config.cfg";

ConfigParser::ConfigParser()
	:_fptrCfg(NULL)
{

}

ConfigParser::~ConfigParser()
{
	// close
	if(_fptrCfg)
		fclose(_fptrCfg);
}

ConfigParser* ConfigParser::instance()
{
	if(!_ptrInstance)
	{
		_ptrInstance = new ConfigParser();
		_ptrInstance->init(_pathCfg);
	}

	return _ptrInstance;
}

void ConfigParser::init(const std::string &path)
{
	// if the log hasn't been initiated
	if(!_fptrCfg)
	{
		_fptrCfg = fopen(path.c_str(), "r");
		
		// parse the configurations
		if(_fptrCfg)
			parseIn();
		else // failed to open
			BWAPI::Broodwar << "ERROR: Can't initiate configParser." << std::endl;
	}
}

void ConfigParser::parseIn()
{
	if(_fptrCfg)
	{
		char rawWord[40];
		std::string word;
		while(fscanf(_fptrCfg, "%s", rawWord) != EOF)
		{
			// store in std string
			word = rawWord;

			// eat the =
			fscanf(_fptrCfg, "%s", rawWord);
			if(rawWord[0] != '=')
				break;

			// read in the params
			if(word == "timelimit")
			{
				int limit;
				fscanf(_fptrCfg, "%d", &limit);
				_params.timeLimit = std::chrono::milliseconds(limit);
			}
			else if(word == "maxChildren")
			{
				int max;
				fscanf(_fptrCfg, "%d", &max);
				_params.maxChildren = max;
			}
			else if(word == "explorationConst")
			{
				double C;
				fscanf(_fptrCfg, "%lf", &C);
				_params.explorationConst = C;
			}
			else if(word == "evaluationMethod")
			{
				int index;
				fscanf(_fptrCfg, "%d", &index);
				_params.evalMethod = (EvaluationMethod)index;
			}
			else if(word == "numGames")
			{
				int num;
				fscanf(_fptrCfg, "%d", &num);
				_numGames = num;
			}
			else if(word == "LogWinMode")
			{
				int isOn;
				fscanf(_fptrCfg, "%d", &isOn);
				_isLogWinMode = isOn > 0 ? true : false;
			}
			else if(word == "NoGUIMode")
			{
				int isOn;
				fscanf(_fptrCfg, "%d", &isOn);
				_isNoGUIMode = isOn > 0 ? true : false;
			}
			else
				BWAPI::Broodwar << "ERROR: Invalid syntax in config file." << std::endl;
		}
	}
}

