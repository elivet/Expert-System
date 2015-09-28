
#include "Link.hpp"
#include "Expert.hpp"
Link::Link(void)
{
	return ;
}

Link::Link(Query* q)
{
	_q.push_back(new Query(q->getName()));
	return ;
}

Link::Link(Link* tmp)
{
	_prior.push_back(tmp);
	return ;
}

Link::Link(Link const &instance)
{
	*this = instance;
	return ;
}

Link& Link::operator=(Link const &rhs)
{
	if ( this != &rhs )
		(void)rhs;
	return *this;
}

Link::~Link(void)
{
	return ;
}

std::vector<Query*>	Link::getQueries()
{
	return _q;
}

std::vector<char>	Link::getNots()
{
	return _not;
}

bool				Link::isNot(char name)
{
	for (unsigned int i = 0; i < _not.size(); i++)
	{
		if (_not[i] == name)
			return false;
	}
	return true;
}

bool				Link::checkLink(Expert* expert)
{
	bool	existence = isNot(_q[0]->getName());
	if (!existence && expert->getQueries()[expert->getQuery(_q[0]->getName())]->_exist == 2)
		return false;
	else if (existence && expert->getQueries()[expert->getQuery(_q[0]->getName())]->_exist != 2)
		return false;
	return true;
}

bool				Link::setExist(Expert*	expert)
{
	if (isNot(_q[0]->getName()))
	{
		if (expert->getQueries()[expert->getQuery(_q[0]->getName())]->_exist == 1)
			return false;
		expert->getQueries()[expert->getQuery(_q[0]->getName())]->_exist = 2;
		_q[0]->_exist = 2;
	}
	else if (!isNot(_q[0]->getName()))
	{
		if (expert->getQueries()[expert->getQuery(_q[0]->getName())]->_exist == 2)
			return false;
		expert->getQueries()[expert->getQuery(_q[0]->getName())]->_exist = 1;
		_q[0]->_exist = 1;
	}
	if (!expert->getQueries()[expert->getQuery(_q[0]->getName())]->checkImplies(expert))
		return false;
	return true;
}


bool 				Link::checkAbsentCondition()
{

	if (_q.size() == 1 && _not.size() == 1 && _q[0]->getName() == _not[0])
		return true;
	return false;
}
