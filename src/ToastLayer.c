#include "external_variables.h"
#include "ToastLayer.h"

static void anim_stopped(struct Animation *animation, bool finished, void *context)
{
  property_animation_destroy((PropertyAnimation*)animation);
}

static void animate_layer(Layer *layer, GRect start, GRect finish, int duration, ToastLayer* this)
{
  PropertyAnimation *anim = property_animation_create_layer_frame(layer, &start, &finish);
  animation_set_duration((Animation*)anim, duration);
  animation_set_curve((Animation*)anim, AnimationCurveEaseInOut);
  animation_set_handlers((Animation*)anim, (AnimationHandlers) {
                                              .stopped = anim_stopped
                                            }, NULL);
  animation_schedule((Animation*)anim);
}

void toast_layer_hide(ToastLayer *this)
{
  if(this->is_visible)
  { 
    // Background
    GRect start = layer_get_frame(text_layer_get_layer(this->bg_layer));
    GRect finish = GRect(0, this->parent_bounds.size.h, 144, this->size.h + TOAST_LAYER_MARGIN);
    animate_layer(text_layer_get_layer(this->bg_layer), start, finish, TOAST_LAYER_ANIM_DURATION, this);

    // Content
    start = layer_get_frame(text_layer_get_layer(this->content_layer));
    finish = GRect(TOAST_LAYER_MARGIN, this->parent_bounds.size.h + TOAST_LAYER_MARGIN, 144 - (2 * TOAST_LAYER_MARGIN), 0);
    animate_layer(text_layer_get_layer(this->content_layer), start, finish, TOAST_LAYER_ANIM_DURATION, this);  

    safe_free(this->content_buffer);  
  }
}

ToastLayer* toast_layer_create(Window *parent)
{
  ToastLayer *this     = malloc(sizeof(ToastLayer));
  this->parent         = parent;
  this->parent_bounds  = layer_get_bounds(window_get_root_layer(parent));
  this->content_layer  = text_layer_create(GRect(TOAST_LAYER_MARGIN, this->parent_bounds.size.h + TOAST_LAYER_MARGIN, this->parent_bounds.size.w - (2 * TOAST_LAYER_MARGIN), this->parent_bounds.size.h));
  this->bg_layer       = text_layer_create(GRect(0, this->parent_bounds.size.h, this->parent_bounds.size.w, this->size.h + TOAST_LAYER_MARGIN));
    
  return this;
}

void toast_layer_destroy(ToastLayer *this)
{
  text_layer_destroy(this->bg_layer);
  text_layer_destroy(this->content_layer);
  safe_free(this);
}

static void timer_callback(void *context) {
  ToastLayer *this = (ToastLayer*)context;

  toast_layer_hide(this);
  this->is_visible = false;
}

void toast_layer_show(ToastLayer *this, char *message, int duration, int offset)
{   
  // Set up
  if (!this->is_visible)
  {
    this->is_visible = true;
    this->duration   = duration;
  
    // Allocate buffer
    int length = strlen(message) + 1;
    this->content_buffer = malloc(length * sizeof(char));
    snprintf(this->content_buffer, length * sizeof(char), "%s", message);
  
    // Create large, scale down
    text_layer_set_font(this->content_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
    text_layer_set_text_alignment(this->content_layer, GTextAlignmentCenter);
    text_layer_set_text(this->content_layer, this->content_buffer);
    text_layer_set_background_color(this->content_layer, duration > 3000 ? PBL_IF_COLOR_ELSE(GColorRed, GColorWhite) : PBL_IF_COLOR_ELSE(GColorLimerick, GColorWhite));
  
    // Create offscreen according to size used by TextLayer
    this->size = text_layer_get_content_size(this->content_layer);
    this->size.h = 168 - offset; // Better wrapping
    text_layer_set_background_color(this->bg_layer, GColorBlack);
  
    // Add BG first
    layer_add_child(window_get_root_layer(this->parent), text_layer_get_layer(this->bg_layer));
    layer_add_child(window_get_root_layer(this->parent), text_layer_get_layer(this->content_layer));
  
    // Background
    GRect start = GRect(0, this->parent_bounds.size.h, 144, 0);
    GRect finish = GRect(0, this->parent_bounds.size.h - this->size.h - TOAST_LAYER_MARGIN, 144, this->size.h + (2 * TOAST_LAYER_MARGIN));
    animate_layer(text_layer_get_layer(this->bg_layer), start, finish, TOAST_LAYER_ANIM_DURATION, this);
  
    // Content
    start = GRect(TOAST_LAYER_MARGIN, this->parent_bounds.size.h, 144, 0);
    finish = GRect(TOAST_LAYER_MARGIN, this->parent_bounds.size.h - this->size.h + TOAST_LAYER_MARGIN, 144 - (2 * TOAST_LAYER_MARGIN), this->size.h);
    text_layer_set_text(this->content_layer, this->content_buffer);
    animate_layer(text_layer_get_layer(this->content_layer), start, finish, TOAST_LAYER_ANIM_DURATION, this);
    
    // Auto hide
    app_timer_register(this->duration, timer_callback, this);
  }
}

bool toast_layer_is_visible(ToastLayer *this)
{
  return this->is_visible;
}