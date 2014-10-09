# Need regular expressions
import fileinput
import re
import sys

for line in fileinput.input("SEFI_conf.ini", inplace=True):
  # If this is the right line

  if line.find("SEFI_support_fault_probability =") != -1:
    # look for the probability
    num = re.search("\d*[.]\d+", line)
    prob = float(line[num.start():num.end()])
    # increment it
    prob += .05
    print("SEFI_support_fault_probability = " + str(prob))
  else:
      sys.stdout.write(line)

