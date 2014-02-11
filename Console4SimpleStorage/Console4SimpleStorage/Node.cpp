#include "Node.h"

Node::Node()
{
	isRoot = false;
	value = 0;
	visited = 0;
	sum = 0;
	propagateCounter = 0;
}

void Node::setState(GameState state)
{
	currentState = state;
}

void Node::setOption(Option o)
{
	opt = o;
}