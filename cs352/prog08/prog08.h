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
#ifndef _PROG08_H_
#define _PROG08_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logging.h"

#define false FALSE
#define true TRUE
#define ARR_BUF_INC 16

Log * context;

typedef enum boolean_t { 
    FALSE, 
    TRUE 
} boolean;


typedef struct PIXEL {
    char color;
    struct PIXEL * up;
    struct PIXEL * down;
    struct PIXEL * left;
    struct PIXEL * right;
} Pixel;

typedef struct {
    Pixel * pixels;
    int height;
    int width;
} Image;

/*------------------------------------------------- header_validate -----
  |  Function header_validate
  |
  |  Purpose:  Upon reading the header line from a text file, it
  |            is validated here. Instead of returning an error,
  |            the program will exit if there's any issue. It was
  |            Dr. McCann's bright idea.
  |       
  |
  |  Parameters: header (IN) -- String of the header line
  |                
  |
  |  Returns:  Nuttin'
  *-------------------------------------------------------------------*/
void header_validate(char *);

/*------------------------------------------------- header_parse -----
  |  Function header_parse
  |
  |  Purpose:  Parses the header line into width and height
  |       
  |
  |  Parameters: header (IN) -- String of the header line
  |                
  |
  |  Returns:  Nuttin'
  *-------------------------------------------------------------------*/
void header_parse(char *, int *, int*);

/*------------------------------------------------- pixel_init -----
  |  Function pixel_init
  |
  |  Purpose: Initializes a Pixel type. Starts by dynamically allocating
  |           space for the pixel. It then locates a pixel (if there is
  |           one) at the location specified, and assigns it the pixel
  |           to it. Also, sets the color on the pixel.
  | 
  |  Parameters: image (IN/OUT) -- image used to assign the pixel too.
  |              When assigning the pixel by location, it must be located
  |              in the image.
  |              row  -- row coordinate for the pixel within the image
  |              col  -- column coordinate for the pixel within the image
  |              Pixel * (OUT) -- Doesn't really do anything with this.
  |                
  |
  |  Returns:  Pointer to a Pixel. The pointer may subsequently be used 
  |            as an argument to the function free(3).
  *-------------------------------------------------------------------*/
Pixel * pixel_init(Image *, int, int, int);

/*------------------------------------------------- image_init -----
  |  Function image_init
  |
  |  Purpose:  Reads serialized image data from a multidimensional 
  |            array and deserializes it into an Image instance. One
  |            line of data per array element. 
  |       
  |
  |  Parameters: pixel_arr (IN) -- Multidimensional array of pixels
  |              width (IN) -- Image width retrieved from the header
  |              height (IN) -- Image height retrieved from the header
  |                
  |
  |  Returns:  New Image instance
  *-------------------------------------------------------------------*/
Image * image_init(char **, size_t, size_t);

/*------------------------------------------------- image_file_init -----
  |  Function image_file_init
  |
  |  Purpose:  Reads image data in from a file before a subsequent call to
  |            image_init
  |       
  |
  |  Parameters: image_file (IN) -- open FILE handle with image data
  |                
  |
  |  Returns:  Image instance
  *-------------------------------------------------------------------*/
Image * image_file_init(FILE *);

/*------------------------------------------------- pixel_free -----
  |  Function pixel_free
  |
  |  Purpose:  Frees allocation of a Pixel instance
  |       
  |
  |  Parameters: pixel (IN) -- Pixel instance
  |                
  |
  |  Returns:  Nuttin
  *-------------------------------------------------------------------*/
void pixel_free(Pixel *);

/*------------------------------------------------- image_free -----
  |  Function image_free
  |
  |  Purpose:  Frees memory allocation of an Image instance
  |       
  |
  |  Parameters: image (IN) -- Image instance to free
  |                
  |
  |  Returns:  Nuttin
  *-------------------------------------------------------------------*/
void image_free(Image *);

/*------------------------------------------------- image_print -----
  |  Function image_print
  |
  |  Purpose:  Recursively prints an image to the screen
  |       
  |
  |  Parameters: image (IN) -- Image instance to print
  |                
  |
  |  Returns:  Nuttin
  *-------------------------------------------------------------------*/
void image_print(Image *);

/*------------------------------------------------- image_pixel_get -----
  |  Function image_pixel_get
  |
  |  Purpose:  Retrieves a Pixel in an Image at the specified location.
  |       
  |
  |  Parameters: image (IN) -- Image instance to search
  |              row (IN) -- row location
  |              col (IN) -- col instance
  |                
  |
  |  Returns:  Pixel instance
  *-------------------------------------------------------------------*/
Pixel * image_pixel_get(Image *, int, int);

/*------------------------------------------------- chararr_get -----
  |  Function chararr_get
  |
  |  Purpose:  Gets data from an array of strings at the specified 
  |            index
  |       
  |
  |  Parameters: arr (IN) -- Array of strings to search
  |              idx (IN) -- Index to retrieve
  |                
  |
  |  Returns:  String at the index mentioned
  *-------------------------------------------------------------------*/
char * chararr_get(char **, int);

/*------------------------------------------------- pixel_flood_fill -----
  |  Function pixel_flood_fill
  |
  |  Purpose:  Flood fills pixels connected to the specified pixel. Recursively
  |            spans out looking for pixels that match the from color and
  |            changes it to the to color. Increments the number of pixels
  |            effected with each chane.
  |       
  |
  |  Parameters: pixel (IN) -- starting point
  |              from (IN) -- Color to search for
  |              to (IN) -- Color to change to
  |              pixels (IN/OUT) -- Number of pixels effected
  |                
  |
  |  Returns:  Nuttin
  *-------------------------------------------------------------------*/
void pixel_flood_fill(Pixel *, char, char, int *);

/*------------------------------------------------- each_triple -----
  |  Function each_triple
  |
  |  Purpose:  Iterates through triples passed on the command line and
  |            executes the given callback for each triple.
  |       
  |
  |  Parameters: argc (IN) -- Number of arguments
  |              argv (IN) -- arguments
  |              image (IN) -- Image instance parsed from the image file
  |              callback (IN) -- call back to call for each triple
  |                
  |
  |  Returns:  Number of images effected
  *-------------------------------------------------------------------*/
int each_triple(int, char **, Image *, Image * (*callback)(Image *, int, int, char, int *));

/*------------------------------------------------- image_fill -----
  |  Function image_fill
  |
  |  Purpose:  Fills the given image with the given color. Find a pixel
  |            at the specified location. Look for nearby pixels of the
  |            same color and change them.
  |       
  |
  |  Parameters: image (IN) -- Image instance to change
  |              row (IN) -- Row position of pixel to start at
  |              col (IN) -- Col position of pixel to start at
  |              color (IN) -- color to change to
  |              pixels (IN/OUT) -- reference to pixels changed
  |                
  |
  |  Returns:  Only success
  *-------------------------------------------------------------------*/
Image * image_fill(Image *, int, int, char, int *);

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
