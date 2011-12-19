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
#ifndef _PROG11_H_
#define _PROG11_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ncurses.h>s
#include <time.h>

#include "logging.h"

Log * context;

#include "hash.h"
#include "orthogonal.h"

#define LINE_BUFFER 255
#define DO_INSERT 'I'
#define DO_QUERY  'Q'
#define DO_PURGE  'P'
#define DO_STATUS 'S'
#define DO_DELETE 'D'
#define ALLOWED_OPERATIONS 255
#define ALLOWED_STUDENTS   500000
#define HASH_LENGTH        5          


typedef struct _Hashtable {
    void * data;
} *Hashtable;

typedef struct _Student {
    char * name;
} *Student;

typedef struct _EntityCollection {
    Node majors;
    Node hometowns;
    Node years;
} *EntityCollection;

typedef void (*OperationHandler)(void *, EntityCollection);

typedef struct _Operation {
    char op_code;
    int query_id;
    char name[128];
    char major[64];
    char year[10];
    char hometown[64];
    char criteria[128];
    OperationHandler handler;
} *Operation;

typedef Operation (*OperationParser)(char *);

EntityCollection entities_new(void);

void entities_delete(EntityCollection);

Node node_get(Node *, char *, char *);

Node major_get(Node *, char *);

Node node_find_item(Node, char *, char *);

Node node_add(Node *, char *, char *);

void node_student_add(Node, char *, char *);

Operation operation_parse_insert(char *);

Operation operation_parse_delete(char *);

Operation operation_parse_query(char *);

Operation operation_parse_status(char *);

Operation operation_parse_purge(char *);

void operation_do_insert(void *, EntityCollection);

void operation_do_delete(void *, EntityCollection);

void operation_do_query(void *, EntityCollection);

void operation_do_query_year(void *, EntityCollection);

void operation_do_query_major(void *, EntityCollection);

void operation_do_query_hometown(void *, EntityCollection);

void operation_do_status(void *, EntityCollection);

void operation_do_purge(void *, EntityCollection);

Operation operation_insert_new(void);

Operation operation_delete_new(void);

Operation operation_query_new(void);

Operation operation_status_new(void);

Operation operation_purge_new(void);

Operation operation_parse(OperationParser *, char *);

void operation_table_init(OperationParser *);

OperationHandler operation_table_get(OperationHandler *, char *);

void operation_table_put(OperationHandler *, char *, OperationHandler);

/** 
 * Hashing function. Creates a hash value used as a key for a hashtable. The algorithm
 * is the DJB hash algorithm developed by Daniel J. Bernstein (DJB). It uses a string
 * and iterates over the string for the length provided. I tend to use the number of
 * items in the hashtable. That way, there is always enough room.
 * 
 * @param str is the string the create a hash from
 * @param length is the number of iterations for the hash
 * @return the int hash value
 */
unsigned int hash(char * str, unsigned int length);


/*------------------------------------------------- usage -----
  |  Function usage
  |
  |  Purpose:  Prints usage for the program
  |       
  |
  |  Parameters: program_name (IN) -- Name of the program
  |                
  |
  |  Returns:  Only success
  *-------------------------------------------------------------------*/
void usage(char *);
#endif
