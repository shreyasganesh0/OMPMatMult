#!/bin/bash

mpicc -o mat_mul_mpiomp -qopenmp src/mat_mul_mpiomp.c

sbatch mpi_scripts/*

squeue -u shreyas.ganesh
