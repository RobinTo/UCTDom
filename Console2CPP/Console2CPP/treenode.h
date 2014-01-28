

#include "gamestate.h"
#include "playerstate.h"


#ifndef __TREENODE_H__
#define __TREENODE_H__

class TreeNode
{
public:
	GameState* gameStatePtr;
	std::vector<PlayerState*> playerStatePtrs;
	bool isRoot;
	
	void addChild();
	void writeChildrenToFile();
};


#endif