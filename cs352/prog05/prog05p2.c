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
 |   Assignment:  Program #5 Part 2 ROT47
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
 |     Due Date:  September 29th, 2011, at the beginning of class
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  ROT47 algorithm implementation. Encrypts/decrypts a string 
 |                passed into STDIN.
 |
 |        Input:  An ascii string that needs to be decrypted or encrypted. Valid
 |                input is anything with the ascii decimal value between 32 and 
 |                126.
 |
 |       Output:  Encrypted/Decrypted string
 |
 |    Algorithm:  ROT47 algorithm. Presents the ascii characterset as 
 |                a rotatable interface that can rotate 47 characters in either
 |                direction and yield the same output. 47 is half the characters
 |                in the applicable set, so rotating 47 places in either direction
 |                always yields the same result. Likewise, rotating twice always
 |                results in the initial location. This allows decryption and
 |                encryption to follow exactly the same algorithm.
 |
 |   Known Bugs:  
 |
 *===========================================================================*/
#include "prog05p2.h"


/*------------------------------------------------- main -----
  |  Function main
  |
  |  Purpose:  Reads input from STDIN using getchar(), iterates over 
  |            each character and prints the result from rotate
  |            to stdout using putchar()
  |       
  |
  |  Parameters: argc (IN) -- Doesn't really do anything with this
  |              argv (IN) -- Doesn't really do anything with this.
  |                
  |
  |  Returns:  Only success
  *-------------------------------------------------------------------*/
int main(int argc, char ** argv) {
  context = logging_init("prog05p2");

  int myc;
  while ((myc = getchar()) != EOF) {
      if (myc == ' ') {
          putchar(myc);
      }
      else if (myc > 31 && myc < 127) {
          putchar(rotate(myc));
      }
  }
  printf("\n");

  logging_dest(context);
  return EXIT_SUCCESS;
}

/*------------------------------------------------- rotate -----
  |  Function rotate
  |
  |  Purpose:  Rotates through the ascii values using the input
  |            character _c_. 
  |
  |  Parameters: c (IN) -- Character to apply ROT47 algorithm to
  |                
  |
  |  Returns:  ROT47 rotated character value
  *-------------------------------------------------------------------*/
char rotate(char c) {
    return (c > (ROTATION_VALUE + 31)) ? c - ROTATION_VALUE : c + ROTATION_VALUE;
}
