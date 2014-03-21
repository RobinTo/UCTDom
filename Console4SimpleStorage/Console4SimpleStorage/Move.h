#pragma once

#include <string>
#include "Option.h"


class Move : public Option
{
public:
	std::string moveString;
	int player;

};