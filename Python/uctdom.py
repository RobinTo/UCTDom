# Main file

import cProfile
from dominion import *
from tree import *
from uct import *
import sys, time, pstats

# Memory test function
def showSize(x, level=0):
	print("\t"*level, x.__class__, sys.getsizeof(x), x)

	if hasattr(x, '__iter__'):
		if hasattr(x, 'items'):
			for xx in x.items():
				showSize(xx, level+1)
		else:
			for xx in x:
				showSize(xx, level+1)

# Add card to available cards
def addCard(card, counter, cards, cardCounters):

	cards.append(card)
	cardCounters.append(counter)

# Return card by name.
def getCardByName(name, cards):
	if name:
		for c in cards:
			if c.name == name:
				return c
	else:
		return None



# Plays games.
def playGame(simulationsPerOption, maxTurns, writeToFile, verbose):
	simulationRounds = simulationsPerOption


	p = player()
	initialNode = treeNode(None, None)

	cards = []
	cardCounters = []

	#Add cards
	addCard(card("Copper", 		1, 0, True), 12, cards, cardCounters)
	addCard(card("Silver", 		2, 3, True), 12, cards, cardCounters)
	addCard(card("Gold", 		3, 6, True), 12, cards, cardCounters)
	addCard(card("Estate", 		1, 2, False), 12, cards, cardCounters)
	addCard(card("Duchy", 		3, 5, False), 12, cards, cardCounters)
	addCard(card("Province", 	6, 8, False), 12, cards, cardCounters)
	
	# Initialize player deck.
	for i in range(0,7):
		p.discard.append(getCardByName("Copper", cards))
	for i in range(0,3):
		p.discard.append(getCardByName("Estate", cards))
	
	# Shuffle and draw cards
	p.shuffle()
	p.endTurn()

	# Current turn
	turns = 0
	
	# To output for graphing	
	turnString = "Turns:"

	while turns < maxTurns:

		nextCard = getNextOption(p, initialNode, cards, turns, simulationRounds, maxTurns)
		p.buyCard(nextCard)
		if verbose:
			print("Turn: " + str(turns))
			print("Current hand:")
			p.printHandString()
			print("With " + str(calculateMoney(p)) + " coins, buy: ")
			print(nextCard.name)

		p.endTurn()

		turnString += str(p.getTotalVP()) + ":"
		turns+=1

	#showSize(p)

	estateCounter = 0
	duchyCounter = 0
	provinceCounter = 0
	for c in p.deck:
		if c.name == "Estate":
			estateCounter+=1
		elif c.name == "Duchy":
			duchyCounter+=1
		elif c.name == "Province":
			provinceCounter+=1
	for c in p.discard:
		if c.name == "Estate":
			estateCounter+=1
		elif c.name == "Duchy":
			duchyCounter+=1
		elif c.name == "Province":
			provinceCounter+=1
	for c in p.hand:
		if c.name == "Estate":
			estateCounter+=1
		elif c.name == "Duchy":
			duchyCounter+=1
		elif c.name == "Province":
			provinceCounter+=1
	for c in p.inPlay:
		if c.name == "Estate":
			estateCounter+=1
		elif c.name == "Duchy":
			duchyCounter+=1
		elif c.name == "Province":
			provinceCounter+=1



	gameString = str(maxTurns)
	gameString += ":" + str(simulationRounds)
	gameString += ":" + str(p.getTotalVP())
	gameString += ":" + str(estateCounter)
	gameString += ":" + str(duchyCounter)
	gameString += ":" + str(provinceCounter)
	print(gameString)


	if writeToFile:
		f = open('results'+str(simulationsPerOption)+'.txt', 'a+')
		f.write(turnString + "\r\n")
		f.write(gameString)
		f.close()

t1 = time.time()

# Play 5 games
plays = 0
while plays < 5:
	playGame(100, 40, True, False)
	plays+=1


# Profiling.
#cProfile.run('playGame(1, 40)', 'restats')

t2 = time.time()

print("Time elapsed: " + str(t2-t1))

#p = pstats.Stats('restats')
#p.strip_dirs().sort_stats('time').print_stats()
