#include <algorithm>
#include <iostream>

#include "player.h"
#include "card.h"

Player::Player(GameState* gameStatePtr2, std::vector<PlayerState*> otherStatePtrs2, PlayerState* selfStatePtr2)
{
	gameStatePtr = gameStatePtr2;
	otherStatePtrs = otherStatePtrs2;
	selfStatePtr = selfStatePtr2;
}

void Player::startUp(std::vector<Card*> startDeck)
{
	selfStatePtr->discard = startDeck;
	shuffle();
	draw(5);
}

void Player::playTurn()
{
	selfStatePtr->actionsLeft = 1;
	selfStatePtr->buysLeft = 1;
	selfStatePtr->moneyLeft = 0;
	// Heavy if else for now.
	for (std::vector<Card*>::iterator iterator = selfStatePtr->hand.begin(); iterator != selfStatePtr->hand.end(); ++iterator)
	//std::vector<Card*>::iterator iterator = selfStatePtr->hand.begin();
	//while (iterator != selfStatePtr->hand.end()) 
	{
		if ((*iterator)->name == "Estate" || (*iterator)->name == "Duchy" || (*iterator)->name == "Province")
		{
			//Do nothing
		}
		else
		{
			if ((*iterator)->name == "Copper")
			{
				selfStatePtr->moneyLeft += 1;
			}
			else if ((*iterator)->name == "Silver")
			{
				selfStatePtr->moneyLeft += 2;
			}
			else if ((*iterator)->name == "Gold")
			{
				selfStatePtr->moneyLeft += 3;
			}
			play(*iterator);
			selfStatePtr->hand.erase(iterator);
			iterator = selfStatePtr->hand.begin();
		}
	}

	// DO AI
	std::cout << "Money: " << selfStatePtr->moneyLeft << std::endl;

}

void Player::endTurn()
{
	selfStatePtr->actionsLeft = 0;
	selfStatePtr->buysLeft = 0;
	selfStatePtr->moneyLeft = 0;

	while(!selfStatePtr->inPlay.empty())
	{
		selfStatePtr->discard.push_back(selfStatePtr->inPlay.back());
		selfStatePtr->inPlay.pop_back();
	}
	while(!selfStatePtr->hand.empty())
	{
		selfStatePtr->discard.push_back(selfStatePtr->hand.back());
		selfStatePtr->hand.pop_back();
	}
	draw(5);
}


//Private functions
void Player::draw(int numberOfCards) // Needs a check on whether there are cards left (i.e. player has below five cards in total).
{
	for (int i = 0; i < numberOfCards; i++)
	{
		if (selfStatePtr->deck.empty())
		{
			shuffle();
		}
		selfStatePtr->hand.push_back(selfStatePtr->deck.back());
		selfStatePtr->deck.pop_back();
	}
}

void Player::discard(Card* cardPtr)
{
	//TODO: Implement discard. Depending on how the card to be discarded is found. Discard should use that information, so as to improve performance.
	//(Don't need to search for the card again).
}

void Player::gain(Card* cardPtr)
{
	selfStatePtr->discard.push_back(cardPtr);
}

void Player::play(Card* cardPtr)
{
	selfStatePtr->inPlay.push_back(cardPtr);
	//Delete card from hand.
}

void Player::shuffle()
{
	std::random_shuffle(selfStatePtr->discard.begin(), selfStatePtr->discard.end());
	while(!selfStatePtr->discard.empty())
	{
		selfStatePtr->deck.push_back(selfStatePtr->discard.back());
		selfStatePtr->discard.pop_back();
	}
}