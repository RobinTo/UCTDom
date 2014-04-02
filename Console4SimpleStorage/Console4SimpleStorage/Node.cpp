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

//void Node::setState(GameState state)
//{
//	currentState = state;
//}

void Node::setOption(Option o)
{
	opt = o;
}

void Node::printSelf(std::ofstream &file) // TODO: Add support for printing each option, such as multiple action cards. (Print to multiple files)
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

		/*if (opt.absoluteCardId == COPPER)
			text += "Copper";
		else if (opt.absoluteCardId == SILVER)
			text += "Silver";
		else if (opt.absoluteCardId == GOLD)
			text += "Gold";
		else if (opt.absoluteCardId == ESTATE)
			text += "Estate";
		else if (opt.absoluteCardId == DUCHY)
			text += "Duchy";
		else if (opt.absoluteCardId == PROVINCE)
			text += "Province";
		else if (opt.absoluteCardId == CURSE)
			text += "Curse";
		else if (opt.absoluteCardId == MARKET)
			text += "Market";
		else if (opt.absoluteCardId == VILLAGE)
			text += "Village";
		else if (opt.absoluteCardId == LABORATORY)
			text += "Laboratory";
		else if (opt.absoluteCardId == SMITHY)
			text += "Smithy";
		else if (opt.absoluteCardId == WOODCUTTER)
			text += "Woodcutter";
		else if (opt.absoluteCardId == WITCH)
			text += "Witch";
		else if (opt.absoluteCardId == MONEYLENDER)
			text += "Moneylender";
		else if (opt.absoluteCardId == FESTIVAL)
			text += "Festival";
		else if (opt.absoluteCardId == GARDENS)
			text += "Gardens";
		else if (opt.type == DRAW)
			text += "DrawCards";
		else
			text += "EndTurn";*/
		
		for (int index = 0; index < INSUPPLY; index++)
		{
			int cardsOfType = currentState.playerStates[playerPlaying].hand[index];
			if (cardsOfType > 0)
			{
				text += " " + CardManager::cardLookupByIndex[index].name + ":" + std::to_string(cardsOfType);
			}
		}

		/*text += "C:" + std::to_string(currentState.playerStates[playerPlaying].hand[COPPER]) +
			", E:" + std::to_string(currentState.playerStates[playerPlaying].hand[ESTATE]) +
			", S:" + std::to_string(currentState.playerStates[playerPlaying].hand[SILVER]) +
			", D:" + std::to_string(currentState.playerStates[playerPlaying].hand[DUCHY]) +
			", G:" + std::to_string(currentState.playerStates[playerPlaying].hand[GOLD]) +
			", P:" + std::to_string(currentState.playerStates[playerPlaying].hand[PROVINCE]) +
			", Cur:" + std::to_string(currentState.playerStates[playerPlaying].hand[CURSE]);
*/
		// Append visited
		text += " Vis:" + std::to_string(visited);

		// Append value
		text += " Val:" + std::to_string(value);

		// Append probability
		text += " Prob:" + std::to_string(probability);

		// Append currentplayer
		text += " Player:" + std::to_string(playerPlaying);

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
		//if ((*iterator)->opt.absoluteCardId == COPPER)
		//	text += "Copper";
		//else if ((*iterator)->opt.absoluteCardId == SILVER)
		//	text += "Silver";
		//else if ((*iterator)->opt.absoluteCardId == GOLD)
		//	text += "Gold";
		//else if ((*iterator)->opt.absoluteCardId == ESTATE)
		//	text += "Estate";
		//else if ((*iterator)->opt.absoluteCardId == DUCHY)
		//	text += "Duchy";
		//else if ((*iterator)->opt.absoluteCardId == PROVINCE)
		//	text += "Province";
		//else if ((*iterator)->opt.absoluteCardId == CURSE)
		//	text += "Curse";
		//else if ((*iterator)->opt.absoluteCardId == MARKET)
		//	text += "Market";
		//else if ((*iterator)->opt.absoluteCardId == VILLAGE)
		//	text += "Village";
		//else if ((*iterator)->opt.absoluteCardId == LABORATORY)
		//	text += "Laboratory";
		//else if ((*iterator)->opt.absoluteCardId == SMITHY)
		//	text += "Smithy";
		//else if ((*iterator)->opt.absoluteCardId == WOODCUTTER)
		//	text += "Woodcutter";
		//else if ((*iterator)->opt.absoluteCardId == WITCH)
		//	text += "Witch";
		//else if ((*iterator)->opt.absoluteCardId == MONEYLENDER)
		//	text += "Moneylender";
		//else if ((*iterator)->opt.absoluteCardId == FESTIVAL)
		//	text += "Festival";
		//else if ((*iterator)->opt.absoluteCardId == GARDENS)
		//	text += "Gardens";
		//else if ((*iterator)->opt.type == DRAW)
		//	text += "DrawCards";
		//else
		//	text += "EndTurn";

		/*text += "C:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[COPPER]) +
			", E:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[ESTATE]) +
			", S:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[SILVER]) +
			", D:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[DUCHY]) +
			", G:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[GOLD]) +
			", P:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[PROVINCE]) +
			", Cur:" + std::to_string((*iterator)->currentState.playerStates[(*iterator)->playerPlaying].hand[CURSE]);
*/
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

		// Append *tchu tchu*
		text += "\";";

		text += "\r\n";
	}

	file << text;
}







