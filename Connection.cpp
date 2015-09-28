
#include "Connection.hpp"
#include "Link.hpp"
#include "Expert.hpp"
Connection::Connection(void): _check(0)
{
	return ;
}

Connection::Connection(Link * link)
{
	_links.push_back(link);
	return ;
}


Connection::Connection(Connection const &instance)
{
	*this = instance;
	return ;
}

Connection& Connection::operator=(Connection const &rhs)
{
	if ( this != &rhs )
	{
		_links = rhs._links;
	}
	return *this;
}

Connection::~Connection(void)
{
	return ;
}

void	Connection::addLink(Link * link)
{
	_links.push_back(link);
	return ;
}

std::vector<Link*> 	Connection::getLinks()
{
	return _links;
}

bool				Connection::checkConnection(Expert*	expert)
{
	if (_check < 2)
	{
		for (unsigned int i = 0; i < _links.size(); i++)
		{
			if (!_links[i]->checkLink(expert))
				return false;
		}
		_check++;
	}
	else
		expert->_pass = true;
	return true;
}

bool				Connection::setExist(Expert*	expert)
{
	for (unsigned int i = 0; i < _links.size(); i++)
	{
		if (!_links[i]->setExist(expert))
			return false;
	}
	return true;
}

bool 				Connection::checkAbsentCondition()
{
	for (unsigned int j = 0; j < _links.size(); j++)
	{
		if (_links[j]->checkAbsentCondition())
			return true;
	}
	return false;
}
