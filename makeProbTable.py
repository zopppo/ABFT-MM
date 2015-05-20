#!/usr/bin/python
"""
startingProbability is the first probability the table will have
step is the amount the probability will be incremented by with each run
numberOfRuns is the number of runs that will be done for each increment

As an example .0001, .001, 5, 2
.0001
.0001
.0011
.0011
.0021
.0021
.0031
.0031
.0041
.0041

"""
import sys

def makeProbabilityTable(startingProbability, stepIncrement, numberOfRuns, runsPerProb):
    probs = []
    currentProbability = startingProbability
    file = open("probability.txt", "w")
    for i in range(0, numberOfRuns):
        for j in range(0, runsPerProb):
            # Write the probability to the file
            file .write(str(currentProbability) + '\n')
        currentProbability += stepIncrement
    file.close()

if len(sys.argv) != 5:
    print("Usage: makeProbTable <startingProbability> <step> <numberOfRuns> <runsPerProb>")
    sys.exit()
startingProbability = float(sys.argv[1])
stepIncrement = float(sys.argv[2])
numberOfRuns = int(sys.argv[3])
runsPerProb = int(sys.argv[4])
makeProbabilityTable(startingProbability, stepIncrement, numberOfRuns, runsPerProb)
