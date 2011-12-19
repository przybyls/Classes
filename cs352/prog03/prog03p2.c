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
 |   Assignment:  Program #3: File Permissions and Hex Dump
 |
 |       Author:  Leo Przybylski
 |     Language:  
 |   To Compile:  gcc -DPROGRAM_LOG_LEVEL=0 prog03p2.c -o prog03p2
 |                There are different log levels:
 |                0 - DEBUG
 |                1 - INFO
 |                2 - WARN
 |                3 - ERROR
 |                The debug level is set at compile time.
 |
 |        Class:  CSc 352 — Systems Programming and UNIX
 |   Instructor:  Dr. Lester McCann
 |     Due Date:  September 15th, 2011, at the beginning of class
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  Decode output from the od command (no arguments). od is a linux
 |                command. With no arguments, it outputs 16-bit octal encoded
 |                strings. The objective is to parse and decode these strings into
 |                their original ascii input.
 |
 |        Input:  Input is basicall what od outputs. It is intended that od will be
 |                piped into the program like this: od hello | prog03p2
 |
 |       Output:  The output would be whatever is in hello
 |
 |    Algorithm:  scanf is used to read each set of digits output from od into a 
 |                variable. Only one set of digits is read at a time. Byte count 
 |                digits are ignored and thrown out by checking for modulus 9.
 |                That is, every 9 sets of digits, a set is thrown out. The last set
 |                set is also always thrown out. In some cases, the data will not end
 |                perfectly, but the last set of digits is always a length.
 | 
 |                Each set of digits is then processed individually. A set of digits
 |                is a set of two ascii characters in reverse. The first character is
 |                parsed by applying a mask to the digits. This will 0 out the first 
 |                bits of the 16-bit string leaving only the last 8-bits which 
 |                represent the first character. Then, to get the second byte/
 |                character, the digits are shifted 8 positions to the right. The 
 |                same mask applied previously is then applied on the second set of
 |                bits. This represents our second byte. The output is then printed
 |                using printf. 
 |
 |   Known Bugs:  
 |
 *===========================================================================*/
#include "logging.h"
#include "prog03p2.h"


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
  context = logging_init("prog03p2");

  unsigned int data;
  int status;
  int section_index = -1;
  
  status = scanf("%o", &data);
  while (status > 0) {
      section_index++;
      if (section_index % 9 == 0) {
          status = scanf(OCTAL_FMT, &data);
          continue;
      }

      unsigned int next;
      
      // Check for more bytes in the stream
      if ((status = scanf(OCTAL_FMT, &next)) > 0) { 
          // Handle data here
          printOctal(data);
      }
      else {
          // data is just a length. Do nothing.
          debug("Found EOF");
      }
      
      // Copy next over data
      data = next;
  }
  
  logging_dest(context);
  return EXIT_SUCCESS;
}

/*------------------------------------------------- printOctal -----
  |  Function printOctal
  |
  |  Purpose:  It takes a 16-bit octal encoded string and prints its
  |            ascii equivalent.
  |
  |  Parameters:
  |      octal_digits (IN) -- Is a 16-bit octal encoded string. It 
  |                contains 2 characters that are 8-bits each 
  |                concatenated together in reverse.
  |                
  |                The first character is
  |                parsed by applying a mask to the digits. This will 0 out the first 
  |                bits of the 16-bit string leaving only the last 8-bits which 
  |                represent the first character. Then, to get the second byte/
  |                character, the digits are shifted 8 positions to the right. The 
  |                same mask applied previously is then applied on the second set of
  |                bits. This represents our second byte. The output is then printed
  |                using printf.
  |
  |  Returns:  Doesn't return anything
  *-------------------------------------------------------------------*/
void printOctal(unsigned int octal_digits) {
    unsigned int mask = 511;
    char second_byte = ((octal_digits >> 8) & mask);
    char first_byte = octal_digits & mask;
    printf("%c%c", first_byte, second_byte);
    debug("First byte is %d", first_byte);
    debug("second byte is %d", second_byte);    
}

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
