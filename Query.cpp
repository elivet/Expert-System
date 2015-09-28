
#include "Query.hpp"
#include "Expert.hpp"

Query::Query(char name): _exist(0), _name(name)
{
	return ;
}

Query::Query(Query const &instance)
{
	*this = instance;
	return ;
}

Query& Query::operator=(Query const &rhs)
{
	if ( this != &rhs )
		(void)rhs;
	return *this;
}

Query::~Query(void)
{
	return ;
}

char		Query::getName()
{
	return _name;
}

void		Query::addImplies(Rule * imp)
{
	_implies.push_back(imp);
	return ;
}

void		Query::addImplied(Rule *imp)
{
 	_implied.push_back(imp);
	return ;
}

std::vector<Rule*>		Query::getImplies()
{
	return _implies;
}

std::vector<Rule*>		Query::getImplied()
{
	return _implied;
}

bool				Query::checkImplies(Expert*	expert)
{
	for (unsigned int i = 0; i < _implies.size(); i++)
	{
		if (!_implies[i]->checkImplies(expert))
			return false;
		if (expert->_pass)
		{
			expert->_pass2++;
			expert->_pass = false;
		}
		if (expert->_pass2 > 500)
			return false;

	}
	return true;
}

bool				Query::checkImplied(Expert*	expert)
{
	for (unsigned int i = 0; i < _implied.size(); i++)
	{
		if (!_implied[i]->checkImplied(expert))
			return false;
		if (expert->_pass)
			expert->_pass = false;
	}
	return true;
}

bool 			Query::checkAbsentCondition(Expert*	expert)
{
	for (unsigned int j = 0; j < _implies.size(); j++)
	{
		if (_implies[j]->checkAbsentCondition())
		{
			if (!_implies[j]->checkImplies(expert))
			return false;
		}
	}
	return true;
}
