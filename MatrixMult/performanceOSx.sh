for((threadCount=1; threadCount<10000; threadCount*=2))
do
  echo "threadCount = $threadCount"
  gcc -D THREAD_COUNT=$threadCount -o matmult matmult.c -lpthread
  /usr/bin/time -lp ./matmult
done
