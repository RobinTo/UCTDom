#ifndef __VICTORYCARD_H__
#define __VICTORYCARD_H__

#include "card.h"

class victoryCard : public card{
public:
	victoryCard(int v)
	{
		vp = v;
	}
	int vp;
};

#endif