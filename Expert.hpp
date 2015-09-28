
#ifndef EXPERT_HPP
# define EXPERT_HPP

#include <iostream>
#include <vector>
#include "Query.hpp"
#include "Link.hpp"
#include "And.hpp"
#include "Or.hpp"
#include "Xor.hpp"
#include "Rule.hpp"
class Expert
{
public:
	Expert(void);
	Expert(Expert const &);
	Expert& operator=(Expert const &);
	~Expert(void);

	void					addQuery(Query* q);
	std::vector<Query*>		getQueries();
	int						getQuery(char name);
	std::vector<char>		getFind();
	std::vector<char>		getTrue();
	bool					checkQuery(char name);
	void					addFind(char name);
	void					addTrue(char name);

	int 					nextElement();
	bool					makeLink(int co, int pr);
	bool					getCase();
	bool					getFirstCase();
	void					checkCase();
	bool 					checkBreak();
	bool					getFirstConnection();
	bool					getInfo(std::string line);
	void					passBreak();
	bool					getSecondConnection();
	void					assignConnections();
	void					assignImplied();
	void					assignImplies();
	void					assignTrue();
	void					assignFind();
	void					clearElements();
	void					linkToConnection(int co);
	bool 					checkSame();

	bool					checkExist();
	bool 					checkAbsentCondition();
	void					newExistant();
	void					displayExist();

	bool					_pass;
	int						_pass2;

private:
	std::vector<Query*>	_queries;
	std::vector<char>	_find;	// tableau des queries dont on cherche a determiner l'existence
	std::vector<char>	_true;	// tableau des queries qui existe des le debut

	std::string			_line;
	Link*				_link;
	Connection*			_firstConnection;
	Connection*			_secondConnection;
	Rule*				_rule;
	char				_case;
	char				_sign;
	int					_index;
	bool 				_not;
	bool				_ifonly;
	bool				_prior;
	Link*				_tmp;
	char 				_tmpSign;
};
#endif
