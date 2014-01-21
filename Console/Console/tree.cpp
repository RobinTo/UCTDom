#include "tree.h"

tree::tree()
{

}

tree::tree(treeNode initialNode)
{
	initial = initialNode;
	initial.parentNode = NULL;
}