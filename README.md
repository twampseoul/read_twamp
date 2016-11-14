# read_twamp

## Ingredients
### Database File
The database file has name ns3.sqlite3. Use sqlite3 command on terminal to open the file.
### C Code File
The code file has name read_twamp.c. Use gcc to compile.
	gcc read_twamp.c -o read_twamp -l sqlite3 -fno-stack-protector
### Input File
This is the input file that the binary read the data from.

## How to cook
1. Download as zip or clone on ns-3-dce folder
2. Define the input on input file (twamp.txt).
3. Compile the C file
4. Execute the binary file

The result can be seen on database file after the simulation(s) are finished.
