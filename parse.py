#!/usr/bin/env python
import sys

input_file = 'result' 

if len(sys.argv) > 1:
    input_file = sys.argv[1]

data = open(input_file, 'r')
summary = open('summary', 'w')

# Read all the lines into a list
lines = data.read().splitlines()
data.close()

avgCorrected = 0
avgRecomputed = 0
avgRecomputeCalls = 0
avgDotProducts = 0
corrects = 0
incorrects = 0

runs = 0

summary.write(lines[0] + ' ')

for i in range(1, len(lines)):
    #print(str(len(lines)) + ' ' + lines[i])
    # If it's a percentage line
    if lines[i].find('%') != -1:
        summary.write('Corrected: ' + str(avgCorrected/runs) + ' ')
        summary.write('Recomputed: ' + str(avgRecomputed/runs) + ' ')
        summary.write('Recomputes: ' + str(avgRecomputeCalls/runs) + ' ')
        summary.write('DotProducts: ' + str(avgDotProducts/runs) + ' ')
        summary.write('Correct: ' + str(corrects) + ' ')
        summary.write('Incorrect: ' + str(incorrects) + '\n')
        
        avgCorrected = 0
        avgRecomputed = 0
        avgRecomputeCalls = 0
        avgDotProducts = 0
        corrects = 0
        incorrects = 0
        runs = 0

        # Write the percentage amount
        summary.write(lines[i] + ' ')
        
    else:
        lines[i] = lines[i].split()
        avgCorrected += int(lines[i][lines[i].index('Corrected:') + 1])
        avgRecomputed += int(lines[i][lines[i].index('Recomputed:') + 1])
        avgRecomputeCalls += int(lines[i][lines[i].index('called:') + 1])
        avgDotProducts += int(lines[i][lines[i].index('called:', lines[i].index('called:') + 1) + 1])
        correct = not bool(lines[i].count('Not'))
        if correct == True:
            corrects += 1
        else:
            incorrects += 1
        runs += 1
        if i == len(lines) - 1:
            summary.write('Corrected: ' + str(avgCorrected/runs) + ' ')
            summary.write('Recomputed: ' + str(avgRecomputed/runs) + ' ')
            summary.write('Recomputes: ' + str(avgRecomputeCalls/runs) + ' ')
            summary.write('DotProducts: ' + str(avgDotProducts/runs) + ' ')
            summary.write('Correct: ' + str(corrects) + ' ')
            summary.write('Incorrect: ' + str(incorrects) + '\n')
        

summary.close()
