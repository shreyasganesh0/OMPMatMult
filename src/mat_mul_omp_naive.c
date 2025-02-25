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

/*   ttype: type to use for representing time */
typedef double ttype;
ttype tdiff(struct timespec a, struct timespec b)
/* Find the time difference. */
{
  ttype dt = (( b.tv_sec - a.tv_sec ) + ( b.tv_nsec - a.tv_nsec ) / 1E9);
  return dt;
}

struct timespec now()
/* Return the current time. */
{
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  return t;
}

int main(int argc, char *argv[]) {
	struct timespec begin, end;
	
	if (argc < 4) {
		printf("Too few arguments\n");
	}
	
	NRA = atoi(argv[1]);
	NCA_RB = atoi(argv[2]);
	NCB = atoi(argv[3]);
	
	init_matrices();


	int nthreads = omp_get_max_threads();
	printf("nthread is %d\n", nthreads);

	begin = now();
	#pragma omp parallel for 
	for (int i = 0; i < NRA; i++) {
	    for (int j = 0; j < NCB; j++) {
		C[i * NCB + j] = 0;
		for (int k = 0; k < NCA_RB; k++) {
		    C[i * NCB + j] += A[i * NCA_RB + k] * B[k * NCB + j];
		}
	    }
	}
        end = now();
        time_spent = tdiff(begin, end);

	printf("\n\n ==== Final matrix \n\n");
	for (int i = 0; i < NRA; i++) {
		for (int j = 0; j < NCB; j++) {
			printf("%d, ", C[i * NCB + j]);
		}
		printf("\n");
	}
	printf("\n --- Time taken %f\n", time_spent);
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
	printf("\n");
}

			
