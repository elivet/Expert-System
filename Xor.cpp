
#include "Xor.hpp"
#include "Expert.hpp"

Xor::Xor(Query* q)
{
	_q.push_back(new Query(q->getName()));
	return ;
}

Xor::Xor(Link* tmp)
{
	_prior.push_back(tmp);
	return ;
}


Xor::Xor(Xor const &instance): Link( )
{
	*this = instance;
	return ;
}

Xor& Xor::operator=(Xor const &rhs)
{
	if ( this != &rhs )
		(void)rhs;
	return *this;
}

Xor::~Xor(void)
{
	return ;
}

bool		Xor::checkLink(Expert* expert)
{
	int 		valid = 0;
	for (unsigned int i = 0; i < _q.size(); i++)
	{
		bool existence = isNot(_q[i]->getName());
		if (!existence && expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist != 2)
			expert->getQueries()[expert->getQuery(_q[i]->getName())]->checkImplied(expert);
		if (!existence && expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist != 2)
			valid++;
		else if (existence && expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist == 2)
			valid++;
	}
	for (unsigned int i = 0; i < _prior.size(); i++)
	{
		if (_prior[i]->checkLink(expert))
			valid++;
	}
	if (valid == 1) // si un seul est vrai
		return true;
	return false;
}

bool				Xor::setExist(Expert* expert)
{
	int 	existence = 0;
	int 	existence2 = 0;
	for (unsigned int i = 0; i < _q.size(); i++)
	{
		if (_q[i]->_exist == 2 && isNot(_q[i]->getName()))
			existence2++;
		else if (_q[i]->_exist == 1 && !isNot(_q[i]->getName()))
			existence2++;
		else if (_q[i]->_exist == 0)
			existence++;
	}
	if (existence2 > 1)
		return false;
	else if (existence2 == 1)
		return true;
	if (existence == 1)
	{
		for (unsigned int i = 0; i < _q.size(); i++)
		{
			if (_q[i]->_exist == 0)
			{
				if (isNot(_q[i]->getName()))
				{
					if (_q[i]->_exist == 1)
						return false;
					expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist = 2;
					_q[i]->_exist = 2;
				}
				else if (!isNot(_q[i]->getName()))
				{
					if (_q[i]->_exist == 2)
						return false;
					expert->getQueries()[expert->getQuery(_q[i]->getName())]->_exist = 1;
					_q[i]->_exist = 1;
				}

				return true;
			}
		}
		for (unsigned int i = 0; i < _q.size(); i++)
		{
				if (!expert->getQueries()[expert->getQuery(_q[i]->getName())]->checkImplies(expert))
					return false;
		}
		for (unsigned int i = 0; i < _prior.size(); i++)
		{
			if (_prior[i]->setExist(expert))
				return true;
		}
	}
	return true;
}
