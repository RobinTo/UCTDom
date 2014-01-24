#include <string>





#ifndef __CARD_H_
#define __CARD_H_

class Card
{
public:
	std::string name;
	int cost;

	Card(std::string name2, int cost2);
	Card(const Card& original);
};



#endif