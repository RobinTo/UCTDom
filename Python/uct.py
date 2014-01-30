from tree import *
from dominion import *
import copy
import math
import time

def copyPlayer(p):
	pCopy = copy.copy(p)
	pCopy.hand = copy.copy(p.hand)
	pCopy.inPlay = copy.copy(p.inPlay)
	pCopy.discard = copy.copy(p.discard)
	pCopy.deck = copy.copy(p.deck)
	return pCopy

def copyNode(n): # Using deepcopy has better performance? o.O
	return copy.deepcopy(n)

def getNextOption(p, currentNode, cards, turn, simulationTurns, maxTurns):
	nodeCopy = copyNode(currentNode)	# Somewhat irrelevant, could always make new empty node as parent
	playerCopy = copyPlayer(p)	# Don't want to change anythin on the real player.

	options = getOptions(calculateMoney(playerCopy), cards)
	for i in range(0, simulationTurns*len(options)):
		newCopy = copyPlayer(playerCopy)
		rollout(newCopy, nodeCopy, cards, turn, maxTurns)


	bestNode = None
	bestValue = 0

	for n in nodeCopy.childNodes:
		if n.value > bestValue or bestNode == None:
			bestNode = n
			bestValue = n.value

	return bestNode.cardOption

def rollout(playerCopy, nodeCopy, cards, turn, maxTurns):
	nodeToExpand = getBestLeaf(playerCopy, nodeCopy)
	
	if nodeToExpand.treeDepth < maxTurns-turn:
		options = getOptions(calculateMoney(playerCopy), cards)
		for o in options:
			editablePlayer = copyPlayer(playerCopy) # Don't modify playercopy with simulations, base playercopy is state before option is chosen.
			newNode = treeNode(nodeToExpand, o)
			nodeToExpand.appendChild(newNode)
			value = simulate(editablePlayer, newNode, cards, turn, maxTurns)
			propagate(value, newNode)
	else:
		editablePlayer = copyPlayer(playerCopy)
		value = simulate(editablePlayer, nodeToExpand, cards, turn, maxTurns)
		propagate(value, nodeToExpand)



def getBestLeaf(playerCopy, nodeCopy):
	nodeCopy.timesChosen += 1
	if nodeCopy.cardOption:
		playerCopy.buyCard(nodeCopy.cardOption)
		playerCopy.endTurn()
	if(len(nodeCopy.childNodes) <= 0):
		return nodeCopy
	else:
		return getBestLeaf(playerCopy, getBestChild(nodeCopy.childNodes))

def getBestChild(nodeList):
	bestValue = 0
	bestNode = None
	for n in nodeList:
		# Exploitation vs. Exploration, currently just using uniform exploration.
		#if n.parent != None:
		#	nodeValue = n.value + 1 * math.sqrt(math.log(n.parent.timesChosen)/n.timesChosen)
		#else:
		#	nodeValue = n.value + 1 * math.sqrt(math.log(1)/n.timesChosen)
		nodeValue = -n.timesChosen # Just for testing, implement equation
		if (nodeValue > bestValue or bestNode == None):
			bestValue = nodeValue
			bestNode = n
	return bestNode

def simulate(playerCopy, node, cards, turn, maxTurns):
	playerCopy.buyCard(node.cardOption)
	playerCopy.endTurn()

	for i in range(turn, maxTurns-node.treeDepth):	# Simulate until game done.
		# Random playout policy
		#cardChosen = getRandom(getOptions(calculateMoney(playerCopy), cards))
		# Greedy playout policy
		cardChosen = getGreedy(calculateMoney(playerCopy), cards)
		playerCopy.buyCard(cardChosen)
		playerCopy.endTurn();

	endSum = 0.0

	for c in playerCopy.deck:
		if not c.coin:
			endSum += c.value
	for c in playerCopy.discard:
		if not c.coin:
			endSum += c.value
	for c in playerCopy.hand:
		if not c.coin:
			endSum += c.value
	for c in playerCopy.inPlay:
		if not c.coin:
			endSum += c.value

	return endSum

def getGreedy(coins, cards):
	c = None
	for o in cards:
		if o.cost <= coins:
			if c == None or o.cost > c.cost:
				c = o;
	return c

def getRandom(options):
	return options[random.randint(0, len(options)-1)]

def propagate(score, propagateNode):
	propagateNode.totalValue += score
	propagateNode.timesPropagated += 1.0
	propagateNode.value = propagateNode.totalValue/propagateNode.timesPropagated
	if propagateNode.parent != None:
		propagate(score, propagateNode.parent)

def getOptions(coins, cards):
	options = []
	for c in cards:
		if c.cost <= coins:
			options.append(c)
	return options

def calculateMoney(player):
	coins = 0
	for c in player.hand:
		if c.coin:
			coins+=c.value
		elif c.coin:
			coins+=c.value
		elif c.coin:
			coins+=c.value
	return coins