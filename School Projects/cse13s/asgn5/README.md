
# Assignment 5: Public Key Cryptography

### 1. Intro

This program implements public key cryptography with the Schmidt-Samoa (SS) algorithm. This program will generate a private and public key pair, and allow you to encrypt and decrypt messages. The program also has command line options that allow you to change various aspects of the key generation, such as the size of the keys, number of iterations in the generation, etc.

### 2. Building the Program

To build the program, enter the terminal and make sure you are in the directory that contains this program. Then, type `make keygen`, `make encrypt`, or `make decrypt`. This will create an executable binary in the directory called `keygen`, `encrypt`, or `decrypt`, respectively, as well as object files for all of the .c files that this program consists of. If you would like to build all three at the same time, type in `make` or `make all`.

### 3. Running the Program

To run the program, enter the terminal and make sure you are in the same directory as the executables created. In the terminal, type the command `./keygen [options]`, `./encrypt [options]`, or `./decrypt [options]`. For more details on all of the command-line options for the respective programs, enter the -h option for any of the executables, and a help menu detailing the various options available will be printed out.

A combination of these options can be typed in and combinations that are "invalid" (ex. just typing in the option -i) will return the help menu. It is also worth noting that if the -h is typed in, only the help message will appear, regardless of what other options are selected.

### 4. Cleaning

If you would like to clean the directory after building the program, go into the terminal in the directory with the program, and type in the command `make clean`. This will remove the executables `keygen`, `encrypt`, and `decrypt`, as well as all of the .o files created by the build process.

### 5. Errors and Bugs

At present, I do not know definitely of any bugs. However, I suspect that there could be some bugs when setting the nbits value to lower numbers, which could cause some issues with the key generation. Running `scan-build make` yields this result:  

`scan-build: Using '/usr/lib/llvm-15/bin/clang' for static analysis
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -c encrypt.c
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -c randstate.c
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -c numtheory.c
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -c ss.c 
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -o encrypt encrypt.o randstate.o numtheory.o ss.o `pkg-config --libs gmp`
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -c decrypt.c 
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -o decrypt decrypt.o randstate.o numtheory.o ss.o `pkg-config --libs gmp`
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -Wall -Wpedantic -Werror -Wextra -c keygen.c
/usr/share/clang/scan-build-15/bin/../libexec/ccc-analyzer -o keygen keygen.o randstate.o numtheory.o ss.o `pkg-config --libs gmp`
scan-build: Analysis run complete.
scan-build: Removing directory '/tmp/scan-build-2023-02-26-193354-48898-1' because it contains no reports.
scan-build: No bugs found.`

