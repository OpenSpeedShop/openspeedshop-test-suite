#PBS -S /bin/csh
#  PBS script file
#
#   submit this script using the command:
#       qsub run.pbs
#
#PBS -l select=1:ncpus=16:model=san
#PBS -N imatmul_omptp

#PBS -l walltime=0:02:00
#PBS -j oe
#PBS -m bea
#PBS -q debug

#
# switch to MPI implmentations, compilers, and OpenSpeedShop versions that work for you
#
source /usr/share/modules/init/csh
module purge
module load modules /home4/jgalarow/privatemodules/osscbtf231p

echo " "
echo " change to directory where your executable is"
cd $HOME/exercises/matmul

setenv OMP_NUM_THREADS 8

# OpenMP Profiling experiment (omptp)
echo "Running ossomptp ./imatmul"
ossomptp "./imatmul"

