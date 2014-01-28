#include <algorithm>
#include <iostream>

#include "player.h"
#include "card.h"

Player::Player(GameState* gameStatePtr2, std::vector<PlayerState*> otherStatePtrs2, PlayerState* selfStatePtr2)
{
	gameStatePtr = gameStatePtr2;
	otherStatePtrs = otherStatePtrs2;
	selfStatePtr = selfStatePtr2;
	uctai.initialize();
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
	std::vector<Card*>::iterator iterator = selfStatePtr->hand.begin();
	while (iterator != selfStatePtr->hand.end()) 
    {
		if ((*iterator)->name == "Estate" || (*iterator)->name == "Duchy" || (*iterator)->name == "Province")
		{
			++iterator;
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
			selfStatePtr->inPlay.push_back(*iterator);
			iterator = selfStatePtr->hand.erase(iterator);
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

void Player::gain(Card* cardPtr)
{
	selfStatePtr->discard.push_back(cardPtr);
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