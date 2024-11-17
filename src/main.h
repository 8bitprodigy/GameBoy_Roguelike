#ifndef MAIN_H
#define MAIN_H

#include </opt/gbdk/include/gb/gb.h>
#include </opt/gbdk/include/stdlib.h>
#include </opt/gbdk/include/stdint.h>
#include </opt/gbdk/include/stdio.h>
#include </opt/gbdk/include/rand.h>
#include </opt/gbdk/include/time.h>

#define screen_width_in_tiles   20
#define screen_height_in_tiles  14
#define screen_h_width_tiles    screen_width_in_tiles/2
#define screen_h_height_tiles   screen_height_in_tiles/2

/* Level Features */
#define abyss                  0
#define floor                  249
#define door                   206
#define corridor               177
#define vertical_wall          186
#define horizontal_wall        205
#define upper_right_corner     187
#define lower_right_corner     188
#define upper_left_corner      201
#define lower_left_corner      200

/* Level Definitions */
#define sector_width             screen_width_in_tiles
#define sector_height            screen_height_in_tiles

/* Level Debug */
/*
#define corridor_debug          37
#define node_debug              43
#define room_debug
#define border_debug            35
#define interior_debug          45
//#define sector_debug
#define sector_dbg_ulc         218
#define sector_dbg_blc         192
#define sector_dbg_brc         217
#define sector_dbg_urc         191
#define debug_plot_lvl_tiles
*/
/* Room Definitions */
#define min_room_width           4
#define min_room_height          4
#define max_room_width           (sector_width  - 2)
#define max_room_height          (sector_height - 2)

#define level_width_in_sectors   3
#define level_height_in_sectors  3
#define max_number_of_sectors    (level_width_in_sectors * level_height_in_sectors)

/* Corridors */
#define num_horizontal_corridors (level_width_in_sectors  - 1)
#define num_vertical_corridors   (level_height_in_sectors - 1)

#define level_width_in_tiles     (sector_width  * level_width_in_sectors )
#define level_height_in_tiles    (sector_height * level_height_in_sectors)

/* Camera Stuff */
#define min_camera_tile_x        0
#define min_camera_tile_y        0
#define max_camera_tile_x        level_width_in_tiles  - screen_width_in_tiles
#define max_camera_tile_y        level_height_in_tiles - screen_height_in_tiles

// Actor tiles
#define player_char              2


#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))


typedef struct {
    uint8_t
        x, y,
        tile,
        dirty_tile,
        type;
} Character;

/*struct Globals {
    uint8_t
        level_number,
        health,
        strength,
        condition,
        money,
        armor,
        experience;
} Globals;*/



#endif// MAIN_H
