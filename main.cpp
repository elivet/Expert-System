#include "Expert.hpp"
#include "Link.hpp"
#include "And.hpp"
#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdio>

int 		main(int argc, char const *argv[])
{
	Expert	* expert;

	if (argc > 1)
	{
		expert = new Expert();
		std::ifstream file;
		std::string			line;

  		file.open(argv[1]);
  		while (std::getline(file, line))
  		{
  			if (!line.empty())
  			{
  				if (!expert->getInfo(line)) // parser chaque ligne
  				{
  					std::cout << "ERROR: syntax error" << std::endl;
  					return 0;
  				}
  			}

		}
		if (!expert->checkExist()) // determiner les queries existants a partir de ceux declares existant par le fichier
		{
			std::cout << "ERROR: fact contradiction" << std::endl;
			return (0);
		}
		expert->displayExist();
	}
	return (0);
}
