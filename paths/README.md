## Shortest Paths -- Assignment 2 (ENSC3402)
> **Author:** @clayton-herbst

#### Description
The goal of this project is to design an implement parallel algorithms to solve the all-pairs-shortest path problem for a number of large graphs using either Dijkstra’s or Floyd-Warshall algorithms. You will use C with the Message-Passing Interface (MPI) to write parallelized code. You will also need to observe and comment on how speedup is affected by changing the size of the problem and the number of nodes you use. A reasonable range for the size of your problems is 1024 to 4096 vertices. A reasonable range for the number of processors is one to 16.

#### Details
 - **Due date**: Friday 25 October 2019
 - Submission Items
   - Code
     - Copy of all source files, job scripts and build-files used.
     - Source files need to include your name and student numbers at the top.
   - Report
     - Address the following points:
       1. How your approach partitions data and processing?
       2. A table showing speedup foor graphs of 2048 vertices vs 1, 2, 4, 8, 16 proocessors
       3. A table showing speedup for 4 processors vs graphs of 256, 512, 1024, 2048, 4096 vertices.
     - Relate performance discussions to various factors in your computation
       - number of compute nodes, load balancing, communication overhead etc.

#### Code Requirements
 - File Reading
 - File Writing
 - Dijkstra's Algorithm or Floyd Warshall Algorithm
