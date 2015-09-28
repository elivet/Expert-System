
#include "Or.hpp"
#include "Expert.hpp"

Or::Or(Query* q)
{
	_q.push_back(new Query(q->getName()));
	return ;
}

Or::Or(Link* tmp)
{
	_prior.push_back(tmp);
	return ;
}

Or::Or(Or const &instance): Link( )
{
	*this = instance;
	return ;
}

Or& Or::operator=(Or const &rhs)
{
	if ( this != &rhs )
		(void)rhs;
	return *this;
}

Or::~Or(void)
{
	return ;
}


bool		Or::checkLink(Expert* expert)
{
	for (unsigned int i = 0; i < _q.size(); i++)
	{
		bool existence = isNot(_q[i]->getName());
		if (!existence && expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist != 2)
			expert->getQueries()[expert->getQuery(_q[i]->getName())]->checkImplied(expert);
		if (!existence && expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist != 2)
			return true;
		else if (existence && expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist == 2)
			return true;
	}
	for (unsigned int i = 0; i < _prior.size(); i++)
	{
		if (_prior[i]->checkLink(expert))
			return true;
	}
	return false;
}

bool				Or::setExist(Expert* expert)
{
	unsigned int 	existence = 0;
	for (unsigned int i = 0; i < _q.size(); i++)
	{
		if (isNot(_q[i]->getName()) && _q[i]->_exist != 1)
		{
			expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist = 2;
			_q[i]->_exist = 2;
		}
		else if (!isNot(_q[i]->getName()) && _q[i]->_exist != 2)
		{
			expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist = 1;
			_q[i]->_exist = 1;
		}
		existence++;
	}
	for (unsigned int i = 0; i < _q.size(); i++)
	{
		if (!expert->getQueries()[expert->getQuery(_q[i]->getName())]->checkImplies(expert)) // recursive
				return false;
	}
	for (unsigned int i = 0; i < _prior.size(); i++)
	{
		if (_prior[i]->setExist(expert))
			return true;
	}
	if (existence == 0) // si tous ont deja ete declares a l'oppose de ce que la deuxieme connection de la regle dit
		return false;
	return true;
}
