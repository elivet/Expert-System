#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <iostream>
#include <vector>
class Link;
class Expert;
class Connection
{
public:
	Connection(void);
	Connection(Link * link);
	Connection(Connection const &);
	Connection& operator=(Connection const &);
	~Connection(void);

	void				addLink(Link * link);
	std::vector<Link*>	getLinks();
	bool				checkConnection(Expert*	expert);
	bool				setExist(Expert*	expert);
	bool 				checkAbsentCondition();
private:
	std::vector<Link*>	_links;
	int					_check;
	// std::vector<Prior*>	_prior;	// les liens prioritairesa checker avant les autres
};
#endif