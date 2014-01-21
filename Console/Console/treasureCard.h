#ifndef __TREASURECARD_H__
#define __TREASURECARD_H__

#include "card.h"

class treasureCard : public card{
public:
	treasureCard(int value);
	int value;
};

#endif