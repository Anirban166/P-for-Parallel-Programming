#!/bin/bash
#SBATCH --job-name=runSixteenCoresCPU
#SBATCH --output=/scratch/ac4743/cpuResultsSixteenCores.out
#SBATCH --time=00:01:30 # change for different core & thread count.
#SBATCH --mem=1000
#SBATCH --cpus-per-task 16
	       
trialrunCount=3
# -DNTHREADS=16

n=100
gcc -DN=100 -fopenmp -O3 CountDistancesLessThanEpsilonBFV.c -lm -o CountDistancesLessThanEpsilonBFV
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
gcc -DN=100 -fopenmp -O3 CountDistancesLessThanEpsilonOV.c -lm -o CountDistancesLessThanEpsilonOV
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

n=1000
gcc -DN=1000 -fopenmp -O3 CountDistancesLessThanEpsilonBFV.c -lm -o CountDistancesLessThanEpsilonBFV
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
gcc -DN=1000 -fopenmp -O3 CountDistancesLessThanEpsilonOV.c -lm -o CountDistancesLessThanEpsilonOV
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

n=10000
gcc -DN=10000 -fopenmp -O3 CountDistancesLessThanEpsilonBFV.c -lm -o CountDistancesLessThanEpsilonBFV
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
gcc -DN=10000 -fopenmp -O3 CountDistancesLessThanEpsilonOV.c -lm -o CountDistancesLessThanEpsilonOV
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

n=100000
gcc -DN=100000 -fopenmp -O3 CountDistancesLessThanEpsilonBFV.c -lm -o CountDistancesLessThanEpsilonBFV
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
gcc -DN=100000 -fopenmp -O3 CountDistancesLessThanEpsilonOV.c -lm -o CountDistancesLessThanEpsilonOV
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