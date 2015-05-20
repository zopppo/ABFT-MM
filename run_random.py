#!/usr/bin/env python
import subprocess

output_file = "result"
count = 10
runs_per = 5

f = open(output_file, 'w')

for i in range(0, count + 1):
    os.sys("./makeMatrix -o A.dat -m 45 -n 45 -l 0 -u 127");
    os.sys("/makeMatrix -o B.dat -m 45 -n 45 -l 0 -u 127"); 
    os.sys("./checksumA -a A.dat -o Aprime.dat");
    os.sys("./checksumB -b B.dat -o Bprime.dat"); 
    os.sys("./multiplyMatrix -a Aprime.dat -b Bprime.dat -o Cprime.dat");
    
    os.sys("./run.sh >> $OUTPUTFILE");
