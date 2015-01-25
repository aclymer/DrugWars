/**
 * ToastLayer for Pebble
 *
 * Author: Chris Lewis
 *
 * Instructions:
 * 1. Create with message
 * 2. Show when ready
 * 3. Destroy when parent Window is destroyed
 */

#include <pebble.h>

#ifndef TOAST_LAYER_H
#define TOAST_LAYER_H

#define TOAST_LAYER_MARGIN        2
#define TOAST_LAYER_ANIM_DURATION 300

typedef struct {
  int duration;
  bool is_visible;
  GSize size;
  GRect parent_bounds;
	AppTimer *toast_timer;
  Window *parent;

  TextLayer *bg_layer;
  TextLayer *content_layer;

  char *content_buffer;
} ToastLayer;

/**
 * Create a ToastLayer and add it to the parent Window
 */
ToastLayer* toast_layer_create(Window *);

/**
 * Destroy a ToastLayer
 */
void toast_layer_destroy(ToastLayer *);

/**
 * Pop up the ToastLayer
 */
void toast_layer_show(ToastLayer *, char *, int, int);

/**
 * Hide the ToastLayer
 */
void toast_layer_hide(ToastLayer *);

/**
 * Determine if the ToastLayer is still visible
 */
bool toast_layer_is_visible(ToastLayer *);

void toast_layer_show_interface(ToastLayer *, char *, int, int);

#endif

