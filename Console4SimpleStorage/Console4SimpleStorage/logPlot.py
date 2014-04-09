import matplotlib.pyplot as plt
from os import listdir
from os.path import isfile, join

mypath = "D:/Github/UCTDom/Console4SimpleStorage/Console4SimpleStorage"

onlyfiles = [ f for f in listdir(mypath) if isfile(join(mypath,f)) ]

wins = 0
losses = 0
draws = 0

for f in onlyfiles:
	if "log" in f and "txt" in f:
		fullPath = join(mypath, f)
		log = open(fullPath, "r")
		for line in log:
			if "Player 0 VP: " in line:
				stringParts = line.split(' ')
				pts0 = int(stringParts[len(stringParts)-1])
			if "Player 1 VP: " in line:
				stringParts = line.split(' ')
				pts1 = int(stringParts[len(stringParts)-1])
		log.close();
		print(str(pts0) + " - " + str(pts1))
		if pts0 > pts1:
			wins+=1
		elif pts1 > pts0:
			losses+=1
		else:
			draws+= 1


print("Wins: " + str(wins))
print("Losses: " + str(losses))
print("Draws: " + str(draws))