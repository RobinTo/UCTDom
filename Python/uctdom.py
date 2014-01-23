from dominion import *
from tree import *
from uct import *

def addCard(card, counter):
	cards.append(card)
	cardCounters.append(counter)

def getCardByName(name):
	if name:
		for c in cards:
			if c.name == name:
				return c
	else:
		return None



p = player()
initialNode = treeNode(None, None)

cards = []
cardCounters = []

# Value just given to score when calculating heuristic.
# Using these values the usual game is buying coins in the
# first and sometimes second round, then only victory cards.
# This shows that the game thinks about the future. (?)
addCard(card("Copper", 		0, 0), 12)
addCard(card("Silver", 		30, 3), 12)
addCard(card("Gold", 		50, 6), 12)

addCard(card("Estate", 		100, 2), 12)
addCard(card("Duchy", 		400, 5), 12)
addCard(card("Province", 	800, 8), 12)

for i in range(0,7):
	p.discard.append(getCardByName("Copper"))
for i in range(0,3):
	p.discard.append(getCardByName("Estate"))
p.shuffle()
p.endTurn()

turns = 0

while turns < 6:
	print("Current hand:")
	p.printHandString()
	print("With " + str(calculateMoney(p)) + " coins, buy: ")
	nextCard = getNextOption(p, initialNode, cards).name
	p.buyCard(getCardByName(nextCard))
	print(nextCard)
	p.endTurn()

	turns+=1