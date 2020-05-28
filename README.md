# Parallel Programming assignment #
This repository contains *Parallel Programming* assignment for Technische Universität München (TUM) [IN2147] course in summer 2019.

The coursework description is available [here](https://www.in.tum.de/caps/lehre/ss19/vorlesungen/parallel-programming/).

There are 9 assignments in this repository. 

# Assignment Description #
## Assignment 1. Mandelbrot ##
- Use Pthreads to parallelize mandelbrot_draw().
- Solution should have a speedup greater than 3.0 using. 4 threads

## Assignment 2. Mandelbrot set (dynamic) ##
- Use POSIX threads to parallelize draw_mandelbrot(...).
- The program should follow the producer/consumer.pattern.
- The speedup on 32 (logical) cores must be at least 16.0 using 32 threads.

## Assignment 3. Gaussian Blur (OpenMP) ##
- Use OpenMP.
- The speedup with 32 cores must be at least 11.
  
## Assignment 4. Familytree ##
- Parallelize the sequential family tree algorithm with OpenMP.
- The goal is a speedup of ≥ 10.

## Assignment 5. Laplace 2D ##
- 2d Laplace equation with fixed boundaries. 
- Problem domain is unit square with uniform mesh. 
- Finite differences are used for the discretization.
- Use OpenMP to parallelize the solver You need to get a speedup of 16 on our server with 32 logical cores. 
- The server has 2 NUMA nodes each with 8 cores.

## Assignment 6. Loop Transformations ##
- Apply loop fusion to the loop in loop_fusion_seq.c.
- Expected speed up  ~180.

## Assignment 7. SIMD ##
- Transposed matrix multiplication C := A trans(B).
- Manually vectorise the code using Intel intrinsics.
- Required speedup is 2.3.

## Assignment 8. Text Searching ##
- Search for words in a large text file.

- Words are separated by spaces, semicolons so on.

- Use MPI_Send and MPI_Recv to parallelize the search.

- A minimum speedup of 5 using 16 processes. The server has 2 NUMA nodes
  
## Assignment 9. Conway’s Game of Life ##
- Use MPI Collectives to parallelize the sequential version of the code. 
- A minimum speedup of 13 with 16 processes.