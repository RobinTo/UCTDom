import random


class player():
	def __init__(self):
		self.deck = []
		self.hand = []
		self.discard = []
		self.inPlay = []

		self.boughtCards = []

	def endTurn(self):
		for c in self.hand:
			self.discard.append(c)
		self.hand[:] = []
		for c in self.inPlay:
			self.discard.append(c)
		self.inPlay[:] = []

		for i in range(0, 5):
			if len(self.deck) <= 0:
				self.shuffle()
			self.drawCard()

	def printHandString(self):
		outHand = ""
		for c in self.hand:
			outHand += c.name + " "
		print outHand

	def drawCard(self):
		self.hand.append(self.deck[0])
		self.deck.remove(self.deck[0])

	def shuffle(self):
		while len(self.discard) > 0:
			c = self.discard[random.randint(0, len(self.discard)-1)]
			self.deck.append(c)
			self.discard.remove(c)

	def buyCard(self, card):
		self.discard.append(card)
		self.boughtCards.append(card)

class card():
	def __init__(self, name, value, cost):
		self.name = name
		self.value = value
		self.cost = cost