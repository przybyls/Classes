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
 |   Assignment:  Program #10: “Curses; Worms!”
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
#include "prog10.h"

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
    context = logging_init("prog10");

    if (argc < 3) {
        error("Incorrect number of arguments. Need number of worms and size.");
        usage(*argv);
        exit(EXIT_FAILURE);
    }

    int worm_cnt = atoi(argv[1]);
    int worm_size = atoi(argv[2]);
    Worm worms[worm_cnt];
    
    int rows, cols;
    initscr();                   /* set up ncurses */
    refresh();                /* clear screen */

    getmaxyx(stdscr, rows, cols);
    
    debug("Got dimentions %d,%d", rows, cols);

    debug("Have %d rows ", rows);
    debug("Worm count is %d ", worm_cnt);
    debug("Worm size is %d ", worm_size);

    Grid grid = grid_new(cols, rows);
    time_t now = time(NULL);
    srand(localtime(&now)->tm_sec); // Set the seed for random numbers to the current second of the minute

    int i;
    for (i = 0; i < worm_cnt; i++) {
        info("Printing worm %d", i);
        worms[i] = worm_new(grid, worm_size);
    }

    /*
      while(1) {
        for (i = 0; i < worm_cnt; i++) {
            worm_move(worms[i]);
        }        
        }*/


    // usleep(PAUSE*10);              /* wait for a bit */
    free(grid);                 // Free the grid!!!!
    
    endwin();
    logging_dest(context);
    return EXIT_SUCCESS;
}

Grid grid_new(int height, int width) {
    Grid retval = malloc(sizeof(struct _Grid));
    retval->height = height;
    retval->width = width;

    int i;
    for (i = 0; i < NUMBER_OF_TELEPORTERS; i++) {
        retval->teleporters[i] = teleporter_new(retval);
        move(retval->teleporters[i]->location->y, retval->teleporters[i]->location->x);
        addch('T');
        refresh();
            usleep(PAUSE);
        debug("Added teleporter at %d,%d", retval->teleporters[i]->location->x, retval->teleporters[i]->location->y);
    }
    return retval;
}

Teleporter teleporter_new(Grid grid) {
    Teleporter retval = malloc(sizeof(struct _Teleporter));
    retval->location = location_rand(grid);
    return retval;
}

Location location_rand(Grid grid) {
    debug("Looking up random location");
    int y = 0;
    while (y == 0) {
        y = grid->height * ((float) rand() / (float) RAND_MAX);
    }
    
    int x = 0;
    while (x == 0) {
        x = grid->width * ((float) rand() / (float) RAND_MAX);
    }
    debug("Random location at %d,%d", x, y);

    return location_new(x, y);
}

Location location_new(int x, int y) {
    Location retval = malloc(sizeof(struct _Location));
    retval->x = x;
    retval->y = y;
    return retval;
}

MoveState worm_state_new(void) {
    MoveState retval = malloc(sizeof(struct _MoveState));
    retval->moving = 0;
    retval->moveTo = NULL;
    return retval;
}

Worm worm_new(Grid grid, int size) {
    Worm retval   = malloc(sizeof(struct _Worm));
    retval->grid  = grid;
    retval->size  = size;
    retval->state = worm_state_new();
    
    int i;
    for (i = 0; i < size; i++) {
        segment_new(retval);
    }
    debug("Creating new segment for worm of size %d", size);
    
    return retval;
}

void worm_clear(Worm worm) {
    worm_print_char(worm, ' ');
}

void worm_print_char(Worm worm, char toprint) {
    Node iterator = worm->front;
    while (iterator != NULL) {
        Segment segment = (Segment) iterator->data;
        move(segment->location->y, segment->location->x);
        addch(toprint);
        refresh();
        usleep(PAUSE);
        
        iterator = iterator->next;
    }
}

void worm_print(Worm worm) {
    worm_print_char(worm, '*');
}


Segment segment_new(Worm worm) {
    Segment retval = malloc(sizeof(struct _Segment));
    
    Node iterator = worm->front;
    Node prev = NULL;
    int i = 0;
    int xdirection = -1;
    int ydirection = 0;
    
    
    while (iterator != NULL) {
        Segment last = (Segment) iterator->data;

        if (prev != NULL) {
            Segment prevseg = (Segment) prev->data;
            if (prevseg->location->y > last->location->y) {
                ydirection = -1;
            }
            else {
                ydirection = 1;
            }
        }

        if (iterator->next == NULL) {
            iterator->next = ll_new(retval);

            int remaining = (worm->size - i);

            // Is there enough space?
            if (last->location->x > 1) {
                retval->location = location_new(last->location->x + xdirection, last->location->y);
            }
            else if (last->location->x < remaining + 1) { // Need more room.
                if (ydirection == 0) { // Have no direction
                    if (last->location->y + 1> remaining) {
                        ydirection = -1;
                    }
                    else {
                        ydirection = 1;
                    }
                }
                retval->location = location_new(last->location->x, last->location->y + ydirection);
            }

            debug("Created segment at %d,%d", retval->location->x, retval->location->y);
            move(retval->location->y, retval->location->x);
            addch('*');
            refresh();
            usleep(PAUSE);
            
            return retval;
        }
        
        prev = iterator;
        iterator = iterator->next;
        i++;
    }
    
    if (worm->front == NULL) {
        worm->front = ll_new(retval);
        retval->location = location_rand(worm->grid);
    }

    debug("Created segment at %d,%d", retval->location->x, retval->location->y);
    
    mvaddch(retval->location->y, retval->location->x, '*');
    refresh();
    usleep(PAUSE);

    return retval;
}

void worm_move(Worm worm) {
    int i;

    // If sitting on a teleporter, teleport
    for (i = 0; i < NUMBER_OF_TELEPORTERS; i++) {
        Teleporter teleporter = worm->grid->teleporters[i];
        if (worm_has_segment_at(worm, teleporter->location->x, teleporter->location->y)) {
            // worm_teleport(worm);
            // return;
        }
    }

    // Ok. Not teleporting, so do a real move
    // First determine if we're already moving
    if (worm->state->moving && worm->state->moveTo != NULL) {
        Location moveTo = worm->state->moveTo;

        debug("Moving to %d,%d", moveTo->x, moveTo->y);

        // Should we bother moving at this point?
        if (worm_has_segment_at(worm, moveTo->x, moveTo->y)) {
            
            // We've reached our destination
            free(worm->state->moveTo);
            worm->state->moveTo = NULL;
            worm->state->moving = 0;

            // Try to move again
            worm_move(worm);
        }
        worm->strategy(worm);
    }
    // Ok not moving yet
    else {
        // Determine strategy
        worm->state->moving = 1;

        // Pick a random place to move to
        worm->state->moveTo = location_rand(worm->grid);
        
        // Keep looking for another place to move to until it's a place not on the worm
        while (worm_has_segment_at_x(worm, worm->state->moveTo->x)
               || worm_has_segment_at_y(worm, worm->state->moveTo->y)) {
            worm->state->moveTo = location_rand(worm->grid);
        }

        debug("Moving to %d,%d", worm->state->moveTo->x, worm->state->moveTo->y);

        // Randomly choose a strategy for getting there
        worm->strategy = worm_move_strategy();

        // Determine the direction to move in
        worm->state->xdirection = worm_move_get_x_direction(worm, worm->state->moveTo);
        worm->state->ydirection = worm_move_get_y_direction(worm, worm->state->moveTo);
        
        // worm-strategy(worm->front, worm->state->moveTo);
        worm->strategy = worm_move_diagonal;
        worm->strategy(worm);
    }
}


int worm_move_get_x_direction(Worm worm, Location moveTo) {
    int retval;
    int width = worm->grid->width;

    retval = 1;

    Segment front = (Segment) worm->front->data;
    Location wormAt = front->location;
    
    // Destination is on the right
    if (moveTo->x > wormAt->x) {
        if (((width - moveTo->x) + wormAt->x) < 
            (moveTo->x - wormAt->x)) {
            return -1;
        }
        return 1;
    }
    // Destination is on the left
    else {
        if ((width - wormAt->x) + moveTo->x <
            (wormAt->x - moveTo->y)) {
            return 1;
        }
        return -1;
    }
    return retval;
}

int worm_move_get_y_direction(Worm worm, Location moveTo) {
    int retval;
    int height = worm->grid->height;

    retval = 1;

    Segment front = (Segment) worm->front->data;
    Location wormAt = front->location;
    
    // Destination is on the right
    if (moveTo->y > wormAt->y) {
        if (((height - moveTo->y) + wormAt->y) < 
            (moveTo->y - wormAt->y)) {
            return -1;
        }
        return 1;
    }
    // Destination is on the left
    else {
        if ((height - wormAt->y) + moveTo->y <
            (wormAt->y - moveTo->y)) {
            return 1;
        }
        return -1;
    }
    return retval;
}

MoveStrategy worm_move_strategy(void) {
    if (rand() % 2 == 1) {
        return &worm_move_diagonal;
    }
    else {
        return &worm_move_block;
    }
}

void worm_move_diagonal(void * ptr) {
    Worm worm = (Worm) ptr;
    
    info("Moving Diagonally");

    MoveState state = worm->state;
    Location moveTo = state->moveTo;

    int oldx = 0;
    int oldy = 0;
    Node iterator = worm->front;   
    if (iterator != NULL) {
        Segment segment = (Segment) iterator->data;
        oldx = segment->location->x;
        oldy = segment->location->y;

        // Assuming this even needs to move 
        if (!worm_has_segment_at_x(worm, state->moveTo->x)) {
            // Moving left and at the end
            if (segment->location->x == 1 && state->xdirection < 0) {
                segment->location->x = worm->grid->width;
            }
            // Moving right and at the end
            else if (segment->location->x == worm->grid->width 
                     && state->xdirection > 0) {
                segment->location->x = 1;
            }
            else {
                segment->location->x += state->xdirection;
            }
        }
 
        // Assuming this even needs to move 
        if (!worm_has_segment_at_y(worm, state->moveTo->y)) {
            // Moving up and at the end
            if (segment->location->y == 1 && state->ydirection < 0) {
                segment->location->y = worm->grid->height;
            }
            // Moving right and at the end
            else if (segment->location->y == worm->grid->height
                     && state->ydirection > 0) {
                segment->location->y = 1;
            }
            else {
                segment->location->y += state->ydirection;
            }
        }
        
        debug("Moved head to %d,%d", segment->location->x, segment->location->y);
        move(segment->location->y, segment->location->x);
        addch('*');
        refresh();
        usleep(PAUSE);

        iterator = iterator->next;
    }
    
    
    while (iterator != NULL) {
        Segment segment = (Segment) iterator->data;
                
        if (iterator->next == NULL) {
            debug("Clearing tail at %d,%d", segment->location->x, segment->location->y);
            move(segment->location->y, segment->location->x);
            addch(' ');
            refresh();
            usleep(PAUSE);
        }

        int tempx = segment->location->x;
        int tempy = segment->location->y;

        segment->location->x = oldx;
        segment->location->y = oldy;

        oldx = tempx;
        oldy = tempy;

        debug("Moved to %d,%d", segment->location->x, segment->location->y);
        
        iterator = iterator->next;
    }
}

void worm_move_block(void * ptr) {
}

void worm_teleport(Worm worm) {
}

int worm_height(Worm worm) {
    int retval;
    Node iterator = worm->front;
    int start;
    int end;

    retval = 1;
    
    if (iterator != NULL) {
        Segment segment = (Segment) iterator->data;
        start = segment->location->y;
    }
    
    while (iterator != NULL) {
        Segment segment = (Segment) iterator->data;
        if (iterator->next == NULL) {
            end = segment->location->y;
        }
        iterator = iterator->next;
    }
    
    if (start < end) {
        retval = end - start;
    }
    else if (start > end) {
        start - end;
    }

    return retval;
}

int worm_width(Worm worm) {
    int retval;
    Node iterator = worm->front;
    int start;
    int end;

    retval = 1;
    
    if (iterator != NULL) {
        Segment segment = (Segment) iterator->data;
        start = segment->location->x;
    }
    
    while (iterator != NULL) {
        Segment segment = (Segment) iterator->data;
        if (iterator->next == NULL) {
            end = segment->location->x;
        }
        iterator = iterator->next;
    }
    
    if (start < end) {
        retval = end - start;
    }
    else if (start > end) {
        start - end;
    }

    return retval;
}

int worm_has_segment_at(Worm worm, int x, int y) {
    Node iterator = worm->front;
    while (iterator != NULL) {
        Segment segment = (Segment) iterator->data;
        if (segment->location->x == x && segment->location->y == y) {
            return TRUE;
        }
        iterator = iterator->next;
    }
    return FALSE;
}

int worm_has_segment_at_x(Worm worm, int x) {
    Node iterator = worm->front;
    while (iterator != NULL) {
        Segment segment = (Segment) iterator->data;
        if (segment->location->x == x) {
            return TRUE;
        }
        iterator = iterator->next;
    }
    return FALSE;
}

int worm_has_segment_at_y(Worm worm, int y) {
    Node iterator = worm->front;
    while (iterator != NULL) {
        Segment segment = (Segment) iterator->data;
        if (segment->location->y == y) {
            return TRUE;
        }
        iterator = iterator->next;
    }
    return FALSE;
}

void worm_destroy(Worm);


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

