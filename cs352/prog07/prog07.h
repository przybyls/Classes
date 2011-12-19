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
#ifndef _PROG05P2_H_
#define _PROG05P2_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logging.h"

#define MAX_BUFFER 255
#define false FALSE
#define true TRUE

Log * context;

typedef enum boolean_t { 
    FALSE, 
    TRUE 
} boolean;

float mspw(int, int);
float mwps(int, int);
int isvowel(char);
int syll_count(char *);
int iseow(char);
int iseos(char);
void concat(char *, char);

#endif
