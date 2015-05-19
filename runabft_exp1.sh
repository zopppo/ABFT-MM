#! /bin/bash
OUTPUTFILE=result
COUNT=10000

if [ -e "$OUTPUTFILE" ]; then
	rm "$OUTPUTFILE"
fi
# if the A.dat file does not exist, make one
if [ ! -e "A.dat" ]; then
	./makeMatrix -o A.dat -m 50 -n 50 -l 0 -u 50 
fi

if [ ! -e "B.dat" ]; then
	./makeMatrix -o B.dat -m 50 -n 50 -l 0 -u 50
fi

./checksumA -a A.dat -o Aprime.dat 
./checksumB -b B.dat -o Bprime.dat 
./multiplyMatrix -a Aprime.dat -b Bprime.dat -o Cprime.dat

until [ $COUNT -lt 1 ];
do
  ./run.sh >> $OUTPUTFILE
  let COUNT-=1
done


