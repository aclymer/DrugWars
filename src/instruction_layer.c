#include "instruction_layer.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *instruction_window;
static ScrollLayer *instruction_layer;
static TextLayer *instruction_text_layer;

static void initialise_ui(void) {
  instruction_window = window_create();
  window_set_background_color(instruction_window, GColorClear);
  window_set_fullscreen(instruction_window, false);
  
	// scroll layer
	instruction_layer = scroll_layer_create(GRect(0, 18, 144, 150));
	layer_add_child(window_get_root_layer(instruction_window), (Layer*)instruction_layer);
	
  // instruction_text_layer
  instruction_text_layer = text_layer_create(GRect(0, 18, 144, 150));
  text_layer_set_text(instruction_text_layer, "Text layer");
  scroll_layer_add_child(instruction_layer, (Layer *)instruction_text_layer);
}

static void destroy_ui(void)
{
  window_destroy(instruction_window);
	scroll_layer_destroy(instruction_layer);
  text_layer_destroy(instruction_text_layer);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_instruction_layer(MenuIndex *cell_index)
{
  initialise_ui();
  window_set_window_handlers(instruction_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(instruction_window, true);
}

void hide_instruction_layer(MenuIndex *data)
{
  window_stack_remove(instruction_window, true);
}
