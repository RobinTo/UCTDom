#pragma once

#include "CardManager.h"

class PlayerState
{
public:
	int deck[INSUPPLY];
	int discard[INSUPPLY];
	int hand[INSUPPLY];
	int inPlay[INSUPPLY];

	int buys;
	int actions;

	PlayerState();
};