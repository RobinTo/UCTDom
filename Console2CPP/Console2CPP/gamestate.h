#include <unordered_map>
#include <string>

#include "supplypile.h"


#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

class GameState
{
public:
	std::unordered_map<std::string, SupplyPile> supplyPiles; 
	
	bool isGameOver();
};


#endif