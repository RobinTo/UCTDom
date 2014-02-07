#pragma once

#include <string>

#define COPPER		0
#define SILVER		1
#define GOLD		2
#define ESTATE		3
#define DUCHY		4
#define PROVINCE	5
#define SUPERCARD	6
#define TOTALCARDS	7

struct Card
{
	int cost;
	std::string name;

}

class CardManager
{
	std::unordered_map<int, int> cardIndexer;	//deck[cardIndexer[COPPER]]
	std::unordered_map<int, Card> cardLookup;	//cardLookup[COPPER]

	
};
	