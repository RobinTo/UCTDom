#include "Player.h"

Player::Player()
{

}

void Player::setCardManager(CardManager& cardManagerRef)
{
	cardManager = cardManagerRef;
}

Option Player::getNextOption(GameState& currentState)
{
	Option option = uct.getNextOption(currentState, stateIndex);
	return option;
}