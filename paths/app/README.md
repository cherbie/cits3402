## Distribute Shortest Paths Application
> **UWA Details**: Clayton Herbst (22245091)
> **GitHub**: @clayton-herbst

#### Usage

Use the Open MPI compiler wrapper to compile the source code files. A *makefile* has been provided.

```
user@user app % make
```

To run the application use the Open MPI command line utility *mpirun*

```
user@user app % mpirun ./sp [-f] filename.in
```

When using the node cluster provided by the University of Western Australia, alter the bash file *./run.sh* to include the hostfile provided by including the following directive: *--hostfile hostfile*. Then the program can simply be run by executing the provided bash file.

```
user@user app % ./run.sh
```

The script titled '*./test-script.sh*' has been provided to consecutively execute a number of programs with varied input graph sizes.

The resulting all-pairs shortest path graph is outputted to a file with the name containing the following format:

>"22245091_DATE_TIME.out"

The all-pairs shortest path is printed to file as a stream of bytes following the specification provided in the project brief. Accompanying time taken to execute is provided in the file with name format:

>"time_DATE_%i_%i" 1. Number of processes 2. Number of vertices.

In order to clean all object files and output file from the project's root directory type the following command into the terminal.
```
user@user app % make clean
```
