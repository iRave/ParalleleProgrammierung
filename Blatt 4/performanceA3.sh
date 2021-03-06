#!/bin/bash

rm out.txt
gcc -o single.o quicksort.c
for((threadCount=1; threadCount<10000; threadCount*=2))
do
  echo "threadCount = $threadCount" >> out.txt
  echo "" >> out.txt
  for((run = 1; run <=10; run++))
  do
    echo "Run $run:" >> out.txt
    gcc -D THREAD_COUNT=$threadCount Blatt4_OpenMP_A3_Final.c -o multi.o -fopenmp
    tMulti=$( { /usr/bin/time -f "%e" ./multi.o -args; } 2>&1 )
    tSingle=$( { /usr/bin/time -f "%e" ./single.o -args; } 2>&1 )
    (bc -l <<<"$tSingle/$tMulti") >> out.txt
    #(time ./main.o) >> out.txt 2>&1
    #time ./main.o
    tail -n 4 out.txt
  done
done
