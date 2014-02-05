#include "GameState.h"

GameState::GameState()
{
	for (int index = 0; index < 6; index++)
	{
		supplyPiles[index] = 0;
		trash[index] = 0;
	}
}