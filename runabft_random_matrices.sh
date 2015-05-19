#! /bin/bash
OUTPUTFILE=result
COUNT=10

if [ -e "$OUTPUTFILE" ]; then
    rm "$OUTPUTFILE"
fi


until [ $COUNT -lt 1 ];
do
    ./makeMatrix -o A.dat -m 2 -n 2 -l 0 -u 127
    ./makeMatrix -o B.dat -m 2 -n 2 -l 0 -u 127 
    ./checksumA -a A.dat -o Aprime.dat 
    ./checksumB -b B.dat -o Bprime.dat 
    ./multiplyMatrix -a Aprime.dat -b Bprime.dat -o Cprime.dat
    ./run.sh >> $OUTPUTFILE
    let COUNT-=1
done


