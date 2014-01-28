#include "playerstate.h"

PlayerState::PlayerState()
{
	actionsLeft = 0;
	buysLeft = 0;
	moneyLeft = 0;

	deck.reserve(100);
	hand.reserve(100);
	discard.reserve(100);
	inPlay.reserve(100);
}