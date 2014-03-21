#pragma once

#include <string>
#include "Option.h"


class Move : public Option
{
public:
	std::string moveString;
	int player;

	Move();
	Move(Option option, int playerr);
};