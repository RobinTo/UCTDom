#include "UCTMonteCarlo.h"
#include <iostream>
#include "CardManager.h"
#include <array>

Option UCTMonteCarlo::doUCT(int maxSimulations, int UCTPlayer, GameState gameState)
{
	// Create inital root node and its children.
	Node* rootNode = requestNewNode();
	//Node* rootNode = new Node();
	rootNode->isRoot = true;
	rootNode->visited = 0;
	rootNode->currentState = gameState;
	rootNode->playerPlaying = UCTPlayer;
	createAllChildren(rootNode);

	if (rootNode->childrenPtrs.size() == 1)
		return rootNode->childrenPtrs.at(0)->opt;

	// Perform UCT
	for (int i = 0; i < maxSimulations; i++)
	{
		expand(select(rootNode), UCTPlayer);
	}

	// Return best option
	Option bestOption;
	int mostVisited = 0;
	for (int i = 0; i < rootNode->childrenPtrs.size(); i++)
	{
		std::cout << cardManager.cardLookupByIndex[rootNode->childrenPtrs.at(i)->opt.card].name;
		std::cout << " visited: " << rootNode->childrenPtrs.at(i)->visited;
		std::cout << " score: " << rootNode->childrenPtrs.at(i)->value << std::endl;

		if (rootNode->childrenPtrs.at(i)->visited > mostVisited)
		{
			bestOption = rootNode->childrenPtrs.at(i)->opt;
			mostVisited = rootNode->childrenPtrs.at(i)->visited;
		}
	}
	//printTree(gameState.turnCounter, rootNode);
	resetNodes();
	return bestOption;
}

Node* UCTMonteCarlo::select(Node* root)
{
	Node* currentNode = root;

	while (currentNode->childrenPtrs.size() > 0 && getUntriedChildren(currentNode).size() == 0)
	{
		//currentNode->visited++;
		currentNode = UCTSelectChild(currentNode);
	}
	return currentNode;
}

void UCTMonteCarlo::expand(Node* node, int UCTPlayer)
{
	if (getUntriedChildren(node).size() > 0)
	{
		Node* randomNode = getRandomNode(getUntriedChildren(node));
		//randomNode->visited++;
		rollout(randomNode, randomNode->currentState, UCTPlayer);
	}
	else
	{

		if (node->currentState.gameFinished())
		{
			rollout(node, node->currentState, UCTPlayer);
		}
		else
		{
			createAllChildren(node);
			Node* randomNode = getRandomNode(node->childrenPtrs);
			//randomNode->visited++;
			rollout(randomNode, randomNode->currentState, UCTPlayer);
		}
	}
}

// UCTPlayer is which player we are currently performing UCT for.
void UCTMonteCarlo::rollout(Node* node, GameState gameState, int UCTPlayer)
{
	int currentPlayer = node->playerPlaying;
	if (node->opt.type == END_TURN)
		currentPlayer++;
	if (currentPlayer >= gameState.playerStates.size())
	{
		gameState.turnCounter++;
		currentPlayer = 0;
	}
	while (!gameState.gameFinished())
	{
		while (gameState.playerStates[currentPlayer].buys > 0)
		{
			int cardChosen = getCardPlayoutPolicy(gameState, currentPlayer);
			gameState.playerStates[currentPlayer].buyCard(cardManager, cardChosen);
			gameState.supplyPiles[cardManager.cardIndexer[cardChosen]] -= 1;
		}
		gameState.playerStates[currentPlayer].endTurn();
		currentPlayer++;
		if (currentPlayer >= gameState.playerStates.size())
			currentPlayer = 0;
		if (currentPlayer == 0)
			gameState.turnCounter++;
	}

	propagate(node, gameState.playerStates[UCTPlayer].calculateVictoryPoints(cardManager));
}

void UCTMonteCarlo::propagate(Node* node, double score)
{
	node->sum += score;
	node->visited++;
	node->value = (double)node->sum / (double)node->visited;
	if (node->opt.type == DRAW)
		node->value *= node->probability;
	if (!node->isRoot)
		propagate(node->parentPtr, score);
}

// Returns best child according to UCT.
Node* UCTMonteCarlo::UCTSelectChild(Node* root)
{
	double bestValue = 0;
	Node* bestNode;
	for (int i = 0; i < root->childrenPtrs.size(); i++)
	{
		double value = (double)root->childrenPtrs.at(i)->value + 10 * sqrt(log((double)root->visited / root->childrenPtrs.at(i)->visited));

		if (value >= bestValue || bestValue == 0)
		{
			bestValue = value;
			bestNode = root->childrenPtrs.at(i);
		}
	}

	return bestNode;
}

void UCTMonteCarlo::createAllChildren(Node* node)
{
	// If parent is end turn, end old players turn and increment turn counter.

	int currentlyPlaying = node->playerPlaying;
	GameState currentState = node->currentState;
	if (node->opt.type == END_TURN)
	{
		currentlyPlaying++;
		if (currentlyPlaying >= node->currentState.playerStates.size())
		{
			currentlyPlaying = 0;
			currentState.turnCounter++;
		}
		currentState.playerStates[currentlyPlaying].buys = 1;
		currentState.playerStates[currentlyPlaying].actions = 1;
	}

	if (node->opt.type == END_TURN)
	{
		// Clean-up
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
		{
			currentState.playerStates[currentlyPlaying].discard[cardIndex] += currentState.playerStates[currentlyPlaying].hand[cardIndex];
			currentState.playerStates[currentlyPlaying].hand[cardIndex] = 0;
			// TODO: move from inplay to discard
		}

		GameState copyState = currentState;

		// Check for shuffle
		int cardCounter = 0;
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
		{
			cardCounter += copyState.playerStates[currentlyPlaying].deck[cardIndex];
		}
		std::array<int, INSUPPLY> guaranteedCards = {0, 0, 0, 0, 0, 0, 0 }; // Not dynamic, must be manually set if INSUPPLY changes!
		if (cardCounter < 5)
		{
			for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
			{
				guaranteedCards[cardIndex] = copyState.playerStates[currentlyPlaying].deck[cardIndex];
				copyState.playerStates[currentlyPlaying].deck[cardIndex] = copyState.playerStates[currentlyPlaying].discard[cardIndex];
				copyState.playerStates[currentlyPlaying].discard[cardIndex] = 0;
			}
		}

		// Finding n and k for draw probability
		int n = 0; // Total cards - those in guaranteedCards and discard. Possible cards to draw.
		std::size_t k = 5;
			// If <= than 5 in cardCounter, then draw fewer cards.
		if (cardCounter < 5)
			k = 5 - cardCounter;
		
		// Append each card in deck to string
		std::string s = "";
		for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
		{
			for (int j = 0; j < copyState.playerStates[currentlyPlaying].deck[cardIndex]; j++)
			{
				s.append(std::to_string(static_cast<long long>(cardIndex)));
				n++;
			}
		}


		// While there are still new combinations of chars in string, create a new draw.
		std::vector<std::array<int, INSUPPLY>> draws;
		std::vector<double> probabilities;
		do
		{
			double probability = 0, nkInCardComboPossibilities = 1, nkPossibilities = 1;

			std::array<int, INSUPPLY> draw = { 0, 0, 0, 0, 0, 0, 0 };// Not dynamic, must be manually set if INSUPPLY changes!
		
			// This is the combinationString, containing a combination of cards.
			std::string combinationString = std::string(s.begin(), s.begin() + k);

			// For each letter in the combinationString
			for (int stringIndex = 0; stringIndex < k; stringIndex++)
			{
				// Convert letter to int, and add to draw
				int cardNumber = atoi(std::string(combinationString.begin() + stringIndex, combinationString.begin() + stringIndex + 1).c_str());
				draw[cardNumber]++;
			}

			// Find possible ways to draw the draw
			for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
			{
				int nInCardCombo = copyState.playerStates[currentlyPlaying].deck[cardIndex];
				int kInCardCombo = draw[cardIndex];
				nkInCardComboPossibilities *= choose(nInCardCombo, kInCardCombo);
			}

			// Add the guaranteedCards
			for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
			{
				draw[cardIndex] += guaranteedCards[cardIndex];
			}

			// Find the total possible outcomes
			nkPossibilities = choose(n, k);

			// Calculate probability
			probability = nkInCardComboPossibilities / nkPossibilities;

			// Finally, add draw to collection of draws, and probability to collection of probabilities (associate a probability with each draw)
			draws.push_back(draw);
			probabilities.push_back(probability);
		} while (next_combination(s.begin(), s.begin() + k, s.end()));

		// For each draw, create child node
		for (int drawCounter = 0; drawCounter < draws.size(); drawCounter++)
		{
			Node* newNodePtr = requestNewNode();
			newNodePtr->currentState = currentState;

			for (int cardIndex = 0; cardIndex < INSUPPLY; cardIndex++)
			{
				newNodePtr->currentState.playerStates[currentlyPlaying].hand[cardIndex] = draws[drawCounter][cardIndex];
				newNodePtr->currentState.playerStates[currentlyPlaying].deck[cardIndex] = copyState.playerStates[currentlyPlaying].deck[cardIndex] - draws[drawCounter][cardIndex] + guaranteedCards[cardIndex];
				newNodePtr->currentState.playerStates[currentlyPlaying].discard[cardIndex] = copyState.playerStates[currentlyPlaying].discard[cardIndex];
				newNodePtr->currentState.supplyPiles[cardIndex] = copyState.supplyPiles[cardIndex];
			}
			node->childrenPtrs.push_back(newNodePtr);
			newNodePtr->parentPtr = node;
			newNodePtr->playerPlaying = currentlyPlaying;
			newNodePtr->opt.card = -2;
			newNodePtr->opt.type = DRAW;
			newNodePtr->probability = probabilities[drawCounter];
		}

	}
	else
	{
		// Create end turn node to enable doing nothing.
		Node* endTurnChild = requestNewNode();
		//Node* endTurnChild = new Node();
		Option o;
		o.type = END_TURN;
		o.card = -1;
		endTurnChild->opt = o;
		endTurnChild->parentPtr = node;
		endTurnChild->currentState = currentState;
		endTurnChild->playerPlaying = currentlyPlaying;
		node->childrenPtrs.push_back(endTurnChild);

		// Add all possible actions.
		/*
		std::vector<Option> possibleActions = getActionOptions();
		for (int i = 0; i < possibleActions.size(); i++)
		{
			Node* newActionNode;
			newActionNode->opt = possibleActions.at(i);
			newActionNode->parentPtr = node;
			newActionNode->currentState = currentState;
			newActionNode->playerPlaying = currentlyPlaying;
			node->childrenPtrs.push_back(newActionNode);
		}
		*/

		// Add all possible buy nodes.
		if (node->currentState.playerStates[currentlyPlaying].buys > 0)
		{
			std::vector<Option> possibleBuys = getBuyOptions(&node->currentState, node->currentState.playerStates[currentlyPlaying].hand);
			for (int i = 0; i < possibleBuys.size(); i++)
			{
				Node* newBuyNode = requestNewNode();
				//Node* newBuyNode = new Node();
				newBuyNode->opt = possibleBuys.at(i);
				newBuyNode->parentPtr = node;
				newBuyNode->currentState = currentState;
				newBuyNode->playerPlaying = currentlyPlaying;
				// Perform the buy action of this node.
				newBuyNode->currentState.playerStates[currentlyPlaying].buyCard(cardManager, newBuyNode->opt.card);
				newBuyNode->currentState.supplyPiles[cardManager.cardIndexer[newBuyNode->opt.card]] -= 1;
				node->childrenPtrs.push_back(newBuyNode);
			}
		}
	
	}
}

Node* UCTMonteCarlo::getRandomNode(std::vector<Node*> nodes)
{
	int randomNode = rand() % nodes.size();

	return nodes.at(randomNode);
}

std::vector<Node*> UCTMonteCarlo::getUntriedChildren(Node* parent)
{
	std::vector<Node*> untriedNodes;

	for (int i = 0; i < parent->childrenPtrs.size(); i++)
	{
		if (parent->childrenPtrs.at(i)->visited <= 0)
			untriedNodes.push_back(parent->childrenPtrs.at(i));
	}

	return untriedNodes;
}

std::vector<Option> UCTMonteCarlo::getBuyOptions(GameState* gameState, const int(&hand)[INSUPPLY])
{
	int currentMoney = 0;
	currentMoney += hand[cardManager.cardIndexer[COPPER]];
	currentMoney += hand[cardManager.cardIndexer[SILVER]] * 2;
	currentMoney += hand[cardManager.cardIndexer[GOLD]] * 3;

	std::vector<Option> options;
	for (int i = 0; i < sizeof(hand) / sizeof(*hand); i++)
	{
		if (cardManager.cardLookupByIndex[i].cost <= currentMoney && gameState->supplyPiles[i] > 0)
		{
			Option o;
			o.type = BUY;
			o.card = i;
			options.push_back(o);
		}
	}

	return options;
}

std::vector<Option> UCTMonteCarlo::getActionOptions()
{
	std::vector<Option> actionOptions;
	return actionOptions;
}

int UCTMonteCarlo::getCardPlayoutPolicy(GameState& gameState, int playerIndex)
{
	std::vector<Option> buyOptions = getBuyOptions(&gameState, gameState.playerStates[playerIndex].hand);

	int cardToReturn = 0;

	std::vector<Option>::iterator iter;

	int highestCost = 0;

	for (iter = buyOptions.begin(); iter != buyOptions.end(); iter++)
	{
		if (cardManager.cardLookupByIndex[(*iter).card].cost > highestCost || highestCost == 0 && cardManager.cardLookupByIndex[(*iter).card].name != "Curse")
		{
			highestCost = cardManager.cardLookupByIndex[(*iter).card].cost;
			cardToReturn = (*iter).card;
		}
	}

	return cardToReturn;
}

// Tree printing
void UCTMonteCarlo::printTree(int turnCounter, Node* rootNodePtr)
{
	//std::cout << "Printing tree" << std::endl;
	std::string fileName = std::to_string(turnCounter) + "uctTree.gv";
	remove(fileName.c_str());
	std::ofstream file;
	file.open(fileName, std::ios::app);
	std::string text = "digraph ucttree{\r\n size = \"10000000000!, 1000000000\";\r\n ratio = \"expand\";\r\n node[color = lightblue2, style = filled];";
	file << text << std::endl;
	printNode(rootNodePtr, file);
	text = "\r\n }";
	file << text << std::endl;
	file.close();
	//std::cout << "Done printing tree" << std::endl;
}
void UCTMonteCarlo::printNode(Node* nodePtr, std::ofstream& file)
{
	nodePtr->printSelf(file);
	for (std::vector<Node*>::iterator it = nodePtr->childrenPtrs.begin(); it != nodePtr->childrenPtrs.end(); ++it)
		printNode(*it, file);
}


// Node allocation
UCTMonteCarlo::UCTMonteCarlo()
{
	int allocatedNodes = 400000;
	emptyNodes.reserve(allocatedNodes);
	usedNodes.reserve(allocatedNodes);
	for (int counter = 0; counter < allocatedNodes; counter++)
	{
		Node* nodePtr = new Node();
		nodePtr->id = allocatedNodes - counter;
		emptyNodes.push_back(nodePtr);
	}
}
UCTMonteCarlo::~UCTMonteCarlo()
{
	for (int counter = 0; counter < emptyNodes.size(); counter++)
	{
		delete emptyNodes[counter];
	}
	emptyNodes.clear();
	for (int counter = 0; counter < usedNodes.size(); counter++)
	{
		delete usedNodes[counter];
	}
	usedNodes.clear();
}
Node* UCTMonteCarlo::requestNewNode()
{
	//std::cout << "Requesting node" << std::endl;
	if (emptyNodes.size() <= 0)
		std::cout << "No more nodes!" << std::endl;

	Node* returnNode = emptyNodes.back();
	usedNodes.push_back(returnNode);
	emptyNodes.pop_back();
	//std::cout << "Done requesting node" << std::endl;
	return returnNode;
}
void UCTMonteCarlo::resetNodes()
{
	//std::cout << "Resetting nodes" << std::endl;
	for (std::vector<Node*>::iterator iterator = usedNodes.begin(); iterator != usedNodes.end(); ++iterator)
	{
		(*iterator)->reset();
		emptyNodes.push_back(*iterator);
	}
	usedNodes.clear();
	//std::cout << "Done resetting nodes" << std::endl;
}

unsigned long long UCTMonteCarlo::choose(unsigned long long n, unsigned long long k)
{
	if (k > n) {
		return 0;
	}
	unsigned long long r = 1;
	for (unsigned long long d = 1; d <= k; ++d) {
		r *= n--;
		r /= d;
	}
	return r;
}