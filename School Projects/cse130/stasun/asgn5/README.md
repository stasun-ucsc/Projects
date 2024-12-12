
# Assignment 5

### 1. Intro

This program reads in text strings separated by newlines and caches the strings.

### 2. Building the Program

To build the program, enter the terminal and make sure you are in the directory that contains this program. Then, type `make`, `make all`, or `make cacher`. This will create an executable binary in the directory called `cacher`, as well as object files for all of the .c files that this program consists of. 

### 3. Running the Program

To run the program, enter the terminal and make sure you are in the same directory as the executables created. In the terminal, type the command `./cacher -N <size> [policy]`.   

The `-N` flag is mandatory and determines the size of the cache. You must input a value greater than 0 for `<size>`.   

Policy is an optional flag you can input to determine the cache replacement policy. 
`-F` implements the FIFO replacement policy. This is also the default if no policy flag is selected.  
`-L` implements the LRU replacement policy.  

`-C` implements the clock replacement policy.  

You will then be prompted for input.

### 4. Cleaning

If you would like to clean the directory after building the program, go into the terminal in the directory with the program, and type in the command `make clean`. This will remove the executables `cacher`, as well as all of the .o files created by the build process.

### 5. Notes

The list.c and list.h files are from my work in PA1 of Patrick Tantalo's CSE 101 course during Spring 2023. The files have been modified for use in this programming assignment. The code for LRU and FIFO were based off of pseudocode given by Mitchell.


