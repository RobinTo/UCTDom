#pragma once

#include <string>









class Card
{
public:
	std::string name;
	int cost;
	int victoryPoints;
	int treasurePoints;

	Card(std::string name2, int cost2, int victoryPoints2, int treasurePoints2);
	Card(const Card& original);
};



