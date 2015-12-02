/*
 ToastLayer for Pebble 
 Author: Chris Lewis
 
 Instructions:
  1. Create with message
  2. Show when ready
  3. Destroy when parent Window is destroyed
 */

#ifndef TOAST_LAYER_H
	#define TOAST_LAYER_H
	#ifdef PBL_PLATFORM_APLITE
		#ifndef GColorLimerick
			#define GColorLimerick GColorBlack
		#else
			#undef GColorLimerick
			#define GColorLimerick GColorBlack
		#endif
		#define GColorRed GColorWhite
	#else
		#define InverterLayer void
		#define inverter_layer_create(...) NULL
		#define inverter_layer_destroy(...)
	#endif
	#define TOAST_LAYER_MARGIN        2
	#define TOAST_LAYER_ANIM_DURATION 400

	typedef struct {
		int duration;
		bool is_visible;
		GSize size;
		GRect parent_bounds;
		Window *parent;
		TextLayer *bg_layer;
		TextLayer *content_layer;
		InverterLayer *burnt_toast_layer;
		char *content_buffer;
	} ToastLayer;

	// Create a ToastLayer and add it to the parent Window
	ToastLayer* toast_layer_create(Window *parent);

	// Destroy a ToastLayer
	void toast_layer_destroy(ToastLayer *this);

	// Pop up the ToastLayer
	void toast_layer_show(ToastLayer *this, char *message, int duration, int offset);

	// Hide the ToastLayer
	void toast_layer_hide(ToastLayer *this);

	// Determine if the ToastLayer is still visible
	bool toast_layer_is_visible(ToastLayer *this);

#endif