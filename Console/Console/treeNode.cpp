#include "treeNode.h"

treeNode::treeNode()
{
	isRoot = false;
}


treeNode::treeNode(bool isRoot2, treeNode* pNodePtr)
{
	if (isRoot2)
	{
		isRoot = true;
		parentNodePtr = NULL;
	}
	else
	{
		isRoot = false;
		parentNodePtr = pNodePtr;
	}
}

void treeNode::appendChild(treeNode* childNodePtr)
{
	childNodePtrs.push_back(childNodePtr);
}
