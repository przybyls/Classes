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
#ifndef _PROG10_H_
#define _PROG10_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#include "logging.h"
#include "linkedlist.h"

// #define false FALSE
// #define true TRUE
#define NUMBER_OF_TELEPORTERS 2
#define PAUSE 200000     /* in microsecs (200000 = 1/5 sec.) */


Log * context;

typedef struct _Location {
    int x;
    int y;
} *Location;

typedef struct _Teleporter {
    Location location;
} *Teleporter;

typedef struct _Grid {
    int height;
    int width;
    
    Teleporter teleporters[NUMBER_OF_TELEPORTERS];
} *Grid;

typedef struct _Segment {
    Location location;
} *Segment;


typedef struct _MoveState {
    int moving;
    Location moveTo;
    int xdirection;
    int ydirection;
} *MoveState;

typedef void (*MoveStrategy)(void *);

typedef struct _Worm {
    int size;
    Grid grid;
    Node front;
    MoveState state;
    MoveStrategy strategy;
} *Worm;



Segment segment_new(Worm);

Teleporter teleporter_new(Grid);

Grid grid_new(int,int);

Location location_rand(Grid);

Location location_new(int,int);

Worm worm_new(Grid, int);

void worm_add(Worm, Segment);

void worm_move(Worm);

MoveStrategy worm_move_strategy(void);

void worm_move_diagonal(void *);

void worm_move_block(void *);

int worm_move_get_y_direction(Worm, Location);
int worm_move_get_x_direction(Worm, Location);

void worm_teleport(Worm);

int worm_height(Worm);

int worm_width(Worm);

void worm_clear(Worm);

void worm_print_char(Worm, char);

void worm_print(Worm);

MoveState worm_state_new(void);

int worm_has_segment_at(Worm, int, int);

int worm_has_segment_at_x(Worm, int);

int worm_has_segment_at_y(Worm, int);

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
void usage(char *);
#endif
