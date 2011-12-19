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
#include "logging.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Log * context;

/*!
 * Prints the log head information including date and log level
 * @param log_level the log level to use
 * @return String of the full log header
 */
char * log_header(int log_level) {
    char * retval;
    char LOG_STR[9];

    if (log_level == LOG_LEVEL_DEBUG) {
        strcpy(LOG_STR, DEBUG_LOG_STR);
    }
    else if (log_level == LOG_LEVEL_INFO) {
        strcpy(LOG_STR, INFO_LOG_STR);
    }
    else if (log_level == LOG_LEVEL_WARN) {
        strcpy(LOG_STR, WARN_LOG_STR);
    }
    else if (log_level == LOG_LEVEL_ERROR) {
        strcpy(LOG_STR, ERROR_LOG_STR);
    }

    time_t now = time(NULL);
    char * nowstr = asctime(localtime(&now));
    nowstr[strlen(nowstr) - 1] = '\0';
    
    retval = strcat(nowstr, LOG_STR);
    return retval;
}

/*!
 * Initialize the logging context
 * @param program a string name for the context
 * @return Log type
 */
Log * logging_init(const char * program) {
    Log * retval = (Log *)malloc(sizeof(Log) * 20);
    char prog[255];
    char suffix [] = ".log";

    strcpy(prog, program);
    
    retval->program  = prog;
    retval->filepath = strcat(prog, suffix);
    retval->logfile  = fopen(retval->filepath, "w");

    context = retval;
    
    return retval;
}

/*!
 * Shutdown logging. You usually want to do this whenever your program is exiting.
 * @param the context you had running
 */
void logging_dest(Log * context) {
    fclose(context->logfile);
    free(context);
}

/*!
 * Outputs a message to the appropriate log with information.
 *
 * @param log_level integer representing the level to log
 * @param message to write to the log
 * @return 1 if the log was successfully written
 */
int printlog(int log_level, char * fmt, ...) {
    char message[256];
    va_list args;
    int retval = 1;
    char * header = log_header(log_level);

    va_start(args, fmt);
    vsprintf(message, fmt, args);
    va_end(args);

    if (log_level >= PROGRAM_LOG_LEVEL) {
        fprintf(context->logfile, "%s: %s\n", header, message);
    }
    
    if (log_level == LOG_LEVEL_ERROR) {
        fprintf(stderr, "%s: %s\n", header, message);        
    }
    return retval;
}

/*!
 * Parses percentages of completions, touchdowns, and interceptions (all per attempt) and the yards per attempt (YpA)
 * as space delimited input from the FILE. Then places the values into the appropriate parameters for output.
 * @param file typically stdin, but it could really be anything
 */
void parseInput(FILE * file, int * attempts, int * completions, int * yards, int * touchdowns, int * interceptions) {

    if (feof(file)) {
        return;
    }

    size_t size = 1024;
    char * line = fgetln(file, &size);

    if (line == NULL) {
      return;
    }
    
    debug("Read %s", line);

    int status = sscanf(line, "%d %d %d %d %d", attempts, completions, yards, touchdowns, interceptions);
    
    if (status < 3) {
      error("Did not parse the right number of parameters from %s", line);
    }
}

/*!
 * The main program loop
 */
int main(int argc, char ** argv) {
    context = logging_init("prog02p3.c");
    FILE * file;

    file = stdin;

    if (file == NULL) {
        error("Couldn't open %s ", argv[1]);
        return EXIT_FAILURE;
    }

    int * attempts      = (int *) malloc(sizeof(int));
    int * completions   = (int *) malloc(sizeof(int));
    int * yards         = (int *) malloc(sizeof(int));
    int * touchdowns    = (int *) malloc(sizeof(int));
    int * interceptions = (int *) malloc(sizeof(int));

    parseInput(file, attempts, completions, yards, touchdowns, interceptions);

    // printf("%4.2f%% %4.2f%% %4.2f%%
    
    free(attempts);
    free(completions);
    free(yards);
    free(touchdowns);
    free(interceptions);
    fclose(file);

    logging_dest(context);
    return EXIT_SUCCESS;
}
