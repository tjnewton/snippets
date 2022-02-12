/* This program show how to use operations on a 2D array of strings
using function pointers. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// data used to build an array of strings (these values loop as needed to fill the length)

#define STATIC_DATA_LENGTH 6
char* static_data[STATIC_DATA_LENGTH] = {
  "here",
  "lies",
  "a",
  "list",
  "of",
  "words"
};

int allocate_data(char**** data, int rows, int cols);
void free_data(char*** data, int rows, int cols);
void apply_by_col(void (*func)(char*, int, int, int, int), char*** data, 
                  int rows, int cols);
void apply_by_row(void (*func)(char*, int, int, int, int), char*** data, 
                  int rows, int cols);
void capitalize_first_col(char* in_str, int row, int col, int rows, int cols);
void print_str(char* in_str, int row, int col, int rows, int cols);

int main(int argc, char** argv)
{ 
  char*** data;
  int data_rows = 4;
  int data_cols = 3;
  int rc;

  // allocate memory for 2D array of strings and fill with default data
  rc = allocate_data(&data, data_rows, data_cols);
  if (rc != 0) {
    return rc;
  } 
  
  // print looped default data in 2D array (or matrix)
  printf("\nOriginal matrix:\n");
  apply_by_row(print_str, data, data_rows, data_cols);

  // capitalize first column and print the result
  printf("\n apply_by_col with capitalize_first_col\n");
  // apply_by_col(capitalize_first_col, data, data_rows, data_cols);
  apply_by_row(capitalize_first_col, data, data_rows, data_cols);
  apply_by_row(print_str, data, data_rows, data_cols);

  // test apply_by_col function with print_str: it's ugly
  printf("\n apply_by_col with print_str\n");
  apply_by_col(print_str, data, data_rows, data_cols);
  
  // free the utilized memory
  free_data(data, data_rows, data_cols);

  return 0;
}

// allocates a two dimensional array of strings initialized with values from static_data, returns non-zero for error
int
allocate_data(char**** data, int rows, int cols)
{
  int i, j;

  // allocate memory for rows
  *data = (char***) malloc(sizeof(char**) * rows);
  if (*data == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    return -1;
  }

  // allocate memory for columns of each row
  for (i = 0; i < rows; i++) {
    (*data)[i] = (char**) malloc(sizeof(char*) * cols);
    if ((*data)[i] == NULL) {
      fprintf(stderr, "Error: malloc failed\n");
      return -1;
    }

    // add default data to each column of the current row
    for (j = 0; j < cols; j++) {
      (*data)[i][j] = strdup(static_data[(i * cols + j) % STATIC_DATA_LENGTH]);
    }
  }
  return 0;
}


// free memory from two dimentional array of strings
void free_data(char*** data, int rows, int cols)
{
  int i, j;
  for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        free(data[i][j]);
      }
    free(data[i]);
  }
  free(data);
}


/*
 * Applies the given function to each entry in a two-dimensional array.
 * Operates one row at a time.
 *
 * The arguments to the applied function are as follows:
 *   1) the current entry;
 *   2) the row number of the current entry;
 *   3) the column number of the current entry;
 *   4) the total number of rows;
 *   5) the total number of columns.
 */
void apply_by_row(void (*func)(char*, int, int, int, int),
                  char*** data, int rows, int cols)
{
  int i, j;
  for (i = 0; i < rows; i++)
  {
    for (j = 0; j < cols; j++)
    {
      func(data[i][j], i, j, rows, cols);
    }
  }
}


// prints an arrays contents, adding new lines after each row
void print_str(char* in_str, int row, int col, int rows, int cols)
{
  printf("%s ", in_str);
  if ((col + 1) % cols == 0) {
    printf("\n");
  }
}


// changes all characters in all strings in the first column to be uppercase 
void
capitalize_first_col(char* in_str, int row, int col, int rows, int cols)
{
  // check if first column
  if (col == 0) {
	  // capitalize each character in the string
	  while (*in_str != '\0') {
		  *in_str = (char)toupper(*in_str);
		  in_str++;
	  }
  }
}


/*
 * Applies the given function to each entry in a two-dimensional array.
 * Operates one column at a time.
 *
 * The arguments to the applied function are as follows:
 *   1) the current entry;
 *   2) the row number of the current entry;
 *   3) the column number of the current entry;
 *   4) the total number of rows;
 *   5) the total number of columns.
 */
void apply_by_col(void (*func)(char*, int, int, int, int),
                  char*** data, int rows, int cols)
{
  int i, j;
  // loop over each column
  for (j = 0; j < cols; j++) {
  	// loop over each row
  	for (i = 0; i < rows; i++) {
  		func(data[i][j], i, j, rows, cols);

  	}
  }
}
