#include <pebble.h>
//#undef APP_LOG
//#define APP_LOG(...)

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

#define NUM_MENU_ICONS 							9
	
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
MenuIndex *num_window_context;
GBitmap 	*menu_icons[NUM_MENU_ICONS];
GBitmap 	*game_icon;

// In-Game Variables
short			Score, value, X;
short			Dice;
short			CurrentCity;
bool			num_window_is_visible;
void 			Exit(void *context);
void 			show_number_window_layer(void *);
void 			hide_number_window_layer(void);
void 			UpdateFreespace(MenuIndex *);
void 			num_selected_callback(struct NumberWindow *, void *);
uint8_t 	current_icon;
uint8_t		menu_number = 0;
GFont 		header_font;
GFont 		cell_font;
GFont 		subtitle_font;

typedef struct {
	const char *title;
	const char *subtitle;
} BasicItem;
	
// Menu Header Heights
const short menu_header_heights[10] =
{
	MENU_CELL_BASIC_HEADER_HT			,
	MENU_CELL_BASIC_HEADER_HT			, 
	SUBTITLED_MENU_HEADER_HT			,
	MENU_CELL_BASIC_HEADER_HT			, 
	MENU_CELL_BASIC_HEADER_HT			, 
	SUBTITLED_MENU_HEADER_HT			,
	MENU_CELL_BASIC_HEADER_HT * 3	,
	MENU_CELL_BASIC_HEADER_HT * 3 ,
	MENU_CELL_BASIC_HEADER_HT * 3	,
	MENU_CELL_BASIC_HEADER_HT * 4
};

// Home Menu
BasicItem menu_items[8]	=
{
	{"BACK",NULL},
	{"PRICES",NULL},
	{"TRENCHCOAT",NULL},
	{"BUY",NULL},
	{"SELL",NULL},
	{"SUBWAY",NULL},
	{"LOAN SHARK",NULL},
	{"BANK",NULL}
};

// Trenchcoat Menu
const char* trenchcoat_items[6] =
{
		"BACK",
		"CASH     $%u   ",
		"DRUGS       %u ",
		"GUNS        %i ",
		"DAMAGE      %i ",
		"CAPACITY    %u "
};

// Subway Menu
const char* locations[7] =
{
		"",
		"THE BRONX",
		"THE GHETTO",
		"CENTRAL PARK",
		"MANHATTEN",
		"CONEY ISLAND",
		"BROOKLYN"
};

// Loan Menu
const char* loan_menu[3] =
{
		"",
		"PAY   ",
		"BORROW"
};

// Bank Menu
const char* bank_menu[3] =
{
		"",
		"WITHDRAW",
		"DEPOSIT"
};

// Chased Menu
const char* chased_menu[6] =
{
	"BEING CHASED",
	"BY %i PIGS!!",
	"GUNS   %i",
	"DAMAGE %i OF 50",
	"RUN",
	"FIGHT"
};

// Confirm Menu
const char* confirm_menu[3] =
{
	"",
	"YES",
	"NO"
};

typedef struct {
	int Price;								// C , H , A , W , S , L
	int Quantity;							// M , N , O , P , Q , R
	const char *Name;
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
//bool TrenchcoatAdd(ushort, ITEMS, MenuIndex *);
void BuyDrugs(int32_t, MenuIndex *);
void SellDrugs(int32_t, MenuIndex *);
void Being_Shot(MenuIndex *);
void Cop_187(MenuIndex *);
void Game_Over(void);
void Subway(MenuIndex *);
void Event_Generator(MenuIndex *);
	
// Pebble wrapper conditional functions
void Num_Input(char *, int, int, int, MenuIndex *);

char	*format;
char	*string;

int LOG10(int val);

// Menu Header Draw function for Title only
void menu_header_simple_draw(GContext *, const Layer *, const char *);

// Menu Header Draw function for Icon and Title
void menu_header_simple_icon_draw(GContext *, const Layer *, const char *, const GBitmap *);

// Menu Header Draw function for Icon, Title, and Subtitle
void menu_header_draw(GContext *, const Layer *, const char *, const char *, const GBitmap *);

// Menu Row Draw function for Title only
void menu_cell_simple_draw(GContext *, const Layer *, const char *);

// Menu Row Draw function for Icon and Title
void menu_cell_simple_icon_draw(GContext *, const Layer *, const char *, const GBitmap *);

//! Menu row drawing function to draw a basic cell with the title, subtitle, and icon. 
//! Call this function inside the `.draw_row` callback implementation, see \ref MenuLayerCallbacks.
//! @param ctx The destination graphics context
//! @param cell_layer The layer of the cell to draw
//! @param title Draws a title in larger text (18 points, Lucida Console font).
//! @param subtitle Draws a subtitle in smaller text (14 points, Lucida Console font).
//! @param icon Draws an icon to the left of the text.
void menu_cell_draw(GContext *, const Layer *, const char *, const char *, const GBitmap *);