
#ifndef QUERY_HPP
# define QUERY_HPP

#include <iostream>
#include <vector>
#include "Connection.hpp"
#include "Rule.hpp"
class Link;
class Expert;
class Query
{
public:
	Query(char name);
	Query(Query const &);
	Query& operator=(Query const &);
	~Query(void);

	char						getName();
	std::vector<Rule*>			getImplies();
	std::vector<Rule*>			getImplied();
	void						addImplies(Rule *imp);
	void						addImplied(Rule *imp);
	bool						checkImplies(Expert* expert);
	bool						checkImplied(Expert* expert);
	bool 						checkAbsentCondition(Expert* expert);

	int							_exist;
	bool						_find;
protected:
	
	char						_name;
	std::vector<Rule*>			_implies;
	std::vector<Rule*>			_implied;
};
#endif