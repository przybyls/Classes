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
#include "linkedlist.h"

Node ll_new(void * data) {
    Node retval = malloc(sizeof(struct _Node));
    retval->size = 0;
    retval->data = data;
    retval->next = NULL;
    return retval;
}

Node ll_add(Node list, void * data) {
    Node newNode = ll_new(data);
    newNode->next = list;
    return newNode;
}

Node ll_remove(Node list, Node item) {
    Node iterator = list;
    Node last = NULL;
    while (iterator != NULL) {
        if (iterator == item) {
            if (last == NULL) {
                last = iterator->next;
                free(item->data);
                free(item);
                return last;
            }
            
            last->next = iterator->next;
            free(item->data);
            free(item);
            return list;
        }
        
        last = iterator;
        iterator = iterator->next;
    }
    
    return list;
}

void ll_destroy(Node list) {
    Node iterator = list;
    while (iterator != NULL) {
        iterator = ll_remove(list, iterator);
    }
}
