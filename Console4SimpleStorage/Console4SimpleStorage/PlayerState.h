#pragma once

#include "CardManager.h"

class PlayerState
{
public:
	int deck[TOTALCARDS];
	int discard[TOTALCARDS];
	int hand[TOTALCARDS];
	int inPlay[TOTALCARDS];

	PlayerState();
};