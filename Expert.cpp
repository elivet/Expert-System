
#include "Expert.hpp"

Expert::Expert(void): _pass(false), _pass2(0), _index(0), _not(false), _ifonly(false), _prior(false)
{
	return ;
}

Expert::Expert(Expert const &instance)
{
	*this = instance;
	return ;
}

Expert& Expert::operator=(Expert const &rhs)
{
	if ( this != &rhs )
	{
		_queries = rhs._queries;
	}
	return *this;
}

Expert::~Expert(void)
{
	return ;
}

void		Expert::addQuery(Query* q)
{
	_queries.push_back(q);
	return ;
}

void		Expert::addFind(char name)
{
	_find.push_back(name);
	return ;
}

void		Expert::addTrue(char name)
{
	_true.push_back(name);
	return ;
}

std::vector<char>		Expert::getFind()
{
	return _find;
}

std::vector<char>		Expert::getTrue()
{
	return _true;
}

std::vector<Query*> Expert::getQueries()
{
	return _queries;
}

int 				Expert::getQuery(char name)
{
	for(unsigned int i = 0; i < _queries.size(); ++i)
	{
		Query* q = _queries[i];
		if (name == q->getName())
			return i;
	}
	return -1;
}


bool		Expert::checkQuery(char name)
{
	for(unsigned int i = 0; i < _queries.size(); ++i)
	{
	    Query* q = _queries[i];
	    if (name == q->getName())
	    	return true;
	}
	return false;
}

bool		Expert::getInfo(std::string line)
{
	_line = line;
	if (_line[0] == '=') // stocker les queries declares vrai des le debut
		assignTrue();
	else if (line[0] == '?') // stocker les queries dont il faut determiner l'existence
		assignFind();
	else if (getFirstConnection()) // stocker la premiere partie de la regle (avant le =>)
	{
		if (getSecondConnection()) // stocker la deuxieme partie de la regle (apres le =>)
		{
			if (!checkSame()) // verifier que la regle nest pas fausse (ex: A + B => A) avec un query qui existe grace a lui meme
				return false;
			assignConnections(); // assigner les regles aux queries concernes
		}
		else
			return false;
	}
	clearElements();
	return true ;
}

void		Expert::assignFind() // stocker les queries dont il faut determiner l'existence
{
	_index = 1;
	while (nextElement() == 1)
	{
		if (_find.size() > 0 && _case == _find[0])
			return ;
		checkCase();
		_find.push_back(_case);
		_queries[getQuery(_case)]->_find = true;
		_index++;
	}
	_case = '\0';
	_sign = '\0';
	return ;
}

void		Expert::assignTrue() // stocker les queries declares vrai des le debut
{
	_index = 1;
	while (nextElement() == 1)
	{
		checkCase();
		_true.push_back(_case);
		_queries[getQuery(_case)]->_exist = 2;
		_index++;
	}
	_case = '\0';
	_sign = '\0';
	return ;
}

void		Expert::assignConnections() // assigner les regles aux queries concernes
{
	_rule = new Rule(_firstConnection, _secondConnection); // creer la regle a partir des deux connections
	assignImplied(); //assigner la regle a tous les queries de la seconde connection en tant que implied
	assignImplies(); //assigner la regle a tous les queries de la premiere connection en tant que implies
	if (_ifonly) // si <=> assigner la regle aux queries de la deuxieme connection en tant que implies et aux queries de la premiere connection en tant que implied
	{
		_rule->_ifonly = true;
		Connection * _tmp = new Connection(*_secondConnection);
		_secondConnection = _firstConnection;
		_firstConnection = _tmp;
		_rule = new Rule(_firstConnection, _secondConnection);
		_rule->_ifonly = true;
		assignImplied();
		assignImplies();
	}
	return ;
}

bool 		Expert::checkSame() // verifier que la regle nest pas fausse (ex: A + B => A) avec un query qui existe grace a lui meme
{
	for (unsigned int j = 0; j < _firstConnection->getLinks().size(); j++)
	{
		for (unsigned int k = 0; k < _secondConnection->getLinks().size(); k++)
		{
			for (unsigned int l = 0; l < _firstConnection->getLinks()[j]->getQueries().size(); l++)
			{
				for (unsigned int m = 0; m < _secondConnection->getLinks()[k]->getQueries().size(); m++)
				{
					if (_firstConnection->getLinks()[j]->getQueries()[l]->getName() == _secondConnection->getLinks()[k]->getQueries()[m]->getName())
						return false;
				}
			}
		}
	}
	return true;
}

void		Expert::assignImplied() //assigner la regle a tous les queries de la seconde connection en tant que implied
{
	for (unsigned int j = 0; j < _secondConnection->getLinks().size();j++)
	{
		for (unsigned int k = 0; k < _secondConnection->getLinks()[j]->getQueries().size(); k++)
			_queries[getQuery(_secondConnection->getLinks()[j]->getQueries()[k]->getName())]->addImplied(_rule);
	}
	return ;
}

void		Expert::assignImplies() //assigner la regle a tous les queries de la premiere connection en tant que implies
{
	for (unsigned int j = 0; j < _firstConnection->getLinks().size();j++)
	{
		for (unsigned int k = 0; k < _firstConnection->getLinks()[j]->getQueries().size(); k++)
			_queries[getQuery(_firstConnection->getLinks()[j]->getQueries()[k]->getName())]->addImplies(_rule);
	}
	return ;
}

bool		Expert::getFirstConnection() // stocker la premiere partie de la regle (avant le =>)
{
	_firstConnection = new Connection();

	for (_index = 0; _line[_index] != '\n' && _line[_index] != '#' && _line[_index] != '\0'; _index++)
	{
		if (checkBreak()) // s'arreter si on est arrive au =>
			return true;
		if (getFirstCase()) // obtenir la premiere lettre
		{
			_index++;
			int ret = nextElement(); // obtenir lelement qui suit
			if (ret == 2 && _prior)
			{
				if (!makeLink(1, 1))
					return false;
				_index++;
			}
			else if (ret != 2 && _prior)
				return false;
			else if (ret == 2) // si lelement est un signe alors creer un lien aussi long qu'il y a de lettre liees avec ce meme signe (ex: A + B + C)
			{
				if (!makeLink(1, 0))
					return false;
				_index++;
			}
			else if (ret == 4) // si lelement est le => alors la connection ne contient qu'une seule lettre que l'on stocke dans la premiere connection
			{
				_link = new Link(_queries[getQuery(_case)]);
				if (_not) // si la lettre est precedee de ! l'ajouter au tableau de not
				{
					_link->_not.push_back(_case);
					_not = false;
				}
				_firstConnection->addLink(_link);
				return true ;
			}
			else
				return false;
		}
		else
			return false;

	}
	return false;
}


bool		Expert::getSecondConnection() // stocker la deuxieme partie de la regle (apres le =>)
{
	int 	nbr = 0;
	_secondConnection = new Connection();
	passBreak(); // avancer l'index apres le =>
	while (_line[_index] != '\n' && _line[_index] != '#' && _line[_index] != '\0')
	{
		if (getFirstCase())
		{
			nbr++;
			_index++;
			int ret = nextElement();
			if (ret == 2 && _prior)
			{
				if (!makeLink(2, 1))
					return false;
				if (_line[_index] == '\n' || _line[_index] == '#' || _line[_index] == '\0')
					return true;
				_index++;
			}
			else if (ret != 2 && _prior)
				return false;
			else if (ret == 2)
			{
				nbr++;
				if (!makeLink(2, 0))
					return false;
				if (_line[_index] != '\n' || _line[_index] != '#' || _line[_index] == '\0')
					return true;
			}
			else if (ret == 0 && nbr == 1) // si on est arrive a la fin de la ligne et quon a trouve quune seule lettre pour la deuxieme connection, la stocker
			{
				_link = new Link(_queries[getQuery(_case)]);
				if (_not)
				{
					_link->_not.push_back(_case);
					_not = false;
				}
				_secondConnection->addLink(_link);
				return true ;
			}
			else if (ret == 0 || (ret == 1 && nbr == 1))
				return true;
			else
				return false;

		}
		else
			return false;
		_index++;
	}
	return false;
}

void		Expert::passBreak() // avancer l'index apres le =>
{
	// std::cout << " pass break " << _line[_index] << _line[_index + 1] << std::endl;
	_index++;
	// std::cout << "passBreak INDEX++: " << _index << std::endl;
	if (_line[_index] == '<' && _line[_index + 1] == '=')
	{
		_ifonly = true;
		_index += 3;
	}
	else if (_line[_index - 1] == '<' && _line[_index] == '=')
	{
		_ifonly = true;
		_index += 2;
	}
	else if (_line[_index] == '=' && _line[_index + 1] == '>')
		_index += 2;
	return ;
}

bool 		Expert::checkBreak() // verifie si index se trouve sur le =>
{
	if ((_line[_index] == '<' && _line[_index + 1] == '=') || (_line[_index] == '=' && _line[_index + 1] == '>'))
	{
		_index--;
		return true;
	}
	else if (_line[_index] == '>')
	{
		_index -= 2;
		return true;
	}
	else if (_line[_index - 1] == '<' && _line[_index] == '=' && _line[_index + 1] == '>')
	{
		_index --;
		return true;
	}
	return false;
}

void		Expert::checkCase() // si le query nexiste pas deja dans le tableau de query de l'expert, l'y stocker
{
	if (!checkQuery(_case))
		_queries.push_back(new Query(_case));
	return ;
}

bool		Expert::getFirstCase() // obternir la premiere lettre de la connection
{

	int ret = nextElement(); // obtenir lelement qui suit
	if (ret == 1) // si il s'agit d'une lettre
	{
		checkCase(); // verifier si elle est stockee, si non, le faire
		return true;
	}
	else if (ret == 3) // si c'est un !
	{
		_index++;
		if (nextElement() == 1) // et que ce qui suit est une lettre, la stocker et signaler que cest un not
		{
			_not = true;
			checkCase(); // verifier si elle est stockee, si non, le faire
			return true;
		}
		return false;
	}
	else if (ret == 5)
	{
		_index++;
		int ret2 = nextElement();
		if (ret2 == 3)
		{
			_index++;
			if (nextElement() == 1) // et que ce qui suit est une lettre, la stocker et signaler que cest un not
			{
				_not = true;
				checkCase(); // verifier si elle est stockee, si non, le faire
				_prior = true;
				return true;
			}
			return false;
		}
		else if (ret2 == 1)
		{
			checkCase(); // verifier si elle est stockee, si non, le faire
			_prior = true;
			return true;
		}
	}
	return false;
}

bool		Expert::getCase() // obtenir la prochaine lettre
{
	_index++;
	int ret = nextElement(); // obtenir le prochain element
	if (ret == 1) // si l'element est une lettre, la stocker dans les queries de l'expert si cest pas deja fait et rajouter la lettre au lien en cours
	{
		checkCase();
		_link->_q.push_back(_queries[getQuery(_case)]);
		return true;
	}
	else if (ret == 3) // si l'element est un !
	{
		_index++;
		if (nextElement() == 1) // et que l'element qui suit est une lettre, la stocker dans les queries de l'expert si cest pas deja fait et rajouter la lettre au lien en cours
		{
			_link->_not.push_back(_case); // stocker le nom du query dans le tableau des not du lien
			checkCase();
			_link->_q.push_back(_queries[getQuery(_case)]);
			return true;
		}
		return false;
	}
	return false;
}

bool		Expert::makeLink(int co, int pr) // faire un lien
{
	char _save = _sign;
	if (_sign == '+' && pr != 3)
	{
		if (pr == 2)
			_link = new And(_tmp);
		else
			_link = new And(_queries[getQuery(_case)]); // creer un type de lien selon son signe en y sauvegardant le premier query trouve
	}
	else if (_sign == '|' && pr != 3)
	{
		if (pr == 2)
			_link = new Or(_tmp);
		else
		_link = new Or(_queries[getQuery(_case)]); // creer un type de lien selon son signe en y sauvegardant le premier query trouve
	}
	else if (_sign == '^' && pr != 3)
	{
		if (pr == 2)
			_link = new Xor(_tmp);
		else
		_link = new Xor(_queries[getQuery(_case)]); // creer un type de lien selon son signe en y sauvegardant le premier query trouve
	}
	if (_not) // si il ls'agit d'un not le rajouter aux not du lien
	{
		_not = false;
		_link->_not.push_back(_case);
	}
	if (pr != 3 && !getCase()) // euh je sais pas pk cest la faut que je teste mais je crois que cest faux
	{
		if (_case == '(')
		{
			int ret3 = getCase();
			_index++;
			if (ret3 && nextElement() == 2)
			{
				_tmp = _link;
				_tmpSign = _sign;
				if (!makeLink(co, 1))
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	_index++;
	int ret = nextElement();
	while (ret == 2 && _sign == _save) // tant qu'on trouve un signe et que le signe reste le meme
	{
		int ret2 = getCase();
		if (!ret2 && pr == 1)
		{
			_prior = false;
			return false;
		}
		else if (!ret && _case == '(')
		{
			int ret4 = getCase();
			_index++;
			if (ret4 && nextElement() == 2)
			{
				_tmp = _link;
				_tmpSign = _sign;
				if (!makeLink(co, 1))
					return false;
			}
			else
				return false;
		}
		else if (!ret2) // si on ne trouve plus de lettre a mettre dans le lien
		{
			linkToConnection(co); // ajouter le lien a la connection
			return true;
		}
		_index++;
		ret = nextElement();
	}
	if (ret == 2 && _sign != _save && pr != 0)
		return false;
	if (ret == 6 && _prior)
	{
		_tmp = _link;
		_prior = false;
		_index++;
		if (nextElement() == 2)
		{
			if (!makeLink(co, 2))
				return false;
		}
		else
			return false;
	}
	else if (ret == 6 && pr == 1)
	{
		_tmp->_prior.push_back(_link);
		_link = _tmp;
		if (!makeLink(co, 3))
			return false;
	}
	else
		linkToConnection(co);
	return true; // ou false ?
}

void		Expert::linkToConnection(int co)
{
	if (co == 1) // si c'est la premiere connection
		_firstConnection->addLink(_link);
	else // si c'est la deuxieme connection
		_secondConnection->addLink(_link);
}

int 		Expert::nextElement() // obtenir le prochain element
{

	while (_line[_index] != '\n' && _line[_index] != '\0' && _line[_index] != '#' && _index < 50)
	{
		if (checkBreak()) // s'arreter si on est sur =>
			return 4;
		if (isupper(_line[_index])) // si on trouve un lettre en majuscule
		{
			if (!checkQuery(_line[_index])) // si elle nest pas stockee, la rajouter au tableau de queries de lexpert mais faut que je verifier parce que je crois que je le fais deux fois dans le code a la suite
				_queries.push_back(new Query(_line[_index]));
			_case = _line[_index]; // stocker la lettre temporairement
			return 1;
		}
		else if (_line[_index] == '^' || _line[_index] == '|' || _line[_index] == '+') // si on trouve un signe
		{
			_sign = _line[_index]; // stocker le signe temporairement
			return 2;
		}
		else if (_line[_index] == '!') // si on trouve un not
			return 3;
		else if (_line[_index] == '(')
		{
			_case = _line[_index];
			return 5;
		}
		else if (_line[_index] == ')')
		{
			_case = _line[_index];
			return 6;
		}
		else if (_line[_index] && _line[_index] != ' ' && _line[_index] != '=' && _line[_index] != '<' && _line[_index] != '>' && _line[_index] == '\n')
		{
			std::cout << "syntax error: <" << _line[_index] << ">" << std::endl;
			exit(0);
		}
		_index++;
	}
	return 0;
}

void		Expert::clearElements() // clear toutes les elements de expert pour le passage de la prochaine ligne
{
	_line.clear();
	_case = '\0';
	_sign = '\0';
	_index = 0;
	_not = false;
	_ifonly = false;
	return ;
}

bool		Expert::checkExist() // determiner les queries existant en fonction de ceux qui sont declares existant dans le fichier de debut
{
	if (_true.size() < 1 || _find.size() < 1)
	{
		std::cout << "Syntax error" << std::endl;
		exit(0);
	}
	for (unsigned int j = 0; j < _true.size(); j++) // parcourir tous les queries existants et regarder les regles et determiner qui existe grace a eux
	{
		if (!_queries[getQuery(_true[j])]->checkImplies(this))
			return false;
	}
	if (!checkAbsentCondition()) // parcourir les regles ou l'implies est un not (ex: !H => K) et determiner qui existe grace a lui
		return false;
	return true ;
}

bool 		Expert::checkAbsentCondition() // parcourir les regles ou l'implies est un not (ex: !H => K) et determiner qui existe grace a lui
{
	for (unsigned int j = 0; j < _queries.size(); j++)
	{
		if (!_queries[j]->checkImplies(this))
			return false;
	}
	return true;
}

void		Expert::newExistant() // stocker les nouveaux queries quon a determine comme existant apres le premier passage avant de faire un deuxieme passage
{
	_true.clear();
	for (unsigned int j = 0; j < _queries.size(); j++)
	{
		if (_queries[j]->_exist == 2)
			_true.push_back(_queries[j]->getName());
	}
}

void		Expert::displayExist() // afficher les queries qui existent
{
	int indic;

	for (unsigned int i = 0; i < _find.size(); i++)
	{
		indic = 0;
		for (unsigned int j = 0; j < _queries.size(); j++)
		{
			if (_find[i] == _queries[j]->getName() && _queries[j]->_exist == 2)
			{
				indic = 1;
				std::cout << _queries[j]->getName() << " exist." << std::endl;
			}
		}
		if (indic == 0)
			std::cout << _find[i] << " doesn't exist." << std::endl;
	}
}
