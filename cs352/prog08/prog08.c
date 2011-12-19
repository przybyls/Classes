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
 |   Assignment:  Program #8 Recursive Flood-Fill
 |
 |       Author:  Leo Przybylski
 |     Language:  
 |   To Compile:  gcc -DPROGRAM_LOG_LEVEL=0 logging.c prog08.c -o prog08
 |                There are different log levels:
 |                0 - DEBUG
 |                1 - INFO
 |                2 - WARN
 |                3 - ERROR
 |                The debug level is set at compile time.
 |
 |        Class:  CSc 352 — Systems Programming and UNIX
 |   Instructor:  Dr. Lester McCann
 |     Due Date:  October 25th, 2011, at the beginning of class
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  Recursive Flood-Fill. Reads an image in from a file and takes triples
 |                which include the location of a pixel and the color to change
 |                to. The program searches for pixels at the location specified in the
 |                triples. Then systematically searches for nearby pixels of the same
 |                color and changes them as well.
 |
 |                CHECK PROG08.H for FUNCTION DOCUMENTATION
 |
 |
 |        Input:  
 |
 |       Output:  Number of pixels changed as well as original and changed images
 |
 |    Algorithm:  Not allowed to use indices access to arrays, so direct memory
 |                access, recursion and references are used to access elements in arrays.
 |                an Image instance is used to represent Pixels of an Image. It is
 |                essentially a graph implementation where Pixels link to each other
 |                in order to use the 4 connected method to change pixels.
 |
 |   Known Bugs:  I programmed this on my mac and when I loaded it on lectura, it 
 |                would not run correctly. I believe the reason is because on OS X,
 |                memory allocation is done in 16 byte increments. I had to work around
 |                this for memory access to elements in an array because they were in
 |                16 bit increments no matter how I allocated. I believe this caused 
 |                problems on lectura because it is probably different. I didn't have
 |                enough time to work around this bug. :(
 |
 *===========================================================================*/
#include "prog08.h"


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
  |  Parameters: argc (IN) -- number of arguments
  |              argv (IN) -- Expects the program name, image file, and triples
  |                
  |
  |  Returns:  Only success
  *-------------------------------------------------------------------*/
int main(int argc, char ** argv) {
    context = logging_init("prog08");
    
    if (argc < 5) {
        error("Incorrect number of arguments. Need a file name and at least one triple");
        usage(*argv);
        exit(EXIT_FAILURE);
    }

    if ((argc - 2) % 3) {
        error("Incorrect number of arguments. Triples have 3 values hence the name");
        usage(*argv);
        exit(EXIT_FAILURE);
    }
    
    char * image_file_name = chararr_get(argv, 1);
    FILE * image_file = fopen(image_file_name, "r");
    Image * image = image_file_init(image_file);

    printf("Original Image:\n\n");
    image_print(image);

    int changed = each_triple(argc, argv, image, image_fill);

    printf("A total of %d pixels were changed\n", changed);

    fclose(image_file);
    free(image_file_name);
    image_free(image);
    logging_dest(context);
    return EXIT_SUCCESS;
}

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
Image * image_file_init(FILE * image_file) {
    Image * retval;
    char * header = (char *)malloc(sizeof(char) * BUFSIZ);
    char c;
    int pos = 0;
    while ((c = fgetc(image_file)) != EOF && c != '\n') {
        *(header + (pos * sizeof(char))) = c;
        pos++;
    }

    unsigned int rows = 0;
    unsigned int columns = 0;
    
    header_parse(header, &rows, &columns);

    unsigned int rowsize = sizeof(char) * (columns + 2);
    unsigned int row_mem_size = sizeof(char) * (columns + 1);
    unsigned int row_actual_size = row_mem_size + 8 - ((row_mem_size + 8) % 16);
    debug("Rowsize is %d\n", rowsize);
    
    char ** pixel_chars = malloc(sizeof(char *) * rows);

    unsigned int row;
    for (row = 0; row < rows; row++) {
        *(pixel_chars + (row * row_actual_size)) = (char *) malloc(row_actual_size);

        char * row_data = malloc(sizeof(char) * 1000);
        row_data = fgets(row_data, 1000, image_file);
          
        if (row_data == NULL || ferror(image_file)) {
            error("Image data invalid! Expected to read %d rows", rows);
            exit(EXIT_FAILURE);
        }

        *(row_data + columns) = '\0';
        *(pixel_chars + (row * row_actual_size)) = strcpy(*(pixel_chars + (row * row_actual_size)), row_data);

        debug("Allocated row %d at %u", row, *(pixel_chars + (row * row_actual_size)));
        debug("Allocated row %s", *(pixel_chars + (row * row_actual_size)));
    }
    
    retval = image_init(pixel_chars, rows, columns);
    free(header);
    free(pixel_chars);

    return retval;
}

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
void header_validate(char * header) {
    if (header == NULL) {
        error("Invalid header '%s'", header);
        exit(EXIT_FAILURE);
    }

    int len = strlen(header);
    
    if (len < 2) {
        error("Invalid header '%s'", header);
        exit(EXIT_FAILURE);
    }

    int i;
    int spaces = 0;
    for (i = 0; i < len; i++) {
        char header_char = *(header + (i * sizeof(char)));
        if (!isdigit(header_char) && !isspace(header_char)) {
            error("Invalid header '%s'", header);
            exit(EXIT_FAILURE);
        }

        spaces += isspace(header_char);
    }    

    if (spaces < 1) {
        error("Invalid header '%s'", header);
        exit(EXIT_FAILURE);
    }
}

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
void header_parse(char * header, int * rows, int * columns) {
    int pos, i;
    char * buffer = (char * )malloc(sizeof(char) * BUFSIZ);
    int len = strlen(header);
    pos = 0;
    for (i = 0; i < len; i++) {
        char header_char = *(header + (i * sizeof(char)));

        if (!isdigit(header_char)) {
            *rows = atoi(buffer);
            pos =  -1;
            buffer[0] = '\0';
        }
        else {
            buffer[pos] = header_char;
            buffer[pos + 1] = '\0';
        }

        pos++;
    }    
    *columns = atoi(buffer);

    debug("Parsed rows %d", *rows);
    debug("Parsed column %d", *columns);
    free(buffer);
}

Image * image_new(void) {
    return (Image *)malloc (sizeof(Image));
}

Pixel * pixel_new(void) {
    Pixel * retval = (Pixel *) malloc(sizeof(Pixel));
    retval->up = NULL;
    retval->down = NULL;
    retval->left = NULL;
    retval->right = NULL;
    return retval;
}

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
Pixel * pixel_init(Image * image, int row, int col, int color) {
    Pixel * retval = pixel_new();
    retval->color = color;

    debug("Row, Col %d, %d", row, col);

    if (image->pixels == NULL || (row == 0 && col == 0)) {
        image->pixels = retval;
        return retval;
    }

    Pixel * pos = image->pixels;
    Pixel * last = pos;
    debug("Last %u", last);
    int cr;
    for (cr = 0; cr < row && pos != NULL; cr++) {
        last = pos;
        pos = pos->down;
    }
    debug("Last %u", last);
    
    if (pos == NULL) {
        pos = last;
    }

    int cc;
    for (cc = 0; cc < col && pos != NULL; cc++) {
        last = pos;
        pos = pos->right;
    }
    
    pos = last;

    if (col == 0) {
        if (pos != NULL) {
            pos->down = retval;
        }
        
        retval->up = pos;
        // debug("setting up to %u for %u", retval->up, retval);
    }
    else {
        pos->right = retval;
        retval->left = pos;
        debug("Setting right to %u for %u", pos->right, pos);
        debug("Color is %c", pos->color);
        
    
        if (row != 0) {
            if (pos->up != NULL) {
                retval->up = pos->up->right;
                debug("Setting up to %u for %u", retval->up, retval);
                pos->up->right->down = retval;
            }
        }
    }

    if (retval->right != NULL) {
        retval->right = NULL;
    }
    
    return retval;
}

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
Image * image_init(char ** pixel_chars, size_t height, size_t width) {
    Image * retval = image_new();
    retval->height = height;
    retval->width  = width;
    unsigned int rowsize = sizeof(char) * (width + 1);
    unsigned int row_mem_size = sizeof(char) * (width + 1);
    unsigned int row_actual_size = row_mem_size + 8 - ((row_mem_size + 8) % 16);
    
    debug("Using actual size %u", row_actual_size);

    int row;
    for (row = 0; row < height; row++) {
        int col;
        char * row_data = *(pixel_chars) + (row * row_actual_size);

        for (col = 0; col < width; col++) {
            char c = *(row_data + (col * sizeof(char)));
            Pixel * pixel = pixel_init(retval, row, col, c);            
        }

        debug("pixel alloc %u", retval->pixels);
    } 

    return retval;
}

void pixel_flood_print_right(Pixel * pixel) {
    if (pixel == NULL) {
        printf("\n");
        return;
    }
    
    printf("%c", pixel->color);   
    pixel_flood_print_right(pixel->right);
}

void pixel_flood_print(Pixel * pixel) {
    if (pixel == NULL) {
        return;
    }

    printf("%c", pixel->color);

    pixel_flood_print_right(pixel->right);
    pixel_flood_print(pixel->down);
}

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
void image_print(Image * image) {
    pixel_flood_print(image->pixels);
}

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
void pixel_flood_fill(Pixel * pixel, char from, char to, int * pixels) {
    debug("Flooding pixels");
    debug("Comparing %c to %c", from, pixel->color);
    if (pixel->color != from) {
        return;
    }

    debug("Changing color to %c", to);
    pixel->color = to;
    (*pixels)++;

    if (pixel->up != NULL) {
        pixel_flood_fill(pixel->up, from, to, pixels);
    }
    if (pixel->down != NULL) {
        pixel_flood_fill(pixel->down, from, to, pixels);
    }
    if (pixel->left != NULL) {
        pixel_flood_fill(pixel->left, from, to, pixels);
    }
    if (pixel->right != NULL) {
        pixel_flood_fill(pixel->right, from, to, pixels);
    }

    debug("Total %d", *pixels);
}

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
Image * image_fill(Image * image, int row, int col, char color, int * pixels) {
    printf("\nFilling starting at (%d, %d) with color %c\n\n", row, col, color);

    int i;
    Pixel * pixel = image->pixels;
    for (i = 0; i < row; i++) {
        pixel = pixel->down;
    }

    for (i = 0; i < col; i++) {
        pixel = pixel->right;
    }

    // Already that color
    if (pixel->color == color) {
        return image;
    }

    char from = pixel->color;
    pixel->color = color;
    (*pixels)++;


    if (pixel->up != NULL) {
        pixel_flood_fill(pixel->up, from, color, pixels);
    }
    if (pixel->down != NULL) {
        pixel_flood_fill(pixel->down, from, color, pixels);
    }
    if (pixel->left != NULL) {
        pixel_flood_fill(pixel->left, from, color, pixels);
    }
    if (pixel->right != NULL) {
        pixel_flood_fill(pixel->right, from, color, pixels);
    }

    debug("Total %d", *pixels);
    
    return image;
}

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
int each_triple(int argc, char ** argv, Image * image, Image * (*callback)(Image *, int, int, char, int *)) {
    int retval = 0;
    int i;
    char * arg = *argv;
    int arg_offset = 2;
    int triple_cnt = (argc - 2) / 3;

    for (i = 0; i < triple_cnt; i++) {
        int rows = atoi(chararr_get(argv, (i * 3) + arg_offset));
        int cols = atoi(chararr_get(argv, (i * 3) + arg_offset + 1));
        char color = *chararr_get(argv, (i * 3) + arg_offset + 2);

        if (rows < 0 || rows >= image->height || cols < 0 || cols >= image->width) {
            error("Supplied triple is outside the image range");
        }
        
        image = callback(image, rows, cols, color, &retval);
        image_print(image);
        debug("Got color %c", color);
        debug("%d changed ", retval);
    }

    return retval;
}

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
char * chararr_get(char ** arr, int idx) {
    char * retval;
    char * start = *arr;
    char * str = *arr;
    int i;
    for (i = 0; i < idx; i++) {
        while (str != NULL && *str != '\0') {
            str += sizeof(char);
        }
        str += sizeof(char);
        start = str;  
    }
    retval = malloc(sizeof(char) * strlen(start) + 1);
    retval = strcpy(retval, start);
    return retval;
}

void pixel_flood_free_right(Pixel * pixel) {
    if (pixel == NULL) {
        return;
    }
    pixel_flood_free_right(pixel->right);
    free(pixel);
}


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
void pixel_flood_free(Pixel * pixel) {
    if (pixel == NULL) {
        return;
    }

    pixel_flood_free_right(pixel->right);
    pixel_flood_free(pixel->down);
    free(pixel);
}

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
void image_free(Image * image) {
    Pixel * pixel = image->pixels;
    pixel_flood_free(image->pixels);
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
    fprintf(stderr, "       %s %s\n", progname, " <filename> <row> <column> <color> [row2] [column2] [color2] ...");
    fprintf(stderr, "               filename - path of the file to read image data from.\n");
    fprintf(stderr, "               <row> <column> <color> - a triple argument. All parts must be present to be valid. Optional triples can be added\n\n");
    fprintf(stderr, "EXAMPLE: \n");
    fprintf(stderr, "        ./prog08 image2.txt 0 0 7 0 1 7 0 3 3 0 4 3 0 3 7\n\n");
    fprintf(stderr, "COPYRIGHT: \n");
    fprintf(stderr, "        Copyright 2011 Leo Przybylski\n");
    fprintf(stderr, "        Educational Community License, Version 2.0 (the \"License\"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.osedu.org/licenses/ECL-2.0\n\n");

}
