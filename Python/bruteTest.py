import itertools

class Node():
	def __init__(self, p):
		self.player = p;
		self.childNodes = []

class Player():
	def __init__(self):
		self.hand = [0, 0, 0, 0, 0, 0]
		self.discard = [0, 0, 0, 0, 0, 0]
		self.deck = [0, 0, 0, 0, 0, 0]

def getOptions(coins):
	options = []
	options.append("copper")
	if coins >= 3:
		options.append("silver")
	if coins >= 6:
		options.append("gold")
	if coins >= 2:
		options.append("estate")
	if coins >= 5:
		options.append("duchy")
	if coins >= 8:
		options.append("province")

cards = dict()
cards["copper"] = 0
cards["silver"] = 1
cards["gold"] = 2
cards["estate"] = 3
cards["duchy"] = 4
cards["province"] = 5

p = Player()

rootNode = Node(p)

gameEnded = False

while not gameEnded:
	



"""
import itertools

num = [1, 2, 3, 4, 5,6,7,8,9,10]
combinations = itertools.combinations(num, 3)
matching = [c for c in combinations if reduce(lambda x, y: x * y, c, 1) == 60]

print(matching)
The magic line is the matching = [c for c in combinations if reduce(lambda x, y: x * y, c, 1) == 60]. It's a list comprehension, equivalent to :

matching = []
for c in combinations:
  if reduce(lambda x, y: x * y, c, 1) == 60:
    matching.append(c)

"""