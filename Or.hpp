#ifndef OR_HPP
# define OR_HPP

#include <iostream>
#include "Link.hpp"
class Or : public Link
{
public:
	Or(Query* q);
	Or(Link* tmp);
	Or(Or const &);
	Or& operator=(Or const &);
	~Or(void);


	virtual bool		checkLink(Expert* expert);
	virtual bool		setExist(Expert* expert);
};
#endif