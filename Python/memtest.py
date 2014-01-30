import sys
from dominion import *
from tree import *
import copy

def showSize(x, level=0):
	print("\t"*level, x.__class__, sys.getsizeof(x), x)

	if hasattr(x, '__iter__'):
		if hasattr(x, 'items'):
			for xx in x.items():
				showSize(xx, level+1)
		else:
			for xx in x:
				showSize(xx, level+1)

top = treeNode(None, None)
child = treeNode(top, card("copper", 0, 3))

childCopy = copy.copy(child)

childCopy.totalValue += 1

childCopy.cardOption = card("silver", 0, 5)

childCopy.parent = None

print(child.parent)
print(childCopy.parent)