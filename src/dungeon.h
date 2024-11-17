#ifndef DUNGEON_H
#define DUNGEON_H

#include "main.h"
#include <stdint.h>


enum
Directions {
    EAST  = 0,
    NORTH = 1,
    WEST  = 2,
    SOUTH = 3
};

enum
Ends {
    START = 0,
    END   = 1
};

enum
Features {
    ROOM         = 0,
    INTERSECTION = 1,
    MAZE         = 2
};

enum
Thresholds {
    EMPTY  = 0,
    DOOR   = 1,
    LOCKED = 2
};

typedef
struct {
    uint8_t
        x, y;
} Position;

typedef
struct {
    uint8_t
        x, y,
        w, h;
} Rectangle;

typedef
struct {
    Position position;
    uint8_t type;
} Threshold;

typedef
struct {
    Position points[2];
    //Rectangle points;
    //Rectangle voids[2];
    uint8_t  bridge;
} Corridor;

typedef
struct {
    Threshold nodes[4];
    Corridor  *corridors[4];
    Rectangle feature; /* the structure in the particular sector, be it a room, maze, intersection, etc. */
    uint8_t  feature_type;
} Sector;


void    plot_lvl_tiles(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void    set_lvl_tile_xy(uint8_t x, uint8_t y, uint8_t tile_index);
uint8_t get_lvl_tile_xy(uint8_t x, uint8_t y);

uint8_t generate_maze(void);
uint8_t generate_intersection(void);
uint8_t populate_room(void);
uint8_t generate_room(uint8_t x, uint8_t y);
uint8_t generate_door(void);
void    generate_horizontal_corridor(uint8_t x, uint8_t y);
void    generate_vertical_corridor(uint8_t x, uint8_t y);
void    generate_corridors(void);
uint8_t generate_sector(uint8_t x, uint8_t y);
void    generate_level(void);


#endif // DUNGEON_H
