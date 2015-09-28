
#ifndef XOR_HPP
# define XOR_HPP

#include <iostream>
#include "Link.hpp"
class Xor : public Link
{
public:
	Xor(Query* q);
	Xor(Link* tmp);
	Xor(Xor const &);
	Xor& operator=(Xor const &);
	~Xor(void);	

	virtual bool		checkLink(Expert* expert);
	virtual bool		setExist(Expert* expert);
};

#endif