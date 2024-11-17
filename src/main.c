#include "main.h"
#include "../res/ibm_bios.h"
#include "dungeon.h"

Character player;
uint8_t   camera_tile_x, camera_tile_y;


void init_gfx(void) {
    init_bkg(0);
    // Load Background tiles and then map
    set_bkg_data(0, 255, ibm_bios_tiles);
    plot_lvl_tiles(0, 0, screen_width_in_tiles+1, screen_height_in_tiles+1);
	// Turn the background map on to make it visible
    SHOW_BKG;
    move_win(0,screen_height_in_tiles*8);
    SHOW_WIN;
}


void handle_scrolling(Character * character, int8_t x, int8_t y) {
    // Check for player movement
    uint8_t old_cam_tile_x = camera_tile_x;
    uint8_t old_cam_tile_y = camera_tile_y;
    int8_t camera_scroll_x = 0, camera_scroll_y = 0;
    if ( screen_h_width_tiles-1  <= character->x && character->x <= (level_width_in_tiles  - screen_h_width_tiles) ) {
        if ( x < 0 ) { camera_scroll_x = (character->x - screen_h_width_tiles    ) - camera_tile_x; }
        if ( 0 < x ) { camera_scroll_x = (character->x - screen_h_width_tiles + 1) - camera_tile_x; }
    }
    if ( screen_h_height_tiles-1 <= character->y && character->y <= (level_height_in_tiles - screen_h_height_tiles) ) {
        if ( y < 0 ) { camera_scroll_y = (character->y - screen_h_height_tiles    ) - camera_tile_y; }
        if ( 0 < y ) { camera_scroll_y = (character->y - screen_h_height_tiles + 1) - camera_tile_y; }
    }
    // Limit camera movement
    int8_t test_x = camera_tile_x + camera_scroll_x;
    int8_t test_y = camera_tile_y + camera_scroll_y;
    if ( min_camera_tile_x <= test_x && test_x < max_camera_tile_x+1 ) {
        camera_tile_x += camera_scroll_x;
    }
    if ( min_camera_tile_y <= test_y && test_y < max_camera_tile_y+1 ) {
        camera_tile_y += camera_scroll_y;
    }
    // Update the screen page
    if ( 0 < camera_scroll_x ) {
        plot_lvl_tiles(camera_tile_x+screen_width_in_tiles-camera_scroll_x, camera_tile_y, camera_scroll_x, screen_height_in_tiles);
    }
    if ( camera_scroll_x < 0 ) {
        plot_lvl_tiles(camera_tile_x, camera_tile_y, abs(camera_scroll_x), screen_height_in_tiles);
    }
    if ( 0 < camera_scroll_y ) {
        plot_lvl_tiles(camera_tile_x, camera_tile_y+screen_height_in_tiles-camera_scroll_y, screen_width_in_tiles, camera_scroll_y);
    }
    if ( camera_scroll_y < 0 ) {
        plot_lvl_tiles(camera_tile_x, camera_tile_y, screen_width_in_tiles, abs(camera_scroll_y));
    }
    // Move camera
    move_bkg(camera_tile_x << 3, camera_tile_y << 3);
}


void init_character(Character * character, uint8_t x, uint8_t y, uint8_t tile) {
    character->x          = x;
    character->y          = y;
    character->tile       = tile;
    character->dirty_tile = get_lvl_tile_xy(x,y);
    set_bkg_tile_xy(x,y,tile);
}


void move_character(Character * character, uint8_t x, uint8_t y) {
    set_bkg_tile_xy(character->x, character->y, character->dirty_tile);
    uint8_t new_x = character->x + x;
    uint8_t new_y = character->y + y;
    if ( 0 < new_x && new_x < level_width_in_tiles-1  ) { character->x = new_x; }
    if ( 0 < new_y && new_y < level_height_in_tiles-1 ) { character->y = new_y; }
    character->dirty_tile = get_lvl_tile_xy(character->x, character->y);
    set_bkg_tile_xy(character->x, character->y, character->tile);

}


void update_player(Character * character, uint8_t input) {
    int8_t mov_x = 0;
    int8_t mov_y = 0;
    if ( input & J_UP     ) mov_y--;
    if ( input & J_DOWN   ) mov_y++;
    if ( input & J_LEFT   ) mov_x--;
    if ( input & J_RIGHT  ) mov_x++;
    if ( input & J_A      ) character->tile++;
    if ( input & J_B      ) character->tile--;
    if ( input & J_START  ) character->dirty_tile++;
    if ( input & J_SELECT ) character->dirty_tile--;
    move_character(  character, mov_x, mov_y);
    handle_scrolling(character, mov_x, mov_y);
}


void main(void)
{

    //printf("\n\n\n\n\n\n\n\n");
    //printf("    Press Start.");
    //waitpad(J_START);
    generate_level();
    DISPLAY_OFF;
    SHOW_BKG;
    init_gfx();
    init_character(&player, 1, 1, player_char);

    uint8_t joypad_state      = joypad();
    uint8_t prev_joypad_state = joypad_state;
    uint8_t changed_input     = 0;
    uint8_t new_input         = 0;
    DISPLAY_ON;
    // Loop forever
    while(1) {
        joypad_state = joypad();
		// Game main loop processing goes here
        changed_input = joypad_state  ^ prev_joypad_state;
        new_input     = changed_input & joypad_state;
        //printf("New input: %u\n", new_input);
        if ( changed_input == 0 ) { continue; }
        update_player(&player, new_input);

		// Done processing, yield CPU and wait for start of next frame
        vsync();
        prev_joypad_state = joypad_state;
    }
}
