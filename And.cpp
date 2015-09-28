
#include "And.hpp"
#include "Expert.hpp"
And::And(Query* q)
{
	_q.push_back(new Query(q->getName()));
	return ;
}

And::And(Link* tmp)
{
	_prior.push_back(tmp);
	return ;
}

And::And(And const &instance): Link( )
{
	*this = instance;
	return ;
}

And& And::operator=(And const &rhs)
{
	if ( this != &rhs )
		(void)rhs;
	return *this;
}

And::~And(void)
{
	return ;
}

bool		And::checkLink(Expert* expert)
{
	for (unsigned int i = 0; i < _q.size(); i++)
	{
		bool existence = isNot(_q[i]->getName()); // doit exister ou ne doit pas exister ?
		if (!existence && expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist != 2)
			expert->getQueries()[expert->getQuery(_q[i]->getName())]->checkImplied(expert);
		if (existence && expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist != 2)
			return false;
		else if (!existence && expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist == 2)
			return false;
	}
	for (unsigned int i = 0; i < _prior.size(); i++)
	{
		if (!_prior[i]->checkLink(expert))
			return false;
	}
	return true;
}

bool				And::setExist(Expert* expert)
{
	for (unsigned int i = 0; i < _q.size(); i++)
	{
		if (isNot(_q[i]->getName())) // ne doit pas exister
		{
			if (_q[i]->_exist == 1) // si il a deja ete sette a : n'existe pas : error : contradiction dans les regles
				return false;
			expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist = 2;
			_q[i]->_exist = 2;
		}
		else if (!isNot(_q[i]->getName())) // doit exister
		{
			if (_q[i]->_exist == 2) // si il a deja ete sette a : existe : error : contradiction dans les regles
				return false;
			expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist = 1;
			_q[i]->_exist = 1;
		}
	}
	for (unsigned int i = 0; i < _q.size(); i++)
	{
		if (!expert->getQueries()[expert->getQuery(_q[i]->getName())]->checkImplies(expert)) // recursive
				return false;
	}
 for (unsigned int i = 0; i < _prior.size(); i++)
 {
 	if (!_prior[i]->setExist(expert))
 		return false;
 }
	return true;
}
