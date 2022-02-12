#include <stdlib.h>
#include <stdio.h>
#include "main.h"

// define max as 11 characters (including the null character) to read from file
#define MAX_NUM_LENGTH 12


/* This function checks the number of input parameters to the program to make 
   sure it is correct. If the number of input parameters is incorrect, it 
   prints out a message on how to properly use the program.
   input parameters:
       int    argc
       char** argv 
    return parameters:
       none
 */
void usage(int argc, char** argv)
{
    if(argc < 4) {
        fprintf(stderr, 
                "usage: %s <input file 1> <input file 2> <output file>\n", 
                argv[0]);
        exit(EXIT_FAILURE);
    }
}


/* This function takes in the two input file names (stored in argv) and
   determines the number of integers in each file. If the two files both 
   have N integers, return N, otherwise return -1. If one or both of the 
   files do not exist, it will exit with EXIT_FAILURE.
   input parameters:
       char** argv
   return parameters:
       -1 if the two input files have different number of integers 
       N  if the two input files both have N integers
 */
int get_num_ints(char** argv)
{
  FILE *fp1;
  FILE *fp2;
  unsigned int integer;
  int intCount1 = 0;
  int intCount2 = 0; 

  // open first file and guard against NULL
  if ((fp1 = fopen(argv[1],"r")) == NULL) {
	  printf("Error opening file 1\n");
	  exit(EXIT_FAILURE);
  }

  // increment intCount1 for each int in file 1
  while (fscanf(fp1, "%u", &integer) != EOF) {
  	intCount1++;
  }
  fclose(fp1);
  
  // open second file and guard against NULL
  if ((fp2 = fopen(argv[2],"r")) == NULL) {
	  printf("Error opening file 2\n");
	  exit(EXIT_FAILURE);
  }

  // increment intCount2 for each int in file 2
  while (fscanf(fp2, "%u", &integer) != EOF) {
  	intCount2++;
  }
  fclose(fp2);
  
  // return -1 if counts dont match, else return count
  return intCount1==intCount2 ? intCount1 : -1;
}


/* This function allocates enough memory to the three arrays to store
   num_ints elements each. This function will exit with EXIT_FAILURE 
   if the program fails to allocate the memory.
   input parameters:
       unsigned int*      input_one
       unsigned int*      input_two
       unsigned long int* output
       int                num_ints
   return parameters:
       none
 */
void allocate_mem(unsigned int** input_one, unsigned int** input_two, 
                  unsigned long int** output, int num_ints)
{
  // allocate blocks of memory for num_ints ints
  *input_one = (unsigned int *)malloc(num_ints * sizeof(unsigned int));
  *input_two = (unsigned int *)malloc(num_ints * sizeof(unsigned int));
  *output = (unsigned long int *)malloc(num_ints * sizeof(unsigned long int));

  // check if any memory failed to allocate
  if (*input_one == NULL || *input_two == NULL || *output == NULL) {
  	exit(EXIT_FAILURE);
  } 
}


/* This function reads in num_ints integers from the two input files and 
   stores them in input_one (first input file) and input_two (second input
   file). If one or both of the files do not exist, it will exit with EXIT_FAILURE.
   input parameters:
       char**        argv
       unsigned int* input_one
       unsigned int* input_two
       int           num_ints
   return parameters:
       none

 */
void get_ints(char** argv, unsigned int* input_one, unsigned int* input_two,
              unsigned long int* output, int num_ints)
{
  // create file pointers to open files 
  FILE *fp1;
  FILE *fp2;
  unsigned int integer;

  // open first file and guard against NULL
  if ((fp1 = fopen(argv[1],"r")) == NULL) {
	  printf("Error opening file 1\n");
	  exit(EXIT_FAILURE);
  }

  // get each int in file 1 and store it in input_one
  while (fscanf(fp1, "%u", &integer) != EOF) {
	*input_one = integer;
	// increment array pointer to next item
	input_one++;
  }
  fclose(fp1);
  
  // open second file and guard against NULL
  if ((fp2 = fopen(argv[2],"r")) == NULL) {
	  printf("Error opening file 2\n");
	  exit(EXIT_FAILURE);
  }

  // get each int in file 2 and store it in input_two
  while (fscanf(fp2, "%u", &integer) != EOF) {
  	*input_two = integer;
	// increment array pointer to next item
	input_two++;
  }
  fclose(fp2);
  
}

/* This function does an element-wise addition between the two input arrays 
   (input_one and input_two) of size num_ints and stores the result in the 
   output array (output).
   input parameters:
       unsigned int*      intput_one
       unsigned int*      intput_two
       unsigned long int* output
       int                num_ints
   return parameters:
       none
 */
void sum_ints(unsigned int* input_one, unsigned int* input_two, 
              unsigned long int* output, int num_ints)
{
  // loop over range of num_ints
  for (int i = 0; i < num_ints; i++) {
	// add arrays one and two to make output
	*output = (unsigned long int)*input_one + (unsigned long int)*input_two;
	// increment all array pointers
	output++;
	input_one++;
	input_two++;
  }
}

/* This function saves the summed output to an output file, whose name was 
   specified with the program execution (i.e., argv[3]). The file will contain 
   one integer value per line, like the input files.
   input parameters:
       char**             argv
       unsigned int*      intput_one
       unsigned int*      intput_two
       unsigned long int* output
       int                num_ints
   return parameters:
       none
 */
void save_output(char** argv, unsigned int* input_one, unsigned int* input_two,
                 unsigned long int* output, int num_ints)
{
  // create file pointer to open file 
  FILE *fp;

  // open output file for writing and guard against NULL
  if ((fp = fopen(argv[3],"w")) == NULL) {
	  printf("Error opening output file\n");
	  exit(EXIT_FAILURE);
  }

  // loop over ints in output array and print to file
  for (int i = 0; i < num_ints; i++) {
  	fprintf(fp, "%lu\n", *output);
	// increment pointer
	output++;
  }

  // close the file
  fclose(fp);

}

/* This program takes in three text file names as input. The first two files 
   contain N integers (where N >=0 and N <= 1,000,000) whose values range 
   from 0 to 4294967295 (i.e., 32-bit unsigned integers). The last file is 
   the output file that this program will generate. For all three files, there
   should be one integer per line.
  
   Program logic: for each line in the two input files, read in the two integers, 
   add them, and then store the sum in the output file. Repeat this process until
   all integers have been read from the input files.

   For example, if the two input files have eight integers each
   input file 1		input file 2		output file
   --------------------------------------------------------
   1		             2		             3
   5			         2			         7
   8			         5	          		13
   1			        12			        13
   2			         2			         4
   10			         2			        12
   1991			         2			      1993
   11231245		     21235			  11252480
 */
int main(int argc, char** argv)
{
    usage(argc, argv);

    // check the number of integers in the input files
    int num_ints = get_num_ints(argv);
    if(num_ints == -1) {
        fprintf(stderr, "ERROR: The two input files have different # of ints\n");
        exit(EXIT_FAILURE);
    }

    unsigned int* input_one = NULL;
    unsigned int* input_two = NULL;
    unsigned long int* output = NULL;
    // allocate memory to store the integers
    allocate_mem(&input_one, &input_two, &output, num_ints);

    // read the integers from the two input files
    get_ints(argv, input_one, input_two, output, num_ints);

    // sum up the numbers
    sum_ints(input_one, input_two, output, num_ints);

    // store the result in the output file 
    save_output(argv, input_one, input_two, output, num_ints);

    // free allocated memory
    free(input_one);
    free(input_two);
    free(output);

    return 0;
}
