#pragma once

#define COPPER		0
#define SILVER		1
#define GOLD		2
#define ESTATE		3
#define DUCHY		4
#define PROVINCE	5
#define SUPERCARD	6
#define TOTALCARDS	7
#define PLAYERS		4

class PlayerState
{
public:
	int deck[TOTALCARDS];
	int discard[TOTALCARDS];
	int hand[TOTALCARDS];
	int inPlay[TOTALCARDS];

	PlayerState();
};