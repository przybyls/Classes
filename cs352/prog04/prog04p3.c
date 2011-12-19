/*
 * Copyright 2011 Leo Przybylski
 *
 * Educational Community License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may
 * obtain a copy of the License at
 * 
 * http://www.osedu.org/licenses/ECL-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an "AS IS"
 * BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/*=============================================================================
 |   Assignment:  Program #4 Part 3 Magic Squares
 |
 |       Author:  Leo Przybylski
 |     Language:  
 |   To Compile:  gcc -DPROGRAM_LOG_LEVEL=0 prog04p3.c -o prog04p3
 |                There are different log levels:
 |                0 - DEBUG
 |                1 - INFO
 |                2 - WARN
 |                3 - ERROR
 |                The debug level is set at compile time.
 |
 |        Class:  CSc 352 — Systems Programming and UNIX
 |   Instructor:  Dr. Lester McCann
 |     Due Date:  September 22th, 2011, at the beginning of class
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  Generate a magic square using the de la Loubere’s method. 
 |
 |        Input:  3 integers passed through STDIN. The first is the size of the
 |                square. The following two are the row x column location
 |                to start from.
 |
 |       Output:  Text serialization of the square. Since de la Loubere’s method
 |                is not guaranteed to create a magic square, a notification
 |                of whether the square is a magic square is necessary.
 |
 |    Algorithm:  de la Loubere’s method. From where 1 is, move the cursor up
 |                and right. Increment 1. Continue this. If a square is not 0, that
 |                means, it's been visited before. If this is the case, go down
 |                one square. There is wrapping of edges. When an edge is reached
 |                that then, the cursor reverts to the other side.
 |
 |   Known Bugs:  
 |
 *===========================================================================*/
#include "logging.h"
#include "prog04p3.h"


/*------------------------------------------------- main -----
  |  Function main
  |
  |  Purpose:  Reads input from STDIN, iterates over it and passes
  |            it to the printOctal function.
  |
  |  Parameters: argc (IN) -- Doesn't really do anything with this
  |              argv (IN) -- Doesn't really do anything with this.
  |                
  |
  |  Returns:  Only success
  *-------------------------------------------------------------------*/
int main(int argc, char ** argv) {
  context = logging_init("prog04p3");

  unsigned int size;
  unsigned int start_row;
  unsigned int start_col;
  int status;
  
  status = scanf("%d %d %d", &size, &start_row, &start_col);
  if (status < 3) {
      error("Invalid argument formatting! Please use 3 integers delimited by whitespace");
      return EXIT_FAILURE;
  }
  
  Square * magic_square = init_square(size);
  magic_square->col = start_col;
  magic_square->row = start_row;
  info("Start row, col %d,%d", start_row, start_col);
  magic_square->data[start_row][start_col] = 1;
  info("value at 1,1 %d\n", magic_square->data[1][1]);

  int i;
  for (i = 0; i < (size * size) - 1; i++) {
      move(magic_square);
  }
  info("value at 1,1 %d\n", magic_square->data[1][1]);
  print(magic_square);
  
  int square_status = test(magic_square);

  free(magic_square);
  logging_dest(context);
  return EXIT_SUCCESS;
}

 /*------------------------------------------------- init_square -----
  |  Function init_square
  |
  |  Purpose:  Dynamically initializes a square of the given size.
  |            By initialization, all the cells in the square need
  |            to start with the value 0. A Square * is returned with
  |            the size and content of the magic square.
  |
  |  Parameters:
  |      parameter_name (IN) -- size. This is the size of the magic square
  |                             A square's width is also its length.
  |
  |  Returns:  Square * with 0'd out cells and size assigned.
  *-------------------------------------------------------------------*/
Square * init_square(int size) {
    Square * retval;
    int i;
    int a;

    retval = malloc(sizeof(Square));
    retval->size  = size;
    retval->value = 1;
    
    retval->data = malloc(sizeof(int *) * size);

    for (i = 0; i < size; i++) {
        retval->data[i] = malloc(sizeof(int) * size);
        for (a = 0; a < size; a++) {
            retval->data[i][a] = 0;
        }
    }
    return retval;
}

 /*------------------------------------------------- move -----
  |  Function move
  |
  |  Purpose:  moves the cursor up and to the right. If an edge is reached,
  |            the cursor is moved to the opposite side. The value is
  |            also incremented and set here.
  |
  |  Parameters:
  |      parameter_name (IN) -- magic_square is the magic square we want
  |                             to move
  |
  |  Returns: nuttin
  *-------------------------------------------------------------------*/
void move(Square * magic_square) {
    int old_row = magic_square->row;
    int old_col = magic_square->col;
    magic_square->data;
    if (magic_square->row == 0) {
        magic_square->row = magic_square->size - 1;
    }
    else {
        magic_square->row--;
    }
    if (magic_square->col == magic_square->size - 1) {
        magic_square->col = 0;
    }
    else {
        magic_square->col++;
    }

    if (magic_square->data[magic_square->row][magic_square->col] != 0) {
        magic_square->row = old_row;
        magic_square->col = old_col;

        if (magic_square->row == (magic_square->size - 1)) {
            magic_square->row = 0;
        }
        else {
            magic_square->row++;
        }
    }

    while (magic_square->data[magic_square->row][magic_square->col] != 0) {
        if (magic_square->row == (magic_square->size - 1)) {
            magic_square->row = 0;
        }
        else {
            magic_square->row++;
        }
    }

    magic_square->data[magic_square->row][magic_square->col] = ++magic_square->value;
}

 /*------------------------------------------------- test -----
  |  Function test
  |
  |  Purpose:  Tests the given square to verify that it is a real
  |            magic square by adding up the horizontals, veritical,
  |            and diagonals
  |
  |  Parameters:
  |      parameter_name (IN) -- square is the magic square to test
  |
  |  Returns:  1 if it's a magic square or 0 if it is not.
  *-------------------------------------------------------------------*/
int test(Square * square) {
    int retval = 0;
    int a;
    int b;
    
    int size = square->size;
    int magic_const = (size * (size * size + 1)) / 2;
    printf("%d is the magic constant\n", magic_const);
    for (a = 0; a < square->size; a++) {
        int sum = 0;
        for (b = 0; b < square->size; b++) {
            sum += square->data[a][b];
        }
        if (sum != magic_const) {
            printf("No - The position %d horizontal has a sum of %d\n", a, sum);
            return 0;
        }
    }

    for (a = 0; a < square->size; a++) {
        int sum = 0;
        for (b = 0; b < square->size; b++) {
            sum += square->data[b][a];
        }
        if (sum != magic_const) {
            printf("No - The position %d vertical has a sum of %d\n", a, sum);
            return 0;
        }
    }


    { // Check diagonal
        int sum = 0;
        for (a = 0; a < square->size; a++) {
            b = a;
            sum += square->data[b][a];
        }
        if (sum != magic_const) {
            printf("No - The upper-left-to-lower-right diagonal has a sum of %d\n", sum);
            return 0;
        }
    }

    { // Check diagonal
        int sum = 0;
        b = square->size - 1;
        for (a = 0; a < square->size; a++) {
            sum += square->data[b][a];
            b--;
        }
        if (sum != magic_const) {
            printf("No - The lower-left-to-upper-right diagonal has a sum of %d\n", sum);
            return 0;
        }
    }
    printf("Yes!\n");
    return 1;
}

 /*------------------------------------------------- print -----
  |  Function print
  |
  |  Purpose:  prints out a magic square to stdout
  |
  |  Parameters:
  |      parameter_name (IN) -- magic_square. A Square * that is the
  |                             square to print
  |
  |  Returns:  void
  *-------------------------------------------------------------------*/
void print(Square * magic_square) {
    int a;
    int b;
    for (a = 0; a < magic_square->size; a++) {
        for (b = 0; b < magic_square->size; b++) {
            printf("%3d", magic_square->data[a][b]);
        }
        printf("\n");
    }
}
