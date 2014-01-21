#include "treeNode.h"

treeNode::treeNode(treeNode* pNode)
{
	parentNode = pNode;
}

void treeNode::addChild(bool state, card* c, int cash)
{
	treeNode n(this);
	n.cash = cash;
	n.state = state;
	n.c = c;
	childNodes.push_back(&n);
}