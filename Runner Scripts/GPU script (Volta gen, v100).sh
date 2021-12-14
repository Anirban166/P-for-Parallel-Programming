#!/bin/bash
#SBATCH --job-name=runGPU
#SBATCH --output=/scratch/ac4743/gpuResults.out
#SBATCH --time=00:01:00
#SBATCH --mem=1000
#SBATCH -G 1
#SBATCH --qos=gpu_class
#SBATCH --constraint v100

module load cuda
srun hostname
srun nvidia-smi
trialrunCount=3

n=100
nvcc -DN=100 -arch=compute_70 -code=sm_70 -lcuda -Xcompiler -fopenmp CountDistancesLessThanEpsilonBFV.cu -o CountDistancesLessThanEpsilonBFV
echo -e "\n Brute force (version one) approach with number of points, N = $n:"
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 5:"
  srun CountDistancesLessThanEpsilonBFV 5
done
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 10:"
  srun CountDistancesLessThanEpsilonBFV 10
done
echo -e "\n\n"
nvcc -DN=100 -arch=compute_70 -code=sm_70 -lcuda -Xcompiler -fopenmp CountDistancesLessThanEpsilonOV.cu -o CountDistancesLessThanEpsilonOV
echo -e "\n\n Optimized (version two) approach with number of points, N = $n:"
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 5:"
  srun CountDistancesLessThanEpsilonOV 5
done
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 10:"
  srun CountDistancesLessThanEpsilonOV 10
done
echo -e "\n\n"

n=1000
nvcc -DN=1000 -arch=compute_70 -code=sm_70 -lcuda -Xcompiler -fopenmp CountDistancesLessThanEpsilonBFV.cu -o CountDistancesLessThanEpsilonBFV
echo -e "\n\n Brute force (version one) approach with number of points, N = $n:"
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 5:"
  srun CountDistancesLessThanEpsilonBFV 5
done
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 10:"
  srun CountDistancesLessThanEpsilonBFV 10
done
echo -e "\n\n"
nvcc -DN=1000 -arch=compute_70 -code=sm_70 -lcuda -Xcompiler -fopenmp CountDistancesLessThanEpsilonOV.cu -o CountDistancesLessThanEpsilonOV
echo -e "\n\n Optimized (version two) approach with number of points, N = $n:"
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 5:"
  srun CountDistancesLessThanEpsilonOV 5
done
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 10:"
  srun CountDistancesLessThanEpsilonOV 10
done
echo -e "\n\n"

n=10000
nvcc -DN=10000 -arch=compute_70 -code=sm_70 -lcuda -Xcompiler -fopenmp CountDistancesLessThanEpsilonBFV.cu -o CountDistancesLessThanEpsilonBFV
echo -e "\n\n Brute force (version one) approach with number of points, N = $n:"
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 5:"
  srun CountDistancesLessThanEpsilonBFV 5
done
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 10:"
  srun CountDistancesLessThanEpsilonBFV 10
done
echo -e "\n\n"
nvcc -DN=10000 -arch=compute_70 -code=sm_70 -lcuda -Xcompiler -fopenmp CountDistancesLessThanEpsilonOV.cu -o CountDistancesLessThanEpsilonOV
echo -e "\n\n Optimized (version two) approach with number of points, N = $n:"
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 5:"
  srun CountDistancesLessThanEpsilonOV 5
done
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 10:"
  srun CountDistancesLessThanEpsilonOV 10
done
echo -e "\n\n"

n=100000
nvcc -DN=100000 -arch=compute_70 -code=sm_70 -lcuda -Xcompiler -fopenmp CountDistancesLessThanEpsilonBFV.cu -o CountDistancesLessThanEpsilonBFV
echo -e "\n\n Brute force (version one) approach with number of points, N = $n:"
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 5:"
  srun CountDistancesLessThanEpsilonBFV 5
done
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 10:"
  srun CountDistancesLessThanEpsilonBFV 10
done
echo -e "\n\n"
nvcc -DN=100000 -arch=compute_70 -code=sm_70 -lcuda -Xcompiler -fopenmp CountDistancesLessThanEpsilonOV.cu -o CountDistancesLessThanEpsilonOV
echo -e "\n\n Optimized (version two) approach with number of points, N = $n:"
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 5:"
  srun CountDistancesLessThanEpsilonOV 5
done
for run in $(seq 1 $trialrunCount)
do
  echo -e "\n\nTrial run #$run for an epsilon of 10:"
  srun CountDistancesLessThanEpsilonOV 10
done
echo -e "\n\n"