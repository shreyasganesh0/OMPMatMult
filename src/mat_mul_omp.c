#include <omp.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *A;
int *B;
int *C;
int NRA;
int NCA_RB;
int NCB;

void init_matrices();

int main(int argc, char *argv[]) {
	struct timespec start, end;
	
	if (argc < 4) {
		printf("Too few arguments\n");
	}
	
	NRA = atoi(argv[1]);
	NCA_RB = atoi(argv[2]);
	NCB = atoi(argv[3]);
	
	init_matrices();


	int tot_calc = NRA * NCB * NCA_RB;
	int nthreads = omp_get_max_threads();
	printf("nthread is %d\n", nthreads);
	int chunk = tot_calc / nthreads;   
	int rem = tot_calc % nthreads;      

	clock_gettime(CLOCK_MONOTONIC, &start);
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
   
	clock_gettime(CLOCK_MONOTONIC, &end);

	printf("\n\n ==== Final matrix \n\n");
	for (int i = 0; i < NRA; i++) {
		for (int j = 0; j < NCB; j++) {
			printf("%d, ", C[i * NCB + j]);
		}
		printf("\n");
	}
	double diffs = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	printf("\n --- Time taken %f\n", diffs);
}

void init_matrices() {
	
	A = malloc(NRA * NCA_RB * sizeof(int));
	B = malloc(NCB * NCA_RB * sizeof(int));
	C = malloc(NRA * NCB * sizeof(int));

	printf("\n\n ----- A mat\n");
	for (int i = 0; i < NRA; i++) {
		for (int j = 0; j < NCA_RB; j++) {
			A[i * NCA_RB + j] = i + j;
			printf("%d, ", A[i * NCA_RB + j]);
		}
		printf("\n");
	}
	printf("\n\n ----- B mat\n");
	for (int i = 0; i < NCA_RB; i++) {
		for (int j = 0; j < NCB; j++) {
			B[i * NCB + j] = i - j; 
			printf("%d, ", B[i * NCB + j]);
		}
		printf("\n");
	}
	for (int i = 0; i < NRA; i++) {
		for (int j = 0; j < NCB; j++) {
			C[i * NCB + j] = 0;
		}
	}
	printf("\n");
}

			
