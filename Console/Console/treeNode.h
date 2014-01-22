#include <list>
#include "card.h"

#ifndef __TREENODE_H__
#define __TREENODE_H__

class treeNode
{
public:
	treeNode();
	treeNode(bool isRoot2, treeNode* pNodePtr);
	bool state;
	bool isRoot;
	card* c;
	int cash;
	int visited;
	double value;
	std::list<treeNode*> childNodePtrs;
	treeNode* parentNodePtr;
	void appendChild(treeNode* child);
private:

};

#endif