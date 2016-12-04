#!/bin/bash

rm out.txt
for((threadCount=1; threadCount<4000; threadCount*=2))
do
	echo "threadCount = $threadCount" | tee -a out.txt
	export THREAD_COUNT=$threadCount
	#make avxomp | tee -a out.txt
	make avxomp > /dev/null
	#./gaussAVX+OMP 2>&1| tee -a out.txt
	./gaussAVX+OMP >> out.txt
done
