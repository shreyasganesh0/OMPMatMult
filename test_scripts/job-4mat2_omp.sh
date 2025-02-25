#!/bin/bash
#SBATCH --job-name=mat_mul8T_OMP2
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=shreyas.ganesh@ufl.edu
#SBATCH --account=eel6763
#SBATCH --qos=eel6763
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=8
#SBATCH --mem-per-cpu=1000mb
#SBATCH -t 11:00:00
#SBATCH -o test_outputs/mat_mul8T2
#0SBATCH -e errors/mat_mul_err_4t
export OMP_NUM_THREADS=8
./mat_mul_test 60 12 10
