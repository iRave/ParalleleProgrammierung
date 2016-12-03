#!/bin/bash
#rm ./out.txt
#for((NC = 1000000; NC <=32000000; NC*=2))
for((NC = 32000000; NC <=128000000; NC*=2))
do
  gcc -D A1 -D N=$NC Blatt4_OpenMP_A2.c -o single.o -lquadmath
  tSingle=$( { /usr/bin/time -f "%e" ./single.o -args; } 2>&1 )
  echo "" >> ./out.txt
  echo "################################################################" >> ./out.txt
  echo "N: $NC" >> ./out.txt
  echo "" >> ./out.txt
  for((threadCount=1; threadCount<4000; threadCount*=2))
  do
    echo "" >> ./out.txt
    echo "ThreadCount = $threadCount" >> ./out.txt
    gcc -D THREAD_COUNT=$threadCount -D N=$NC -D A2 Blatt4_OpenMP_A2.c -o multi.o -lquadmath -fopenmp
    tMulti=$( { /usr/bin/time -f "%e" ./multi.o -args; } 2>&1 )
    (bc -l <<<"$tSingle/$tMulti") >> ./out.txt
    echo "" >> ./out.txt
  done
done
