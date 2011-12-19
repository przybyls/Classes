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
 |   Assignment:  Program #11: The One with the 3D Orthogonal List
 |
 |       Author:  Leo Przybylski
 |     Language:  
 |   To Compile:  make -DPROGRAM_LOG_LEVEL=0 
 |                There are different log levels:
 |                0 - DEBUG
 |                1 - INFO
 |                2 - WARN
 |                3 - ERROR
 |                The debug level is set at compile time.
 |
 |        Class:  CSc 352 — Systems Programming and UNIX
 |   Instructor:  Dr. Lester McCann
 |     Due Date:  December 1st, 2011, at the beginning of class
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  
 |
 |
 |        Input:  
 |
 |       Output:  
 |
 |    Algorithm:  
 |
 |   Known Bugs:  
 |
 *===========================================================================*/
#include "prog11.h"

/*------------------------------------------------- main -----
  |  Function main
  |
  |  Purpose:  Parses command line options searching for a filename. Reads in the
  |            file line-by-line and parses it into Operations. Operations are then
  |            carried out.
  |       
  |
  |  Parameters: argc (IN) -- number of arguments
  |              argv (IN) -- Expects the program name and operation file name
  |                
  |
  |  Returns:  Only success
  *-------------------------------------------------------------------*/
int main(int argc, char ** argv) {
    context = logging_init("prog11");
    char * filename;

    if (argc < 1) {
        error("Incorrect number of arguments. Need number of worms and size.");
        usage(*argv);
        exit(EXIT_FAILURE);
    }

    OperationParser operation_table[ALLOWED_OPERATIONS];
    operation_table_init(operation_table);

    EntityCollection entities = entities_new();
    
    filename = strdup(argv[1]); // Free this

    FILE * commands = fopen(filename, "r");
    
    char * line = (char *) malloc(sizeof(char) * LINE_BUFFER);
    while ((line = fgets(line, LINE_BUFFER, commands)) != NULL) {
        int line_size = strlen(line);
        line[line_size - 1] = '\0'; // trim trailing newline

        Operation op = operation_parse(operation_table, line);
        if (op != NULL) {
            op->handler(op, entities);
        }
    }

    fclose(commands);
    free(filename);
    logging_dest(context);
    return EXIT_SUCCESS;
}


/*------------------------------------------------- entities_new -----
  |  Function entities_new
  |
  |  Purpose:  Initializes the EntityCollection used to contain majors,
  |            hometowns, and years.
  |       
  |
  |  Parameters: 
  |                
  |
  |  Returns:  New EntityCollection instance
  *-------------------------------------------------------------------*/
EntityCollection entities_new(void) {
    EntityCollection retval = (EntityCollection) malloc(sizeof(struct _EntityCollection));
    retval->majors    = NULL;
    retval->years     = NULL;
    retval->hometowns = NULL;
    return retval;
}

/*------------------------------------------------- entities_delete -----
  |  Function entities_delete
  |
  |  Purpose:  Clears out the EntityCollection
  |       
  |
  |  Parameters: EntityCollection to clear
  |                
  |
  |  Returns:  nada
  *-------------------------------------------------------------------*/
void entities_delete(EntityCollection entities) {
    int i = 0;
    if (entities->majors != NULL) {
        Node iterator = entities->majors;
        while (iterator != NULL) {
            Node iterator2 = iterator->next[hash("students", HASH_LENGTH)];

            while (iterator2 != NULL) {
                Node temp = iterator2->next[hash("students", HASH_LENGTH)];
                free(iterator2->data);
                free(iterator2);
                iterator2 = temp;
            }
            Node temp = iterator->next[hash("majors", HASH_LENGTH)];
            free(iterator->data);
            free(iterator);
            iterator = temp;
        }
    }
    entities->majors = NULL;

    if (entities->hometowns != NULL) {
        Node iterator = entities->majors;
        while (iterator != NULL) {
            Node temp = iterator->next[hash("hometowns", HASH_LENGTH)];
            free(iterator->data);
            free(iterator);
            iterator = temp;
        }
    }
    entities->hometowns = NULL;
    
    if (entities->years != NULL) {
        Node iterator = entities->majors;
        while (iterator != NULL) {
            Node temp = iterator->next[hash("years", HASH_LENGTH)];
            free(iterator->data);
            free(iterator);
            iterator = temp;
        }
    }
    entities->years = NULL;
}

Operation operation_insert_new(void) {
    Operation retval = (Operation) malloc(sizeof(struct _Operation));
    retval->op_code = DO_INSERT;
    retval->name[0] = '\0';
    retval->major[0] = '\0';
    retval->year[0] = '\0';
    retval->hometown[0] = '\0';
    retval->handler = operation_do_insert;
    return retval;
}

Operation operation_delete_new(void) {
    Operation retval = (Operation) malloc(sizeof(struct _Operation));
    retval->op_code = DO_DELETE;
    retval->criteria[0] = '\0';
    retval->handler = operation_do_delete;
    return retval;
}

Operation operation_query_new(void) {
    Operation retval = (Operation) malloc(sizeof(struct _Operation));
    retval->op_code = DO_QUERY;
    retval->criteria[0] = '\0';
    retval->handler = operation_do_query;
    return retval;
}

Operation operation_status_new(void) {
    Operation retval = (Operation) malloc(sizeof(struct _Operation));
    retval->op_code = DO_STATUS;
    retval->handler = operation_do_status;
    return retval;
}

Operation operation_purge_new(void) {
    Operation retval = (Operation) malloc(sizeof(struct _Operation));
    retval->op_code = DO_PURGE;
    retval->handler = operation_do_purge;
    return retval;
}


Operation operation_parse(OperationParser * parsers, char * line) {
    char * token;
    debug("Line is %s", line);
    token = strsep(&line, ",");

    unsigned int key = hash(token, 1);
    Operation retval = parsers[key](line);
    return retval;
}

Operation operation_parse_insert(char * line) {
    Operation retval = operation_insert_new();
    char * token;
    info("Now parsing an insert operation");
    token = strsep(&line, ",");

    if (token != NULL) {
        strcat(retval->name, token);
    }
    token = strsep(&line, ",");

    if (token != NULL) {
        strcat(retval->name, ", ");
        strcat(retval->name, token);
    }
    token = strsep(&line, ",");

    if (token != NULL && !isspace(token[0]) && token[0] != '\0') { 
        strcat(retval->name, " ");
        strcat(retval->name, token);
        strcat(retval->name, ".");
    }

    token = strsep(&line, ",");

    if (token != NULL) {
        strcpy(retval->major, token);
    }
    token = strsep(&line, ",");
    if (token != NULL) {
        strcat(retval->year, token);
        strcat(retval->year, "-");
    }
    token = strsep(&line, ",");
    if (token != NULL) {
        strcpy(retval->year, token);
    }
    token = strsep(&line, ",");
    if (token != NULL) {
        strcpy(retval->hometown, token);
    }

    return retval;
}

Operation operation_parse_delete(char * line) {
    info("Now parsing an delete operation");
    Operation retval = operation_delete_new();
    strcpy(retval->criteria, line);
    return retval;
}

Operation operation_parse_query(char * line) {
    info("Now parsing an query operation");
    Operation retval = operation_query_new();
    char * token;

    token = strsep(&line, ",");

    if (token != NULL) {
        retval->query_id = atoi(token);
    }
    strcpy(retval->criteria, line);
    return retval;
}

Operation operation_parse_status(char * line) {
    info("Now parsing an status operation");
    return operation_status_new();
}

Operation operation_parse_purge(char * line) {
    info("Now parsing an purge operation");
    return operation_purge_new();
}

void operation_table_init(OperationParser * table) {
    char op_code[2];
    unsigned int key;
    op_code[1] = '\0';

    op_code[0] = DO_INSERT;
    key = hash(op_code, 1);
    table[key]   = operation_parse_insert;

    op_code[0] = DO_QUERY;
    key = hash(op_code, 1);
    table[key]   = operation_parse_query;

    op_code[0] = DO_PURGE;
    key = hash(op_code, 1);
    table[key] = operation_parse_purge;

    op_code[0] = DO_DELETE;
    key = hash(op_code, 1);
    table[key] = operation_parse_delete;

    op_code[0] = DO_STATUS;
    key = hash(op_code, 1);
    table[key] = operation_parse_status;
}

OperationHandler operation_table_get(OperationHandler * table, char * key) {
    unsigned int real_key = hash(key, 1);
    return table[real_key];
}

void operation_do_insert(void * op, EntityCollection entities) {
    Operation operation = (Operation) op;
    debug("Inserting %s", operation->name);
    Node major;
    Node hometown;
    Node year;
    Node student;
    Node students;
    
    if (entities->majors == NULL) {
        entities->majors = ll_new(operation->major);
    }

    // Add student to the major
    major = or_find(entities->majors, "majors", operation->major);    
    if (major == NULL) {
        major = or_add(entities->majors, "majors", operation->major);
        entities->majors = major;
    }
    
    students = major->next[hash("students", HASH_LENGTH)];
    if (students == NULL) {        
        students = ll_new(operation->name);
        major->next[hash("students", HASH_LENGTH)] = students;
    }
                                                            
    student = or_find(major, "students", operation->name);
    if (student == NULL) {
        students = or_add(major, "students", operation->name);
        debug("Added %s to %s", operation->name, operation->major);
    }
    student = NULL;
    students = NULL;
    
    if (entities->hometowns == NULL) {
        entities->hometowns = ll_new(operation->hometown);
        debug("Added hometown %s", operation->hometown);
    }                                           

    // Add student to the hometown
    hometown = or_find(entities->hometowns, "hometowns", operation->hometown);    
    if (hometown == NULL) {
        debug("Adding hometown %s", operation->hometown);
        hometown = or_add(entities->hometowns, "hometowns", operation->hometown);
        entities->hometowns = hometown;
    }

    students = hometown->next[hash("students", HASH_LENGTH)];
    if (students == NULL) {        
        students = ll_new(operation->name);
        hometown->next[hash("students", HASH_LENGTH)] = students;
    }

    student = or_find(hometown, "students", operation->name);
    if (student == NULL) {
        students = or_add(hometown, "students", operation->name);
        debug("Added %s to %s", operation->name, operation->hometown);
    }

    student = NULL;
    students = NULL;
    
     if (entities->years == NULL) {
         entities->years = ll_new(operation->year);         
     }

     // Add student to the years
     year = or_find(entities->years, "years", operation->year);    
     if (year == NULL) {
         year = or_add(entities->years, "years", operation->year);
     }

     students = year->next[hash("students", HASH_LENGTH)];
     if (students == NULL) {        
         students = ll_new(operation->name);
         year->next[hash("students", HASH_LENGTH)] = students;
     }

     student = or_find(year, "students", operation->name);
     if (student == NULL) {
         students = or_add(year, "students", operation->name);
         debug("Added %s to %s", operation->name, operation->year);
     }
}

void operation_do_query(void * op, EntityCollection entities) {
    Operation operation = (Operation) op;
    OperationHandler queries[4] = {NULL, operation_do_query_year, operation_do_query_major, operation_do_query_hometown};
    queries[operation->query_id](op, entities);
}

void operation_do_query_major(void * op, EntityCollection entities) {
    Operation operation = (Operation) op;
    debug("Handling major query for %s", operation->criteria);

    printf("The student(s) studying %s\n", operation->criteria);

    int count = 0;
    Node iterator = entities->majors;    
    while (iterator != NULL) {
        debug("Comparing %s to %s", iterator->data, operation->criteria);
        if (iterator->data != NULL && strcmp(iterator->data, operation->criteria) == 0) {
            iterator = iterator->next[hash("students", HASH_LENGTH)];
            while (iterator != NULL) {
                printf("%s\n", (char *) iterator->data);
                iterator = iterator->next[hash("students", HASH_LENGTH)];
            }
            printf("\n");
            return;
        }
        iterator = iterator->next[hash("majors", HASH_LENGTH)];
    }
}

void operation_do_query_hometown(void * op, EntityCollection entities) {
    Operation operation = (Operation) op;
    debug("Handling homeland query for %s", operation->criteria);

    printf("The student(s) from %s with the latest expected year of graduation:\n", operation->criteria);

    int count = 0;
    Node iterator = entities->hometowns;    
    while (iterator != NULL) {
        debug("Comparing %s to %s", iterator->data, operation->criteria);
        if (iterator->data != NULL && strcmp(iterator->data, operation->criteria) == 0) {
            iterator = iterator->next[hash("students", HASH_LENGTH)];
            while (iterator != NULL) {
                printf("%s\n", (char *) iterator->data);
                iterator = iterator->next[hash("students", HASH_LENGTH)];
            }
            printf("\n");
            return;
        }
        iterator = iterator->next[hash("hometowns", HASH_LENGTH)];
    }
}

int convert_start_year(char * range) {
    char year[5];
    year[0] = '\0';
    strncpy(year, range, 4);
    return atoi(year);
}

int convert_end_year (char * range) {
    return atoi(&range[5]);
}

void operation_do_query_year(void * op, EntityCollection entities) {
    Operation operation = (Operation) op;
    debug("Handling year query for %s", operation->criteria);

    int low_year_crit  = convert_start_year(operation->criteria);
    int high_year_crit = convert_end_year(operation->criteria);

    int count = 0;
    Node iterator = entities->years;    
    while (iterator != NULL) {
        debug("Comparing %s to %s", iterator->data, operation->criteria);
        int low_year  = convert_start_year(iterator->data);
        int high_year = convert_end_year(iterator->data);

        if (low_year >= low_year_crit && high_year <= high_year_crit) {
            Node iterator2 = iterator->next[hash("students", HASH_LENGTH)];
            while (iterator2 != NULL) {
                count++;
                iterator2 = iterator2->next[hash("students", HASH_LENGTH)];
            }
        }

        iterator = iterator->next[hash("years", HASH_LENGTH)];
    }
    printf("%d students were admitted from 2005 through 2008.\n\n", count);
}

void operation_do_delete(void * op, EntityCollection entities) {
    Operation operation = (Operation) op;
    debug("Handling delete for %s", operation->criteria);
}

void operation_do_status(void * op, EntityCollection entities) {
    Operation operation = (Operation) op;
    debug("Handling status ");
    
    Node iterator;
    int count;
    int total;

    total = 0;

    iterator = entities->years;
    count = 0;
    while (iterator != NULL) {
        Node iterator2 = iterator->next[hash("students", HASH_LENGTH)];
        while (iterator2 != NULL) {
            count++;
            iterator2 = iterator2->next[hash("students", HASH_LENGTH)];
        }
        iterator = iterator->next[hash("years", HASH_LENGTH)];        
    }
    printf("%d node(s) in the year admitted dimension list\n", count);
    total += count;    

    iterator = entities->majors;
    count = 0;
    while (iterator != NULL) {        
        Node iterator2 = iterator->next[hash("students", HASH_LENGTH)];
        while (iterator2 != NULL) {
            count++;
            iterator2 = iterator2->next[hash("students", HASH_LENGTH)];
        }
        iterator = iterator->next[hash("majors", HASH_LENGTH)];
    }
    printf("%d node(s) in the major dimension list\n", count);
    total += count;

    iterator = entities->hometowns;
    count = 0;
    while (iterator != NULL) {
        Node iterator2 = iterator->next[hash("students", HASH_LENGTH)];
        while (iterator2 != NULL) {
            count++;
            iterator2 = iterator2->next[hash("students", HASH_LENGTH)];
        }
        iterator = iterator->next[hash("hometowns", HASH_LENGTH)];        
    }
    printf("%d node(s) in the hometown dimension list\n", count);
    total += count;
    printf ("%d data item node(s)\n\n", total);
    
}

void operation_do_purge(void * op, EntityCollection entities) {
    Operation operation = (Operation) op;
    debug("Handling purge ");
    
    entities_delete(entities);
}


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
void usage(char * progname) {
    fprintf(stderr, "USAGE: \n");
    fprintf(stderr, "       %s %s\n", progname, " <filename>");
    fprintf(stderr, "               filename - path of the file to read commands from.\n");
    fprintf(stderr, "EXAMPLE: \n");
    fprintf(stderr, "        ./%s commands.txt\n\n", progname);
    fprintf(stderr, "COPYRIGHT: \n");
    fprintf(stderr, "        Copyright 2011 Leo Przybylski\n");
    fprintf(stderr, "        Educational Community License, Version 2.0 (the \"License\"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.osedu.org/licenses/ECL-2.0\n\n");

}

