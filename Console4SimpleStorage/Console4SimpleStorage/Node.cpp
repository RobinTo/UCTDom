#include "Node.h"

Node::Node()
{
	isRoot = false;
	value = 0;
	visited = 0;
	sum = 0;
	propagateCounter = 0;
	parentPtr = nullptr;
}

Node::~Node()
{

}

void Node::reset()
{
	isRoot = false;
	value = 0;
	visited = 0;
	sum = 0;
	propagateCounter = 0;
	childrenPtrs.clear();
	parentPtr = nullptr;
}

void Node::setState(GameState state)
{
	currentState = state;
}

void Node::setOption(Option o)
{
	opt = o;
}

void Node::findMoves(CardManager cardManager)
{
	int currentMoney = 0;
	currentMoney += currentState.playerStates[0].hand[cardManager.cardIndexer[COPPER]];
	currentMoney += currentState.playerStates[0].hand[cardManager.cardIndexer[SILVER]] * 2;
	currentMoney += currentState.playerStates[0].hand[cardManager.cardIndexer[GOLD]] * 3;

	for (int i = 0; i < sizeof(currentState.playerStates[0].hand) / sizeof(*currentState.playerStates[0].hand); i++)
	{
		if (cardManager.cardLookupByIndex[i].cost <= currentMoney && currentState.supplyPiles[i] > 0)
		{
			Option o;
			o.type = BUY;
			o.card = i;
			untriedMoves.push_back(o);
		}
	}
}