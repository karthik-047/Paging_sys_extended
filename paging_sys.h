#ifndef KRISHNAMOORTHYNATARAJANKE_PROJ2_H
#define KRISHNAMOORTHYNATARAJANKE_PROJ2_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define ITERATIONS 100  //no. of page calls	
#define RAND_LIMIT 16	//upper bound for page number


void fifo(int);
void lru(int);
void second(int);
int check_cache(int mem[][ITERATIONS+1], int,int,int);
int load_init(int mem[][ITERATIONS+1], int,int,int);
int ret_max(int mem[][ITERATIONS+1], int age[],int,int);
int hit_loc(int mem[][ITERATIONS+1], int,int,int);
int age_n_ref(int mem[][ITERATIONS+1], int age[],int ref_bit[],int,int);

#endif
