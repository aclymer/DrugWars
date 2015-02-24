#include <pebble.h>

// Setup settings data
typedef struct {
	bool vibrate;
	bool invert;
	bool light;
	short days;
} SETTINGS_DATA;

extern SETTINGS_DATA Settings;