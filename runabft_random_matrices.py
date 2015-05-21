#!/usr/bin/env python
import subprocess

# The file that we want to save the results in
output_file = "result"
# The probability table
prob_table = "probability.txt"
# The number of trials we will be running 
count = int(subprocess.check_output('cat ' + prob_table + ' | wc -l', shell=True)) - 1
# The number of runs per trial

with open(output_file, 'w') as out, open(prob_table, 'r') as prob:
    oldProbability = ""
    for i in range(0, count + 1):
            currentProbability = prob.readline()
            if currentProbability != oldProbability:
                out.write(currentProbability.rstrip("\n") + "%" + '\n')
                oldProbability = currentProbability

            # Randomly create matrices
            subprocess.call(['./makeMatrix', '-o', 'A.dat', '-m', '45', '-n', '45', '-l', '0', '-u', '127'])
            subprocess.call(['./makeMatrix', '-o', 'B.dat', '-m', '45', '-n', '45', '-l', '0', '-u', '127'])
            # Check sum the matrices
            subprocess.call(['./checksumA', '-a', 'A.dat', '-o' , 'Aprime.dat'])
            subprocess.call(['./checksumB', '-b', 'B.dat', '-o' , 'Bprime.dat'])
            # Multiply the matrices to create the golden Cprime
            subprocess.call(['./multiplyMatrix', '-a', 'Aprime.dat', '-b', 'Bprime.dat', '-o', 'Cprime.dat'])
            # Run the abft driver with the matrices
            output = subprocess.check_output("./run.sh", shell=True)
            # decode the output from a binary string to utf-8
            output = output.decode("utf-8")
            out.write(output)
#os.sys("./run.sh >> $OUTPUTFILE")

