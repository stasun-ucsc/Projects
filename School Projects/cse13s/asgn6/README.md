
# Assignment 5: Public Key Cryptography

### 1. Intro

This program implements file compression and decompression with the Lempel-Ziv(LZ78) algorithm. This program when given a data file, will allow you to compress the file into an output file. This compressed file can then be decompressed back into its original state. To ensure that the decompresser only decompresses files compressed by the program, a magic number is checked in the compressed file. If the number does not match, an error message will be printed out and the program will terminate.  

### 2. Building the Program

To build the program, enter the terminal and make sure you are in the directory that contains this program. Then, type `make encode` or `make decode`. This will create an executable binary in the directory called `encode`, or `decode`, respectively, as well as object files for all of the .c files that this program consists of. If you would like to build both at the same time, type in `make` or `make all`.

### 3. Running the Program

To run the program, enter the terminal and make sure you are in the same directory as the executables created. In the terminal, type the command `./encode [options]` or `./decode [options]`. The command line options are the following for both exexutables:

-v: Prints out a verbose output for the compression/decompression  
-i [infile]: Specifies an input file to compress/decompress
-o [outfile]: Specifies an output file to output the compressed/decompressed file into
-h: Prints out a help message with instructions on running the program

For more details on all of the command-line options for the respective programs, enter the -h option for any of the executables, and a help menu detailing the various options available will be printed out.

A combination of these options can be typed in and combinations that are "invalid" (ex. just typing in the option -i) will return the help menu. It is also worth noting that if the -h is typed in, only the help message will appear, regardless of what other options are selected.

### 4. Cleaning

If you would like to clean the directory after building the program, go into the terminal in the directory with the program, and type in the command `make clean`. This will remove the executables `encode` and `decode`, as well as all of the .o files created by the build process.

### 5. Errors and Bugs

At present, I do not know definitely of any bugs. However, there maybe some issues when decoding text by typing directly into stdin. Running `scan-build make` yields this result:  

`scan-build: Using '/usr/lib/llvm-15/bin/clang' for static analysis
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -gdwarf-4 -c encode.c
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -gdwarf-4 -c trie.c
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -gdwarf-4 -c word.c
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -gdwarf-4 -c io.c
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -o encode encode.o trie.o word.o io.o
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -gdwarf-4 -c decode.c
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -o decode decode.o trie.o word.o io.o
scan-build: Analysis run complete.
scan-build: Removing directory '/tmp/scan-build-2023-03-13-033424-81211-1' because it contains no reports.
scan-build: No bugs found.`   


