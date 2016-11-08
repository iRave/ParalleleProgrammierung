//
//  main.c
//  PosixOverhead
//
//  Created by Till Busse on 03/11/16.
//  Copyright © 2016 Till Busse. All rights reserved.
//
// @ Strey:
// 2)
// Das Program braucht bei 10 Ausführungen im Mittel eine Zeit von 0.262s.
// Die Erstellung eines Threads braucht dementsprechend 26µs.
// Das erstellen und joinen von Threads ist sehr schnell, darf dennoch nicht vernachlässigt werden. Für sehr kleine Operationen könnte der entstehende Overhead den gewonnen Geschwindigkeitsvorteil durch die Parallelisierung übersteigen. 

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
