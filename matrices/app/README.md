## Sparse Matrix Operation Command Line Utility Tool
> Created by @clayton-herbst (22245091)

#### Compiling

In order to compile the command line utility program use the provided makefile in the projects root folder.

```
    @user~app: make         # builds the project and returns executable.
```

Optional makefile cleaning functionality is provided. Just use the following command.

```
    @user~app: make clean   # removes all output files and object files.
```


#### Usage

```
    ./mop operation -f file1.in [file2.in] [-l | -s] [-t %i]

    operation:
        --sm %f
        --ad
        --ts
        --tr
        --mm
```
