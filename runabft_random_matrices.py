#!/usr/bin/env python
import subprocess

# The file that we want to save the results in
output_file = "result"
# The probability table
prob_table = "probability.txt"

# Get the number of lines in the probability.txt file
p = subprocess.Popen('cat ' + prob_table + ' | wc -l', shell=True, stdout=subprocess.PIPE)
lines, err = p.communicate()

# The number of trials we will be running
count = int(lines) - 1

out = open(output_file, 'w')
prob = open(prob_table, 'r')

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
    p = subprocess.Popen('./run.sh', shell=True, stdout=subprocess.PIPE)
    output, err = p.communicate()
    # decode the output from a binary string to utf-8
    output = output.decode("utf-8")
    out.write(output)

out.close()
prob.close()
#os.sys("./run.sh >> $OUTPUTFILE")

