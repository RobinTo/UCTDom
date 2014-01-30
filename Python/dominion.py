import random
class player():
	def __init__(self):
		self.deck = []
		self.hand = []
		self.discard = []
		self.inPlay = []

		#self.boughtCards = []

	def endTurn(self):
		for c in self.hand:
			self.discard.append(c)
		self.hand = []
		for c in self.inPlay:
			self.discard.append(c)
		self.inPlay = []

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
		self.deck.pop(0)

	def shuffle(self):
		while len(self.discard) > 0:
			i = random.randint(0, len(self.discard)-1)
			self.deck.append(self.discard[i])
			self.discard.pop(i)

	def buyCard(self, card):
		self.discard.append(card)
		#self.boughtCards.append(card)

	def getTotalVP(self):
		vp = 0
		for c in self.deck:
			vp += self.getCardPoints(c)
		for c in self.discard:
			vp += self.getCardPoints(c)
		for c in self.hand:
			vp += self.getCardPoints(c)
		for c in self.inPlay:
			vp += self.getCardPoints(c)
		return vp

	def getCardPoints(self, card):
		if not card.coin:
			return card.value
		else:
			return 0


class card():
	def __init__(self, name, value, cost, coin=False):
		self.name = name
		self.value = value
		self.cost = cost
		self.coin = coin