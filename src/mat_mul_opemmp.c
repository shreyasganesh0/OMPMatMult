#include <omp.h> 
#include <stdio.h>
#include <stdlib.h>

int *A;
int *B;
int *C;
int NRA;
int NCA_RB;
int NCB;

void init_matrices();

int main(int argc, char *argv[]) {
	
	if (argc < 4) {
		printf("Too few arguments\n");
	}
	
	NRA = atoi(argv[1]);
	NCA_RB = atoi(argv[2]);
	NCB = atoi(argv[3]);
	
	init_matrices();

	int tot_calc = NRA * NCB * NCA_RB;
	int nthreads = omp_get_max_threads();
	int chunk = tot_calc / nthreads;   
	int rem = tot_calc % nthreads;      

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int start, end;
        
        if (tid < rem) {
            start = tid * (chunk + 1);
            end = start + chunk + 1;
        } else {
            start = tid * chunk + rem;
            end = start + chunk;
        }
        
        int i = 0, j = 0, k = 0, temp = 0;
        for (int t = 0; t < start; t++) {
            k++;  
            temp++;
            k = k - ((k >= NCA_RB) * NCA_RB);
            j = j + (temp >= NCA_RB);
            temp = temp - ((temp >= NCA_RB) * NCA_RB);
            i = i + (j >= NCB);
            j = j - ((j >= NCB) * NCB);
        }
        
        for (int iter = start; iter < end; iter++) {
            int prod = A[i * NCA_RB + k] * B[k * NCB + j];
            #pragma omp atomic
            C[i * NCB + j] += prod;
            
            k++;  
            temp++;
            k = k - ((k >= NCA_RB) * NCA_RB);
            j = j + (temp >= NCA_RB);
            temp = temp - ((temp >= NCA_RB) * NCA_RB);
            i = i + (j >= NCB);
            j = j - ((j >= NCB) * NCB);
        }
    } 
   

	printf("\n\n ==== Final matrix \n\n");
	for (int i = 0; i < NRA; i++) {
		for (int j = 0; j < NCB; j++) {
			printf("%d, ", C[i * NCB + j]);
		}
		printf("\n");
	}
}

void init_matrices() {
	
	A = malloc(NRA * NCA_RB * sizeof(int));
	B = malloc(NCB * NCA_RB * sizeof(int));
	C = malloc(NCA_RB * NCA_RB * sizeof(int));

	int indx = 0;
	int rid = 0;
	for (int i = 0; i < NRA * NCA_RB; i++) {
		A[rid * NCA_RB + indx] = indx + rid;
		indx++;
		rid += (indx >= NCA_RB) * 1;
		indx = indx - (indx >= NCA_RB) * NCA_RB;
	}
	
	for (int i = 0; i < NCB * NCA_RB; i++) {
		B[rid * NCB + indx] = rid - indx; 
		indx++;
		rid += (indx >= NCB) * 1;
		indx = indx - (indx >= NCB) * NCB;
	}
}

			
