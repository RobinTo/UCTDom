import matplotlib.pyplot as plt
import string

class game():
	def __init__(self, sims, end, estate, duchy, province):
		self.simulations = sims
		self.endScore = end
		self.estates = estate
		self.duchys = duchy
		self.provinces = province

games = []

f = open('resultsFormatted.txt', 'r')

for line in f:
	gameParts = string.split(line, ':')
	g = game(gameParts[1], gameParts[2], gameParts[3], gameParts[4], gameParts[5])
	games.append(g)

f.close()


f = open('ruleBasedResults.txt', 'r')

for line in f:
	line = line.replace('\r', '')
	line = line.replace('\n', '')
	print(line)
	print("--")
	gameParts = string.split(line, ':')
	g = game(gameParts[1], gameParts[2], gameParts[3], gameParts[4], gameParts[5])
	games.append(g)

f.close()


x = range(0, 10)
yDict = dict()

print(len(games))
for g in games:
	if not (g.simulations in yDict):
		yDict[g.simulations] = []

	yDict[g.simulations].append(g.endScore)
	print("Appended game")

lineDict = dict()

blue = 'b'
green = 'g'
red = 'r'
cyan = 'c'
magenta = 'm'
yellow = 'y'
black = 'k'


for k, a in yDict.iteritems():
	print(len(a))
	lineDict[k] = plt.plot(x, a)

text = ""

colors = []
colors.append('b')
colors.append('g')
colors.append('r')
colors.append('c')
colors.append('m')
colors.append('y')
colors.append('k')

counter = 0
for l, a, in lineDict.iteritems():
	text += str(l) + ": " + colors[counter] + "\n"
	plt.setp(a, color=colors[counter], linewidth=2.0)
	counter+=1

plt.text(6, 90, text)

plt.grid(True)
plt.show()