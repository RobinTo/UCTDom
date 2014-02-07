#pragma once

#define END_TURN	0
#define ACTION		1
#define BUY			2

class Option
{
public:
	int type;
	int card;
};