clean:
	rm -f gaussAVXonly
	rm -f gaussAVX+OMP
	rm -f gaussAVXonlyD
	rm -f gaussAVX+OMPD
	rm -f gaussStd
gauss:
	gcc gauss5x5.c -o gaussStd
avxonly:
	gcc gauss5x5_AVXonly.c -o gaussAVXonly -mavx -march=native -std=c99 -DFILTER_COUNT=10 -DRUN_COUNT=30 -DTHREAD_COUNT=${THREAD_COUNT}
avxomp:
	gcc gauss5x5_AVX+OMP.c -o gaussAVX+OMP -mavx -march=native -std=c99 -DFILTER_COUNT=10 -DRUN_COUNT=30 -DTHREAD_COUNT=${THREAD_COUNT} -fopenmp

avxonlyd:
	gcc gauss5x5_AVXonly.c -o gaussAVXonlyD -mavx -march=native -std=c99 -DFILTER_COUNT=10 -DDEBUG=1 -DRUN_COUNT=30 -DTHREAD_COUNT=${THREAD_COUNT}
runavxonlyd: avxonly
	@echo Running AVXonly:
	@./gaussAVXonlyD
avxompd:
	gcc gauss5x5_AVX+OMP.c -o gaussAVX+OMPD -mavx -march=native -std=c99 -DFILTER_COUNT=10 -DDEBUG=1 -DRUN_COUNT=30 -DTHREAD_COUNT=${THREAD_COUNT} -fopenmp
runavxompd: avxompd
	@echo Running AVX+OMP:
	@./gaussAVX+OMPD
all: avxonly avxomp
alld: avxonlyd avxompd
runall: all
	@echo Running AVXonly:
	@./gaussAVXonly
	@echo Running AVX+OMP:
	@./gaussAVX+OMP
runalld: alld
	@echo Running AVXonly:
	@./gaussAVXonlyD
	@echo Running AVX+OMP:
	@./gaussAVX+OMPD
