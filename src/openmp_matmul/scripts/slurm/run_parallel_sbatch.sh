#!/bin/bash
#SBATCH --time=00:04:00
#SBATCH -N 1
#SBATCH -J matmul-pc-omptp
 
set | grep SLURM | sort

module load mpi/openmpi-2.1.0-gcc-ppc64le
module load openspeedshop

SLURM_ID=$SLURM_JOB_ID

echo ""
echo "Setting OMP_NUM_THREADS=4"
export OMP_NUM_THREADS=4
echo ""
echo ""
echo "osspcsamp ./matmul"
osspcsamp "./matmul"
echo ""
echo ""
echo "ossomptp ./matmul"
ossomptp "./matmul"
echo ""
