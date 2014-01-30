#include <string>

#include "Card.h"

Card::Card(const Card& original)
{
	this->name = original.name;
	this->cost = original.cost;
}

Card::Card(std::string name2, int cost2, int victoryPoints2, int treasurePoints2)
{
	name = name2;
	cost = cost2;
	victoryPoints = victoryPoints2;
	treasurePoints = treasurePoints2;
}