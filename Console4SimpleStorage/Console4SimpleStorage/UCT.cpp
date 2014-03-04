#include "UCT.h"

UCT::UCT()
{
	int allocatedNodes = 200000;
	emptyNodes.reserve(allocatedNodes);
	usedNodes.reserve(allocatedNodes);
	for (int counter = 0; counter < allocatedNodes; counter++)
	{
		Node* nodePtr = new Node();
		nodePtr->id = allocatedNodes - counter;
		emptyNodes.push_back(nodePtr);
	}
}

UCT::~UCT()
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

void UCT::initialize(CardManager& cm, int simulations2)
{
	simulations = simulations2;
	cardManager = cm;
}

std::vector<Option> UCT::getActionOptions(GameState* gameState, const int (&hand)[INSUPPLY])
{
	// Return all action cards in hand
	std::vector<Option> options;
	return options;
}

std::vector<Option> UCT::getBuyOptions(GameState* gameState, const int (&hand)[INSUPPLY])
{
	int currentMoney = 0;
	currentMoney += hand[cardManager.cardIndexer[COPPER]];
	currentMoney += hand[cardManager.cardIndexer[SILVER]] * 2;
	currentMoney += hand[cardManager.cardIndexer[GOLD]] * 3;
	
	std::vector<Option> options;
	for(int i = 0; i < sizeof(hand)/sizeof(*hand); i++)
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
	//gameState->cardManager.cardIndexer[COPPER]; returns int index in hand etc of COPPPER
	//gameState->cardManager.cardLookup[COPPER]; returns card struct based on DEFINE name
	//gameState->cardManager.cardLookupByIndex[1];	returns card struct based on array position

	// Return all cards in stockpile with cost <= currentMoney
}

Option UCT::getNextOption(GameState currentState, int stateIndex)
{
	//Create rootNode
	Node* rootNodePtr = requestNewNode();
	rootNodePtr->isRoot = true;
	rootNodePtr->currentState = currentState;

	if (currentState.playerStates[stateIndex].buys == 0)
	{
		Option option;
		option.type = END_TURN;
		return option;
	}

	// While more simulations
	int simulationCounter = 0;
	while(simulationCounter < simulations)
	{
		selectAndExpand(rootNodePtr, currentState, stateIndex);

		if (stateIndex == currentState.playerStates.size() - 1)
		{
			stateIndex = 0;
		}
		else
		{
			stateIndex++;
		}

		simulationCounter ++;
		//std::cout << (emptyNodes.size()) << std::endl;
	}

	Option o;
	double highestVisited = 0;

	for (int i = 0; i<rootNodePtr->childrenPtrs.size(); i++)
	{
		std::cout << "Score for " << cardManager.cardLookupByIndex[rootNodePtr->childrenPtrs.at(i)->opt.card].name << " was: " << rootNodePtr->childrenPtrs.at(i)->value << std::endl;
		std::cout << "Propagated : " << rootNodePtr->childrenPtrs.at(i)->propagateCounter << std::endl;
		std::cout << "Visited : " << rootNodePtr->childrenPtrs.at(i)->propagateCounter << std::endl;
		if (rootNodePtr->childrenPtrs.at(i)->propagateCounter > highestVisited || highestVisited == 0)
		{
			o = rootNodePtr->childrenPtrs.at(i)->opt;
			highestVisited = rootNodePtr->childrenPtrs.at(i)->propagateCounter;
		}
	}
	printTree(currentState.turnCounter, rootNodePtr);
	resetNodes();
	return o;
}

Node* UCT::selectBestLeaf(Node* rootNode)
{
	double bestValue = 0;
	Node* bestNode = rootNode;

	if (rootNode->isRoot)
		rootNode->visited++;

	std::vector<Node*> unvisitedNodes;
	for (int i = 0; i < rootNode->childrenPtrs.size(); i++)
	{
		if (rootNode->childrenPtrs.at(i)->visited <= 0)
		{
			unvisitedNodes.push_back(rootNode->childrenPtrs.at(i));
		}
	}
	if (unvisitedNodes.size() > 0)
	{
		int randomChild = rand() % unvisitedNodes.size();
		
		unvisitedNodes.at(randomChild)->visited++;
		return unvisitedNodes.at(randomChild);
	}
	else
	{
		for (int i = 0; i < rootNode->childrenPtrs.size(); i++)
		{
			double thisValue = (double)rootNode->childrenPtrs.at(i)->value + 10 * sqrt(log((double)rootNode->propagateCounter/rootNode->childrenPtrs.at(i)->propagateCounter));
			if(thisValue > bestValue || bestValue == 0)
			{
				bestValue = thisValue;
				bestNode = rootNode->childrenPtrs.at(i);
			}
		}
	}

	if (bestNode->childrenPtrs.size() <= 0)
	{
		bestNode->visited++;
		return bestNode;
	}
	else
	{
		bestNode->visited++;
		return selectBestLeaf(bestNode);
	}
}

void UCT::selectAndExpand(Node* startNode, GameState currentState, int stateIndex)
{
	// Select best leaf
	Node* bestLeafPtr = selectBestLeaf(startNode);

	if (bestLeafPtr->currentState.gameFinished())
	{
		int result = rollout(stateIndex, bestLeafPtr->currentState);
		propagate(bestLeafPtr, result);
	}
	else
	{
		if (bestLeafPtr->visited <= 0) // If it was an unvisited node, perform a rollout.
		{
			int result = rollout(stateIndex, bestLeafPtr->currentState);
			propagate(bestLeafPtr, result);
		}
		else // Else, create all children and rollout a random childnode.
		{
			if (!currentState.gameFinished())	// If treeDepth is less than turns left.
			{
				std::vector<Option> actionOptions = getActionOptions(&currentState, bestLeafPtr->currentState.playerStates[stateIndex].hand);
				std::vector<Option> buyOptions = getBuyOptions(&currentState, bestLeafPtr->currentState.playerStates[stateIndex].hand);

				// TODO: for each action

				std::vector<Option>::iterator iter;

				for (iter = actionOptions.begin(); iter != actionOptions.end(); iter++)
				{
					if (currentState.playerStates[stateIndex].actions > 0)
					{
						Node* newNode = requestNewNode();
						newNode->parentPtr = bestLeafPtr;
						newNode->setState(currentState);	// Copy, not reference.

						// playAction(&newNode->currentState, actionIterator);
						// simulate(newNode->currentState, stateIndex, newNode, turns, maxTurns);
					}
				}

				for (iter = buyOptions.begin(); iter != buyOptions.end(); iter++)
				{
					if (currentState.playerStates[stateIndex].buys > 0)
					{
						Node* newNode = requestNewNode();
						newNode->parentPtr = bestLeafPtr;
						newNode->setState(bestLeafPtr->currentState);
						newNode->currentState.turnCounter++;
						newNode->setOption(*iter);
						newNode->playerPlaying = stateIndex;
						bestLeafPtr->childrenPtrs.push_back(newNode);
						buyCard(newNode->currentState.playerStates[stateIndex], (*iter).card, newNode->currentState);
						newNode->currentState.playerStates[stateIndex].buys--;
					}
				}

				int randomChild = rand() % bestLeafPtr->childrenPtrs.size();

				if (bestLeafPtr->childrenPtrs.at(randomChild)->visited <= 0)
				{	
					int result = rollout(stateIndex, bestLeafPtr->childrenPtrs.at(randomChild)->currentState);
					propagate(bestLeafPtr->childrenPtrs.at(randomChild), result);
					bestLeafPtr->childrenPtrs.at(randomChild)->visited++;
				}
			}
		}
	}
}

void UCT::buyCard(PlayerState& pState, int cardToBuy, GameState& gameState)
{
	pState.discard[cardToBuy] += 1;
	gameState.supplyPiles[cardToBuy] -= 1;
	pState.buys -= 1;
}

int UCT::rollout(int playerIndex, GameState gameState)
{
	int currentPlayer = playerIndex;
	
	gameState.playerStates[currentPlayer].endTurn();

	if (playerIndex == gameState.playerStates.size() - 1)
	{
		playerIndex = 0;
	}
	else
	{
		playerIndex++;
	}

	if (currentPlayer == 0)
		gameState.turnCounter++;

	while(!gameState.gameFinished())
	{
		int cardChosen = playoutPolicy(gameState, currentPlayer);
		buyCard(gameState.playerStates[currentPlayer], cardChosen, gameState);
		gameState.playerStates[currentPlayer].endTurn();

		if (playerIndex == gameState.playerStates.size() - 1)
		{
			playerIndex = 0;
		}
		else
		{
			playerIndex++;
		}

		if(currentPlayer == 0)
			gameState.turnCounter++;
	}

	return gameState.playerStates[playerIndex].calculateVictoryPoints(cardManager);
}

// Currently epsilon greedy
int UCT::playoutPolicy(GameState& gameState, int playerIndex)
{
	std::vector<Option> buyOptions = getBuyOptions(&gameState, gameState.playerStates[playerIndex].hand);

	int cardToReturn = 0;

	int chance = rand() % 10;
	if (chance <= 9)
	{
		std::vector<Option>::iterator iter;

		int highestCost = 0;

		for(iter=buyOptions.begin(); iter != buyOptions.end(); iter++)
		{
			if (cardManager.cardLookupByIndex[(*iter).card].cost > highestCost || highestCost == 0  && cardManager.cardLookupByIndex[(*iter).card].name != "Curse")
			{
				highestCost = cardManager.cardLookupByIndex[(*iter).card].cost;
				cardToReturn = (*iter).card;
			}
		}
	}
	else
	{
		int randomCardToReturn = rand() % (buyOptions.size());
		cardToReturn = buyOptions[randomCardToReturn].card;
	}

	return cardToReturn;
}

void UCT::propagate(Node* endNode, int result)
{
	endNode->sum += result;
	endNode->propagateCounter += 1;
	//std::cout << "Visited: " << endNode->visited << std::endl;
	//std::cout << "Propagated: " << endNode->propagateCounter << std::endl;
	endNode->value = ((double)endNode->sum/(double)endNode->propagateCounter);
	if(!endNode->isRoot)
	{
		propagate(endNode->parentPtr, result);
	}
}

void UCT::nextPlayerTurn(int& playerIndex, GameState& gameState)
{
}

Node* UCT::requestNewNode()
{
	if (emptyNodes.size() <= 0)
		std::cout << "No more nodes!" << std::endl;
	
	Node* returnNode = emptyNodes.back();
	usedNodes.push_back(returnNode);
	emptyNodes.pop_back();
	return returnNode;
}

void UCT::resetNodes()
{
	for (std::vector<Node*>::iterator iterator = usedNodes.begin(); iterator != usedNodes.end(); ++iterator)
	{
		(*iterator)->reset();
		emptyNodes.push_back(*iterator);
	}
	usedNodes.clear();
}

//void UCT::setNodes(std::vector<Node*>& emptyNodePtrs)
//{
//	emptyNodes = emptyNodePtrs;
//	usedNodes.reserve(emptyNodePtrs.size());
//}

void UCT::printTree(int turnCounter, Node* rootNodePtr)
{
	std::cout << "Printing tree." << std::endl;
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
	std::cout << "Done printing tree." << std::endl;
}

void UCT::printNode(Node* nodePtr, std::ofstream& file)
{
	nodePtr->printSelf(file);
	for (std::vector<Node*>::iterator it = nodePtr->childrenPtrs.begin(); it != nodePtr->childrenPtrs.end(); ++it)
		printNode(*it, file);
}
