#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define ITERATIONS 100
#define RAND_LIMIT 16
const char file_4[] = "krishnamoorthynatarajanke_proj2_output_4frames.txt";
const char file_8[] = "krishnamoorthynatarajanke_proj2_output_8frames.txt";
const char ip_file[] = "krishnamoorthynatarajanke_proj2_input.txt";

void fifo(int);
void lru(int);
void second(int);
int check_cache(int mem[][ITERATIONS+1], int,int,int);
int load_init(int mem[][ITERATIONS+1], int,int,int);
int ret_max(int mem[][ITERATIONS+1], int age[],int,int);
int hit_loc(int mem[][ITERATIONS+1], int,int,int);
int age_n_ref(int mem[][ITERATIONS+1], int age[],int ref_bit[],int,int);


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
	//printf("\n Oldest page is %d with age %d",ind,max);
	if(ref_bit[ind]==1){
		ref_bit[ind]=0;
		age_ref[ind]=0;
		max=0;
		//printf("\n Ref bit of oldest page %d is 1 with age %d",ind,max);
	}
	else
		break;
	}
	//printf("\n Finally, the oldest page is %d with age %d",ind,max);
	for(i=0;i<fsize;i++){
		if(ind==mem[i][index]){
			ind = i;
		}
	}
	//printf("\n Oldest page present at ind %d",ind);
	return ind;
}

int ret_max(int mem[][ITERATIONS+1],int age[],int index,int fsize){
	int max=0,ind=0,i;
	for(i=0;i<RAND_LIMIT;i++){
		if(age[i]>max){
			max = age[i];
			ind = i;
		}
	}
	//printf("\n Oldest page is %d with age %d",ind,max);
	for(i=0;i<fsize;i++){
		if(ind==mem[i][index]){
			ind = i;
		}
	}
	//printf("\n Oldest page present at ind %d",ind);
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
	//printf("\n Space availabe at loc: %d",loc);
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
	//printf("\n Cache status returned");
	return stat;
}

void second(int f_size){
	int age[RAND_LIMIT];
	int i,j,load,hit,a,b,load_ind,max_age,age_ind,rst_age,s_fault_count=0;
	int ref_bit[RAND_LIMIT];
	int memory_blk[f_size][ITERATIONS+1];
	int fault_second[ITERATIONS];
	for(i=0;i<f_size;i++)
  	    for(j=0;j<(ITERATIONS+1);j++)
		    memory_blk[i][j] = -1;
	for(i=0;i<RAND_LIMIT;i++){
		age[i] = 0;
		ref_bit[i] = -1;
	}
	FILE *fpr,*fpw;
    fpr = fopen(ip_file,"r");
    if(fpr==NULL){
	    perror("Error reading ref string \n");
	    exit(EXIT_FAILURE);
    }
	if(f_size==4){
		fpw = fopen(file_4,"a");
		if(fpw==NULL){
			perror("Error wrting result lru4--");
			exit(EXIT_FAILURE);
		}
	}	
	else if(f_size==8){
		fpw = fopen(file_8,"a");
		if(fpw==NULL){
			perror("Error wrting result lru8--");
			exit(EXIT_FAILURE);
		}
	}
	fprintf(fpw,"\t\tPolicy: Second Chance | Frame Size:  %d\n\n",f_size);
 	for(i=0;i<ITERATIONS;i++){
		fscanf(fpr,"%d,",&load);
        printf("\n Loaded page at %d iteration: %d",(i+1),load);
		for(j=0;j<f_size;j++){
			age_ind = memory_blk[j][i];
			//printf("\n Value of age_index: %d",age_ind);
			if(age_ind!=-1)
                age[age_ind]++;
		}
		if((ref_bit[load]==0)||(ref_bit[load]==1))
			ref_bit[load] = 1;
		else if (ref_bit[load]==-1)
			ref_bit[load]++;
		hit = check_cache(memory_blk,i,f_size,load);
 		printf("\n Cache stat: %d",hit);
		if(hit==1){
			printf("\n Cache hit");
			fault_second[i] = 0;
			printf("\n Copying same content");
			for(j=0;j<f_size;j++)
				memory_blk[j][i+1] = memory_blk[j][i];

		}
		else if(hit==0){
			printf("\n Cache miss");
			fault_second[i] = 1;
			s_fault_count++;
			load_ind = load_init(memory_blk,i,f_size,load);
			if(load_ind==-1){
				max_age  = age_n_ref(memory_blk,age,ref_bit,i,f_size);
				rst_age = memory_blk[max_age][i];
				printf("\n Page to be replaced is %d at index %d",rst_age,max_age);
				age[rst_age]=0;
				for(j=0;j<f_size;j++)
					memory_blk[j][i+1] = memory_blk[j][i];
				ref_bit[memory_blk[max_age][i]]=-1;
				memory_blk[max_age][i+1] = load;			
			}
			else{
				for(j=1;j<=load_ind;j++)
					memory_blk[j][i+1] = memory_blk[j-1][i];
				memory_blk[0][i+1] = load;
				}
		}
	if(fault_second[i]==0)
			fprintf(fpw,"Page referenced: %d | No page fault\n",load);
		else
			fprintf(fpw,"Page referenced: %d | Page fault occured\n",load);
	}
	fclose(fpr);
	printf("\n Total number of page faults: %d",s_fault_count);
	fprintf(fpw,"\n\nTotal Page Faults for Second Chance: %d\n\n\n\n",s_fault_count);
	fclose(fpw);  	
	printf("\n Total number of page faults: %d",s_fault_count);
}


void fifo(int f_size){
	int i,j,load,hit,a,b,load_ind,max_age,age_ind,rst_age,f_fault_count=0;
	int memory_blk[f_size][ITERATIONS+1];
    int age[RAND_LIMIT];
	int fault_fifo[ITERATIONS];
    for(i=0;i<RAND_LIMIT;i++)
        age[i] = 0;
    for(i=0;i<f_size;i++)
  	    for(j=0;j<(ITERATIONS+1);j++)
		    memory_blk[i][j] = -1;
	FILE *fpr,*fpw;
    fpr = fopen(ip_file,"r");
    if(fpr==NULL){
	    perror("Error reading ref string \n");
	    exit(EXIT_FAILURE);
    }
	if(f_size==4){
		fpw = fopen(file_4,"a");
		if(fpw==NULL){
			perror("Error wrting result fifo4--");
			exit(EXIT_FAILURE);
		}
	}	
	else if(f_size==8){
		fpw = fopen(file_8,"a");
		if(fpw==NULL){
			perror("Error wrting result fifo8--");
			exit(EXIT_FAILURE);
		}
	}
	fprintf(fpw,"\t\tPolicy: FIFO | Frame Size:  %d\n\n",f_size);		
    for(i=0;i<ITERATIONS;i++){
        fscanf(fpr,"%d,",&load);
        printf("\n Loaded page at %d iteration: %d",(i+1),load);
		for(j=0;j<f_size;j++){
			age_ind = memory_blk[j][i];
			//printf("\n Value of age_index: %d",age_ind);
			if(age_ind!=-1)
                age[age_ind]++;
		}
		//printf("\n Need page %d at time %d",load,i);
		hit = check_cache(memory_blk,i,f_size,load);
		//printf("\n Cache stat: %d",hit);
		if(hit==1){
			printf("\n Cache hit");
			fault_fifo[i]=0;
			for(j=0;j<f_size;j++)
				memory_blk[j][i+1] = memory_blk[j][i];
		}
		if(hit==0){
			printf("\n Cache miss");
			fault_fifo[i]=1;
			f_fault_count++;
			load_ind = load_init(memory_blk,i,f_size,load);
			if(load_ind==-1){
				//printf("\n Space not available");
				max_age = ret_max(memory_blk,age,i,f_size);
				rst_age = memory_blk[max_age][i];
				age[rst_age] = 0;
				for(j=0;j<f_size;j++)
					memory_blk[j][i+1] = memory_blk[j][i];
				memory_blk[max_age][i+1] = load;
			}
			else{
				//printf("\n Space available at location: %d",load_ind);
				for(j=1;j<=load_ind;j++)
					memory_blk[j][i+1] = memory_blk[j-1][i];
				memory_blk[0][i+1] = load;	
			}

		}
		if(fault_fifo[i]==0)
			fprintf(fpw,"Page referenced: %d | No page fault\n",load);
		else
			fprintf(fpw,"Page referenced: %d | Page fault occured\n",load);
	}
	fclose(fpr);
	printf("\n Total number of page faults: %d",f_fault_count);
	fprintf(fpw,"\n\nTotal Page Faults for FIFO: %d\n\n\n\n",f_fault_count);
	fclose(fpw);
}

void lru(int f_size){

	int load,a,b,i=0,j=0,mem_loc, hit,hit_ind,load_ind,mem_ind,l_fault_count=0;
	int memory_blk[f_size][ITERATIONS+1];
	int fault_lru[ITERATIONS];
    for(i=0;i<f_size;i++)
  	    for(j=0;j<(ITERATIONS+1);j++)
		    memory_blk[i][j] = -1;
	FILE *fpr,*fpw;
    fpr = fopen(ip_file,"r");
    if(fpr==NULL){
	    perror("Error reading ref string \n");
	    exit(EXIT_FAILURE);
    }
	if(f_size==4){
		fpw = fopen(file_4,"a");
		if(fpw==NULL){
			perror("Error wrting result lru4--");
			exit(EXIT_FAILURE);
		}
	}	
	else if(f_size==8){
		fpw = fopen(file_8,"a");
		if(fpw==NULL){
			perror("Error wrting result lru8--");
			exit(EXIT_FAILURE);
		}
	}
	fprintf(fpw,"\t\tPolicy: LRU | Frame Size:  %d\n\n",f_size);	
	for(i=0;i<ITERATIONS;i++){
		fscanf(fpr,"%d,",&load);
		printf("\n Loaded page at %d iteration: %d",(i+1),load);
		hit = check_cache(memory_blk,i,f_size,load);
		if(hit==1){
			printf("\n Cache hit");
			fault_lru[i]=0;
			hit_ind = hit_loc(memory_blk,i,f_size,load);
			for(j=1;j<=hit_ind;j++)
 				memory_blk[j][i+1] = memory_blk[j-1][i];
			for(j=(hit_ind+1);j<f_size;j++)
				memory_blk[j][i+1] = memory_blk[j][i]; 
			memory_blk[0][i+1] = load;
		}
		else if(hit==0){
			printf("\n Cache miss");
			fault_lru[i]=1;
			l_fault_count++;
			load_ind = load_init(memory_blk,i,f_size,load);
			if(load_ind==-1){
				for(j=1;j<f_size;j++)
					memory_blk[j][i+1]=memory_blk[j-1][i];
			}
			else{
				for(j=1;j<=load_ind;j++)
					memory_blk[j][i+1]=memory_blk[j-1][i];
			}
			memory_blk[0][i+1] = load;
		}
		if(fault_lru[i]==0)
			fprintf(fpw,"Page referenced: %d | No page fault\n",load);
		else
			fprintf(fpw,"Page referenced: %d | Page fault occured\n",load);
	}
	fclose(fpr);
	printf("\n Total number of page faults: %d",l_fault_count);
	fprintf(fpw,"\n\nTotal Page Faults for LRU: %d\n\n\n\n",l_fault_count);
	fclose(fpw);
}
int main(void){
  int frame_choice,alg_choice,rand_string;
  int i,j;
  time_t t;
  srand((unsigned) time(&t));
  printf("\n Enter the replacement policy choice:");
  printf("\n \t1.FIFO \n \t2.LRU \n \t3.Second Chance");
  scanf("%d",&alg_choice);
  printf("\n Enter the number of page frames [4 or 8]:");
  scanf("%d",&frame_choice);
  if((frame_choice!=4)&&(frame_choice!=8)){
	  printf("\n Only 4 and 8 are the valid choices");
	  exit(EXIT_FAILURE);
  }
  FILE *fp;
  fp = fopen(ip_file,"w");
  if(fp==NULL){
	  perror("Error opening file to write\n");
	  exit(EXIT_FAILURE);
  }
  for(i=0;i<ITERATIONS;i++){
        rand_string = rand() % RAND_LIMIT;
        fprintf(fp,"%d,",rand_string);
  }
  fclose(fp);
  switch(alg_choice){
		case 1: printf("\n \t\tImplementing FIFO\n");
				fifo(frame_choice);
		break;
		case 2: printf("\n \t\tImplementing LRU\n");
    			lru(frame_choice);
		break;
	  	case 3: printf("\n \t\tImplementing Second Chance\n");
			  	second(frame_choice);
	 	break;
	  default:printf("\n Invalid entry. Please enter a number in range [1,3]\n");
	  exit(0);
  }    
  return 0;
}