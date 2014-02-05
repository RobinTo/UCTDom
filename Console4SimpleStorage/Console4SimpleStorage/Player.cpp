#include "Player.h"

Player::Player()
{

}

void Player::playTurn(GameState& currentState)
{
	uct.findBestChoice(currentState);	// Either let AI play, or return something that the player can 'do'
}