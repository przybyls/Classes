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
#ifndef _PROG04P3_H
#include <stdio.h>
#include <stdlib.h>

#define false FALSE
#define true TRUE

Log * context;

typedef struct {
    int row;
    int col;
    int size;
    int value;
    int ** data;
} Square;

/*------------------------------------------------- FUNCTION_NAME -----
  |  Function FUNCTION_NAME
  |
  |  Purpose:  EXPLAIN WHAT THIS FUNCTION DOES TO SUPPORT THE CORRECT
  |      OPERATION OF THE PROGRAM, AND HOW IT DOES IT.
  |
  |  Parameters:
  |      parameter_name (IN, OUT, or IN/OUT) -- EXPLANATION OF THE
  |              PURPOSE OF THIS PARAMETER TO THE FUNCTION.
  |                      (REPEAT THIS FOR ALL FORMAL PARAMETERS OF
  |                       THIS FUNCTION.
  |                       IN = USED TO PASS DATA INTO THIS FUNCTION,
  |                       OUT = USED TO PASS DATA OUT OF THIS FUNCTION
  |                       IN/OUT = USED FOR BOTH PURPOSES.)
  |
  |  Returns:  IF THIS FUNCTION SENDS BACK A VALUE VIA THE RETURN
  |      MECHANISM, DESCRIBE THE PURPOSE OF THAT VALUE HERE.
  *-------------------------------------------------------------------*/
Square * init_square(int size);
void move(Square * magic_square);
void print(Square * magic_square);

#endif
