#!/bin/bash

for((threadCount=1; threadCount<10000; threadCount*=2))
do
  echo "threadCount = $threadCount" >> out.txt
  for((NC = 1000000; NC <=32000000; NC*=2))
  do
    echo "" >> out.txt
    echo "NCount = $NC" >> out.txt
    gcc -D THREAD_COUNT=$threadCount -D N=$NC Blatt4_OpenMP_A2.c -O0 -o main.o -lquadmath -fopenmp 
  #  /usr/bin/time -f "%e" ./main.o
    (time ./main.o) >> out.txt 2>&1
  #time ./main.o
  done
done
