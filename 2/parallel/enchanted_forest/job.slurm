#!/bin/bash
#SBATCH --job-name=enchanted_forest
#SBATCH --output=result.txt
#SBATCH --partition=nu-wide
#SBATCH --ntasks=4
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1

module load gcc /7.2.0
module load mpich /3.2.1-gcc-7.2.0

mpiexec ./bin/enchanted_forest ../test/test_set_2/input/job002.txt
