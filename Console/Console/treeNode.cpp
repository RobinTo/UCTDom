#include "treeNode.h"

treeNode::treeNode()
{

}

treeNode::treeNode(treeNode* pNode)
{
	parentNode = pNode;
}

void treeNode::appendChild(treeNode node)
{
	childNodes.push_back(node);
}

void treeNode::addChild(bool state, card* c, int cash)
{
	treeNode n;
	n.cash = cash;
	n.state = state;
	n.c = c;
	childNodes.push_back(n);
}