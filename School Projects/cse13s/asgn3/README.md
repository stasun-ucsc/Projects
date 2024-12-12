
# Assignment 3: Sorting: Putting Your Affairs in Order

### 1. Intro

This program implements 4 sorting function, quick\_sort, heap\_sort, shell\_sort, and batcher\_sort. These sorting functions are all implemented in their own .c files. The files are compiled through a Makefile. The sorting functions are tested through command lines in sorting.c which is the main file for the code for the command lines. When the command lines are ran, the statistics, and the specified number of array elements are printed out, as long with the sorting name of whichever sorting function is ran through the command lines. There are multiple header files, such as gaps.h, which is used in the shell\_sort function, and stats.h and stats.c which creates the Stats structure used in all of the sorting functions.

### 2. Building the Program

To build the program, enter the terminal and make sure you are in the directory that contains this program. Then, type one of `make`, `make all`, or `make sorting`. This will create an executable binary in the directory called `sorting` as well as object files for all of the .c files that this program consists of.

### 3. Running the Program

To run the program, enter the terminal and make sure you are in the same directory as the executable `sorting`. In the terminal, type the command `./sorting [options]`. The command line options are the following:

-a: Runs all sorting algorithms
-h: Runs the Heap Sort sorting algorithm
-b: Runs the Batcher Sort sorting algorithm
-s: Runs the Shell Sort sorting algorithm
-q: Runs the Quick Sort sorting algorithm
-r \[seed\]: Set the random seed to \[seed\], with \[seed\] being any integer. The default seed is 13371453.
-n \[size\]: Sets the size of the array to be sorted to \[size\]. The default size should be 100.
-p \[elements\]: Prints outs \[elements\] elements from the array, with \[elements\] being the number of elements you would like to print out. The default number of elements to print out should be 100.
-H: Prints a help message with instructions on running the program

A combination of these options can be typed in and combinations that are "invalid" (ex. just typing in the option -r) will return the help menu. It is also worth noting that if the -H is typed in, only the help message will appear, regardless of what other options are selected.

### 4. Cleaning

If you would like to clean the directory after building the program, go into the terminal in the directory with the program, and type in the command `make clean`. This will remove the executable `sorting` as well as all of the .o files created by the build process.

### 5. Errors and Bugs

As of this moment, there are no bugs that I'm particularly aware of, though I admittedly have not extensively tested out the edge cases in the command line options. Running the command `scan-build make` yields this following result:

`scan-build: Using '/usr/lib/llvm-13/bin/clang' for static analysis
/usr/share/clang/scan-build-14/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -c sorting.c 
Can't exec "gcc": No such file or directory at /usr/share/clang/scan-build-14/bin/../libexec/ccc-analyzer line 457.
make: *** [Makefile:11: sorting.o] Error 255
scan-build: Analysis run complete.
scan-build: Removing directory '/tmp/scan-build-2023-02-05-213035-8079-1' because it contains no reports.
scan-build: No bugs found.`

I admittedly still do not really know what the "Can't exec" and "Error 255" means. However, it again also does say that no bugs were found and doesn't seem to affect the code's ability to run. 

