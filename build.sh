#!/bin/bash
gcc -fopenmp -o mat_mul_omp src/mat_mul_omp_naive.c

sbatch scripts/job-4mat_omp.sh

sbatch scripts/job-1mat100_omp.sh
sbatch scripts/job-1mat200_omp.sh
sbatch scripts/job-1mat400_omp.sh
sbatch scripts/job-1mat800_omp.sh
sbatch scripts/job-1mat1600_omp.sh

sbatch scripts/job-2mat200_omp.sh

sbatch scripts/job-4mat400_omp.sh
sbatch scripts/job-4mat200_omp.sh

sbatch scripts/job-8mat800_omp.sh
sbatch scripts/job-8mat200_omp.sh

sbatch scripts/job-16mat1600_omp.sh
sbatch scripts/job-16mat200_omp.sh

squeue -u shreyas.ganesh

