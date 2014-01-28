from dominion import *


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

def countMoney(play):
	coins = 0
	for c in play.hand:
		if c.name == "Copper":
			coins+=1
		elif c.name == "Silver":
			coins+=2
		elif c.name == "Gold":
			coins+=3
	return coins

def getNextOption(c):
	if c >= 8:
		return getCardByName("Province")
	elif c >= 6 and turns < 30:
		return getCardByName("Gold")
	elif c >= 5:
		return getCardByName("Duchy")
	elif c>=3 and turns < 30:
		return getCardByName("Silver")
	elif c>=2:
		return getCardByName("Estate")
	else:
		return getCardByName("Copper")

gameC = 0
while gameC < 20:
	gameC+=1
	p = player()

	cards = []
	cardCounters = []

	# Value just given to score when calculating heuristic.
	# Using these values the usual game is buying coins in the
	# first and sometimes second round, then only victory cards.
	# This shows that the game thinks about the future. (?)
	addCard(card("Copper", 		0, 0), 12)
	addCard(card("Silver", 		0, 3), 12)
	addCard(card("Gold", 		0, 6), 12)

	addCard(card("Estate", 		1, 2), 12)
	addCard(card("Duchy", 		3, 5), 12)
	addCard(card("Province", 	6, 8), 12)

	for i in range(0,7):
		p.discard.append(getCardByName("Copper"))
	for i in range(0,3):
		p.discard.append(getCardByName("Estate"))
	p.shuffle()
	p.endTurn()

	turns = 0

	turnString = "Turns:"

	while turns < 40:
		print("Current hand:")
		p.printHandString()
		c = countMoney(p)
		print("With " + str(c) + " coins, buy: ")

		nextCard = getNextOption(c)

		p.buyCard(nextCard)

		print(nextCard.name)
		p.endTurn()

		turnString += str(p.getTotalVP()) + ":"

		turns+=1

	endSum = 0
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
		endSum += c.value
	for c in p.discard:
		if c.name == "Estate":
			estateCounter+=1
		elif c.name == "Duchy":
			duchyCounter+=1
		elif c.name == "Province":
			provinceCounter+=1
		endSum += c.value
	for c in p.hand:
		if c.name == "Estate":
			estateCounter+=1
		elif c.name == "Duchy":
			duchyCounter+=1
		elif c.name == "Province":
			provinceCounter+=1
		endSum += c.value
	for c in p.inPlay:
		if c.name == "Estate":
			estateCounter+=1
		elif c.name == "Duchy":
			duchyCounter+=1
		elif c.name == "Province":
			provinceCounter+=1
		endSum += c.value





	f = open('resultsRuleBased.txt', 'a+')

	gameString = str(40)
	gameString += ":" + "rulebased"
	gameString += ":" + str(endSum)
	gameString += ":" + str(estateCounter)
	gameString += ":" + str(duchyCounter)
	gameString += ":" + str(provinceCounter) + "\r\n"
	print(gameString)
	f.write(turnString + "\r\n")
	f.write(gameString)
	f.close()