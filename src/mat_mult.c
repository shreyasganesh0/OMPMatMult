/*mat_mult.c */
//Shreyas Ganesh
 
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int **A;
int **B;
int **C;
int NRA;
int NCB;
int NCA_RB;

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

#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

int main (int argc, char *argv[])
{
    int	numtasks,            /* number of tasks in partition */
        taskid,                /* a task identifier */
        numworkers,            /* number of worker tasks */
        source,                /* task id of message source */
        dest,                  /* task id of message destination */
        mtype,                 /* message type */
        rows,                  /* rows of matrix A sent to each worker */
        averow, extra, offset, /* used to determine rows sent to each worker */
        i, j, k, rc;           /* misc */
    
	NRA = atoi(argv[1]);
	NCA_RB = atoi(argv[2]);
	NCB = atoi(argv[3]);


    MPI_Status status;

    //clock_t begin, end;
    struct timespec begin, end;
    double time_spent;
        
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    if (numtasks < 2 ) {
      printf("Need at least two MPI tasks. Quitting...\n");
      MPI_Abort(MPI_COMM_WORLD, rc);
      exit(1);
      }
    numworkers = numtasks-1;

/**************************** master task ************************************/
   if (taskid == MASTER)
   {

// Do some basic error checking
      if (NRB != NCA) {
        printf ("Matrix a column size must equal matrix b row size.\n");
        return 1;
      } 
               
      init_matrices();

      /* Send matrix data to the worker tasks */
      averow = NRA/numworkers;
      extra = NRA%numworkers;
      offset = 0;
      mtype = FROM_MASTER;

        //begin = clock();
        begin = now();
      
      for (dest=1; dest<=numworkers; dest++)
      {
         rows = (dest <= extra) ? averow+1 : averow;   	
         printf("Sending %d rows to task %d offset=%d\n",rows,dest,offset);
         MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&A[offset][0], rows*NCA, MPI_INT, dest, mtype,
                   MPI_COMM_WORLD);
         MPI_Send(&B, NRB*NCB, MPI_INT, dest, mtype, MPI_COMM_WORLD);         
         offset = offset + rows;
      }

      /* Receive results from worker tasks */
      mtype = FROM_WORKER;
      for (i=1; i<=numworkers; i++)
      {
         source = i;
         MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&C[offset][0], rows*NCB, MPI_INT, source, mtype, 
                  MPI_COMM_WORLD, &status);
         printf("Received results from task %d\n",source);
      }

        //end = clock();
        end = now();
        time_spent = tdiff(begin, end);
        
      /* Print results */
      printf ("\n");
      printf("******************************************************\n");
      printf("Result Matrix:\n");
      for (i=0; i<NRA; i++)
      {
         printf("\n"); 
         for (j=0; j<NCB; j++) 
            printf("%d\t", C[i][j]);
      }
      
      printf("\n******************************************************\n");
      printf ("\n");
      printf("total time: %.8f sec\n", time_spent);
      printf ("\n");
   }


/**************************** worker task ************************************/
   if (taskid > MASTER)
   {
      mtype = FROM_MASTER;
      MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&A, rows*NCA, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&B, NRB*NCB, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
            
      #pragma omp parallel for
      for (i=0; i<rows; i++)
         for (j=0; j<NCB; j++)
         {
           C[i][j] = 0;
           for (k=0; k<NCA; k++)            
              C[i][j] = C[i][j] + A[i][k] * B[k][j];
         }
         
      mtype = FROM_WORKER;
      MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&C, rows*NCB, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
   }
   MPI_Finalize();
}



void init_matrices() {
	
	A = malloc(NRA * sizeof(int));
	B = malloc(NCA_RB * sizeof(int));
	C = malloc(NRA * sizeof(int));
	
	for (int i = 0; i < NRA; i++) {
		A[i] = malloc(NCA_RB * sizeof(int));
	}
	for (int i = 0; i < NCA_RB; i++) {
		B[i] = malloc(NCB * sizeof(int));
	}
	for (int i = 0; i < NRA; i++) {
		C[i] = malloc(NCB * sizeof(int));
	}

	printf("\n\n ----- A mat\n");
	for (int i = 0; i < NRA; i++) {
		for (int j = 0; j < NCA_RB; j++) {
			A[i][j] = i + j;
			printf("%d, ", A[i][j]);
		}
		printf("\n");
	}
	printf("\n\n ----- B mat\n");
	for (int i = 0; i < NCA_RB; i++) {
		for (int j = 0; j < NCB; j++) {
			B[i][j] = i - j; 
			printf("%d, ", B[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
