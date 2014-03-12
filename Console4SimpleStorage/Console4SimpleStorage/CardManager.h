#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#define COPPER		0
#define SILVER		1
#define GOLD		2
#define ESTATE		3
#define DUCHY		4
#define PROVINCE	5
#define CURSE		6
#define ADVENTURER	7
#define BUREAUCRAT	8
#define CELLAR		9
#define CHANCELLOR	10
#define CHAPEL		11
#define COUNCILROOM	12
#define FEAST		13
#define FESTIVAL	14
#define GARDENS		15
#define LABORATORY	16
#define LIBRARY		17
#define MARKET		18
#define MILITIA		19
#define MINE		20
#define MOAT		21
#define MONEYLENDER	22
#define REMODEL		23
#define SMITHY		24
#define SPY			25
#define THIEF		26
#define THRONEROOM	27
#define VILLAGE		28
#define WITCH		29
#define WOODCUTTER	30
#define WORKSHOP	31

#define TOTALCARDS	32
#define INSUPPLY	10


struct Card
{
	int cost;
	std::string name;
	int id;

};
// copper, silver, gold, estate, duchy, province, curse,
//	thief, militia, spy, cellar, chapel, chancellor, workshop, feast, remodel, throneRoom, library, mine, moat,	// Choice cards
//	councilRoom, moneylender, adventurer, bureaucrat, witch,													// Complex cards
//	market, laboratory, festival, village, smithy, woodcutter,													// Simple cards
//	gardens;																									// Victory card

class CardManager
{
public:
	std::unordered_map<int, int> cardIndexer;			//deck[cardIndexer[COPPER]]
	std::unordered_map<int, Card> cardLookup;			//cardLookup[7] returns adventurer
	std::unordered_map<int, Card> cardLookupByIndex;	//cardLookupByIndex[7] returns whatever is at index 7

	void initialize();
};
	