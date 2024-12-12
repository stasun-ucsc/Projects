
### 1. Intro

This program implements a mini HTTP server. This server implements two commands, GET and PUT.

### 2. Building the Program

To build the program, enter the terminal and make sure you are in the directory that contains this program. Then, type `make`, `make all`, or `make httpserver`. This will create an executable binary in the directory called `httpserver`, as well as object files for all of the .c files that this program consists of.

### 3. Running the Program

To run the program, enter the terminal and make sure you are in the same directory as the executables created. In the terminal, type the command `./httpserver [port]`. Port is an integer that represents the port number your server will be listening for traffic from.

Note that ports 0 - 1023 are reserved and will not be able to be used. 
For more info on what ports you can use visit this link:  
`https://www.ibm.com/docs/en/ztpf/2021?topic=overview-port-numbers`  

### 4. Cleaning

If you would like to clean the directory after building the program, go into the terminal in the directory with the program, and type in the command `make clean`. This will remove the executables `httpserver`, as well as all of the .o files created by the build process.

### 5. Known Errors  

There appears to be some errors in functionality with binary files, especially with PUT. This is believed to be due to the use of strlen() when determining how much text to write, which can encounter problems if there are 0s in the file, which is being interpreted as the end of a string.

