#include <unordered_set>
#include <algorithm>
#include <iostream>

#include "Node.h"


Node::Node()
{
	isRoot = false;
	deck.reserve(15);
	discard.reserve(15);
	hand.reserve(5);
	children.reserve(500);
}

Node::Node(NodePool* nodePoolPtr2)
{
	isRoot = false;
	nodePoolPtr = nodePoolPtr2;
	deck.reserve(15);
	discard.reserve(15);
	hand.reserve(5);
	children.reserve(500);
}

void Node::doYourThing()
{
	if (isBuy)
	{
		std::vector<Draw> draws = findAllDraws();
		for (std::vector<Draw>::iterator iterator = draws.begin(); iterator != draws.end(); ++iterator)
		{
			children.push_back(createDrawNode(*iterator));
		}
	}
	else
	{
		std::vector<Card*> buys = findAllBuys();
		for (std::vector<Card*>::iterator iterator = buys.begin(); iterator != buys.end(); ++iterator)
		{
			children.push_back(createBuyNode(*iterator));
		}
	}
}


Node* Node::createBuyNode(Card* buy)
{
	Node* newNodePtr = nodePoolPtr->requestNewNodePtr();
	newNodePtr->isBuy = true;
	newNodePtr->deck = this->deck;
	newNodePtr->hand = this->hand;
	newNodePtr->discard = this->discard;
	newNodePtr->supplyPiles = this->supplyPiles;
	
	if (buy != NULL)
	{
		newNodePtr->supplyPiles[buy->name].cardsLeft --;
		newNodePtr->discard.push_back(buy);
	}
	newNodePtr->boughtCard = buy;

	return newNodePtr;
}

Node* Node::createDrawNode(Draw draw)
{
	Node* newNodePtr = nodePoolPtr->requestNewNodePtr();
	newNodePtr->isBuy = false;
	newNodePtr->deck = draw.currentDeck;
	newNodePtr->hand = draw.currentDraw;
	newNodePtr->discard = this->discard;
	newNodePtr->supplyPiles = this->supplyPiles;
	newNodePtr->boughtCard = NULL;

	return newNodePtr;
}

std::vector<Card*> Node::findAllBuys()
{
	int money = 0;
	for (std::vector<Card*>::iterator iterator = hand.begin(); iterator != hand.end(); ++iterator)
	{
		money += (*iterator)->treasurePoints;
	}

	std::vector<Card*> buys;
	for (std::unordered_map<std::string, SupplyPile>::iterator iterator = supplyPiles.begin(); iterator != supplyPiles.end(); ++iterator)
	{
		if (money >= iterator->second.cardPtr->cost)
			buys.push_back(iterator->second.cardPtr);
	}

	buys.push_back(NULL);

	return buys;
}

std::vector<Draw> Node::findAllDraws()
{
	//Discard hand after buy
	for (std::vector<Card*>::iterator iterator = hand.begin(); iterator != hand.end(); ++iterator)
	{
		discard.push_back(*iterator);
	}
	hand.clear();


	std::vector<Draw> draws;
	draws.reserve(5);

	//if (deck.size() < 5)
	//{	
		draws.push_back(Draw(deck));

		for (int sizeCounter = 0; sizeCounter < 5; sizeCounter++)
		{
			std::vector<Draw> newDraws;
			newDraws.reserve(5);
			bool deckEmpty = false;
			for (std::vector<Draw>::iterator drawIterator = draws.begin(); drawIterator != draws.end(); ++drawIterator)
			{
				if (drawIterator->currentDeck.size() <= 0)
				{
					std::vector<Card*> discardCopy = discard;
					discardCopy.reserve(15);
					deckEmpty = true;
					while (discardCopy.size() > 0)
					{		
						drawIterator->currentDeck.push_back(discardCopy.back());
						discardCopy.pop_back();
					}
				}
				std::vector<Card*> uniqueCards = findUniqueCards(drawIterator->currentDeck);
				for (std::vector<Card*>::iterator iterator = uniqueCards.begin(); iterator != uniqueCards.end(); ++iterator)
				{
					Draw newDraw(drawIterator->currentDeck, drawIterator->currentDraw, *iterator);
					newDraw.currentDraw.push_back(*iterator);
					newDraws.push_back(newDraw);
				}
			}
			if (deckEmpty)
				discard.clear();
			draws.clear();
			draws = newDraws;
		}
	//}
	//else
	//{
	//	//initialize by creating draws for one card.
	//	std::vector<Draw> currentDraws;
	//	std::vector<Card*> uniqueCards = findUniqueCards(deck);
	//	for (std::vector<Card*>::iterator iterator = uniqueCards.begin(); iterator != uniqueCards.end(); ++iterator)
	//	{
	//		Draw newDraw(deck, *iterator);
	//		newDraw.currentDraw.push_back(*iterator);
	//		currentDraws.push_back(newDraw);
	//	}

	//	//Continue to find the four remaining cards.
	//	for (int counter = 0; counter < 4; counter++)
	//	{
	//		std::vector<Draw> newDraws;
	//		// Do this for each draw created
	//		for (std::vector<Draw>::iterator drawIterator = currentDraws.begin(); drawIterator != currentDraws.end(); ++drawIterator)
	//		{
	//			if (drawIterator->currentDeck.size() <= 0)
	//			{
	//				while (discard.size() > 0)
	//				{		
	//					drawIterator->currentDeck.push_back(discard.back());
	//					discard.pop_back();
	//				}
	//			}

	//			std::vector<Card*> uniqueCards = findUniqueCards(drawIterator->currentDeck);
	//			//Do this for each unique card
	//			for (std::vector<Card*>::iterator cardIterator = uniqueCards.begin(); cardIterator != uniqueCards.end(); ++cardIterator)
	//			{
	//				Draw newDraw(drawIterator->currentDeck, drawIterator->currentDraw, *cardIterator);
	//				newDraw.currentDraw.push_back(*cardIterator);
	//				newDraws.push_back(newDraw);
	//			}
	//		}
	//		currentDraws.clear();
	//		currentDraws = newDraws;
	//	}
	//	
	//	
	//	//Remove duplicates
	//	//currentDraws = removeDuplicates(currentDraws);

	//	//Convert draws to vector<Card*>
	//	/*for (std::vector<Draw>::iterator iterator = currentDraws.begin(); iterator != currentDraws.end(); ++iterator)
	//	{
	//		draws.push_back(iterator->currentDraw);
	//	}*/
	//	draws = currentDraws;
	//}
	
	return draws;
}

std::vector<Card*> Node::findUniqueCards(std::vector<Card*> pile)
{
	std::unordered_set<Card*> uniqueCards(pile.begin(),pile.end());
	pile.resize(uniqueCards.size());
	std::copy(uniqueCards.begin(), uniqueCards.end(), pile.begin());

	return pile;
}

std::vector<Draw> Node::removeDuplicates(std::vector<Draw> draws)
{
	//Count card types, before removing duplicates.
	for (std::vector<Draw>::iterator drawIterator = draws.begin(); drawIterator != draws.end(); ++drawIterator)
	{	
		for (std::vector<Card*>::iterator cardIterator = drawIterator->currentDraw.begin(); cardIterator != drawIterator->currentDraw.end(); ++cardIterator)
		{	
			if ((*cardIterator)->name == "Copper")
				drawIterator->copper++;
			else if((*cardIterator)->name == "Estate")
				drawIterator->estate++;
			else if((*cardIterator)->name == "Silver")
				drawIterator->silver++;
			else if((*cardIterator)->name == "Duchy")
				drawIterator->duchy++;
			else if((*cardIterator)->name == "Gold")
				drawIterator->gold++;
			else
				drawIterator->province++;
		}
	}

	for (std::vector<Draw>::iterator drawIterator = draws.begin(); drawIterator != draws.end(); ++drawIterator)
	{

	}

	return draws;
}

void Node::printSelf(int currentDepth)
{
	if (isBuy)
	{
		if (boughtCard == NULL)
			std::cout << "Buy:" << "Nothing" << std::endl;
		else
			std::cout << "Buy:" << boughtCard->name << std::endl;
	}
	else
	{
		std::cout << "Draw:" << hand[0]->name << "," << hand[1]->name << "," << hand[2]->name << "," << hand[3]->name << "," << hand[4]->name << std::endl;
	}
	
	if (children.size() > 0)
	{
		for (std::vector<Node*>::iterator iterator = children.begin(); iterator != children.end(); ++iterator)
		{
			for (int counter = 0; counter < currentDepth; counter++)
				std::cout << "->";
			(*iterator)->printSelf(currentDepth+1);
		}
	}
}

