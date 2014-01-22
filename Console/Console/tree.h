#include "treeNode.h"

#ifndef __TREE_H__
#define __TREE_H__

class tree
{
public:
	tree();
	tree(treeNode* initialNode);
	treeNode initial = new treeNode();
};

#endif