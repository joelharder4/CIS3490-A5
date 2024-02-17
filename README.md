# Assignment 5
> Course Code: **CIS\*3490**<br>
> Author: **Joel Harder**<br>
> Last Edited: **April 10, 2023**<br>

## Description

#### Problem 1 - Finding all Subset Sums
> The Subset Sum Problem is a task where the objective is to identify all possible sums that can be obtained by adding together different subsets of a given set of numbers. These two programs will both prompt for a file to read from and a sum to search for.

`P11.out` - A brute force algorithm for finding all subset sums.<br>
`P12.out` - A backtracking algorithm that finds all subset sums.

Files must contain a list of integers seperated by spaces, use `data_P1.txt` an an example.

#### Problem 2 - Person-job Assignment Problem
> The Person-Job Assignment Problem is an optimization problem where the goal is to assign a set of individuals to a set of jobs, such that the overall cost or time is minimized. Each individual has different skills and efficiencies for each job, and each job requires specific skills. The challenge lies in finding the optimal assignment that best utilizes the individuals’ skills while also meeting the job requirements. Both programs will prompt for a file to read from.

`P21.out` - A brute force algorithm for the person-job assignment problem. This brute force method will take a very long time to run (usually 100 seconds), so don't end the program if it seems like nothing is happening.<br>
`P22.out` - A branch and bound algorithm for the person-job assignment problem.

Files must be an `n x n` matrix of integers representing people's skills and skills required for the job. An example can be found in `data_P2.txt`.

## Compiling
To compile all programs type
```
make all
```

<br>To compile individual programs type
```
make P<problem number>
```

## Running
To run any program type
```
./P<problem number>
```
