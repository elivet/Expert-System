#ifndef AND_HPP
# define AND_HPP

#include <iostream>
#include <vector>
#include "Link.hpp"
class And : public Link
{
public:
	And(Query *q);
	And(Link* tmp);
	And(And const &);
	And& operator=(And const &);
	~And(void);

	virtual bool		checkLink(Expert* expert);
	virtual bool		setExist(Expert* expert);
};
#endif