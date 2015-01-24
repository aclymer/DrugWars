#include <pebble.h>

typedef struct {
  Window *parent;
  char *content_buffer;
	void *callback_context;
} ConfirmWindow;

void show_confirm_window(ConfirmWindow *);
void hide_confirm_window(void);
