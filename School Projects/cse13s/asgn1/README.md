
# Assignment 1: Getting Acquainted with Unix and C

### 1. Intro

The purpose of this program is to demonstrate the Monte Carlo method of approximating the value of pi. The program will create two plots, the first of which being a visual demonstration of the Monte Carlo method and the general premise of how it works. The second is a plot of the method's accuracy over many iterations, plotting the method's error from the actual value of pi against the number of iterations that have been run.

### 2. Running the Program

Running the program is pretty simple! Once you are in the directory that contains the program files, type in the terminal the command:  
`$ ./plot.sh`  
This will compile the monte\_carlo.c program, creating an executable file called 'monte\_carlo' as well as an object file called 'monte\_carlo.o'. The program will then proceed to run on its own. After a few seconds, the program will generate two .pdf files in the directory, one titled 'monte\_carlo\_1.pdf' and the other titled 'monte\_carlo\_2.pdf'. These pdfs contain the two aforementioned plots above. That's really all there is to running the program, there are no other command line options that my program accepts.

### 3. Cleaning

If you wish to clean the directory after running the program, run the following command in the terminal:  
`$ make clean`
This will get rid of the 'monte\_carlo' executable as well as any object files in the directory. However, note that this will __NOT__ get rid of the generated pdfs and those will have to be deleted separately if you wish to do so.

### 4. Closing

That's all from me!  __Thanks for Reading!__

