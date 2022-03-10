
#include "krishnamoorthynatarajanke_proj2.h"


//check age and ref bit for second chance
int age_n_ref(int mem[][ITERATIONS+1],int age[],int ref_bit[],int index,int fsize){
	int max=0,ind=-1,i;
	int age_ref[RAND_LIMIT];
	for(i=0;i<RAND_LIMIT;i++)
		age_ref[i]=age[i];
	while(1){
	for(i=0;i<RAND_LIMIT;i++){
		if(age_ref[i]>max){
			max = age_ref[i];
			ind = i;
		}
	}
	if(ref_bit[ind]==1){
		ref_bit[ind]=0;
		age_ref[ind]=0;
		max=0;
	}
	else
		break;
	}
	for(i=0;i<fsize;i++){
		if(ind==mem[i][index]){
			ind = i;
		}
	}
	return ind;
}
//return index of oldest page - FIFO
int ret_max(int mem[][ITERATIONS+1],int age[],int index,int fsize){
	int max=0,ind=0,i;
	for(i=0;i<RAND_LIMIT;i++){
		if(age[i]>max){
			max = age[i];
			ind = i;
		}
	}
	for(i=0;i<fsize;i++){
		if(ind==mem[i][index]){
			ind = i;
		}
	}
	return ind;	
}

int hit_loc(int mem[][ITERATIONS+1],int index,int fsize,int curr_page){
	int i,loc=-1;
	for(i=0;i<fsize;i++){
		if(mem[i][index]== curr_page){
			loc = i;
			break;
		}
	}
	return loc;
}

//check for space in cache and return index
int load_init(int mem[][ITERATIONS+1],int index,int fsize,int curr_page)
{
	int i,loc=-1;
	for(i=0;i<fsize;i++){
		if(mem[i][index]==-1){
			loc = i;
			break;
		}
	}
	return loc;	
}

int check_cache(int mem[][ITERATIONS+1],int index,int fsize,int curr_page){
	int i,j,stat=0;
	for(i=0;i<fsize;i++){
		if(mem[i][index]==curr_page){
			stat = 1; //present in cache
			break;
		}
	}
	return stat;
}

