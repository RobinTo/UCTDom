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
	childrenPtrs.clear();
	std::vector<Node*>().swap(childrenPtrs);
	currentState.~currentState();

	isRoot = false;
	value = 0;
	visited = 0;
	sum = 0;
	probability = 0;
	parentPtr = nullptr;
	playerPlaying = -1;
	opt.absoluteCardId = -1;
	opt.type = -1;

	flags = NOFLAG;
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
		/*if ((*iterator)->visited == 0)
			continue;*/

		// Append *tchu tchu*
		text += "\"";

		// Append id
		text += std::to_string(id);

		// Append self
		text += " Type:" + std::to_string(opt.type);
		text += CardManager::cardLookup[opt.absoluteCardId].name;
		
		for (int index = 0; index < INSUPPLY; index++)
		{
			int cardsOfType = currentState.playerStates[playerPlaying].hand[index];
			if (cardsOfType > 0)
			{
				text += " " + CardManager::cardLookupByIndex[index].name + ":" + std::to_string(cardsOfType);
			}
		}

		// Append visited
		text += " Vis:" + std::to_string(visited);

		// Append value
		text += " Val:" + std::to_string(value);

		// Append probability
		text += " Prob:" + std::to_string(probability);

		// Append currentplayer
		text += " Player:" + std::to_string(playerPlaying);

		// Append turnCounter
		text += " Turn:" + std::to_string(currentState.turnCounter);

		// Append *tchu tchu*
		text += "\"";

		// Append the arrow
		text += " -> ";

		// Append *tchu tchu*
		text += "\"";

		// Append child id
		text += std::to_string((*iterator)->id);

		// Append child text
		text += " Type:" + std::to_string((*iterator)->opt.type);
		text += CardManager::cardLookup[(*iterator)->opt.absoluteCardId].name;
		
		for (int index = 0; index < INSUPPLY; index++)
		{
			int cardsOfType = (*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[index];
			if (cardsOfType > 0)
			{
				text += " " + CardManager::cardLookupByIndex[index].name + ":" + std::to_string(cardsOfType);
			}
		}
		
		// Append visited
		text += " Vis:" + std::to_string((*iterator)->visited);

		// Append value
		text += " Val:" + std::to_string((*iterator)->value);

		// Append probability
		text += " Prob:" + std::to_string((*iterator)->probability);

		// Append currentplayer
		text += " Player:" + std::to_string((*iterator)->playerPlaying);

		// Append turnCounter
		text += " Turn:" + std::to_string((*iterator)->currentState.turnCounter);

		// Append *tchu tchu*
		text += "\";";

		text += "\r\n";
	}

	file << text;
}







