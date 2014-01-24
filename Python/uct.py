from tree import *
from dominion import *
import copy
import math
import time

def getNextOption(p, currentNode, cards, turn, simulationTurns):
	nodeCopy = copy.deepcopy(currentNode)	# Somewhat irrelevant, could always make new empty node as parent
	playerCopy = copy.deepcopy(p)	# Don't want to change anythin on the real player.

	options = getOptions(calculateMoney(playerCopy), cards)
	for i in range(0, simulationTurns*len(options)):
		newCopy = copy.deepcopy(playerCopy)
		rollout(newCopy, nodeCopy, cards, turn)

	#nodeCopy.printTreeBelow(0)
	#print(len(nodeCopy.childNodes))

	bestNode = None
	bestValue = 0

	for n in nodeCopy.childNodes:
		if n.value > bestValue or bestNode == None:
			bestNode = n
			bestValue = n.value

	return bestNode.cardOption

def rollout(playerCopy, nodeCopy, cards, turn):
	nodeToExpand = getBestLeaf(playerCopy, nodeCopy)
	
	# Here we have the base player, getBestLeaf buys cards when it goes down the tree
	# and thus the deck and discard is correct.
	#if nodeToExpand.cardOption:
	#	print(nodeToExpand.cardOption.name)
	#time.sleep(.1)
	if nodeToExpand.treeDepth < 40-turn:
		options = getOptions(calculateMoney(playerCopy), cards)
		for o in options:
			editablePlayer = copy.deepcopy(playerCopy) # Don't modify playercopy with simulations, base playercopy is state before option is chosen.
			newNode = treeNode(nodeToExpand, o)
			nodeToExpand.appendChild(newNode)
			value = simulate(editablePlayer, newNode, cards, turn)
			propagate(value, newNode)
	else:
		editablePlayer = copy.deepcopy(playerCopy)
		value = simulate(editablePlayer, nodeToExpand, cards, turn)
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
		#if n.parent != None:
		#	nodeValue = n.value + 1 * math.sqrt(math.log(n.parent.timesChosen)/n.timesChosen)
		#else:
		#	nodeValue = n.value + 1 * math.sqrt(math.log(1)/n.timesChosen)
		nodeValue = -n.timesChosen # Just for testing, implement equation
		if (nodeValue > bestValue or bestNode == None):
			bestValue = nodeValue
			bestNode = n
	return bestNode

def simulate(playerCopy, node, cards, turn):
	playerCopy.buyCard(node.cardOption)
	playerCopy.endTurn()
	#print(7-node.treeDepth)
	for i in range(turn, 39-node.treeDepth):	# Simulate until game done. 6 turns in utcdom.py while loop
		opts = getOptions(calculateMoney(playerCopy), cards)
		cardChosen = opts[random.randint(0, len(opts)-1)]
		playerCopy.buyCard(cardChosen)
		playerCopy.endTurn();

	endSum = 0.0

	for c in playerCopy.deck:
		endSum += c.value
	for c in playerCopy.discard:
		endSum += c.value
	for c in playerCopy.hand:
		endSum += c.value
	for c in playerCopy.inPlay:
		endSum += c.value

	return endSum

def propagate(score, propagateNode):
	#if score > propagateNode.value:
	#	propagateNode.value = score
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
		if c.name == "Copper":
			coins+=1
		elif c.name == "Silver":
			coins+=2
		elif c.name == "Gold":
			coins+=3
	return coins