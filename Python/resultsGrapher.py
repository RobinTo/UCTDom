import matplotlib.pyplot as plt
import string

def readResults(fileName):
	f = open(fileName, "r")

	resultsArrays = []
	for line in f:
		if "Turns" in line:
			r = string.split(line, ":")
			r.remove(r[0])
			r.remove(r[len(r)-1])
			resultsArrays.append(r)

	f.close()
	return resultsArrays

def graphResults(resultsArrays):
	for arr in resultsArrays:
		plt.plot(range(0, len(arr)), arr)

def graphAverage(resultsArrays):
	averageArray = []

	for i in range(0, len(resultsArrays[0])):
		averageArray.append(0)

	for arr in resultsArrays:
		for i in range(0, len(arr)):
			averageArray[i] += int(arr[i])
	for i in range(0, len(averageArray)):
		averageArray[i] = averageArray[i]/len(resultsArrays)

	print(averageArray)
	plt.plot(range(0, len(averageArray)), averageArray)

graphAverage(readResults("treasure10victory.txt"))
graphAverage(readResults("treasure13victory.txt"))
graphAverage(readResults("treasure14victory.txt"))
graphAverage(readResults("treasure15victory.txt"))
graphAverage(readResults("treasure16victory.txt"))
graphAverage(readResults("treasure17victory.txt"))
graphAverage(readResults("treasure18victory.txt"))
graphAverage(readResults("treasure19victory.txt"))
graphAverage(readResults("treasure20victory.txt"))
graphAverage(readResults("treasure25victory.txt"))
graphAverage(readResults("treasure30victory.txt"))

#graphAverage(readResults("resultsRandomPlay.txt"))
#graphAverage(readResults("resultsRandomPlayoutPolicy.txt"))
#graphAverage(readResults("resultsRuleBased.txt"))
#graphResults(readResults("resultsRandomPlay.txt"))
#graphResults(readResults("resultsRandomPlayoutPolicy.txt"))
#graphResults(readResults("resultsRuleBased.txt"))
#graphResults(readResults("resultsUCTgreedy.txt"))

plt.grid(True)
plt.show()
