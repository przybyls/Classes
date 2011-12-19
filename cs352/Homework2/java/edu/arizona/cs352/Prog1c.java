/*
 * Copyright 2011 Leo Przybylski
 * 
 * Licensed under the Educational Community License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.opensource.org/licenses/ecl2.php
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*=============================================================================
 |   Assignment:  Prog1c
 |
 |       Author:  Leo Przybylski
 |     Language:  przybyls@lectura:~$ java -version
 |                java version "1.6.0_26"
 |                Java(TM) SE Runtime Environment (build 1.6.0_26-b03)
 |                Oracle JRockit(R) (build R28.1.4-7-144370-1.6.0_26-20110617-2130-linux-x86_64, compiled mode)
 |   To Compile:  cd java;
 |                javac edu/arizona/cs352/Prog1c.java
 |
 |        Class:  Csc352 Systems Programming and UNIX
 |   Instructor:  Dr. Lester McCann
 |     Due Date:  September 01, 2011 at 5pm
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  Learn to handle command line arguments from a Unix shell. The
 |                program parses arguments and sorts them ignoring leading hyphens
 |
 |        Input:  The program takes any number of arguments that may or may not be
 |                prefixed with one or more hyphens.
 |
 |       Output:  An ascending sorted list of arguments passed in is printed out.
 |
 |    Algorithm:  The approach used is to iterate over args passed into the main
 |                method and create a collection of arguments. A Comparator
 |                is applied to ignore leading '-' and '--'.
 |
 |   Required Features Not Included:  
 |
 |   Known Bugs: 
 |
 *===========================================================================*/
package edu.arizona.cs352;

import java.util.Comparator;
import java.util.List;

import static java.util.Arrays.asList;
import static java.util.Collections.sort;

/**
 *
 * <h3>Description</h3>
 * Learn to handle command line arguments from a Unix shell. The
 * program parses arguments and sorts them ignoring leading hyphens
 *
 * <h3>Input</h3>  
 * The program takes any number of arguments that may or may not be
 * prefixed with one or more hyphens.
 *
 * <h3>Output</h3>  
 * An ascending sorted list of arguments passed in is printed out.
 *
 * <h3>Algorithm</h3>
 * The approach used is to iterate over args passed into the main
 * method and create a collection of arguments. A Comparator
 *                is applied to ignore leading '-' and '--'.
 *
 * @author Leo Przybylski (przybyls [at] arizona.edu)
 */
public class Prog1c {
    private List<String> args;
    
    /*------------------------------------------------- main -----
      |  Function main
      |
      |  Purpose:  Entry point for the application. Handles parsing of arguments
      |            and creates the Prog1c instance. Then prints the {@link String}
      |            value of the {@link Prog1c} instance. In here a {@link Comparator}
      |            is applied to the arguments to produce a sorted {@link List}
      |
      |  Parameters:
      |      args (IN) -- arguments passed in from the commandline
      |
      |  Returns:  returns nothing.
      *-------------------------------------------------------------------*/
    public Prog1c(final String ... args) {
        this.args = (List<String>) asList(args);
        sort(getArgs(), new Comparator<String>() {
                
                /*------------------------------------------------- compareTo -----
                  |  Function compareTo
                  |
                  |  Purpose:  Used when sorting the arguments {@link List} instance.
                  | Compares two {@link String} instances. The {@link String} instances may or 
                  | may not have preceeding '--' or '-'. When they do, these are stripped from the 
                  | {@link String} instance. A copy is made first, so the original is not effected.
                  |
                  |  Parameters:
                  |      a - a String to compare
                  |      b - another String to compare
                  |
                  |  Returns:  0 if same, -1 if a is smaller than b, and 1 if a is larger than b
                  *-------------------------------------------------------------------*/
                /**
                 * Used when sorting the arguments {@link List} instance.
                 * Compares two {@link String} instances. The {@link String} instances may or 
                 * may not have preceeding '--' or '-'. When they do, these are stripped from the 
                 * {@link String} instance. A copy is made first, so the original is not effected.
                 *
                 * @return 0 if same, -1 if a is smaller than b, and 1 if a is larger than b
                 */
                @Override
                public int compare(final String a, final String b) {
                    int preidx = 0;
                    if (a.startsWith("--")) {
                        preidx = "--".length();
                    }
                    else if (a.startsWith("-")) {
                        preidx = "-".length();
                    }

                    final String stripped1 = a.substring(preidx);
                    
                    preidx = 0; // reset
                    if (b.startsWith("--")) {
                        preidx = "--".length();
                    }
                    else if (b.startsWith("-")) {
                        preidx = "-".length();
                    }

                    final String stripped2 = b.substring(preidx);
                    

                    return stripped1.compareTo(stripped2);
                }
            });
    }

    /**
     * Gets the args attribute. 
     * @return Returns the args.
     */
    public List<String> getArgs() {
        return args;
    }

    /**
     * Sets the args attribute value.
     * @param args The args to set.
     */
    public void setArgs(List<String> args) {
        this.args = args;
    }

    /*------------------------------------------------- main -----
      |  Function main
      |
      |  Purpose:  Entry point for the application. Handles parsing of arguments
      |            and creates the Prog1c instance. Then prints the {@link String}
      |            value of the {@link Prog1c} instance.
      |
      |  Parameters:
      |      args (IN) -- arguments passed in from the commandline
      |
      |  Returns:  returns nothing.
      *-------------------------------------------------------------------*/
    /**
     * Iterates over args and creates a space-delimited {@link String} instance
     *
     * @return space-delimited {@link String} instance of a sorted {@link List} of {@link String} instances
     */ 
    @Override
    public String toString() {
        if (args == null || args.size() < 1) {
            return "";
        }

        final StringBuilder retval = new StringBuilder();

        for (final String arg : getArgs()) {
            retval.append(arg).append(" ");
        }
        
        return retval.toString();
    }
    
    /*------------------------------------------------- main -----
      |  Function main
      |
      |  Purpose:  Entry point for the application. Handles parsing of arguments
      |            and creates the Prog1c instance. Then prints the {@link String}
      |            value of the {@link Prog1c} instance.
      |
      |  Parameters:
      |      args (IN) -- arguments passed in from the commandline
      |
      |  Returns:  returns nothing.
      *-------------------------------------------------------------------*/
    /**
     *  Entry point for the application. Handles parsing of arguments
     *            and creates the Prog1c instance. Then prints the {@link String}
     *            value of the {@link Prog1c} instance.
     * 
     * @param args is an array of {@link String} instances representing arguments
     * passed in from the command line
     */
    public static void main(final String ... args) {
        final String output = new Prog1c(args).toString();
        System.out.println(output);
    }           
}