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
 |   Assignment:  Program #7 Part 1 Flesch-Kincaid Grade Level Readability
 |
 |       Author:  Leo Przybylski
 |     Language:  
 |   To Compile:  gcc -DPROGRAM_LOG_LEVEL=0 logging.c prog07.c -o prog07
 |                There are different log levels:
 |                0 - DEBUG
 |                1 - INFO
 |                2 - WARN
 |                3 - ERROR
 |                The debug level is set at compile time.
 |
 |        Class:  CSc 352 — Systems Programming and UNIX
 |   Instructor:  Dr. Lester McCann
 |     Due Date:  October 20th, 2011, at the beginning of class
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  Flesch-Kincaid algorithm to determine the grade level of some
 |                ascii text.
 |
 |        Input:  
 |
 |       Output:  # of words, sentences, syllables and the grade level of the text.
 |
 |    Algorithm:  Uses Flesch Kincaid algoritm calculating the mean words per
 |                sentence and mean syllables per word to determine the grade of
 |                ascii text. It's awesome!
 |
 |   Known Bugs:  
 |
 *===========================================================================*/
#include "prog07.h"


/*------------------------------------------------- main -----
  |  Function main
  |
  |  Purpose:  Reads input from STDIN using getchar(), iterates over 
  |            each character and determines whether the character
  |            denotes the end of a sentence or word. After reaching
  |            EOF, prints output of flesch kincaid algorithm results
  |            and exits.
  |       
  |
  |  Parameters: argc (IN) -- Doesn't really do anything with this
  |              argv (IN) -- Doesn't really do anything with this.
  |                
  |
  |  Returns:  Only success
  *-------------------------------------------------------------------*/
int main(int argc, char ** argv) {
  context = logging_init("prog07");

  int word_cnt = 0;
  int sent_cnt = 0;
  int syll_cnt = 0;
  char buffer[MAX_BUFFER];
  buffer[0] = '\0';
  debug("Does this work");
  char c;
  boolean eos = false;
  while ((c = getchar()) != EOF) {
      if (iseos(c)) {
          eos = true;
          sent_cnt++;
      }

      if (iseow(c) || eos) {
          // reset the buffer, this is a word
          if (strlen(buffer) > 0) {
              int syllcnt = 0;
              syllcnt = syll_count(buffer);
              debug("Counted %d syllables for %s\n", syllcnt, buffer);
              word_cnt++;
              syll_cnt++;
          }
          buffer[0] = '\0';
      }
      else {
          // Keep reading the buffer
          concat(buffer, c);
      }
  }

  if (strlen(buffer) > 0) {
      int syllcnt = 0;
      syllcnt = syll_count(buffer);
      debug("Counted %d syllables for %s\n", syllcnt, buffer);
      word_cnt++;
      syll_cnt++;
  }

  // Never got the last end of sentence character
  if (!eos) {
      sent_cnt++;
  }
  
  debug("mspw = %.2f", mspw(syll_cnt, word_cnt)); 
  debug("mwps = %.2f", mwps(word_cnt, sent_cnt));
  printf("%d sentences\n", sent_cnt);
  printf("%d words\n", word_cnt);
  printf("%d syllables\n", syll_cnt);
  printf("%.2f is the text’s grade level\n", 
         0.39 * mwps(word_cnt, sent_cnt) + 11.8 * mspw(syll_cnt, word_cnt) - 15.5);
  
  logging_dest(context);
  return EXIT_SUCCESS;
}

/*------------------------------------------------- mspw -----
  |  Function mspw
  |
  |  Purpose:  Calculates means syllables per word
  |       
  |
  |  Parameters: syll_cnt (IN) -- Syllable count
  |              word_cnt (IN) -- word count
  |                
  |
  |  Returns:  Mean syllables per word
  *-------------------------------------------------------------------*/
float mspw(int syll_cnt, int word_cnt) {
    if (word_cnt == 0) return 0;
    return (syll_cnt / word_cnt);
}

/*------------------------------------------------- mwps -----
  |  Function mwps
  |
  |  Purpose: Calculates mean words per sentence
  |       
  |
  |  Parameters: word_cnt (IN) -- Word count
  |              sent_cnt (IN) -- Sentence count
  |
  |  Returns:  Mean words per sentence
  *-------------------------------------------------------------------*/
float mwps(int word_cnt, int sent_cnt) {
    if (sent_cnt == 0) return 0;
    return (word_cnt / sent_cnt);
}

/*------------------------------------------------- isvowel -----
  |  Function isvowel
  |
  |  Purpose:  Compares a character against a string "aeiou" to see
  |            if the lowercase version of that character exists
  |            in the string.
  |       
  |
  |  Parameters: c (IN) -- Character to check
  |                
  |
  |  Returns: true if the character is a vowel. False otherwise.
  *-------------------------------------------------------------------*/
int isvowel(char c) {
    char * vowels = "aeiou";
    int len = strlen(vowels);
    int i = 0;
    while (vowels[i] != '\0') {
        if (c == vowels[i]) {
            return true;
        }
        i++;
    }
    return false;
}

/*------------------------------------------------- syll_count -----
  |  Function syll_count
  |
  |  Purpose:  Counts syllables in a word. Dipthongs (ie., 
  |            aa, ae, ee, etc...) all count as one syllable. Silent
  |            syllables do not count.
  |       
  |
  |  Parameters: word (IN) -- String that is a word. Doesn't have to 
  |                           be a word, but you want it to be.
  |                
  |
  |  Returns:  Number of syllables in the word as an int
  *-------------------------------------------------------------------*/
int syll_count(char * word) {
    int retval = 0;
    int len = strlen(word);
    char last = '\0';
    
    int i = 0;
    while (word[i] != '\0') {
        char c = tolower(word[i]);
        if (isvowel(c)
            && !isvowel(last)
            && !(i == len - 1 && c == 'e')) {
            debug("%c is a vowel", c);
            retval++;
        }
        else {
            debug("%c is not a vowel", word[i]);
        }
        last = c;
        i++;
    }
    return retval;
}

/*------------------------------------------------- iseow -----
  |  Function iseow
  |
  |  Purpose:  Determines if the character denotes an end of a
  |            word.
  |       
  |
  |  Parameters: c (IN) - character to check
  |                
  |
  |  Returns:  true if is alpha or -. false otherwise
  *-------------------------------------------------------------------*/
int iseow(char c) {
    return !(isalpha(c) || c == '-');
}

/*------------------------------------------------- iseos -----
  |  Function iseos
  |
  |  Purpose:  Determines whether a character is the end of the sentence.
  |            End of sentence characters are period, question mark, and
  |            exclamation point punctuation markers.
  |       
  |
  |  Parameters: c (IN) - character to check
  |                
  |
  |  Returns:  true if '!', '?' or '.'. false otherwise
  *-------------------------------------------------------------------*/
int iseos(char c) {
    return (c == '!' || c == '?' || c == '.');
}

/*------------------------------------------------- concat -----
  |  Function concat
  |
  |  Purpose:  Concatenates a character to the end of a string
  |       
  |
  |  Parameters: dest (IN/OUT) -- destination string
  |              source (IN) -- Character to append
  |                
  |
  |  Returns:  Nuttin'
  *-------------------------------------------------------------------*/
void concat(char * dest, char source) {
    size_t dest_len = strlen(dest);
    size_t i;

    dest[dest_len] = source;
    dest[dest_len + 1] = '\0';
}
