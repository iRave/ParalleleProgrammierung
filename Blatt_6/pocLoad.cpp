//
// Created by Till Busse on 02/12/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

#define SIZE 10

__attribute__((aligned(32))) float a[SIZE], b[SIZE], c[SIZE];
__m256 m1, m2, m3;

int main(){
    for (int i=0; i<SIZE; i++) {
        a[i] = 1;
        b[i] = 2;
    }
    for (int i=0; i<SIZE; i=i+8) {
        m1 = _mm256_load_ps(a+i);
        m2 = _mm256_load_ps(b+i);
        m3 = _mm256_mul_ps(m1,m2);
        _mm256_store_ps(c+i,m3);
    }
}