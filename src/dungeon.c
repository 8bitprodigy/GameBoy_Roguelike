#include "dungeon.h"
#include "main.h"
#include <stdint.h>



const Rectangle NULL_RECT = {
    .x = NULL,
    .y = NULL,
    .w = NULL,
    .h = NULL,
};

const uint8_t wall_tiles[] = {
    abyss,
    vertical_wall,
    vertical_wall,
    horizontal_wall,
    horizontal_wall,
    upper_left_corner,
    upper_right_corner,
    lower_left_corner,
    lower_right_corner,
};

const uint8_t offset[] = {0,0,0,4,6};
/*
unsigned
char
dungeon_level[ level_width_in_tiles   ][ level_height_in_tiles   ];
*/
Sector
level_sectors[ level_width_in_sectors ][ level_height_in_sectors ];

struct {
    Corridor horizontal_corridors[ level_width_in_sectors ][ level_height_in_sectors ];
    Corridor vertical_corridors[   level_width_in_sectors ][ level_height_in_sectors ];
} Corridors;


inline
uint8_t
rand_range(uint8_t min, uint8_t max) {
    uint8_t difference = max - min;
    return (rand() % difference) + min;
} /* rand_range */

//inline  /* Check if a tile is in a rectangle        */
uint8_t
is_in_rect(uint8_t x, uint8_t y, Rectangle *r) {
    if (r == NULL) return 0;
    return (r->x <= x && x <= r->x+r->w) && (r->y <= y && y <= r->y+r->h);
} /* is_in_rect */


void
plot_lvl_tiles(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    /* For loop through patch, checking for which sector it's in. */
    uint8_t
        i,
        j;
    uint8_t tile;
    for ( i = 0; i < w; i++ ) {
        for ( j = 0; j < h; j++ ) {
            tile = get_lvl_tile_xy(i+x,j+y);
#ifdef debug_plot_lvl_tiles
            set_bkg_tile_xy(x+i,y+j,x+i+y+j);
            if (tile) {
#endif
                set_bkg_tile_xy(x+i,y+j,get_lvl_tile_xy(i+x,j+y));
#ifdef debug_plot_lvl_tiles
            }
#endif
        }
    }
} /* plot_lvl_tiles */


uint8_t
generate_room(uint8_t x, uint8_t y){
    return x+y;
} /* generate_room */

inline
Sector
*get_sector(uint8_t x, uint8_t y) {
    return &level_sectors[x/sector_width][y/sector_height];
} /* get_sector */

/***********************
 *                     *
 *  C O R R I D O R S  *
 *                     *
 ***********************/


void
generate_horizontal_corridor(uint8_t x, uint8_t y) {
    Corridor *c = &Corridors.horizontal_corridors[x][y];
    Position
        *node_left  = &level_sectors[x][y].nodes[EAST].position,
        *node_right = &level_sectors[x][y+1].nodes[WEST].position;

    c->points[START] = *node_left;
    c->points[END]   = *node_right;

    c->bridge =
        rand_range(
            node_left->x+1,
            node_right->x-1
        );

    //generate_h_corridor_voids(node_left->y, node_right->y, c);

#ifdef corridor_debug
    set_bkg_tile_xy(2, 2, corridor_debug);
    uint8_t min = MIN(
        Corridors.horizontal_corridors[x][y].points[START].y,
        Corridors.horizontal_corridors[x][y].points[END].y);
    uint8_t max = MAX(
        Corridors.horizontal_corridors[x][y].points[START].y,
        Corridors.horizontal_corridors[x][y].points[END].y);

    for (uint8_t i =  Corridors.horizontal_corridors[x][y].points[START].x;
         i         <= Corridors.horizontal_corridors[x][y].bridge;
         i++) {
            set_lvl_tile_xy(i, Corridors.horizontal_corridors[x][y].points[START].y, corridor_debug);
    }
    for (
        uint8_t i = MIN(
            Corridors.horizontal_corridors[x][y].points[START].y,
            Corridors.horizontal_corridors[x][y].points[END].y
        );
        i < MAX(
            Corridors.horizontal_corridors[x][y].points[START].y,
            Corridors.horizontal_corridors[x][y].points[END].y
        );
        i++){
            set_lvl_tile_xy(Corridors.horizontal_corridors[x][y].bridge, i, corridor_debug);
    }
    for (uint8_t i =  Corridors.horizontal_corridors[x][y].bridge;
         i         <= Corridors.horizontal_corridors[x][y].points[END].x;
         i++) {
            set_lvl_tile_xy(i, Corridors.horizontal_corridors[x][y].points[END].y,   corridor_debug);
    }
#endif /* corridor_debug */

    level_sectors[x][y].corridors[EAST] = c;
    level_sectors[(x+1)%level_width_in_sectors][(y+1)%level_width_in_sectors].corridors[WEST] = c;
} /* generate_horizontal_corridor */


void
generate_vertical_corridor(uint8_t x, uint8_t y) {
    Corridor *c = &Corridors.vertical_corridors[x][y];
    Position
        *node_top = &level_sectors[x][y].nodes[SOUTH].position,
        *node_bot = &level_sectors[x][y+1].nodes[NORTH].position;

    c->points[START] = *node_top;
    c->points[END]   = *node_bot;

    c->bridge =
        rand_range(
            node_top->y+1,
            node_bot->y-1
        );

    //generate_v_corridor_voids(node_top->x, node_bot->x, c);

#ifdef corridor_debug
    set_bkg_tile_xy(2, 2, corridor);
    uint8_t min = MIN(
        Corridors.horizontal_corridors[x][y].points[START].x,
        Corridors.horizontal_corridors[x][y].points[END].x
    );
    uint8_t max = MAX(
        Corridors.horizontal_corridors[x][y].points[START].x,
        Corridors.horizontal_corridors[x][y].points[END].x
    );

    for (uint8_t i =  Corridors.horizontal_corridors[x][y].points[START].y;
         i         <= Corridors.horizontal_corridors[x][y].bridge;
         i++) {
            set_lvl_tile_xy(Corridors.horizontal_corridors[x][y].points[START].x, i, corridor_debug);
    }
    for (uint8_t i = min; i < max; i++) {
            set_lvl_tile_xy(i, Corridors.horizontal_corridors[x][y].bridge, corridor_debug);
    }
    for (uint8_t i =  Corridors.horizontal_corridors[x][y].bridge;
         i         <= Corridors.horizontal_corridors[x][y].points[END].y;
         i++) {
            set_lvl_tile_xy(Corridors.horizontal_corridors[x][y].points[END].x,  i,  corridor_debug);
    }
#endif /* corridor_debug */

    level_sectors[x][y].corridors[SOUTH] = c;
    level_sectors[(x+1)%level_width_in_sectors][(y+1)%level_width_in_sectors].corridors[NORTH] = c;
} /* generate_horizontal_corridor */

void
generate_corridors(void) {
    uint8_t
        x,
        y;
    for ( x = 0; x < level_width_in_sectors-1; x++ ) {
        for ( y = 0; y < level_height_in_sectors; y++ ) {
            generate_horizontal_corridor(x, y);
        }
    }
    for ( x = 0; x < level_width_in_sectors; x++ ) {
        for ( y = 0; y < level_height_in_sectors-1; y++ ) {
            generate_vertical_corridor(x, y);
        }
    }
} /* generate_corridors */


/*****************************
 *                           *
 *  T I L E   P I C K I N G  *
 *                           *
 *****************************/


uint8_t
is_in_h_corridor(uint8_t x, uint8_t y, Corridor *c) {
    if (!c) return abyss;
    uint8_t start_y = MIN(c->points[START].y, c->points[END].y);
    uint8_t end_y   = MAX(c->points[START].y, c->points[END].y);

    if (((y == c->points[START].y) && ((c->points[START].x <= x) && (x <= c->bridge))) ||
        ((x == c->bridge)          && ((start_y            <= y) && (y <= end_y)))     ||
        ((y == c->points[END].y)   && ((c->bridge          <= x) && (x <= c->points[END].x))))
    {
        return corridor;
    }
    return abyss;
} /* is_in_h_corridor */


uint8_t
is_in_v_corridor(uint8_t x, uint8_t y, Corridor *c) {
    if (!c) return abyss;
    uint8_t start_x = MIN(c->points[START].x, c->points[END].x);
    uint8_t end_x   = MAX(c->points[START].x, c->points[END].x);

    if (((x == c->points[START].x) && ((c->points[START].y <= y) && (y <= c->bridge))) ||
        ((y == c->bridge)          && ((start_x            <= x) && (x <= end_x)))     ||
        ((x == c->points[END].x)   && ((c->bridge          <= y) && (y <= c->points[END].y))))
    {
        return corridor;
    }
    return abyss;
} /* is_in_h_corridor */


uint8_t
get_corridors_tile(uint8_t x, uint8_t y, Sector *sector) {
    uint8_t
        result_x = 0,
        result_y = 0;
    if (x < sector->feature.x) {
        result_x = is_in_h_corridor(x, y, sector->corridors[WEST]);
    }
    else if (sector->feature.x + sector->feature.w < x) {
        result_x = is_in_h_corridor(x, y, sector->corridors[EAST]);
    }
    if (y < sector->feature.y) {
        result_y = is_in_v_corridor(x, y, sector->corridors[NORTH]);
    }
    else if (sector->feature.y + sector->feature.h < y) {
        result_y = is_in_v_corridor(x, y, sector->corridors[SOUTH]);
    }
    if (result_x || result_y) return corridor;
    return abyss;
} /* get_corridor_tile */


uint8_t
get_door_tile(uint8_t x, uint8_t y, Sector *sector) {
    uint8_t i;
    for (i = 0; i < 4; i++) {
        if (sector->corridors[i] == NULL) continue;
        if (x == sector->nodes[i].position.x && y == sector->nodes[i].position.y) {
            return door;
        }
    }
    return abyss;
} /* get_door_tile */


uint8_t
get_wall_tile(uint8_t x, uint8_t y, Sector *sector) {
    uint8_t node = get_door_tile(x,y,sector);
    if (node) return node;

    Rectangle *r           = &sector->feature;
    uint8_t   wall_tile_x  = 0;
    uint8_t   wall_tile_y  = 0;

    if      (x == r->x)      wall_tile_x = 1; /* left   */
    else if (x == r->x+r->w) wall_tile_x = 2; /* right  */
    if      (y == r->y)      wall_tile_y = 3; /* top    */
    else if (y == r->y+r->h) wall_tile_y = 4; /* bottom */

    if (!(wall_tile_x && wall_tile_y)) return wall_tiles[ wall_tile_x + wall_tile_y ];
    return wall_tiles[ wall_tile_x + offset[wall_tile_y] ];
} /* get_wall_tile */


uint8_t
get_room_tile(uint8_t x, uint8_t y, Sector *sector) {
    Rectangle *r = &sector->feature;
    Rectangle hall;
    hall.x=r->x+1;
    hall.y=r->y+1;
    hall.w=r->w-2;
    hall.h=r->h-2;
    if (is_in_rect(x,y,&hall)) return floor;
    return get_wall_tile(x, y, sector);
} /* get_room_tile */

uint8_t
get_feature_tile(uint8_t x, uint8_t y, Sector *sector) {
    switch (sector->feature_type){
    case ROOM:
        return get_room_tile(x,y,sector);
    case INTERSECTION:
        //return get_intersection_tile(x,y,sector);
    case MAZE:
        //return get_maze_tile(x,y,sector);
        break;
    }
    return abyss;
} /* get_feature_tile */


uint8_t
get_sector_tile(uint8_t x, uint8_t y, Sector *sector) {
    if (is_in_rect(x, y, &sector->feature)) {
        return get_feature_tile(x,y,sector);
    } else {
        return get_corridors_tile(x,y,sector);
    }
    return abyss;
} /* get_sector_tile */


/*
 * Get Level Tile (x,y)
 * --------------------
 *
 * Get the level tile at the (x,y) position.
 */
uint8_t
get_lvl_tile_xy(uint8_t x, uint8_t y) {
    return get_sector_tile(x, y, get_sector(x,y));
} /* get_lvl_tile */


/*******************
 *                 *
 *  S E C T O R S  *
 *                 *
 *******************/

/*
 * Generate Sector
 * ---------------
 *
 * Generate sector, its type, its features, and its nodes.
 */
uint8_t
generate_sector(uint8_t x, uint8_t y) {
    if ( (x >= level_width_in_sectors) || (y >= level_height_in_sectors) ) {
        printf("Error!\nSector index out of bounds!\n");
        return 0;
    }
    uint8_t sector_x_offset = x * sector_width;
    uint8_t sector_y_offset = y * sector_height;
    uint8_t
        i,
        j;

#ifdef sector_debug
    for ( i = 0; i < sector_width; i++ ) {
        for ( j = 0; j < sector_height; j++ ) {
            if      ( (i == 0)                && (j == 0)                 ) {
                set_lvl_tile_xy(sector_x_offset,     sector_y_offset, sector_dbg_ulc);     }
            else if ( (i == sector_width - 1) && (j == 0)                 ) {
                set_lvl_tile_xy(sector_x_offset + i, sector_y_offset, sector_dbg_urc);     }
            else if ( (i == 0)                && (j == sector_height - 1) ) {
                set_lvl_tile_xy(sector_x_offset,     sector_y_offset + j, sector_dbg_blc); }
            else if ( (i == sector_width - 1) && (j == sector_height - 1) ) {
                set_lvl_tile_xy(sector_x_offset + i, sector_y_offset + j, sector_dbg_brc); }
            else    set_lvl_tile_xy(sector_x_offset+i, sector_y_offset+j, 0);//3+((x+1)*(y+1))+x);
        }
    }
#endif /* sector_debug */

    /* Initialize the sector's feature's width and height. */
    level_sectors[x][y].feature.w = rand_range(min_room_width, max_room_width);
    level_sectors[x][y].feature.h = rand_range(min_room_height, max_room_height);
    /* Initialize the feature's position within the sector */
    level_sectors[x][y].feature.x = rand_range(
        sector_x_offset + 1,
        sector_x_offset + max_room_width - level_sectors[x][y].feature.w - 1
    );
    level_sectors[x][y].feature.y = rand_range(
        sector_y_offset + 1,
        sector_y_offset + max_room_height - level_sectors[x][y].feature.h - 1
    );
    level_sectors[x][y].feature_type = ROOM;

#ifdef room_debug
    for (
        i = level_sectors[x][y].feature.x;
        i < level_sectors[x][y].feature.x + level_sectors[x][y].feature.w;
        i++
    ) {
        for (
            j = level_sectors[x][y].feature.y;
            j < level_sectors[x][y].feature.y + level_sectors[x][y].feature.h;
            j++
        ) {
            if (
                i == level_sectors[x][y].feature.x ||
                i == level_sectors[x][y].feature.x + level_sectors[x][y].feature.w - 1 ||
                j == level_sectors[x][y].feature.y ||
                j == level_sectors[x][y].feature.y + level_sectors[x][y].feature.h - 1
            ) {
                set_lvl_tile_xy(i, j, border_debug);
            } else {
                set_lvl_tile_xy(i, j, interior_debug);
            }
        }
    }
#endif /* room_debug */

    /* set up nodes for each side of the sector feature. */
    level_sectors[x][y].nodes[EAST].position.x = level_sectors[x][y].feature.x + level_sectors[x][y].feature.w;
    level_sectors[x][y].nodes[EAST].position.y = rand_range(
        level_sectors[x][y].feature.y + 1,
        level_sectors[x][y].feature.y + level_sectors[x][y].feature.h - 1
    );
    level_sectors[x][y].nodes[NORTH].position.y = level_sectors[x][y].feature.y;
    level_sectors[x][y].nodes[NORTH].position.x = rand_range(
        level_sectors[x][y].feature.x + 1,
        level_sectors[x][y].feature.x + level_sectors[x][y].feature.w - 1
    );
    level_sectors[x][y].nodes[WEST].position.x = level_sectors[x][y].feature.x;
    level_sectors[x][y].nodes[WEST].position.y = rand_range(
        level_sectors[x][y].feature.y + 1,
        level_sectors[x][y].feature.y + level_sectors[x][y].feature.h - 1
    );
    level_sectors[x][y].nodes[SOUTH].position.y = level_sectors[x][y].feature.y + level_sectors[x][y].feature.h;
    level_sectors[x][y].nodes[SOUTH].position.x = rand_range(
        level_sectors[x][y].feature.x + 1,
        level_sectors[x][y].feature.x + level_sectors[x][y].feature.w - 1
    );

#ifdef node_debug
    set_lvl_tile_xy(
        level_sectors[x][y].nodes[EAST].position.x,
        level_sectors[x][y].nodes[EAST].position.y,
        node_debug
    );
    set_lvl_tile_xy(
        level_sectors[x][y].nodes[NORTH].position.x,
        level_sectors[x][y].nodes[NORTH].position.y,
        node_debug
    );
    set_lvl_tile_xy(
        level_sectors[x][y].nodes[WEST].position.x,
        level_sectors[x][y].nodes[WEST].position.y,
        node_debug
    );
    set_lvl_tile_xy(
        level_sectors[x][y].nodes[SOUTH].position.x,
        level_sectors[x][y].nodes[SOUTH].position.y,
        node_debug
    );
#endif /* node_debug */

    return 1;
} /* generate_sector */


void generate_sectors(void) {
    uint8_t
    x,
    y;
    for ( x = 0; x < level_width_in_sectors; x++ ) {
        for ( y = 0; y < level_height_in_sectors; y++ ) {
            generate_sector(x, y);
        }
    }
} /* generate_sectors */


void generate_level(void) {
    initrand(clock());
    generate_sectors();
    generate_corridors();
    //return 1;
} /* generate_level */
