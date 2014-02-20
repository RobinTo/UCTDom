#include "Node.h"

Node::Node()
{
	reset();
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
	untriedMoves.clear();
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

	for (int i = 0; i < INSUPPLY; i++)
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