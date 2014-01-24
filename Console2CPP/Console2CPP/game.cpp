#include <iostream>
#include <unordered_map>

#include "game.h"
#include "playerstate.h"


void Game::initialize()
{
	// Create gamestate
	gameStatePtr = new GameState();

	// Create playerstates
	PlayerState* playerState1Ptr = new PlayerState();
	PlayerState* playerState2Ptr = new PlayerState();
	PlayerState* playerState3Ptr = new PlayerState();
	PlayerState* playerState4Ptr = new PlayerState();
	std::vector<PlayerState*> p1OtherPtrs;
	p1OtherPtrs.push_back(playerState2Ptr);
	p1OtherPtrs.push_back(playerState3Ptr);
	p1OtherPtrs.push_back(playerState4Ptr);
	std::vector<PlayerState*> p2OtherPtrs;
	p2OtherPtrs.push_back(playerState1Ptr);
	p2OtherPtrs.push_back(playerState3Ptr);
	p2OtherPtrs.push_back(playerState4Ptr);
	std::vector<PlayerState*> p3OtherPtrs;
	p3OtherPtrs.push_back(playerState1Ptr);
	p3OtherPtrs.push_back(playerState2Ptr);
	p3OtherPtrs.push_back(playerState4Ptr);
	std::vector<PlayerState*> p4OtherPtrs;
	p4OtherPtrs.push_back(playerState1Ptr);
	p4OtherPtrs.push_back(playerState2Ptr);
	p4OtherPtrs.push_back(playerState3Ptr);
	
	// Create players
	players.push_back(Player(gameStatePtr, p1OtherPtrs, playerState1Ptr));
	players.push_back(Player(gameStatePtr, p2OtherPtrs, playerState2Ptr));
	players.push_back(Player(gameStatePtr, p3OtherPtrs, playerState3Ptr));
	players.push_back(Player(gameStatePtr, p4OtherPtrs, playerState4Ptr));
}

void Game::prepareCards()
{
	// Create cards
	Card* cardEstatePtr = new Card("Estate", 2);
	Card* cardDuchyPtr = new Card("Duchy", 5);
	Card* cardProvincePtr = new Card("Province", 8);
	Card* cardCopperPtr = new Card("Copper", 0);
	Card* cardSilverPtr = new Card("Silver", 3);
	Card* cardGoldPtr = new Card("Gold", 6);

	// Create supply piles
	gameStatePtr->supplyPiles["Estate"] = (SupplyPile(cardEstatePtr, 24));		//Normally 12 (8 if 2 players), excluding those in players hands.
	gameStatePtr->supplyPiles["Duchy"] = (SupplyPile(cardDuchyPtr, 12));		//Normally 12 (8 if 2 players), excluding those in players hands.
	gameStatePtr->supplyPiles["Province"] = (SupplyPile(cardProvincePtr, 12));	//Normally 12 (8 if 2 players), excluding those in players hands.
	gameStatePtr->supplyPiles["Copper"] = (SupplyPile(cardCopperPtr, 60));		//Normally 60 - those in player's hands.
	gameStatePtr->supplyPiles["Silver"] = (SupplyPile(cardSilverPtr, 40));		//Normally 40
	gameStatePtr->supplyPiles["Gold"] = (SupplyPile(cardGoldPtr, 30));			//Normally 30

	// Deal cards to players
	for (std::vector<Player>::iterator iterator = players.begin(); iterator != players.end(); ++iterator)
	{
		std::vector<Card*> startDeck;
		startDeck.push_back(gameStatePtr->supplyPiles["Estate"].cardPtr);
		startDeck.push_back(gameStatePtr->supplyPiles["Estate"].cardPtr);
		startDeck.push_back(gameStatePtr->supplyPiles["Estate"].cardPtr);
		startDeck.push_back(gameStatePtr->supplyPiles["Copper"].cardPtr);
		startDeck.push_back(gameStatePtr->supplyPiles["Copper"].cardPtr);
		startDeck.push_back(gameStatePtr->supplyPiles["Copper"].cardPtr);
		startDeck.push_back(gameStatePtr->supplyPiles["Copper"].cardPtr);
		startDeck.push_back(gameStatePtr->supplyPiles["Copper"].cardPtr);
		startDeck.push_back(gameStatePtr->supplyPiles["Copper"].cardPtr);
		startDeck.push_back(gameStatePtr->supplyPiles["Copper"].cardPtr);
		gameStatePtr->supplyPiles["Estate"].cardsLeft -= 3;
		gameStatePtr->supplyPiles["Copper"].cardsLeft -= 7;
		iterator->startUp(startDeck);
	}
}

void Game::loopGame()
{
	int counter = 1; //For now, to not create inf while loop. Play 40 turns.
	while(!gameStatePtr->isGameOver() && counter <= 40) 
	{
		std::vector<Player>::iterator iterator;
		for (iterator = players.begin(); iterator != players.end(); ++iterator)
		{
			iterator->playTurn();
			iterator->endTurn();
		}
		std::cout << "Turn " << counter << " finished" << std::endl;
			
		counter ++;
	}
}