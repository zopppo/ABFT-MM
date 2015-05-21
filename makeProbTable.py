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

# If logarithmic is true the increments will be multiplied instead of added 
def makeProbabilityTable(startingProbability, stepIncrement, numberOfRuns, runsPerProb, logarithmic):
    probs = []
    currentProbability = startingProbability
    file = open("probability.txt", "w")
    for i in range(0, numberOfRuns):
        for j in range(0, runsPerProb):
            # Write the probability to the file
            file .write('{:.7f}'.format(currentProbability) + '\n')
        if logarithmic:
            currentProbability *= stepIncrement
        else: 
            currentProbability += stepIncrement
    file.close()

if len(sys.argv) < 5:
    print("Usage: makeProbTable <startingProbability> <step> <numberOfRuns> <runsPerProb> [-l]")
    sys.exit()

startingProbability = float(sys.argv[1])
stepIncrement = float(sys.argv[2])
numberOfRuns = int(sys.argv[3])
runsPerProb = int(sys.argv[4])
if len(sys.argv) > 5 and sys.argv[5] == "-l":
    logarithmic = True
else:
    logarithmic = False 

makeProbabilityTable(startingProbability, stepIncrement, numberOfRuns, runsPerProb, logarithmic)
