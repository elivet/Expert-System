
#include "Rule.hpp"
#include "Expert.hpp"

Rule::Rule(void): _ifonly(false), _check(0), _check2(0)
{
	return ;
}

Rule::Rule(Connection * implies, Connection * implied)
{
	_implies = implies;
	_implied = implied;
	return ;
}


Rule::Rule(Rule const &instance)
{
	*this = instance;
	return ;
}

Rule& Rule::operator=(Rule const &rhs)
{
	if ( this != &rhs )
	{
		_implies = rhs._implies;
		_implied = rhs._implied;
	}
	return *this;
}

Rule::~Rule(void)
{
	return ;
}

bool		Rule::checkImplies(Expert*	expert)
{
	if (expert->_pass)
		return true;
	if ((_ifonly && _check < 3) || !_ifonly)
	{
		_check++;
		if (_implies->checkConnection(expert)) // si la regle s'avere vrai
		{
			if (!_implied->setExist(expert)) // dire qui existe et qui n'existe pas
				return false;
		}
	}
	return true;
}

bool		Rule::checkImplied(Expert*	expert)
{
	if (expert->_pass)
		return true;
	if ((_ifonly && _check2 < 5) || !_ifonly)
	{
		_check2++;
		if (_implies->checkConnection(expert)) // si la regle s'avere vrai
		{
			if (!_implied->setExist(expert)) // dire qui existe et qui n'existe pas
				return false;
		}
	}
	return true;
}


bool 			Rule::checkAbsentCondition()
{
	if (_implies->checkAbsentCondition())
			return true;
	return false;
}
