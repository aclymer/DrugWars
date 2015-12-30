#include <pebble.h>
#undef APP_LOG
#define APP_LOG(...)
#define safe_memory_free(pointer_address) do{if(pointer_address != NULL && *pointer_address != NULL){free(*pointer_address);*pointer_address = NULL;}}while(0)
#define safe_free(pointer) safe_memory_free((void **) &(pointer))

// Player Data
typedef struct {
	int 			Price;
	int 			Quantity;
} DRUGS;

typedef struct {
	int 			Capacity;
	int 			Damage;
	int 			Quantity;
	int 			Ammo;
	int 			Price;
} GUNS;

typedef struct {
	int 			Price;
	int 			Capacity;
	int 			Freespace;
	GUNS 			Guns[4];
	DRUGS 		Drug[7];				
} INVENTORY;

typedef struct {
	int 			Balance;
	int 			Cash;
	int 			Debt;
} FINANCIAL_DATA;

typedef struct {
	int 			Cops;
	int				CurrentCity;
	int 			Damage;
	int 			Day;
	int				Dice;
	int				MenuNumber;
	int 			Health;
	FINANCIAL_DATA 	Money;
	INVENTORY 		Trenchcoat;
} PLAYER_DATA;

extern PLAYER_DATA Player;

// Setup settings data
typedef struct {
	bool autosave;
	bool vibrate;
	bool light;
	short days;
} SETTINGS_DATA;

extern SETTINGS_DATA Settings;

// Monetary unit designations
extern const char postfix[4];

extern int LOG10(int val);
extern int EXP	(int val);