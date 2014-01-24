#include <string>

#include "card.h"

Card::Card(const Card& original)
{
	this->name = original.name;
	this->cost = original.cost;
}

Card::Card(std::string name2, int cost2)
{
	name = name2;
	cost = cost2;
}