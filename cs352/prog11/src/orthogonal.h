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
#ifndef _ORTHOGONAL_H_
#define _ORTHOGONAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logging.h"
#include "hash.h"

#define ORTHOGONAL_DIMENSIONS 3
#define HASH_LENGTH        5
#define HASH_SIZE          500000

typedef struct _Node {
    int * size;
    struct _Node * next[HASH_SIZE];
    void * data;
} *Node;

Node ll_new(void *);

Node ll_add(Node, void *);

Node or_add(Node, char *, void *);

Node ll_remove(Node, Node);

Node or_remove(Node, char *, Node);

Node or_find(Node, char *, char *);

void or_set(Node, char *, Node);

void ll_destroy(Node);
#endif

