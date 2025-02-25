#!/bin/bash
#SBATCH --job-name=mat_mul8T2N1600_OMP
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=shreyas.ganesh@ufl.edu
#SBATCH --account=eel6763
#SBATCH --qos=eel6763
#SBATCH --nodes=2
#SBATCH --ntasks=8
#SBATCH --ntasks-per-node=4
#SBATCH --ntasks-per-socket=2
#SBATCH --cpus-per-task=8
#SBATCH --mem-per-cpu=1000mb
#SBATCH -t 11:00:00
#SBATCH -o outputs/mat_mul8T2N1600
#0SBATCH -e errors/mat_mul_err_8t2n1600
export OMP_NUM_THREADS=8
./mat_mul_mpiomp 200 200 200
