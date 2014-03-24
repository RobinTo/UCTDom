#pragma once

#define END_TURN	0
#define ACTION		1
#define BUY			2
#define DRAW		3
#define TRASH		4
#define GAIN		5
#define THIEFTRASH	6
#define THIEFGAIN	7

class Option
{
public:
	int type;
	int absoluteCardId;
	int absoluteExtraCardId;
};