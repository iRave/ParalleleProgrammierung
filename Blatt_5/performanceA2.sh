#!/bin/bash

rm out.txt
avSingleTime=0
for((singleRun=0; singleRun<2; singleRun++))
do
  temp=$( { /usr/bin/time -f "%e" ./serial.o -args; } 2>&1 )
  avSingleTime=$(bc -l <<<"$temp + $avSingleTime")
done

avSingleTime=$(bc -l <<<"$avSingleTime/2.0")
avMultiTime=0
for((processCount=4; processCount<=2048; processCount*=2))
do
  echo $processCount
  echo "ProcessCount = $processCount" >> out.txt
  echo "" >> out.txt
  mpicc -o parallel.o paraMandel.c -DPROCESS_COUNT=$processCount -std=c99
  avMultiTime=0
  echo "Still alive 1"
  echo ""
  for((run = 1; run <=10; run++))
  do
    echo "Still alive 2"
    echo ""
    temp=$( { /usr/bin/time -f "%e" mpirun -np $processCount parallel.o -args; } 2>&1 )
    echo "Temp: " $temp
    echo ""
    avMultiTime=$(bc -l <<<"$temp + $avMultiTime")
 #   tail -n 4 out.txt
  done
  echo "Still alive 3"
  (bc -l <<<"$avSingleTime/$avMultiTime/10.0") >> out.txt
  echo "still alive 4"
done
