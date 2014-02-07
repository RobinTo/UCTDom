#include "Player.h"

Player::Player()
{

}

Option Player::getNextOption(GameState& currentState)
{
	uct.getNextOption(currentState, stateIndex);	// Either let AI play, or return something that the player can 'do'
	Option option; // Only temporary to avoid compile errors.
	return option;
}