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
 |   Assignment:  Program #9 PCX Image Files
 |
 |       Author:  Leo Przybylski
 |     Language:  
 |   To Compile:  gcc -DPROGRAM_LOG_LEVEL=0 logging.c prog09.c -o prog09
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
#include "prog09.h"

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
    context = logging_init("prog09");
    char * filename;
    ImageFile textfile;
    ImageFile binaryfile;
    char * prefix;
    char * bfilename;
    
    if (argc < 1) {
        error("Incorrect number of arguments. Need a file name.");
        usage(*argv);
        exit(EXIT_FAILURE);
    }

    filename = strdup(argv[1]); // Free this
    debug("Got file %s", filename);

    textfile = image_file_new(filename); // Free this
    image_file_open(textfile, "r", image_file_error);
    image_file_close(textfile, image_file_error);        
    
    debug("text file %s", textfile->filename);
    
    prefix = prefix_get(filename); // Free this
    debug("Prefix is %s", prefix);

    AsciiImage source = image_file_read_ascii(textfile);
    PcxImage dest = pcx_image_new(source->height, source->width);
    image_file_free(textfile);
    
    pcx_image_init(dest, source);
    
    bfilename = malloc(sizeof(char) * (strlen(PCX_EXT) + strlen(prefix) + 1)); // Free this
    bfilename[0] = '\0';
    strcat(bfilename, prefix);
    strcat(bfilename, PCX_EXT);
    
    binaryfile = image_file_new(bfilename);    // Free this
    pcx_image_save(dest, binaryfile);
    image_file_free(binaryfile);

    free(bfilename);
    free(prefix);
    free(filename);
    logging_dest(context);
    return EXIT_SUCCESS;
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

char a2pcx(char achar) {
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Start of Generic Pixel and  Image Functions                                //
////////////////////////////////////////////////////////////////////////////////

Pixel pixel_new(void) {
    Pixel retval = malloc(sizeof(struct PIXEL));
    retval->up = NULL;
    retval->down = NULL;
    retval->left = NULL;
    retval->right = NULL;
    return retval;
}

Pixel ascii_pixel_init(AsciiImage image, int x, int y, char color) {
    Pixel retval = pixel_new();
    retval->color = color;

    if (x == 0 && y == 0) {
        image->pixels = retval;
        return retval;
    }

    int row;
    Pixel current = image->pixels;
    Pixel up = NULL;
    for (row = 0; row < y - 1 && current->down != NULL; row++) {
        current = current->down;
        
        if (row == y - 2) {
            up = current;
            int col;
            for (col = 0; col < x && up != NULL; col++) {
                up = up->right;
            }            
        }
    }

    int col;
    for (col = 0; col < x - 1 && current->right != NULL; col++) {
        current = current->right;
    }
    
    if (col == 0) {
        current->down = retval;
        retval->up = current;
        return retval;
    }

    up->down = retval;
    current->right = retval;
    retval->left = current;
    retval->up = up;

    return retval; 
}

AsciiImage image_file_read_ascii(ImageFile file) {
    char * header;
    int rows;
    int columns;
    AsciiImage retval;

    header = malloc(sizeof(char) * LINE_SIZE_LIMIT);
    image_file_open(file, "r", image_file_error);
    header = fgets(header, LINE_SIZE_LIMIT, file->handle);
    trim(header);
    header_parse(header, &rows, &columns);
    
    retval = ascii_image_new(rows, columns);

    debug("New ascii image with %d columns", columns);
    debug("Row count is %d", rows);

    int row;
    for (row = 0; row < rows; row++) {
        char * row_data = malloc(sizeof(char) * LINE_SIZE_LIMIT);
        row_data = fgets(row_data, LINE_SIZE_LIMIT, file->handle);
        trim(row_data);
        ascii_image_add_char_row(retval, row_data);
        free(row_data);
    }

    debug("Done populating ascii image");
    
    free(header);

    return retval;
}

PcxImage image_file_read_pcx(ImageFile file);


Pixel pixel_row_a2b(Pixel tochange) {
    info("Converting pixel from ASCII to binary");
    Pixel retval = pixel_new();
    retval->color = tochange->color;
    return retval;
}

void * image_file_error(int errcode, char * message) {
    if (errcode == ENOENT) {
        error("(%d) No such file or directory: %s", errcode, message);
    }
    else if (errcode == EACCES) {
        error("(%d) Permission denied: %s", errcode, message);
    }
    else if (errcode == EINVAL) {
        return;
    }
    else {
        error("(%d) Unable to do something", errcode, message);
    }
    
    exit(EXIT_FAILURE);

    return NULL;
}

ImageFile image_file_new(char * filename) {
    ImageFile retval = malloc(sizeof(struct _IMG_FILE));
    retval->filename = filename;
    retval->handle   = NULL;
    return retval;
}

void image_file_free(ImageFile file) {
    image_file_close(file, image_file_error);
    free(file);
}

ImageFile image_file_open(ImageFile file, char * mode, ErrorCallback callback) {
    debug("Trying to open %s", file->filename);
    file->handle = fopen(file->filename, mode);
    if (errno) {
        callback(errno, file->filename);
    }
}

void image_file_close(ImageFile file, ErrorCallback callback) {
    debug("Closing %s", file->filename);
    if (file != NULL && file->handle != NULL) {
        fclose(file->handle);
        file->handle = NULL;
    }

    if (errno) {
        callback(errno, file->filename);
    }
}

void pixel_flood_free_right(Pixel pixel) {
    if (pixel == NULL) {
        return;
    }
    pixel_flood_free_right(pixel->right);
    free(pixel);
}


/*------------------------------------------------- pixel_flood_free -----
  |  Function pixel_flood_free
  |
  |  Purpose:  Frees allocation of a Pixel instance
  |       
  |
  |  Parameters: pixel (IN) -- Pixel instance
  |                
  |
  |  Returns:  Nuttin
  *-------------------------------------------------------------------*/
void pixel_flood_free(Pixel pixel) {
    if (pixel == NULL) {
        return;
    }

    pixel_flood_free_right(pixel->right);
    pixel_flood_free(pixel->down);
    free(pixel);
}

void pixel_flood_print_right(Pixel pixel) {
    if (pixel == NULL) {
        printf("\n");
        return;
    }
    
    printf("%c", pixel->color);   
    pixel_flood_print_right(pixel->right);
}

void pixel_flood_print(Pixel pixel) {
    debug("Flood printing");
    if (pixel == NULL) {
        return;
    }

    printf("%c", pixel->color);

    pixel_flood_print_right(pixel->right);
    pixel_flood_print(pixel->down);
}

void image_add_row(Image image, Pixel start) {
    /*
    if (image == NULL) {
        debug("image is NULL");
    }
    // Pixel row = image->pixels;
    while (row->down != NULL) {
        row = row->down;
    }
    row->down = start;
    */
}


////////////////////////////////////////////////////////////////////////////////
// End of Generic Pixel and  Image Functions                                  //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Start of ASCII Image Functions                                             //
////////////////////////////////////////////////////////////////////////////////
AsciiImage ascii_image_new(int rows, int columns) {
    AsciiImage retval = malloc(sizeof(struct _ASCII_IMG));
    retval->height = rows;
    retval->width = columns;
    return retval;
}

void ascii_image_add_char_row(AsciiImage image, char * row) {
    debug("Got a row of size %d", strlen(row));
    debug("Image width is %d", image->width);
    if (strlen(row) < image->width) {
        debug("Returning");
        return;
    }

    Pixel start;
    Pixel last = image->pixels;
    while (last != NULL && last->down != NULL) {
        last = last->down;
    }
    
    Pixel up = last;
    Pixel current = NULL;
    int cidx;
    for (cidx = 0; cidx < strlen(row); cidx++) {        
        Pixel pixel = pixel_new();
        pixel->color = row[cidx];

        if (up != NULL) {
            up->down = pixel;
            pixel->up = up;
        }

        if (current == NULL) {
            current = pixel;
            start = current;
        }
        else {
            current->right = pixel;
            current = current->right;
        }
    }
    
    if (last != NULL) {
        last->down = start;
    }
    else {
        image->pixels = start;
    }

}

void ascii_image_free(AsciiImage image) {
    pixel_flood_free(image->pixels);
    free(image);
}

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
void ascii_image_print(AsciiImage image) {
    debug("Printing image");
    if (image->pixels == NULL) {
        debug("NULL image");
    }
    pixel_flood_print(image->pixels);
}

////////////////////////////////////////////////////////////////////////////////
// End of ASCII Image Functions                                               //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Start of PCX Image Functions                                               //
////////////////////////////////////////////////////////////////////////////////
void pcx_image_init(PcxImage dest, AsciiImage src) {
    info("Initializing PCX");
    Pixel row = src->pixels;
    while (row != NULL) {
        pcx_image_add_ascii_row(dest, row);
        row = row->down;         
    }
}

void pcx_image_add_ascii_row(PcxImage image, Pixel start) {
    Pixel new_start = pixel_row_a2b(start);
    image_add_row((Image) image, new_start);
}

void pcx_image_free(PcxImage image) {
    pixel_flood_free(image->pixels);
    free(image);
}

PcxImage pcx_image_new(int height, int width) {
    debug("New pcx image");
    PcxImage retval = malloc(sizeof(struct _PCX));
    retval->height = height;
    retval->width = width;
    return retval;
}


void pcx_image_save(PcxImage image, ImageFile file) {
    PCXHEAD * header = malloc(sizeof(PCXHEAD));
    header->Identifier = 0x0A;
    header->Version = 3;
    header->Encoding = 1;
    header->BitsPerPixel = 8;
    header->XStart = 0;
    header->YStart = 0;
    header->XEnd = image->width;
    header->YEnd = image->height;
    header->HorzRes = 96;
    header->VertRes = 96;
    header->Reserved1 = 0;
    header->BytesPerLine = image->width;
    header->PaletteType = 2;

    image_file_open(file, "wb", image_file_error);
    fwrite(header, sizeof(PCXHEAD), 1, file->handle);

    image_file_close(file, image_file_error);
}

////////////////////////////////////////////////////////////////////////////////
// End of PCX Image Functions                                                 //
////////////////////////////////////////////////////////////////////////////////


char * prefix_get(char * filename) {    
    int i;
    for (i = strlen(filename) - 1; i >= 0; i--) {
        if (filename[i] == '.') {
            return strndup(filename, i);
        }
    }
    return filename;
}

void trim(char * str) {
    int i;
    for (i = 1; i < strlen(str);i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
        }
    }
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

/*!
 * Prints the header portion of the log output containing the log level and date/time of the entry
 *
 * @param log_level int value for the log level. 
 * @return string value of the log header
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
