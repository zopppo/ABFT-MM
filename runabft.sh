#! /bin/bash
OUTPUTFILE=result
COUNTFILE=count
HOST=sanguine.usrc.newmexicoconsortium.org
# Check if the variable runs exists
if [ ! -e $COUNTFILE ]
then
   echo "1" > count
else
  declare -i currentCount=$(cat $COUNTFILE)
  let "m = 1 + $currentCount"
  echo $m > $COUNTFILE
fi

# if the A.dat file does not exist, make one
if [ ! -e "A.dat" ]; then
  ./makeMatrix -o A.dat -m 50 -n 50 -l 0 -u 100
fi

if [ ! -e "B.dat" ]; then
  ./makeMatrix -o B.dat -m 50 -n 50 -l 0 -u 100
fi

if [ ! -e "Cprime.dat" ]; then
  ./checksumA -a A.dat -o Aprime.dat
  ./checksumB -b B.dat -o Bprime.dat
  ./multiplyMatrix -a Aprime.dat -b Bprime.dat -o Cprime.dat
fi

  echo "Run $(cat $COUNTFILE)" >> $OUTPUTFILE

  RUNS=10
  until [ $RUNS -lt 1 ];
  do
    ./run.sh >> $OUTPUTFILE
    let RUNS-=1
  done
#  ssh $HOST "cd ../resilience/fsefi/TEMU_MM/tracecap/ini/ ; python changeConfig.py"


