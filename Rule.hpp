#ifndef RULE_HPP
# define RULE_HPP

#include <iostream>
#include <vector>
#include "Connection.hpp"
class Expert;
class Rule
{
public:
	Rule(void);
	Rule(Connection* implies, Connection* implied);
	Rule(Rule const &);
	Rule& operator=(Rule const &);
	~Rule(void);
	
	bool			checkImplies(Expert*	expert);
	bool			checkImplied(Expert*	expert);
	bool 			checkAbsentCondition();

	bool			_ifonly;
private:
	Connection*				_implies;
	Connection*				_implied;
	int						_check;
	int						_check2;
};
#endif