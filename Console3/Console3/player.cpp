#include "player.h"
#include "card.h"

player::player()
{
	srand(time(NULL));
}

void player::buyCard(card* c)
{
	discard.push_back(c);
}

void player::drawCard()
{
	if (deck.empty())
	{
		shuffle();
	}

	hand.push_back(deck.back());
	deck.pop_back();
}

void player::endTurn()
{
	while (!hand.empty())
	{
		discard.push_back(hand.back());
		hand.pop_back();
	}
	while (!inPlay.empty())
	{
		discard.push_back(inPlay.back());
		inPlay.pop_back();
	}
	drawCard();
	drawCard();
	drawCard();
	drawCard();
	drawCard();
}

void player::shuffle()
{
	while (!discard.empty())
	{
		int cardPos = rand() % discard.size();
		deck.push_back(discard.at(cardPos));
		discard.erase(discard.begin()+cardPos);
	}
}

int player::moneyInHand()
{
	int coins = 0;
	for(int i = 0; i < hand.size(); i++)
	{
		if (hand.at(i)->name == "Copper")
			coins += 1;
		else if (hand.at(i)->name == "Silver")
			coins += 2;
		else if (hand.at(i)->name == "Gold")
			coins += 3;
	}
	return coins;
}

int player::victoryPoints()
{
	int vp = 0;
	for(int i = 0; i < hand.size(); i++)
	{
		if (hand.at(i)->name == "Estate")
			vp += 1;
		else if (hand.at(i)->name == "Duchy")
			vp += 3;
		else if (hand.at(i)->name == "Province")
			vp += 6;
	}
	for(int i = 0; i < inPlay.size(); i++)
	{
		if (inPlay.at(i)->name == "Estate")
			vp += 1;
		else if (inPlay.at(i)->name == "Duchy")
			vp += 3;
		else if (inPlay.at(i)->name == "Province")
			vp += 6;
	}
	for(int i = 0; i < discard.size(); i++)
	{
		if (discard.at(i)->name == "Estate")
			vp += 1;
		else if (discard.at(i)->name == "Duchy")
			vp += 3;
		else if (discard.at(i)->name == "Province")
			vp += 6;
	}
	for(int i = 0; i < deck.size(); i++)
	{
		if (deck.at(i)->name == "Estate")
			vp += 1;
		else if (deck.at(i)->name == "Duchy")
			vp += 3;
		else if (deck.at(i)->name == "Province")
			vp += 6;
	}
	return vp;
}