
# Assignment 1

### 1. Intro

This program reads in text through stdin and implements two commands: get and set. Get outputs the contents of a given file path through stdout. Set writes specified amount of data to a specified location.

### 2. Building the Program

To build the program, enter the terminal and make sure you are in the directory that contains this program. Then, type `make`, `make all`, or `make memory`. This will create an executable binary in the directory called `memory`, as well as object files for all of the .c files that this program consists of. 

### 3. Running the Program

To run the program, enter the terminal and make sure you are in the same directory as the executables created. In the terminal, type the command `./memory`.

You will then be prompted for input.

To use `get`, you must input in this format:
`get
[filepath]`
with filepath being the filepath for the file you want to print the contents of.

To use `set`, you must input in this format:
`set
[filepath]
[content length]
[content]`
with `[filepath]` being the filepath of the file you want to write to, `[content length]` being the number of bytes or characters you want to write to that file, and finally `[content]` being the actual contents that you will be writing to the file.  

### 4. Cleaning

If you would like to clean the directory after building the program, go into the terminal in the directory with the program, and type in the command `make clean`. This will remove the executables `memory`, as well as all of the .o files created by the build process.

