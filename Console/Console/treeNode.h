#include <list>
#include "card.h"

#ifndef __TREENODE_H__
#define __TREENODE_H__

class treeNode
{
public:
	treeNode();
	treeNode(treeNode* pNode);
	bool state;
	card* c;
	int cash;
	int visited;
	double value;
	std::list<treeNode> childNodes;
	treeNode* parentNode;

	void addChild(bool state, card* c, int cash);
	void appendChild(treeNode child);
private:

};

#endif