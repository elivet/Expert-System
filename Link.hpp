#ifndef LINK_HPP
# define LINK_HPP

#include <iostream>
#include "Query.hpp"

class Link
{
public:
	Link(void);
	Link(Query* q);
	Link(Link* tmp);
	Link(Link const &);
	Link& operator=(Link const &);
	~Link(void);

	std::vector<Query*>		getQueries();
	std::vector<char>		getNots();
	virtual bool			checkLink(Expert* expert);
	bool					isNot(char name);
	virtual bool			setExist(Expert* expert);	
	bool 					checkAbsentCondition();
	
	std::vector<Query*>	_q;
	std::vector<char>	_not;
	std::vector<Link*>	_prior;

protected:
	
};
#endif