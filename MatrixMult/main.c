//
//  main.c
//  PosixOverhead
//
//  Created by Till Busse on 03/11/16.
//  Copyright © 2016 Till Busse. All rights reserved.
//
// @ Strey:
// Das Program braucht bei 10 Ausführungen im Mittel eine Zeit von 0.262s.
// Die Erstellung eines Threads braucht dementsprechend 26µs.

#include <stdio.h>
#include <pthread.h>
#define N 10000

void dummyFunc(){}

int main(int argc, const char * argv[]) {
    pthread_t id;
    for (int i = 0; i < N; i++) {
        pthread_create(&id, NULL, dummyFunc, NULL);
        pthread_join(id, NULL);
    }
    
    return 0;
}
