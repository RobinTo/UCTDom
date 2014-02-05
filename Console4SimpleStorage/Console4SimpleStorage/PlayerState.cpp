#include "PlayerState.h"

PlayerState::PlayerState()
{
	for (int index = 0; index < 6; index++)
	{
		deck[index] = 0;
		discard[index] = 0;
		hand[index] = 0;
		inPlay[index] = 0;
	}
}