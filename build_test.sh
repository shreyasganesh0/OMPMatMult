gcc -fopenmp -o mat_mul_test src/mat_mul_omp.c

sbatch test_scripts/job-4mat_omp.sh
sbatch test_scripts/job-4mat2_omp.sh
sbatch test_scripts/job-4mat3_omp.sh
sbatch test_scripts/job-4mat4_omp.sh
sbatch test_scripts/job-4mat5_omp.sh
sbatch test_scripts/job-4mat6_omp.sh
sbatch test_scripts/job-4mat7_omp.sh
sbatch test_scripts/job-4mat8_omp.sh

squeue -u shreyas.ganesh
