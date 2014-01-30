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
		std::vector<std::vector<Card*>> draws = findAllDraws();
		for (std::vector<std::vector<Card*>>::iterator iterator = draws.begin(); iterator != draws.end(); ++iterator)
		{
			createDrawNode(*iterator);
		}
	}
	else
	{
		std::vector<Card*> buys = findAllBuys();
		for (std::vector<Card*>::iterator iterator = buys.begin(); iterator != buys.end(); ++iterator)
		{
			createBuyNode(*iterator);
		}
	}
}


void Node::createBuyNode(Card* buy)
{
	Node* newNode = nodePoolPtr->requestNewNodePtr();
	newNode->isBuy = true;
	newNode->deck = this->deck;
	newNode->hand = this->hand;
	newNode->discard = this->discard;
	newNode->supplyPiles = this->supplyPiles;
	
	if (buy != NULL)
		newNode->supplyPiles[buy->name].cardsLeft --;
	newNode->boughtCard = buy;
}

void Node::createDrawNode(std::vector<Card*> draw)
{

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

std::vector<std::vector<Card*>> Node::findAllDraws()
{
	std::vector<std::vector<Card*>> draws;

	return draws;
}