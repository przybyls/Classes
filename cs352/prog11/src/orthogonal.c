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
#include "orthogonal.h"

Node ll_new(void * data) {
    Node retval = (Node) malloc(sizeof(struct _Node));
    retval->size = 0;
    retval->data = strdup((char *) data);
    int i;
    return retval;
}

Node ll_add(Node list, void * data) {
    Node newNode = ll_new(data);
    newNode->next[0] = list;
    return newNode;
}

Node or_add(Node list, char * key, void * data) {
    Node newNode = ll_new(data);
    unsigned int real_key = hash(key, HASH_LENGTH);
    newNode->next[real_key] = list;
    return newNode;
}

Node ll_remove(Node list, Node item) {
    Node iterator = list;
    Node last = NULL;
    while (iterator != NULL) {
        if (iterator == item) {
            if (last == NULL) {
                last = iterator->next[0];
                free(item->data);
                free(item);
                return last;
            }
            
            last->next[0] = iterator->next[0];
            free(item->data);
            free(item);
            return list;
        }
        
        last = iterator;
        iterator = iterator->next[0];
    }
    
    return list;
}

Node or_remove(Node list, char * key, Node item) {
    unsigned int real_key = hash(key, strlen(key));
    Node iterator = list;
    Node last = NULL;
    while (iterator != NULL) {
        if (iterator == item) {
            if (last == NULL) {
                last = iterator->next[real_key];
                free(item->data);
                free(item);
                return last;
            }
            
            last->next[real_key] = iterator->next[real_key];
            free(item->data);
            free(item);
            return list;
        }
        
        last = iterator;
        iterator = iterator->next[real_key];
    }
    
    return list;
}

Node or_find(Node list, char * key, char * item) {
    Node iterator;
    iterator = list;
    
    while (iterator != NULL) {
        if (strcmp((char *) iterator->data, item) == 0) {
            return iterator;
        }
        iterator = iterator->next[hash(key, HASH_LENGTH)];
    }
    return NULL;    
}


void ll_destroy(Node list) {
    Node iterator = list;
    while (iterator != NULL) {
        iterator = ll_remove(list, iterator);
    }
}

void or_destroy(Node list, char * key) {
    Node iterator = list;
    or_remove(list, key, iterator);
}
