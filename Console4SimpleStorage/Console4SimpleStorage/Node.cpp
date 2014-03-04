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

void Node::printSelf(std::ofstream &file)
{
	// For each child, print "counter self -> child"
	std::string text = "";

	for (std::vector<Node*>::iterator iterator = childrenPtrs.begin(); iterator != childrenPtrs.end(); ++iterator)
	{
		// Append *tchu tchu*
		text += "\"";

		// Append id
		text += std::to_string(id);

		// Append self
		if (opt.card == COPPER)
			text += "Copper";
		else if (opt.card == SILVER)
			text += "Silver";
		else if (opt.card == GOLD)
			text += "Gold";
		else if (opt.card == ESTATE)
			text += "Estate";
		else if (opt.card == DUCHY)
			text += "Duchy";
		else if (opt.card == PROVINCE)
			text += "Province";
		else if (opt.card == CURSE)
			text += "Curse";
		else
			text += "Nothing";
		
		text += "C:" + std::to_string(currentState.playerStates[0].hand[COPPER]) +
			", E:" + std::to_string(currentState.playerStates[0].hand[ESTATE]) +
			", S:" + std::to_string(currentState.playerStates[0].hand[SILVER]) +
			", D:" + std::to_string(currentState.playerStates[0].hand[DUCHY]) +
			", G:" + std::to_string(currentState.playerStates[0].hand[GOLD]) +
			", P:" + std::to_string(currentState.playerStates[0].hand[PROVINCE]) +
			", Cur:" + std::to_string(currentState.playerStates[0].hand[CURSE]);

		// Append propagateCounter
		text += " Prop:" + std::to_string(propagateCounter);

		// Append *tchu tchu*
		text += "\"";

		// Append the arrow
		text += " -> ";

		// Append *tchu tchu*
		text += "\"";

		// Append child id
		text += std::to_string((*iterator)->id);

		// Append child text
		if ((*iterator)->opt.card == COPPER)
			text += "Copper";
		else if ((*iterator)->opt.card == SILVER)
			text += "Silver";
		else if ((*iterator)->opt.card == GOLD)
			text += "Gold";
		else if ((*iterator)->opt.card == ESTATE)
			text += "Estate";
		else if ((*iterator)->opt.card == DUCHY)
			text += "Duchy";
		else if ((*iterator)->opt.card == PROVINCE)
			text += "Province";
		else if ((*iterator)->opt.card == CURSE)
			text += "Curse";
		else
			text += "Nothing";
		
		text += "C:" + std::to_string((*iterator)->currentState.playerStates[0].hand[COPPER]) +
			", E:" + std::to_string((*iterator)->currentState.playerStates[0].hand[ESTATE]) +
			", S:" + std::to_string((*iterator)->currentState.playerStates[0].hand[SILVER]) +
			", D:" + std::to_string((*iterator)->currentState.playerStates[0].hand[DUCHY]) +
			", G:" + std::to_string((*iterator)->currentState.playerStates[0].hand[GOLD]) +
			", P:" + std::to_string((*iterator)->currentState.playerStates[0].hand[PROVINCE]) +
			", Cur:" + std::to_string((*iterator)->currentState.playerStates[0].hand[CURSE]);

		
		// Append propagateCounter
		text += " Prop:" + std::to_string((*iterator)->propagateCounter);

		// Append *tchu tchu*
		text += "\";";

		text += "\r\n";
	}

	file << text;
}







