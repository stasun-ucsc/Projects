
# Assignment 4: The Game of Life

### 1. Intro

This program implements John Conway's Game of Life. Given an input that sets the parameters for the universe as well as the cells to populate, certain rules determine whether or not cells in the universe live or die. If the cell is alive at the start of the generation, it stays alive if it has 2 or 3 neighbors. If a cell is dead, it comes back to life if it is surrounded by exactly three neighbors. In every other scenario, the cell dies or remains dead. This program also makes use of the `ncurses.h` library to animate the various generations of the universe if desired. There also is an option to run the game in a toroidal universe. Upon the game's completion, the universe's final state is printed to `stdout` by default or a file if specified.  

### 2. Building the Program

To build the program, enter the terminal and make sure you are in the directory that contains this program. Then, type one of `make`, `make all`, or `make life`. This will create an executable binary in the directory called `life` as well as object files for all of the .c files that this program consists of.

### 3. Running the Program

To run the program, enter the terminal and make sure you are in the same directory as the executable `sorting`. In the terminal, type the command `./life [options]`. The command line options are the following:

-t: Plays the Game of Life on a toroidal universe  
-s: Silences `ncurses`. `ncurses` will animate the generations in the Game of Life being played out and silencing will prevent the animation  
-n *generations*: Specifies the number of generations the universe goes through. The default number is 100  
-i *input*: Specifies the input file to be used to generate and populate the universe. The default input is `stdin`    
-o *output*: Specifies the output file the final state of the universe will be printed to. The default output is `stdout`  
-h: Prints a help message with instructions on running the program  

A combination of these options can be typed in and combinations that are "invalid" (ex. just typing in the option -i) will return the help menu. It is also worth noting that if the -h is typed in, only the help message will appear, regardless of what other options are selected.

### 4. Cleaning

If you would like to clean the directory after building the program, go into the terminal in the directory with the program, and type in the command `make clean`. This will remove the executable `life` as well as all of the .o files created by the build process.

### 5. Errors and Bugs

At present, inputting universe parameters through `stdin` is non-functional. The only current way to input the universe parameters is by enabling the `-i` option and specifying the file name. Running `scan-build make life` yields this result:  

`scan-build: Using '/usr/lib/llvm-15/bin/clang' for static analysis
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -c life.c 
Can't exec "gcc": No such file or directory at /usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer line 457.
make: *** [Makefile:11: life.o] Error 255
scan-build: Analysis run complete.
scan-build: Removing directory '/tmp/scan-build-2023-02-12-050732-15377-1' because it contains no reports.
scan-build: No bugs found.`

I admittedly still do not really know what the "Can't exec" and "Error 255" means. However, it again also does say that no bugs were found and doesn't seem to affect the code's ability to run. 

