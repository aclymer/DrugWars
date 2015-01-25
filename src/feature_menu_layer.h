#include <pebble.h>

typedef enum ITEMS {
	TOTAL				= 0,
 	COCAINE 		= 1,
 	HEROINE 		= 2,
 	ACID				= 3,
 	WEED				= 4,
 	SPEED				= 5,
	LUDES				= 6,
	CAPACITY		= 7,
	GUNS 				= 8
} ITEMS;

enum LOCATIONS {
		NEVERMIND			= 0,
		BRONX       	= 1,
		GHETTO       	= 2,
		CENTRAL_PARK 	= 3,
		MANHATTEN    	= 4,
		CONEY_ISLAND 	= 5,
		BROOKLYN     	= 6
};

#define NUM_MENU_ICONS 							1
	
#define MENU_CELL_BASIC_HT					17
	
#define	MENU_CELL_BASIC_HEADER_HT		26
#define SUBTITLED_MENU_HEADER_HT	 	44

#define NUM_MENU_SECTIONS						1
#define NUM_HOME_MENU_ITEMS 				7
#define NUM_PRICES_MENU_ITEMS				7
#define NUM_SELL_MENU_ITEMS					7

#define BASIC_ITEM_LENGTH						17
#define MAX_ITEM_LENGTH							63
	
#define SHORT_MESSAGE_DELAY					2
#define LONG_MESSAGE_DELAY					5
#define PUNISHMENT_DELAY						10

MenuLayer *home_menu_layer;

GBitmap 	*menu_icons[NUM_MENU_ICONS];
GBitmap 	*menu_background;

// In-Game Variables
short			Score, value, X;
short			Dice;
short			CurrentCity;
bool			num_window_is_visible;
void 			show_number_window_layer(void *);
void 			hide_number_window_layer(void);
void 			UpdateFreespace(MenuIndex *);
void 			num_selected_callback(struct NumberWindow *, void *);
uint8_t 	current_icon;
uint8_t		menu_number = 0;

typedef struct {
	char 		*title;
	char 		*subtitle;
	GBitmap *icon;
} BasicItem;
	
// Menu Header Heights
const short menu_header_heights[9] =
{
	MENU_CELL_BASIC_HEADER_HT			,
	MENU_CELL_BASIC_HEADER_HT			, 
	SUBTITLED_MENU_HEADER_HT			,
	MENU_CELL_BASIC_HEADER_HT			, 
	MENU_CELL_BASIC_HEADER_HT			, 
	SUBTITLED_MENU_HEADER_HT			,
	MENU_CELL_BASIC_HEADER_HT * 3	,
	MENU_CELL_BASIC_HEADER_HT * 3 ,
	MENU_CELL_BASIC_HEADER_HT * 3
};

// Home Menu
const BasicItem menu_items[7]	=
{
	{"PRICES     ",NULL,NULL},
	{"TRENCHCOAT ",NULL,NULL},
	{"BUY        ",NULL,NULL},
	{"SELL       ",NULL,NULL},
	{"JET        ",NULL,NULL},
	{"LOAN SHARK ",NULL,NULL},
	{"BANK       ",NULL,NULL}
};

// Jet Menu
const char* locations[7] =
{
		"NEVERMIND   ",
		"THE BRONX   ",
		"THE GHETTO  ",
		"CENTRAL PARK",
		"MANHATTEN   ",
		"CONEY ISLAND",
		"BROOKLYN    "
};

// Trenchcoat Menu
const char* trenchcoat_items[6] =
{
		"...CLOSE IT!!!      ",
		"CASH $%u            ",
		"DRUGS         : %u  ",
		"GUNS            : %u",
		"DAMAGE       : %i   ",
		"CAPACITY    : %u    "
};

// Loan Menu
const char* loan_menu[3] =
{
		"..BACK       ",
		"PAY          ",
		"BORROW       "
};

// Bank Menu
const char* bank_menu[3] =
{
		"             ",
		"WITHDRAW     ",
		"DEPOSIT      "
};

// Chased Menu
const char* chased_menu[6] =
{
	"BEING CHASED    ",
	"BY %i PIGS!!    ",
	"GUNS       %i   ",
	"DAMAGE  %i OF 50",
	"RUN             ",
	"FIGHT           "
};

const void* prev_menu_items[3][8] = {
{ "   ", "<--", "<--", "<--", "<--", "<--", "<--", "<--" },
{  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL },
{  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL }
};

typedef struct {
	int Price;								// C , H , A , W , S , L
	int Quantity;							// M , N , O , P , Q , R
	char *Name;
} DRUGS;

typedef struct {
	int Price;
	int Capacity;							//	T = 100;
	int Guns;									//	= 0;
	int Freespace;						//	K	= 200;
	DRUGS Drug[7];				
} Inventory;

typedef struct {
	int Balance;							//		V = 0;
	int Cash;									//		Z = 2000;
	int Debt;									//		Y = 0;
} FinancialData;

Inventory Trenchcoat;
FinancialData Money;
int Health;									//		V = 50;
int Damage;									//		J = 0;
int Cops;
int Day;										//		B = 1;

// In-Game functions
bool TrenchcoatAdd(ushort, ITEMS, MenuIndex *);
void BuyDrugs(int32_t, MenuIndex *);
void SellDrugs(int32_t, MenuIndex *);
void Being_Shot(MenuIndex *);
void Cop_187(MenuIndex *);
void Game_Over(void);
void Subway(MenuIndex *);
void Event_Generator(MenuIndex *);
	
// Pebble wrapper conditional functions
int Menu(char* Title, int num, char* m_Item1, char* m_Item2, ... );
void Num_Input(char *, int, int, int, MenuIndex *);

char			*string;