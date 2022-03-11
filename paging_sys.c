#include "paging_sys.h"

//filenames
char file_4[50] = "paging_sys_output_4frames.txt";
char file_8[50] = "paging_sys_output_8frames.txt";
char ip_file[50] = "paging_sys_input.txt";


//second chance
void second(int f_size){
	int age[RAND_LIMIT];  //for counting age
	int i,j,load,hit,a,b,load_ind,max_age,age_ind,rst_age,s_fault_count=0;
	int ref_bit[RAND_LIMIT];  //for storing ref bit
	int memory_blk[f_size][ITERATIONS+1];  //frame memory
	int fault_second[ITERATIONS];   //for storing page faults
	time_t now;
	for(i=0;i<f_size;i++)
  	    for(j=0;j<(ITERATIONS+1);j++)
		    memory_blk[i][j] = -1;
	for(i=0;i<RAND_LIMIT;i++){
		age[i] = 0;
		ref_bit[i] = -1;
	}
	//opening files for read and write
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
		time(&now);
		fscanf(fpr,"%d,",&load);
        printf("\n Page %d referenced at time %s",load,ctime(&now));
		for(j=0;j<f_size;j++){   //incrementing age for pages in frame
			age_ind = memory_blk[j][i];
			if(age_ind!=-1)
                age[age_ind]++;
		}
		if((ref_bit[load]==0)||(ref_bit[load]==1))  //modiying ref bit
			ref_bit[load] = 1;
		else if (ref_bit[load]==-1)
			ref_bit[load]++;
		hit = check_cache(memory_blk,i,f_size,load);
		if(hit==1){
			printf("\n Cache hit");
			fault_second[i] = 0;
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
		//storing in txt file
		if(fault_second[i]==0)
			fprintf(fpw,"Page referenced: %d at %s No page fault\n\n",load,ctime(&now));
		else
			fprintf(fpw,"Page referenced: %d at %s Page fault occured\n\n",load, ctime(&now));
	}
	fclose(fpr);
	printf("\n Total number of page faults: %d",s_fault_count);
	fprintf(fpw,"\n\nTotal Page Faults for Second Chance: %d\n\n\n\n",s_fault_count);
	fclose(fpw);  	
}

//First in first out
void fifo(int f_size){
	int i,j,load,hit,a,b,load_ind,max_age,age_ind,rst_age,f_fault_count=0;
	int memory_blk[f_size][ITERATIONS+1]; //frame memory
    int age[RAND_LIMIT]; //for counting age
	int fault_fifo[ITERATIONS];  //for storing page faults
	time_t now;
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
		time(&now);
        fscanf(fpr,"%d,",&load);
        printf("\n Page %d referenced at time %s",load,ctime(&now));
		for(j=0;j<f_size;j++){
			age_ind = memory_blk[j][i];
			if(age_ind!=-1)
                age[age_ind]++;
		}
		hit = check_cache(memory_blk,i,f_size,load);
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
				max_age = ret_max(memory_blk,age,i,f_size);
				rst_age = memory_blk[max_age][i];
				age[rst_age] = 0;
				for(j=0;j<f_size;j++)
					memory_blk[j][i+1] = memory_blk[j][i];
				memory_blk[max_age][i+1] = load;
			}
			else{
				for(j=1;j<=load_ind;j++)
					memory_blk[j][i+1] = memory_blk[j-1][i];
				memory_blk[0][i+1] = load;	
			}

		}
		if(fault_fifo[i]==0)
			fprintf(fpw,"Page referenced: %d at %s No page fault\n\n",load,ctime(&now));
		else
			fprintf(fpw,"Page referenced: %d at %s Page fault occured\n\n",load,ctime(&now));
	}
	fclose(fpr);
	printf("\n Total number of page faults: %d",f_fault_count);
	fprintf(fpw,"\n\nTotal Page Faults for FIFO: %d\n\n\n\n",f_fault_count);
	fclose(fpw);
}

//Least Recently Used
void lru(int f_size){

	int load,a,b,i=0,j=0,mem_loc, hit,hit_ind,load_ind,mem_ind,l_fault_count=0;
	int memory_blk[f_size][ITERATIONS+1];
	int fault_lru[ITERATIONS];
	time_t now;
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
		time(&now);
		fscanf(fpr,"%d,",&load);
		printf("\n Page %d referenced at time %s",load,ctime(&now));
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
			fprintf(fpw,"Page referenced: %d at %s No page fault\n\n",load,ctime(&now));
		else
			fprintf(fpw,"Page referenced: %d at %s Page fault occured\n\n",load,ctime(&now));
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
  FILE *fp;
  fp = fopen(ip_file,"w");
  if(fp==NULL){
	  perror("Error opening file to write\n");
	  exit(EXIT_FAILURE);
  }
  //generating random int and storing as page numbers
  for(i=0;i<ITERATIONS;i++){
        rand_string = rand() % RAND_LIMIT;
        fprintf(fp,"%d,",rand_string);
  }
  fclose(fp);
  while(1){
	printf("\n Enter the replacement policy choice:");
	printf("\n \t1.FIFO \n \t2.LRU \n \t3.Second Chance\n \t4.Exit\n \t");
	scanf("%d",&alg_choice);
	if(alg_choice!=4){
		printf("\n Enter the number of page frames [4 or 8]:");
		scanf("%d",&frame_choice);	
		if((frame_choice!=4)&&(frame_choice!=8)){
			printf("\n Only 4 and 8 are the valid choices");
			exit(EXIT_FAILURE);
		}
	}
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
			case 4: printf("\n \t\tExiting...\n");
		  		  	exit(EXIT_SUCCESS);
			break;
		default:printf("\n Invalid entry. Please enter a number in range [1,4]\n");
		exit(0);
	}
}    
  return 0;
}
