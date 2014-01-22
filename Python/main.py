import copy
import random
import math

class player():
	def __init__(self):
		self.deck = []
		self.hand = []
		self.discard = []
		self.inPlay = []

	def endTurn(self):
		for c in self.hand:
			self.discard.append(c)
		self.hand = []
		for c in self.inPlay:
			self.discard.append(c)
		self.inPlay = []
		for i in range(0,5):
			if len(self.deck) <= 0:
				self.shuffle();
			self.hand.append(self.deck[0])

	def shuffle(self):
		for c in self.discard:
			self.deck.append(c)
		self.discard = []


class card():
	def __init__(self, name, value, cost):
		self.name = name
		self.value = value
		self.cost = cost

class tree:
	def __init__(self):
		self.initialNode = treeNode(None, None)

class treeNode:
	def __init__(self, parent, cardOption):
		self.value = 5
		self.cardOption = cardOption
		self.timesChosen = 1
		self.childNodes = []
		self.parent = parent
		if parent == None:							# Initial node
			self.treeDepth = 0
		else:
			self.treeDepth = parent.treeDepth+1

	def appendChild(self, child):
		self.childNodes.append(child)



def gameFinished():
	return False;

def addCard(card, amount):
	cards.append(card)
	cardCounters.append(amount)

def buyCard(p, card):
	if card:
		p.discard.append(card)

def getCardByName(name):
	if name == None:
		return None;
	for c in cards:
		if c.name == name:
			return c

def getBestNode():
	return None

def getNextMove():
	for i in range(0, 700):
		step(t);

	best = None
	bv = 0
	for sn in t.initialNode.childNodes:
		print sn.cardOption.name
		print sn.value
		if sn.value > bv or best == None:
			best = sn
			bv = sn.value

	returnCard = best.cardOption;

	if returnCard == None:
		print("None")
	else:
		print(returnCard.name)

	return returnCard;

# Add another node to the tree and run a simulation for it.
def step(tree):

	nodeToExpand = getLeafNode(t.initialNode)
	if nodeToExpand.treeDepth <= 3:
		for o in getOptions(5):		# o is a card, getOptions return array of possible cardbuys with n coins.
			newNode = treeNode(nodeToExpand, o)
			#print newNode.cardOption.name
			nodeToExpand.appendChild(newNode)
			value = simulate(newNode)
			propagate(value, newNode)


# Get the best leaf node.
def getLeafNode(currentNode):
	if (len(currentNode.childNodes) <= 0):
		return currentNode
	else:
		return getLeafNode(getBestNode(currentNode.childNodes))

# Gets best node from list of nodes, to determine best path to expand on.
def getBestNode(listOfNodes):
	bestValue = 0
	bestNode = None
	for n in listOfNodes:
		nodeValue = 5-n.timesChosen 	# EvE at it's best!
		if (nodeValue > bestValue or bestNode == None):
			bestValue = nodeValue
			bestNode = n;
	bestNode.timesChosen+=1
	return bestNode

def simulate(node):
	playerCopy = copy.deepcopy(p)
	buyCard(playerCopy, node.cardOption)
	playerCopy.endTurn()

	for i in range(0, 2):
		opts = getOptions(5)	# Irrelevant what the player draws, use deterministic 5 coins in hand draw.
		cardChosen = opts[random.randint(0, len(opts)-1)]
		#print(cardChosen.name)
		buyCard(playerCopy, cardChosen)
		playerCopy.endTurn();

	endSum = 0

	for c in playerCopy.deck:
		endSum += c.cost
	for c in playerCopy.discard:
		endSum += c.cost
	for c in playerCopy.hand:
		endSum += c.cost
	for c in playerCopy.inPlay:
		endSum += c.cost
	
	return endSum

def propagate(score, nodeToExpand):
	if score > nodeToExpand.value:
		nodeToExpand.value = score
	if nodeToExpand.parent != None:
		propagate(score, nodeToExpand.parent)

def getOptions(coins):
	options = []
	for c in cards:
		if c.cost <= coins:
			options.append(c)
	return options

random.seed()
t = tree()
cards = []
cardCounters = []

addCard(card("Copper", 		1, 0), 12)
addCard(card("Silver", 		2, 3), 12)
addCard(card("Gold", 		3, 6), 12)

addCard(card("Estate", 		1, 2), 12)
addCard(card("Duchy", 		3, 5), 12)
addCard(card("Province", 	5, 8), 12)

p = player();
for i in range(0, 7):						# 7 Coppers
	buyCard(p, getCardByName("Copper"))
for i in range(0, 3):						# 3 Estates
	buyCard(p, getCardByName("Estate"))

turnCounter = 0
getNextMove()
#while not gameFinished() and turnCounter < 40:
#	buyCard(p, getNextMove())
#	turnCounter+=1