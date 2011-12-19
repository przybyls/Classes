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
unsigned int hash(char* str, unsigned int length) {
   unsigned int retval = 1;
   
   unsigned int i = 0;
   for (i = 0; i < length; str++, i++) { // increment i to complete an iteration. iterate str to pass through the string
      retval = ((retval << 1) + retval) + (*str);
   }

   return retval;
}
