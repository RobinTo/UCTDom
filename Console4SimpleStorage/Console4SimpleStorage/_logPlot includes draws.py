import matplotlib.pyplot as plt
from os import listdir
import os
from os.path import isfile, join

onlyfiles = [ f for f in listdir(os.getcwd()) if isfile(join(os.getcwd(),f)) ]

wins = 0
losses = 0
draws = 0

winScores = []
loseScores = []
winDiffScores = []
loseDiffScores = []

highestDiff = 0
lowestDif = 0

for f in onlyfiles:
	if "log" in f and "txt" in f:
		fullPath = join(os.getcwd(), f)
		log = open(fullPath, "r")
		equalTurns = False
		whoStart = 0
		for line in log:
			if "Turns:" in line:
				stringParts = line.split(':')
				numberTurns = len(stringParts) - 2
				if numberTurns % 2 == 0:
					equalTurns = True
			if "Player 0 VP: " in line:
				stringParts = line.split(' ')
				pts0 = int(stringParts[len(stringParts)-1])
			if "Player 1 VP: " in line:
				stringParts = line.split(' ')
				pts1 = int(stringParts[len(stringParts)-1])
		log.close();
		if pts0 > pts1 or (pts0 == pts1 and whoStart == 1 and not equalTurns):
			wins+=1
			winScores.append(pts0)
			winDiffScores.append(pts0-pts1)
		elif pts1 > pts0 or (pts0 == pts1 and whoStart == 0 and not equalTurns):
			losses+=1
			loseScores.append(pts1)
			loseDiffScores.append(pts0-pts1)
		else:
			draws+= 1
			print (str(pts0) + ',' + str(pts1) + ',' + str(whoStart) +',' + str(numberTurns) +',' +str(equalTurns) + ',' + f)

			
winPercentage = 100*float(wins)/(wins + draws + losses)


#output = open(join(os.getcwd(), "output.txt"), "w+")
#output.write(str(winPercentage))
#output.close()

print("Wins: " + str(wins) + ", averaging delta: " + str(sum(winDiffScores)/len(winDiffScores)))
print("Win percentage: " + str(winPercentage) + "%")
print("Highest win: " + str(max(winDiffScores)))
if losses > 0:
	print("Losses: " + str(losses) + ", averaging delta: " + str(sum(loseDiffScores)/len(loseDiffScores)))
	print("Worst loss: " + str(min(loseDiffScores)))
else:
	print("Losses: " + str(losses) + ", averaging delta: " + str(0))
	print("Worst loss: " + str(0))
print("Draws: " + str(draws))