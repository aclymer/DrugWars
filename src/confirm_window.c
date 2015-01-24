#include "confirm_window.h"
#include <pebble.h>

static Window *s_window;
static MenuLayer *confirm_menu;

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
	return 2;
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
	return 64;
}

static int16_t menu_get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{
	return 26;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data)
{
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data)
{
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{
}

static void initialise_ui(void)
{
  s_window = window_create();
  window_set_fullscreen(s_window, false);
  
  // confirm_menu
  confirm_menu = menu_layer_create(GRect(0, 0, 144, 152));
	
	// Set all the callbacks for the menu layer
	menu_layer_set_callbacks(confirm_menu, NULL, (MenuLayerCallbacks)
													 {
														 .get_num_rows 			= menu_get_num_rows_callback,
														 .get_header_height = menu_get_header_height_callback,
														 .get_cell_height		= menu_get_cell_height_callback,
														 .draw_header 			= menu_draw_header_callback,
														 .draw_row 					= menu_draw_row_callback,
														 .select_click 			= menu_select_callback,
													 });
	
	// Bind the menu layer's click config provider to the window for interactivity
	menu_layer_set_click_config_onto_window(confirm_menu, s_window);
	
	// Add it to the window for display
	layer_add_child(window_get_root_layer(s_window), menu_layer_get_layer(confirm_menu));
}

static void destroy_ui(void)
{
  window_destroy(s_window);
  menu_layer_destroy(confirm_menu);
}

static void handle_window_unload(Window* window)
{
  destroy_ui();
}

void show_confirm_window(ConfirmWindow *this)
{
  initialise_ui();
  window_set_window_handlers(this->parent, (WindowHandlers) {.unload = handle_window_unload, } );
  window_stack_push(this->parent, true);
}

void hide_confirm_window(void)
{
  window_stack_remove(s_window, true);
}
