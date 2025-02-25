#!/bin/bash
#SBATCH --job-name=mat_mul16T1600_OMP
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=shreyas.ganesh@ufl.edu
#SBATCH --account=eel6763
#SBATCH --qos=eel6763
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=16
#SBATCH --mem-per-cpu=1000mb
#SBATCH -t 11:00:00
#SBATCH -o outputs/mat_mul16T1600
#0SBATCH -e errors/mat_mul_err_16t1600
export OMP_NUM_THREADS=16
./mat_mul_omp 1600 1600 1600
