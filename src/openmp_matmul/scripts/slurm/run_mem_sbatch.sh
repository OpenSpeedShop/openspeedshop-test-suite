#!/bin/bash
#SBATCH --time=00:04:00
#SBATCH -N 1
#SBATCH -J mem-matmul
 
set | grep SLURM | sort

module load mpi/openmpi-2.1.0-gcc-ppc64le
module load openspeedshop

SLURM_ID=$SLURM_JOB_ID

echo ""
echo ""
echo "ossmem ./matmul"
ossmem "./matmul"
echo ""
echo ""
