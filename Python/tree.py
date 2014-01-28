class treeNode:
	def __init__(self, parent, cardOption):
		self.value = 0.0
		self.timesPropagated = 0.0
		self.totalValue = 0.0
		self.cardOption = cardOption
		self.timesChosen = 1
		self.childNodes = []
		self.parent = parent
		if(not parent):
			self.treeDepth = 0
		else:
			self.treeDepth = parent.treeDepth+1

	def appendChild(self, child):
		self.childNodes.append(child)

	def printTreeBelow(self, depthCounter):
		if (self.cardOption):
			print(str(depthCounter * "\t") + "TreeNode: " + self.cardOption.name + " Value: " + str(self.value))
		else:
			print("StartNode")
		for c in self.childNodes:
			c.printTreeBelow(depthCounter+1)
