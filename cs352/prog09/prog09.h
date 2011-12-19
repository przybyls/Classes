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
#ifndef _PROG09_H_
#define _PROG09_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "logging.h"

#define false FALSE
#define true TRUE
#define ARR_BUF_INC 16
#define PCX_EXT ".pcx"
#define LINE_SIZE_LIMIT 255

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
} *Pixel;

typedef struct _PCX {
    Pixel pixels;
    int height;
    int width;
} *PcxImage;

typedef struct _ASCII_IMG {
    Pixel pixels;
    int height;
    int width;
} *AsciiImage;

typedef struct _IMG_FILE {
    char * filename;
    FILE * handle;
} *ImageFile;

typedef struct {
    Pixel pixels;
} *Image;

typedef void * (*ErrorCallback)(int, char *);

typedef struct _PcxHeader
{
    char	Identifier;        /* PCX Id Number (Always 0x0A) */
    char	Version;           /* Version Number */
    char	Encoding;          /* Encoding Format */
    char	BitsPerPixel;      /* Bits per Pixel */
    short	XStart;            /* Left of image */
    short	YStart;            /* Top of Image */
    short	XEnd;              /* Right of Image */
    short	YEnd;              /* Bottom of image */
    short	HorzRes;           /* Horizontal Resolution */
    short	VertRes;           /* Vertical Resolution */
    char	Palette[48];       /* 16-Color EGA Palette */
    char	Reserved1;         /* Reserved (Always 0) */
    char	NumBitPlanes;      /* Number of Bit Planes */
    short	BytesPerLine;      /* Bytes per Scan-line */
    short	PaletteType;       /* Palette Type */
    short	HorzScreenSize;    /* Horizontal Screen Size */
    short	VertScreenSize;    /* Vertical Screen Size */
    char	Reserved2[54];     /* Reserved (Always 0) */
} PCXHEAD;


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
// Image * image_init(char **, size_t, size_t);

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
// Image * image_file_init(FILE *);

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
// Pixel * image_pixel_get(Image *, int, int);

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

char a2pcx(char);

////////////////////////////////////////////////////////////////////////////////
// Start of Generic Pixel and  Image Functions                                //
////////////////////////////////////////////////////////////////////////////////

Pixel pixel_new(void);

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
Pixel ascii_pixel_init(AsciiImage, int, int, char);

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
void pixel_free(Pixel);

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
void pixel_flood_print_right(Pixel);

void pixel_flood_print(Pixel);

/*------------------------------------------------- ascii_image_print -----
  |  Function ascii_image_print
  |
  |  Purpose:  Recursively prints an image to the screen
  |       
  |
  |  Parameters: image (IN) -- Image instance to print
  |                
  |
  |  Returns:  Nuttin
  *-------------------------------------------------------------------*/
void ascii_image_print(AsciiImage);

void pixel_flood_free(Pixel);

void pixel_flood_free_right(Pixel);

Pixel pixel_row_a2b(Pixel);

ImageFile image_file_new(char *);

PcxImage image_file_read_pcx(ImageFile);

AsciiImage image_file_read_ascii(ImageFile);

void * image_file_error(int, char *);

ImageFile image_file_open(ImageFile, char *, ErrorCallback);

void image_file_close(ImageFile, ErrorCallback);

void image_file_free(ImageFile);

////////////////////////////////////////////////////////////////////////////////
// End of Generic Pixel and  Image Functions                                  //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Start of ASCII Image Functions                                             //
////////////////////////////////////////////////////////////////////////////////

AsciiImage ascii_image_new(int, int);

void ascii_image_free(AsciiImage);

void ascii_image_add_char_row(AsciiImage, char *);

void image_add_row(Image, Pixel);

////////////////////////////////////////////////////////////////////////////////
// End of ASCII Image Functions                                               //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Start of PCX Image Functions                                               //
////////////////////////////////////////////////////////////////////////////////
void pcx_image_add_ascii_row(PcxImage, Pixel);

void pcx_image_add_row(PcxImage, Pixel);

PcxImage pcx_image_new(int, int);

void pcx_image_init(PcxImage, AsciiImage);

void pcx_image_add_ascii_row(PcxImage, Pixel);

void pcx_image_free(PcxImage);

void pcx_image_save(PcxImage, ImageFile);
////////////////////////////////////////////////////////////////////////////////
// End of PCX Image Functions                                                 //
////////////////////////////////////////////////////////////////////////////////
    
char * prefix_get(char *);

void trim(char *);

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
