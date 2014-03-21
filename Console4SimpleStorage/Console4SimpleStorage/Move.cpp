#include "Move.h"

Move::Move()
{

}

Move::Move(Option option, int playerr)
{
	player = playerr;
	absoluteCardId = option.absoluteCardId;
	type = option.type;
}