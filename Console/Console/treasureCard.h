#ifndef __TREASURECARD_H__
#define __TREASURECARD_H__

#include "card.h"

class treasureCard : public card{
public:
	treasureCard();
	treasureCard(int v)
	{
		value = v;
	};
	int value;
};

#endif