#! /bin/bash
OUTPUTFILE=mmTest
COUNT=5000

if [ -e "$OUTPUTFILE" ]; then
    rm "$OUTPUTFILE"
fi


until [ $COUNT -lt 1 ];
do
    ./makeMatrix -o A.dat -m 45 -n 45 -l 0 -u 127
    ./makeMatrix -o B.dat -m 45 -n 45 -l 0 -u 127 
    ./testMM -a A.dat -b B.dat -o C.dat >> $OUTPUTFILE
    let COUNT-=1
done


