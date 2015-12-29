#include <pebble.h>
#include "instructions_layer.h"

static Window 								    *s_window;
static ScrollLayer 						    *s_scroll_layer;
static TextLayer 							    *s_text_layer;

const char* instructions_text = "  THIS IS A GAME OF BUYING AND SELLING.\n\n  YOUR GOAL IS TO PAY OFF YOUR DEBT TO THE LOAN SHARK, AND THEN MAKE AS MUCH MONEY AS POSSIBLE.\n\n  ADD FUNDS TO YOUR BANK ACCOUNT TO EARN INTEREST AND KEEP IT SAFE FROM MUGGERS.\n\n  WATCH-OUT FOR THE POLICE IF YOU DEAL TOO HEAVILY!\n\nPRICE RANGES:\n\n COCAINE: 15k-28k\n HEROINE:   5k-12k\n ACID:         1k-4.2k\n WEED:       300-720\n SPEED:       70-220\n LUDES:         10-50";

static void initialise_ui(void)
{
	s_window 									      = window_create();
	Layer *window_layer							= window_get_root_layer(s_window);
	GRect bounds 								    = layer_get_bounds(window_layer);
  GRect max_text_bounds           = GRect(0, 0, bounds.size.w, 2000);
	s_text_layer 								    = text_layer_create(max_text_bounds);
	s_scroll_layer 								  = scroll_layer_create(bounds);

	text_layer_set_text							(s_text_layer, instructions_text);
	text_layer_set_text_alignment   (s_text_layer, GTextAlignmentCenter);
  scroll_layer_set_click_config_onto_window	(s_scroll_layer, s_window);
	scroll_layer_add_child          (s_scroll_layer, text_layer_get_layer(s_text_layer));
	scroll_layer_set_content_size   (s_scroll_layer, text_layer_get_content_size(s_text_layer));
	layer_add_child								  (window_layer, scroll_layer_get_layer(s_scroll_layer));
	
#ifdef PBL_ROUND
	scroll_layer_set_paging					(s_scroll_layer, true);
#endif
}

static void destroy_ui(void)
{
	window_destroy								  (s_window);
}

static void handle_window_unload(Window* window)
{
	destroy_ui();
}

void show_instructions_layer(MenuIndex *cell_index)
{
  initialise_ui();
	window_set_window_handlers			(s_window, (WindowHandlers) {.unload = handle_window_unload,});
	window_stack_push							  (s_window, true);
}

void hide_instructions_layer(void)
{
	window_stack_remove							(s_window, true);
}
