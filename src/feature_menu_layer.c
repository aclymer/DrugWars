#include "feature_menu_layer.h"
#include "instruction_layer.h"
#include "ToastLayer.h"
#include <time.h>
#include <math.h>
#include <stdarg.h>

Window 				*window;
NumberWindow	*number_window;
ToastLayer 		*message_layer;
void Event_Generator(MenuIndex *cell_index)
{
	Player.Dice = (rand() % 21);
	
	APP_LOG(APP_LOG_LEVEL_INFO, "Persist size: %i", persist_get_size(HIGH_SCORE_KEY));
	
	Player.Trenchcoat.Drug[COCAINE].Price 	= (rand() % 12001	+ 16000	)				;
	Player.Trenchcoat.Drug[HEROINE].Price 	= (rand() % 7001	+ 5000	)				;
	Player.Trenchcoat.Drug[ACID].Price 			= (rand() % 35		+ 10		) * 100	;
	Player.Trenchcoat.Drug[WEED].Price 			= (rand() % 43		+ 33		) * 10	;
	Player.Trenchcoat.Drug[SPEED].Price	 		= (rand() % 16		+ 7			) * 10	;
	Player.Trenchcoat.Drug[LUDES].Price	 		= (rand() % 5			+ 1			) * 10	;
	Player.Dice = rand() % 6 + 9;
	//Player.Dice++;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Event_Generator - Dice: %i", Player.Dice);
	
	switch(Player.Dice)
	{
		case 1:
		toast_layer_show(message_layer, "RIVAL DEALERS ARE SELLING CHEAP LUDES!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
		Player.Trenchcoat.Drug[LUDES].Price = rand() % 9 + 1;
		break;
		
		case 2:
		toast_layer_show(message_layer, "WEED PRICES HAVE BOTTOMED OUT!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
		Player.Trenchcoat.Drug[WEED].Price = (rand() % 28 + 13	) * 10	;
		break;

		case 3:
		toast_layer_show(message_layer, "PIGS ARE SELLING CHEAP HEROINE FROM LAST WEEK'S RAID!!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
		Player.Trenchcoat.Drug[HEROINE].Price = rand() % 1151 + 850;
		break;

		case 4:
		case 5:
		toast_layer_show(message_layer, "ADDICTS ARE BUYING HEROINE AT OUTRAGEOUS PRICES!!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
		Player.Trenchcoat.Drug[HEROINE].Price = rand() % 25001 + 18000;
		break;

		case 6:
		case 7:
		toast_layer_show(message_layer, "PIGS MADE A BIG COKE BUST!\nPRICES ARE OUTRAGEOUS!!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
		Player.Trenchcoat.Drug[COCAINE].Price = rand() % 30001 + 28000;
		break;

		case 8:
		X = rand() % 7 + 1;
		string = malloc((strlen("YOU WERE MUGGED IN THE SUBWAY!!!\nYOU LOST $100000 AND 30 OF YOUR COCAINE!") + 1) * sizeof(char));
		snprintf(string,
						 (strlen("YOU WERE MUGGED IN THE SUBWAY!!!\nYOU LOST $100000 AND 30 OF YOUR COCAINE!") + 1) * sizeof(char),
						 "YOU WERE MUGGED IN THE SUBWAY!!!\nYOU LOST $%i AND %i OF YOUR %s!",
						 (int) (Player.Money.Cash * 0.33333),
						 (int) (Player.Trenchcoat.Drug[X].Quantity * 0.33333),
						 Player.Trenchcoat.Drug[X].Name);
		toast_layer_show(message_layer, string, PUNISHMENT_DELAY, menu_header_heights[Player.MenuNumber]);
		free(string);
		Player.Money.Cash *= 0.666667;
		Player.Trenchcoat.Drug[X].Quantity *= 0.666667;
		break;
		
		case 9:
		case 10:
		case 11:
		if (Player.Trenchcoat.Drug[TOTAL].Quantity >= 30)
		{		
			if (Player.Dice == 9)	Player.Cops = 1;
			if (Player.Dice == 10)	Player.Cops = 3;
			if (Player.Dice == 11)	Player.Cops = 4;

			string = (char*)malloc((strlen("OFFICER HARDASS AND %i DEPUTIES ARE AFTER YOU!") + 1) * sizeof(char));
			if (Player.Cops > 1)
				snprintf(string, (strlen("OFFICER HARDASS AND %i DEPUTIES ARE AFTER YOU!") + 1) * sizeof(char),
							 "OFFICER HARDASS AND %i DEPUTIES ARE AFTER YOU!", Player.Cops - 1);
			else
				snprintf(string, (strlen("OFFICER HARDASS IS AFTER YOU!") + 1) * sizeof(char),
							 "OFFICER HARDASS IS AFTER YOU!");
			Player.MenuNumber = 8;
			toast_layer_show(message_layer, string, SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
			free(string);
			menu_layer_reload_data(home_menu_layer);
		}	
		break;
		
		case 12:
		case 13:
		X = rand() % 3 + 1;
		if (Player.Money.Cash >= Player.Trenchcoat.Guns[X].Price + 100 && Player.Trenchcoat.Freespace >= Player.Trenchcoat.Guns[X].Capacity)
		{
			confirm_header = (char*)malloc((strlen("WILL YOU BUY AMMO FOR YOUR \n.38 SPECIAL\n FOR $400? ") + 1) * sizeof(char));
			strcat(confirm_header, Player.Trenchcoat.Guns[X].Name);
			snprintf(confirm_header,
							 ((strlen("WILL YOU BUY AMMO FOR YOU \n.38 SPECIAL\n FOR $400? ") + 1) * sizeof(char)),
							 (Player.Trenchcoat.Guns[X].Quantity > 0 ? "WILL YOU BUY AMMO FOR YOUR \n%s \nFOR $%i? " : "WILL YOU BUY A \n%s \n FOR $%i? "),
							 Player.Trenchcoat.Guns[X].Name,
							 Player.Trenchcoat.Guns[X].Price);
			p_MenuCallbackContext[0] = NULL;
			p_MenuCallbackContext[1] = &Buy_Gun;
			Player.Trenchcoat.Guns[0].Damage = X;
			Player.MenuNumber = 9;
			menu_layer_reload_data(home_menu_layer);
		}
		break;
		
		case 14:
		toast_layer_show(message_layer, "THERE'S SOME WEED HERE THAT SMELLS DANK!!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);	
		confirm_header = malloc((strlen("YOU WANNA SMOKE IT?") + 1) * sizeof(char));
		strcpy(confirm_header, "WILL YOU SMOKE IT?");
		p_MenuCallbackContext[0] = NULL;
		p_MenuCallbackContext[1] = &Smoke_It;
		Player.MenuNumber = 9;
		menu_layer_reload_data(home_menu_layer);
		break;
	
		case 15:
		if (Player.Money.Cash >= 300)
		{
			confirm_header = malloc((strlen("WILL YOU BUY A BIGGER TRENCHCOAT FOR $200?") + 1) * sizeof(char));
			strcpy(confirm_header, "WILL YOU BUY A BIGGER TRENCHCOAT FOR $200?");
			p_MenuCallbackContext[0] = NULL;
			p_MenuCallbackContext[1] = Buy_Trenchcoat;
			Player.MenuNumber = 9;
			menu_layer_reload_data(home_menu_layer);
		}
		break;
		
		case 16:
		if (Player.Trenchcoat.Freespace >= 8)
		{
			X = rand() % 6 + 1; // [1-6]
			int Y = rand() % 8 + 1; // [1-8]
			string = (char*)malloc((strlen("YOU FOUND %i UNITS OF COCAINE ON A DEAD DUDE IN THE SUBWAY!") + 1) * sizeof(char));
			snprintf(string,
							 (strlen("YOU FOUND %i UNITS OF COCAINE ON A DEAD DUDE IN THE SUBWAY!") + 1) * sizeof(char),
							 "YOU FOUND %i UNITS OF %s\nON A DEAD DUDE IN THE SUBWAY!",
							 Y, Player.Trenchcoat.Drug[X].Name
							);				
			toast_layer_show(message_layer, string, LONG_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
			free(string);
			Player.Trenchcoat.Drug[X].Quantity += Y;
			UpdateFreespace(cell_index);
			return;																									// goto void Home() {;
		}
		break;
		
		case 17:
		toast_layer_show(message_layer, "THE MARKET IS FLOODED WITH CHEAP HOME-MADE ACID!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
		Player.Trenchcoat.Drug[ACID].Price = (rand() % 51 + 20) * 10;
		break;	
		
		case 18:
		toast_layer_show(message_layer, "LUCKY YOU! YOU CAUGHT A CAB AND SAVED A DAY!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
		Player.Day--;
		break;	
	}
}

void Intro(MenuIndex *cell_index)
{
	Player.MenuNumber = 0;
	toast_layer_show(message_layer, "MADE FOR PEBBLE\nv1.31\nBY A.CLYMER\n2015\nCOLORADO ,USA", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
	
	Player.Cops																= 0;
	Player.Health															= 50;
	Player.Day																= 1;
	Player.CurrentCity												= BRONX;
	Player.Damage															= 0;
	
	Player.Money.Cash 												= 2000;
	Player.Money.Balance											= 0;
	Player.Money.Debt 												= 5000;
	
	Player.Trenchcoat.Price										= 200;
	Player.Trenchcoat.Capacity	 							= 100;
	Player.Trenchcoat.Freespace								= 100;	
	
	Player.Trenchcoat.Drug[TOTAL].Name				= "TOTAL";
	Player.Trenchcoat.Drug[TOTAL].Price				= 0;
	Player.Trenchcoat.Drug[TOTAL].Quantity		= 0;
	
	Player.Trenchcoat.Drug[COCAINE].Name			= "COCAINE";
	Player.Trenchcoat.Drug[COCAINE].Price			= rand() % 12001 + 16000;
	Player.Trenchcoat.Drug[COCAINE].Quantity	= 0;
	
	Player.Trenchcoat.Drug[HEROINE].Name			= "HEROINE";
	Player.Trenchcoat.Drug[HEROINE].Price			= rand() % 7001 + 5000;
	Player.Trenchcoat.Drug[HEROINE].Quantity	= 0;

	Player.Trenchcoat.Drug[ACID].Name					= "ACID   ";
	Player.Trenchcoat.Drug[ACID].Price				= (rand() % 33 + 10) * 100;
	Player.Trenchcoat.Drug[ACID].Quantity			= 0;

	Player.Trenchcoat.Drug[WEED].Name					= "WEED   ";
	Player.Trenchcoat.Drug[WEED].Price				= (rand() % 43 + 30) * 10;
	Player.Trenchcoat.Drug[WEED].Quantity			= 0;

	Player.Trenchcoat.Drug[SPEED].Name				= "SPEED  ";
	Player.Trenchcoat.Drug[SPEED].Price				= (rand() % 16 + 7) * 10;
	Player.Trenchcoat.Drug[SPEED].Quantity		= 0;

	Player.Trenchcoat.Drug[LUDES].Name				= "LUDES  ";
	Player.Trenchcoat.Drug[LUDES].Price				= (rand() % 5 + 1) * 10;
	Player.Trenchcoat.Drug[LUDES].Quantity		= 0;
	
	Player.Trenchcoat.Guns[TOTAL].Name				= "TOTAL";
	Player.Trenchcoat.Guns[TOTAL].Quantity		= 0;
	
	Player.Trenchcoat.Guns[1].Name						= ".38 SPECIAL";
	Player.Trenchcoat.Guns[1].Price						= 350;
	Player.Trenchcoat.Guns[1].Damage					= 25;
	Player.Trenchcoat.Guns[1].Capacity				= 7;
	Player.Trenchcoat.Guns[1].Quantity				= 0;
	
	Player.Trenchcoat.Guns[2].Name						= "44 MAGNUM";
	Player.Trenchcoat.Guns[2].Price						= 400;
	Player.Trenchcoat.Guns[2].Damage					= 25;
	Player.Trenchcoat.Guns[2].Capacity				= 7;
	Player.Trenchcoat.Guns[2].Quantity				= 0;
	
	Player.Trenchcoat.Guns[3].Name						= "BARETTA 9MM";
	Player.Trenchcoat.Guns[3].Price						= 500;
	Player.Trenchcoat.Guns[3].Damage					= 25;
	Player.Trenchcoat.Guns[3].Capacity				= 9;
	Player.Trenchcoat.Guns[3].Quantity				= 0;

	UpdateFreespace(cell_index);
	//app_timer_register(SHORT_MESSAGE_DELAY, Show_Instructions, cell_index);
/*
	if (0)
	{	
		toast_layer_show(message_layer, "THIS IS A GAME OF BUYING AND SELLING. YOUR GOAL IS TO PAY- OFF YOUR DEBT TO THE LOAN SHARK, AND THEN MAKE AS");
		toast_layer_show(message_layer, "MUCH MONEY AS POSSIBLE IN A 1 MONTH PERIOD. WATCH-OUT FOR THE POLICE IF YOU DEAL TOO HEAVILY!");
		toast_layer_show(message_layer, "PRICES FOR DRUGS ARE: COCAINE: 15000-28000 HEROINE: 5000-12000 ACID: 1000-4200");
		toast_layer_show(message_layer, "WEED: 300-720 SPEED: 70-220 LUDES: 10-50");				
	}
*/
	return;
}

// Display Instruction Layer
void Show_Instructions(void *data)
{
	return;
	confirm_header = malloc((strlen("WOULD YOU LIKE TO SEE THE INSTRUCTIONS?") + 1) * sizeof(char));
	strcpy(confirm_header, "WOULD YOU LIKE TO SEE THE INSTRUCTIONS?");
	p_MenuCallbackContext[0] = hide_instruction_layer;
	p_MenuCallbackContext[1] = show_instruction_layer;
	Player.MenuNumber = 9;
	menu_layer_reload_data(home_menu_layer);
	APP_LOG(APP_LOG_LEVEL_INFO, "Showing Instructions...");
}

// A callback is used to specify the amount of sections of menu items
// With this, you can dynamically add and remove sections
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data)
{
	return NUM_MENU_SECTIONS;
}

// Each section has a number of items;  we use a callback to specify this
// You can also dynamically add and remove items using this
uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
	switch(Player.MenuNumber)
	{
		case 0: 	return 7;		break;
		case 1: 	return 7;		break;
		case 2: 	return 6;		break;	
		case 3:
		case 4: 	return 7;		break;
		case 5: 	return 6;		break;
		case 6:		return 3;		break;
		case 7:		return 3;		break;
		case 8:		return 5;		break;
		case 9:		return 2;		break;
		default: 	return 1;		break;
	}
}

// A callback is used to specify the height of the section header
int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
	// This is a define provided in pebble.h that you may use for the default height
	return menu_header_heights[Player.MenuNumber];
}

// A callback is used to specify the height of the cell
int16_t menu_get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{
	// This is a define provided in pebble.h that you may use for the default height
	switch(Player.MenuNumber)
	{
		case 9:		return (150 - menu_header_heights[Player.MenuNumber]) / 2;
		default:	return MENU_CELL_BASIC_HT; 
	}
}

// Here we draw what each header is
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data)
{
	// Deterine which section we're working with
	switch(Player.MenuNumber)
	{
		// Main Menu Header
		case 0:
		if (Player.Day == 1)
			menu_header_simple_icon_draw(ctx, cell_layer, "DRUG WARS", game_icon);
		else
		{
			string = (char*)malloc((strlen(" DAY 00") + 1) * sizeof(char));
			snprintf(string, ((strlen(" DAY 00") + 1) * sizeof(char)), " DAY %i", Player.Day);
			menu_header_simple_icon_draw(ctx, cell_layer, string, game_icon);
			free(string);
		}
		break;

		// Prices Menu Header
		case 1:
		menu_header_simple_icon_draw(ctx, cell_layer, menu_items[Player.MenuNumber].title, menu_icons[Player.MenuNumber]);
		break;

		// Trenchcoat Menu Header
		case 2:
		string = (char*)malloc((strlen("CAPACITY 999") + 1) * sizeof(char));
		snprintf(string, ((strlen("CAPACITY 999") + 1) * sizeof(char)), "CAPACITY %u", Player.Trenchcoat.Capacity);
		menu_header_draw(ctx, cell_layer, menu_items[Player.MenuNumber].title, string, menu_icons[Player.MenuNumber]);
		free(string);
		break;

		// Buy Menu Header
		case 3:
		string = (char*)malloc(((strlen("BUY $9.999K") + 1) * sizeof(char)));
		if (Player.Money.Cash > 9999)
		{
			snprintf(string, ((strlen("BUY $9.999K") + 1) * sizeof(char)), "%s", "BUY $");
			floatstrcat(string, Player.Money.Cash, LOG10(Player.Money.Cash) % 3);
		}
		else
			snprintf(string, ((strlen("BUY $9.999K") + 1) * sizeof(char)), "%s%i", "BUY $", (int)Player.Money.Cash);
		menu_header_simple_icon_draw(ctx, cell_layer, string, menu_icons[Player.MenuNumber]);
		free(string);
		break;

		// Sell Menu Header
		case 4:
		string = (char*)malloc(((strlen("SELL $9.999K") + 1) * sizeof(char)));
		if (Player.Money.Cash > 9999)
		{
			snprintf(string, ((strlen("SELL $9.999K") + 1) * sizeof(char)), "%s", "SELL $");
			floatstrcat(string, Player.Money.Cash, (LOG10(Player.Money.Cash) % 3 - 2));
		}
		else
			snprintf(string, ((strlen("SELL $9.999K") + 1) * sizeof(char)), "%s%i", "SELL $", (int)Player.Money.Cash);
		menu_header_simple_icon_draw(ctx, cell_layer, string, menu_icons[Player.MenuNumber]);
		free(string);
		break;

		// Subway Menu Header
		case 5:
		string = (char*)malloc((strlen("STATEN ISLAND") + 1) * sizeof(char));
		strcpy(string, locations[Player.CurrentCity]);
		menu_header_draw(ctx, cell_layer, menu_items[Player.MenuNumber].title, string, menu_icons[Player.MenuNumber]);
		free(string);
		break;

		// Loan Shark Menu Header
		case 6:
		string = (char*)malloc(((strlen("DEBT $9.999K") + 1) * sizeof(char)));
		if (Player.Money.Debt > 99999)
		{
			snprintf(string, ((strlen("DEBT $9.999K") + 1) * sizeof(char)), "%s", "DEBT $");
			floatstrcat(string, Player.Money.Debt, LOG10(Player.Money.Debt) % 3 + 1);
		}
		else
			snprintf(string, ((strlen("DEBT $9.999K") + 1) * sizeof(char)), "%s%i", "DEBT $", (int)Player.Money.Debt);
		menu_header_draw(ctx, cell_layer, menu_items[Player.MenuNumber].title, string, menu_icons[Player.MenuNumber]);
		free(string);
		break;

		// Bank Menu Header
		case 7:
		string = (char*)malloc(((strlen("BALANCE $9.999K") + 1) * sizeof(char)));
		if (Player.Money.Balance > 9999)
		{
			snprintf(string, ((strlen("BALANCE $9.999K") + 1) * sizeof(char)), "%s", "BALANCE $");
			floatstrcat(string, Player.Money.Balance, 0);
		}
		else
			snprintf(string, ((strlen("BALANCE $9.999K") + 1) * sizeof(char)), "%s%i", "BALANCE $", (int)Player.Money.Balance);
		menu_header_draw(ctx, cell_layer, menu_items[Player.MenuNumber].title, string, menu_icons[Player.MenuNumber]);
		free(string);
		break;

		// Chased Menu Header
		case 8:
		string = (char*)malloc((strlen(chased_menu[0]) + 2) * sizeof(char));
		snprintf(string, ((strlen(chased_menu[0]) + 2) * sizeof(char)), chased_menu[0], Player.Cops);
		if (Player.Cops != 1)
			strcat(string, "S");
		menu_header_draw(ctx, cell_layer, string, chased_menu[1], menu_icons[Player.MenuNumber]);
		free(string);
		break;	

		// Confirm Menu Header
		case 9:
		menu_header_long_draw(ctx, cell_layer, confirm_header);
		break;
	}
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data)
{
	int len = (strlen("9.999K") + 1) * sizeof(char);
	switch(Player.MenuNumber)
	{
		// Draw Main Menu
		default:
		string = (char*)malloc( (strlen(menu_items[cell_index->row + 1].title) + 1) * sizeof(char));
		snprintf(string, (strlen(menu_items[cell_index->row + 1].title) + 1) * sizeof(char), "%s", menu_items[cell_index->row + 1].title);
		break;

		// Draw Prices Menu
		case 1:
		string = (char*)malloc((strlen(Player.Trenchcoat.Drug[cell_index->row].Name) + 9) * sizeof(char));
		snprintf(string,((strlen(Player.Trenchcoat.Drug[cell_index->row].Name) + 9) * sizeof(char)), "%s  $%i",
						 Player.Trenchcoat.Drug[cell_index->row].Name,
						 (int) Player.Trenchcoat.Drug[cell_index->row].Price);
		break;

		// Draw Trenchcoat Menu
		case 2:
		string = (char*)malloc(strlen(trenchcoat_items[cell_index->row]) * sizeof(char) + len);
		switch(cell_index->row)
		{			
			case 1:
			snprintf(string, (strlen(trenchcoat_items[cell_index->row]) * sizeof(char)), trenchcoat_items[cell_index->row],	Player.Trenchcoat.Guns[0].Ammo);
			break;

			case 2:
			snprintf(string, ((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char)), trenchcoat_items[cell_index->row], Player.Damage);
			break;

			case 3:
			snprintf(string, ((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char)), trenchcoat_items[cell_index->row], Player.Trenchcoat.Drug[TOTAL].Quantity);
			break;

			case 4:
			snprintf(string, ((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char)), trenchcoat_items[cell_index->row], Player.Trenchcoat.Guns[0].Quantity);
			break;

			case 5:
			snprintf(string, ((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char)), trenchcoat_items[cell_index->row], Player.Trenchcoat.Freespace);
			break;
		}
		
		break;
		
		// Draw Buy/Sell Menu
		case 3:
		case 4:
		string = (char*)malloc((strlen(Player.Trenchcoat.Drug[cell_index->row].Name) + 10) * sizeof(char));
		snprintf(string,((strlen(Player.Trenchcoat.Drug[cell_index->row].Name) + 10) * sizeof(char)), "%s     %u",
						 Player.Trenchcoat.Drug[cell_index->row].Name,
						 Player.Trenchcoat.Drug[cell_index->row].Quantity);
		break;

		// Draw Subway Menu
		case 5:
		if (cell_index->row >= Player.CurrentCity)
		{
			string = (char*)malloc((strlen(locations[cell_index->row + 1]) + 1) * sizeof(char));
			strcpy(string, locations[cell_index->row + 1]);
		}
		else
		{
			string = (char*)malloc((strlen(locations[cell_index->row]) + 1) * sizeof(char));
			strcpy(string, locations[cell_index->row]);
		}
		break;

		// Draw Loan Shark Menu
		case 6:
		string = (char*)malloc((strlen(loan_menu[cell_index->row]) + 1) * sizeof(char));
		strcpy(string, loan_menu[cell_index->row]);
		break;

		// Draw Bank Menu
		case 7:
		string = (char*)malloc((strlen(bank_menu[cell_index->row]) + 1) * sizeof(char));
		strcpy(string, bank_menu[cell_index->row]);
		break;

		// Draw Chased Menu
		case 8:
		switch(cell_index->row)
		{
			case 0:		
			string = (char*)malloc((strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char));
			snprintf(string, (strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char), chased_menu[cell_index->row + 2], Player.Trenchcoat.Guns[0].Ammo);
			break;

			case 1:
			string = (char*)malloc((strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char));
			snprintf(string, (strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char), chased_menu[cell_index->row + 2], Player.Trenchcoat.Guns[0].Quantity);
			break;

			case 2:
			string = (char*)malloc((strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char));
			snprintf(string, (strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char), chased_menu[cell_index->row + 2], Player.Damage);
			break;

			default:
			string = (char*)malloc((strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char));
			snprintf(string, (strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char), "%s", chased_menu[cell_index->row + 2]);
			break;
		}
		break;
		
		// Draw Confirm Menu
		case 9:
		string = (char*)malloc((strlen(confirm_menu[cell_index->row]) + 1) * sizeof(char));
		strcpy(string, confirm_menu[cell_index->row]);
		break;
	}
	
	if (Player.MenuNumber > 0 && Player.MenuNumber < 8 && cell_index->row == 0)
		menu_cell_simple_icon_draw(ctx, cell_layer, "BACK", menu_icons[0]);	
	else if (Player.MenuNumber == 0)
		menu_cell_simple_icon_draw(ctx, cell_layer, string, menu_icons[cell_index->row + 1]);
	else if (Player.MenuNumber == 9)
		menu_cell_simple_centered_draw(ctx, cell_layer, string);
	else
		menu_cell_simple_draw(ctx, cell_layer, string);
	
	free(string);
}

// Here we capture when a user selects a menu item
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "menu_select_layer(menu[%u], row[%i])", Player.MenuNumber, cell_index->row);
	
	if (Player.MenuNumber > 0 && Player.MenuNumber < 8 && cell_index->row == 0)
	{
		int temp = Player.MenuNumber - 1;
		Player.MenuNumber = 0;
		menu_layer_reload_data(menu_layer);
		menu_layer_set_selected_index(menu_layer, MenuIndex(0, temp), MenuRowAlignCenter, false);
	}
	else
	{
		// Use the Player.MenuNumber to specify which item will receive the select action
		switch (Player.MenuNumber)
		{
			// Main Menu
			case 0:
			Player.MenuNumber = cell_index->row + 1;
			menu_layer_reload_data(menu_layer);
			menu_layer_set_selected_index(home_menu_layer, MenuIndex(cell_index->section, 0), MenuRowAlignTop, false);
			return;
			
			// Prices Menu
			case 1:
			if (Player.Money.Cash / Player.Trenchcoat.Drug[cell_index->row].Price < Player.Trenchcoat.Freespace)
			{
				format = (char*)malloc((strlen("AFFORD 999") + 1) * sizeof(char));
				snprintf(format, (strlen("AFFORD 999") + 1) * sizeof(char), "AFFORD %i",
								 (int) (Player.Money.Cash / Player.Trenchcoat.Drug[cell_index->row].Price));
			}
			else
			{
				format = (char*)malloc((strlen("CARRY") + 1) * sizeof(char));
				snprintf(format, (strlen("CARRY 999") + 1) * sizeof(char), "CARRY %i",
								 Player.Trenchcoat.Freespace);
			}				
			string = (char*)malloc((strlen("COCAINE\nYOU CAN AFFORD 999\nYOU HAVE 000") + 1) * sizeof(char));
			snprintf(string, ((strlen("COCAINE\nYOU CAN AFFORD 999\nYOU HAVE 000") + 1) * sizeof(char)),
							 "%s\nYOU CAN %s\nYOU HAVE %i", Player.Trenchcoat.Drug[cell_index->row].Name, format,
							 Player.Trenchcoat.Drug[cell_index->row].Quantity);
			toast_layer_show(message_layer, string, SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
			free(format);
			free(string);
			break;
		
			// Buy Menu
			case 3:
			if (cell_index->row < 7)
			{
				if (Player.Trenchcoat.Freespace < 1)
					toast_layer_show(message_layer, "YOU'RE GONNA NEED A BIGGER COAT!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
				else if (Player.Money.Cash < Player.Trenchcoat.Drug[cell_index->row].Price)
					toast_layer_show(message_layer, "YOU CAN'T AFFORD THAT SHIT!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
				else
				{
					string = (char*)malloc((strlen("BUY HOW MUCH ") + strlen(Player.Trenchcoat.Drug[cell_index->row].Name) + 2) * sizeof(char));
					strcpy(string, "BUY HOW MUCH ");
					strcat(string, Player.Trenchcoat.Drug[cell_index->row].Name);
					strcat(string, "?");
					Num_Input(string, (Player.Money.Cash / Player.Trenchcoat.Drug[cell_index->row].Price > Player.Trenchcoat.Freespace ?
														 Player.Trenchcoat.Freespace :
														 Player.Money.Cash / Player.Trenchcoat.Drug[cell_index->row].Price), 0, 1, 0, cell_index
									 );
					free(string);
					return;
				}
			}
			break;

			// Sell Menu
			case 4:
			if (cell_index->row < 7)
			{
				if (Player.Trenchcoat.Drug[cell_index->row].Quantity > 0)
				{
					string = (char*)malloc((strlen("SELL HOW MUCH ") + strlen(Player.Trenchcoat.Drug[cell_index->row].Name) + 2) * sizeof(char));
					strcpy(string, "SELL HOW MUCH ");
					strcat(string, Player.Trenchcoat.Drug[cell_index->row].Name);
					strcat(string, "?");
					Num_Input(string, Player.Trenchcoat.Drug[cell_index->row].Quantity, 0, 1, 0, cell_index);
					free(string);
				}
				else
					toast_layer_show(message_layer, "YOU DON'T HAVE ANYTHING TO SELL!!!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
				return;
			}
			break;
			
			// Subway Menu
			case 5:
			if (cell_index->row >= Player.CurrentCity)
				Player.CurrentCity = cell_index->row + 1;
			else
				Player.CurrentCity = cell_index->row;

			Player.Day++;
			Player.Money.Debt 		*= 1.1;
			Player.Money.Balance *= 1.06;

			if (Player.Day == 31)
				Game_Over(cell_index);

			Player.MenuNumber = 0;
			Event_Generator(cell_index);
			menu_layer_set_selected_index(menu_layer, MenuIndex(0, 0), MenuRowAlignCenter, false);
			break;
			
			// Loan Shark
			case 6:			
			if (Player.CurrentCity != BRONX)
			{
				toast_layer_show(message_layer, "THE LOAN SHARK ONLY DEALS IN THE BRONX.", LONG_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);

				APP_LOG(APP_LOG_LEVEL_DEBUG, "Current City: %i", Player.CurrentCity);
				break;
			}
			else
			{
				if (cell_index->row == 1)
				{			
					string = (char*)malloc((strlen("HOW MUCH TO PAY?") + 1) * sizeof(char));
					strcpy(string, "HOW MUCH TO PAY?");
					int high = (Player.Money.Debt < (Player.Money.Cash - 100) ? Player.Money.Debt : Player.Money.Cash - 100);
					int delta = EXP(LOG10(high) - 2);
					Num_Input(string, high, 0, (delta > Player.Money.Debt ? Player.Money.Debt : delta), high, cell_index);
					free(string);
				}
				if (cell_index->row == 2)
				{		
					if (Player.Money.Debt >= 50000)
						toast_layer_show(message_layer, "YOU THINK HE'S CRAZY BRO!!!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
					else
					{
						string = (char*)malloc((strlen("HOW MUCH TO BORROW?") + 1) * sizeof(char));
						strcpy(string, "HOW MUCH TO BORROW?");
						Num_Input(string, 50000 - Player.Money.Debt, 0, (1000 > 50000 - Player.Money.Debt ? 50000 - Player.Money.Debt : 1000), 0, cell_index);
						free(string);
					}
				}
			}
			break;

			// Bank Menu
			case 7:
			APP_LOG(APP_LOG_LEVEL_INFO, "Balance: %d LOG10(%d): %i EXP(LOG10(%d)): %d",
							(int)Player.Money.Balance,
							(int)Player.Money.Balance,
							LOG10(Player.Money.Balance),
							(int)Player.Money.Balance,
							EXP(LOG10(Player.Money.Balance)));
			if (Player.CurrentCity != BROOKLYN)
			{
				toast_layer_show(message_layer, "THE BANK IS IN BROOKLYN!!!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
				break;
			}
			else
			{
				if (cell_index->row == 1)
				{			
					string = (char*)malloc((strlen("HOW MUCH TO WITHDRAW?") + 1) * sizeof(char));
					strcpy(string, "HOW MUCH TO WITHDRAW?");
					Num_Input(string, Player.Money.Balance, 0, EXP(LOG10(Player.Money.Balance) - 2), 0, cell_index);
					free(string);
				}
				if (cell_index->row == 2)
				{			
					string = (char*)malloc((strlen("HOW MUCH TO DEPOSIT?") + 1) * sizeof(char));
					strcpy(string, "HOW MUCH TO DEPOSIT?");
					Num_Input(string, Player.Money.Cash, 0, EXP(LOG10(Player.Money.Cash) - 2), Player.Money.Cash / 2, cell_index);
					free(string);
				}
			}
			break;
			
			
			// Chased Menu
			case 8:
			if (cell_index->row == 3)
			{
				X = rand() % 3;
				APP_LOG(APP_LOG_LEVEL_DEBUG, "Chased rand() = %i", X);
				if (X == 2)	{
						Player.MenuNumber = 0;
					if (Player.Cops < 2)
						toast_layer_show(message_layer, "YOU LOST HIM IN AN ALLEY!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
					else
						toast_layer_show(message_layer, "YOU LOST THEM IN AN ALLEY!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
					if (Player.Day == 31)
						Game_Over(cell_index);
					else if (Player.Money.Cash >= 1200 && Player.Damage > 0)
					{
						confirm_header = malloc((strlen("WILL YOU PAY $1000 FOR A DOCTOR TO SEW YOU UP?") + 1) * sizeof(char));
						strcpy(confirm_header, "WILL YOU PAY $1000 FOR A DOCTOR TO SEW YOU UP?");
						p_MenuCallbackContext[0] = NULL;
						p_MenuCallbackContext[1] = &Doctor;
						Player.MenuNumber = 9;
						menu_layer_reload_data(home_menu_layer);	
					}
					else {
						Player.Cops = 0;
						break;
					}
				}
				else {
					if (Player.Cops < 2)
						toast_layer_show(message_layer, "YOU CAN'T SHAKE HIM!!!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
					else
						toast_layer_show(message_layer, "YOU CAN'T SHAKE THEM!!!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
					app_timer_register(SHORT_MESSAGE_DELAY + TOAST_LAYER_ANIM_DURATION, (void*)Being_Shot, cell_index);
				}
			}
			if (cell_index->row == 4)	{
				if (Player.Trenchcoat.Guns[0].Quantity == 0)
					toast_layer_show(message_layer, "YOU DON'T HAVE ANY GUNS!!!\nYOU HAVE TO RUN!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
				else if (Player.Trenchcoat.Guns[0].Ammo == 0)
					toast_layer_show(message_layer, "YOU DON'T HAVE ANY AMMO!!!\nYOU HAVE TO RUN!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);					
				else
				{
					for(X = 1; X < 4; X++) {  
						if (Player.Trenchcoat.Guns[X].Ammo > 0)
							break;
					}
					Player.Trenchcoat.Guns[X].Ammo--;
					X = rand() % 3 + 1;
					if (X == 2)
					{
						if (--Player.Cops < 1) {
							Player.Cops = 0;
							Cop_187(cell_index);
							break;
						}
						else
						{
							toast_layer_show(message_layer, "YOU KILLED ONE!!!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
							app_timer_register(SHORT_MESSAGE_DELAY + TOAST_LAYER_ANIM_DURATION, (void*)Being_Shot, cell_index);
						}
					}
					else
					{
						toast_layer_show(message_layer, "YOU MISSED!!!\n\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
						app_timer_register(SHORT_MESSAGE_DELAY + TOAST_LAYER_ANIM_DURATION, (void*)Being_Shot, cell_index);
					}
					UpdateFreespace(cell_index);
				}
			}
			break;
			
			case 9:
			APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", confirm_header);
			free(confirm_header);
			Player.MenuNumber = 0;
			if (p_MenuCallbackContext[cell_index->row] != NULL)
			{
				p_MenuCallbackContext[cell_index->row](cell_index);
				p_MenuCallbackContext[0] = NULL;
				p_MenuCallbackContext[1] = NULL;
			}
			APP_LOG(APP_LOG_LEVEL_INFO, (cell_index->row ? "YES" : "NO"));
			menu_layer_reload_data(menu_layer);
			break;
			
			default:
			Player.MenuNumber = 0;
			break;
		}
		
		menu_layer_reload_data(menu_layer);
	}
}

void number_window_selected_callback(struct NumberWindow *number_window, void *context)
{
	switch(Player.MenuNumber)
	{
		case 3:		BuyDrugs(number_window_get_value(number_window), (MenuIndex*)context);	break;
		case 4:		SellDrugs(number_window_get_value(number_window), (MenuIndex*)context);	break;
		
		case 6:
			if (((MenuIndex*)context)->row == 1)
			{
				Player.Money.Cash -= number_window_get_value(number_window);
				Player.Money.Debt -= number_window_get_value(number_window);
			}
			else if (((MenuIndex*)context)->row == 2)
			{
				Player.Money.Debt += number_window_get_value(number_window);
				Player.Money.Cash += number_window_get_value(number_window);
			}
		break;
		
		case 7:
			if (((MenuIndex*)context)->row == 1)
			{
				Player.Money.Balance -= number_window_get_value(number_window);
				Player.Money.Cash += number_window_get_value(number_window);
			}
			else if (((MenuIndex*)context)->row == 2)
			{
				Player.Money.Balance += number_window_get_value(number_window);	
				Player.Money.Cash -= number_window_get_value(number_window);		
			}
		break;
		
	}
	
	window_stack_remove(number_window_get_window(number_window), true);
	menu_layer_reload_data(home_menu_layer);
}

void number_window_incremented_callback(struct NumberWindow *number_window, void *context)
{
}

void number_window_decremented_callback(struct NumberWindow *number_window, void *context)
{
}

// This initializes the menu upon window load
void window_load(Window *window)
{
	// Here we load the bitmap assets
	// resource_init_current_app must be called before all asset loading
	int num_menu_icons = 0;
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RETURN_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PRICES_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TRENCHCOAT_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BUY_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SELL_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SUBWAY_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOAN_SHARK_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BANK_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DWPD_ICON);
	game_icon										 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_GAME_ICON);
	
	// Load custom font	
	header_font 	= fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LUCIDA_CONSOLE_REG_22));
	cell_font 		= fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LUCIDA_CONSOLE_REG_18));
	subtitle_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LUCIDA_CONSOLE_REG_14));
	confirm_font	= fonts_get_system_font(FONT_KEY_GOTHIC_24);
	
	// And also load the background
	//menu_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_BRAINS);

	// Now we prepare to initialize the menu layer
	// We need the bounds to specify the menu layer's viewport size
	// In this case, it'll be the same as the window's
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);

	// Create the menu layer
	home_menu_layer = menu_layer_create(bounds);

	// Set all the callbacks for the menu layer
	menu_layer_set_callbacks(home_menu_layer, NULL, (MenuLayerCallbacks)
													 {
														 .get_num_sections 	= menu_get_num_sections_callback,
														 .get_num_rows 			= menu_get_num_rows_callback,
														 .get_header_height = menu_get_header_height_callback,
														 .get_cell_height		= menu_get_cell_height_callback,
														 .draw_header 			= menu_draw_header_callback,
														 .draw_row 					= menu_draw_row_callback,
														 .select_click 			= menu_select_callback,
													 });
	
	// Bind the menu layer's click config provider to the window for interactivity
	menu_layer_set_click_config_onto_window(home_menu_layer, window);
	// Add it to the window for display
	layer_add_child(window_layer, menu_layer_get_layer(home_menu_layer));
}

void Num_Input(char *text, int high, int low, int delta, int set, MenuIndex *cell_index)
{
	*p_NumWindowContext = *cell_index;
	number_window_set_label(number_window, text);
	number_window_set_max(number_window, high);
	number_window_set_min(number_window, low);
	number_window_set_step_size(number_window, delta);
	number_window_set_value(number_window, set);
	window_set_status_bar_icon(number_window_get_window(number_window), menu_icons[Player.MenuNumber]);
  window_stack_push(number_window_get_window(number_window), true);
}

void BuyDrugs(int32_t howMany, MenuIndex *cell_index)
{	
	APP_LOG(APP_LOG_LEVEL_INFO,"Buying %li units of %s", howMany, Player.Trenchcoat.Drug[cell_index->row].Name);
	Player.Trenchcoat.Drug[cell_index->row].Quantity 	+= howMany;
	Player.Money.Cash 																	-= howMany * Player.Trenchcoat.Drug[cell_index->row].Price;
	UpdateFreespace(cell_index);
}

void SellDrugs(int32_t howMany, MenuIndex *cell_index)
{
	APP_LOG(APP_LOG_LEVEL_INFO,"Selling %li units of %s", howMany, Player.Trenchcoat.Drug[cell_index->row].Name);
	Player.Trenchcoat.Drug[cell_index->row].Quantity 	-= howMany;
	Player.Money.Cash 																	+= howMany * Player.Trenchcoat.Drug[cell_index->row].Price;
	UpdateFreespace(cell_index);
}

void Buy_Trenchcoat(MenuIndex *cell_index)
{
	if (cell_index->row)
	{
		Player.Trenchcoat.Capacity += 20;
		Player.Money.Cash -= 200;
		UpdateFreespace(cell_index);
	}		
}

void Buy_Gun(MenuIndex *cell_index)
{
	if (Player.Trenchcoat.Guns[Player.Trenchcoat.Guns[0].Damage].Quantity < 1)
	{
		Player.Trenchcoat.Guns[Player.Trenchcoat.Guns[0].Damage].Quantity = 1;
		Player.Trenchcoat.Guns[Player.Trenchcoat.Guns[0].Damage].Ammo = Player.Trenchcoat.Guns[Player.Trenchcoat.Guns[0].Damage].Capacity;
	}
	else
		Player.Trenchcoat.Guns[Player.Trenchcoat.Guns[0].Damage].Ammo += Player.Trenchcoat.Guns[Player.Trenchcoat.Guns[0].Damage].Capacity;

	Player.Money.Cash -= Player.Trenchcoat.Guns[Player.Trenchcoat.Guns[0].Damage].Price;
	UpdateFreespace(cell_index);
}

void Smoke_It(MenuIndex *cell_index)
{
		toast_layer_show(message_layer, "YOU HALLUCINATE, STUMBLE ON TO THE TRACKS AND GET HIT BY A TRAIN!",
										 LONG_MESSAGE_DELAY, menu_header_heights[0]);
		app_timer_register(LONG_MESSAGE_DELAY + TOAST_LAYER_ANIM_DURATION, (void*)Game_Over, cell_index);
}

void Being_Shot(MenuIndex *cell_index)
{
	format = malloc((strlen("YOU'VE BEEN KILLED!!!") + 1) * sizeof(char));
	X = rand() % 2;
	if (X == 0)
		if (Player.Cops < 2)
			strcpy(format, "HE MISSED!!!");
		else
			strcpy(format, "THEY MISSED!!!");		
	else
	{
		Player.Damage += 3 + 3 * (int)(rand() % Player.Cops + 1);
		if (Player.Damage < 50)	
			strcpy(format, "YOU'VE BEEN HIT!!!");	
		else
		{
			strcpy(format, "YOU'VE BEEN KILLED!!!");
			app_timer_register(SHORT_MESSAGE_DELAY + TOAST_LAYER_ANIM_DURATION, (void*)Game_Over, cell_index);
		}
	}
	
	string = malloc((strlen("THEY'RE FIRING AT YOU!!!\nYOU'VE BEEN KILLED!!!") + 1) * sizeof(char));
	if (Player.Cops < 2)
		snprintf(string, (strlen("HE'S FIRING AT YOU!!!\nYOU'VE BEEN KILLED!!!") + 1) * sizeof(char), "HE'S' FIRING AT YOU!!!\n%s", format);
	else
		snprintf(string, (strlen("THEY'RE FIRING AT YOU!!!\nYOU'VE BEEN KILLED!!!") + 1) * sizeof(char), "THEY'RE FIRING AT YOU!!!\n%s", format);
	free(format);
	toast_layer_show(message_layer, string, SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
	free(string);
	return;
}

void Cop_187(MenuIndex *cell_index)
{		
	X = (rand() % 1250 + 750);
	Player.Money.Cash += X;
	string = (char*)malloc((strlen("\nHOLY SHIT! YOU KILLED ALL OF THEM AND FOUND $2000 ON OFFICER HARDASS!!") + 1) * sizeof(char));
	if (Player.Dice == 9)
		snprintf(string,  (strlen("\nHOLY SHIT! YOU KILLED HIM AND FOUND $2000 ON HIS CARCASS!!") + 1) * sizeof(char), "\nHOLY SHIT! YOU KILLED HIM AND FOUND $%u ON HIS CARCASS!!", X);
	else
		snprintf(string,  (strlen("\nHOLY SHIT! YOU KILLED ALL OF THEM AND FOUND $2000 ON OFFICER HARDASS!!") + 1) * sizeof(char), "\nHOLY SHIT! YOU KILLED ALL OF THEM AND FOUND $%u ON OFFICER HARDASS!!", X);
	toast_layer_show(message_layer, string, LONG_MESSAGE_DELAY, 0);
	free(string);
	if (Player.Money.Cash >= 1200 && Player.Damage > 0)
	{
		confirm_header = malloc((strlen("WILL YOU PAY $1000 FOR A DOCTOR TO SEW YOU UP?") + 1) * sizeof(char));
		strcpy(confirm_header, "WILL YOU PAY $1000 FOR A DOCTOR TO SEW YOU UP?");
		p_MenuCallbackContext[0] = NULL;
		p_MenuCallbackContext[1] = &Doctor;
		Player.MenuNumber = 9;
		menu_layer_reload_data(home_menu_layer);	
	}
	else
	{
		Player.MenuNumber = 0;
		menu_layer_reload_data(home_menu_layer);
	}
}

void Doctor(MenuIndex *cell_index)
{	
	Player.Money.Cash -= 1000;
	Player.Damage 			= 0;
}

void Exit(MenuIndex *cell_index)
{
	toast_layer_show(message_layer, "\n\nTHANKS FOR PLAYING! \nREMEMBER TO WATCH YOUR BACK. \nPEACE!!!", LONG_MESSAGE_DELAY, 0);
	window_stack_pop_all(true);
}

void UpdateFreespace(MenuIndex *cell_index)
{
	Player.Trenchcoat.Guns[TOTAL].Ammo = 0;
	Player.Trenchcoat.Guns[TOTAL].Quantity = 0;
	for (short i = 1; i < 4; i++)
	{
		Player.Trenchcoat.Guns[TOTAL].Ammo += Player.Trenchcoat.Guns[i].Quantity * Player.Trenchcoat.Guns[i].Ammo;
		Player.Trenchcoat.Guns[TOTAL].Quantity += Player.Trenchcoat.Guns[i].Quantity;
	}
	
	Player.Trenchcoat.Drug[TOTAL].Quantity = 0;
	for (short i = 1; i < 7; i++)
	{
		Player.Trenchcoat.Drug[TOTAL].Quantity += Player.Trenchcoat.Drug[i].Quantity;
	}	
	Player.Trenchcoat.Freespace = Player.Trenchcoat.Capacity
		- Player.Trenchcoat.Drug[TOTAL].Quantity
		- Player.Trenchcoat.Guns[TOTAL].Ammo * Player.Trenchcoat.Guns[TOTAL].Quantity;
}

void Play_Again(MenuIndex *cell_index)
{
	confirm_header = malloc((strlen("\n\nPLAY AGAIN?") + 1) * sizeof(char));
	strcpy(confirm_header, "PLAY AGAIN?");
	Player.MenuNumber = 9;
	p_MenuCallbackContext[0] = &Exit;
	p_MenuCallbackContext[1] = &Intro;
	menu_layer_reload_data(home_menu_layer);
}

void Game_Over(MenuIndex *cell_index)
{
	Score = Player.Money.Balance - Player.Money.Debt;
	if (Score < 0) Score = 0;
	unsigned long int HighScore = (persist_exists(HIGH_SCORE_KEY) ? persist_read_int(HIGH_SCORE_KEY) : Score);
	if (Score > HighScore)
	{
		HighScore = Score;
		persist_write_int(HIGH_SCORE_KEY, HighScore);
		string = (char*)malloc((strlen("GAME OVER!\n\nNEW HIGH SCORE:\n$9999999999") + 1) * sizeof(char));
		snprintf(string, ((strlen("GAME OVER!\n\nNEW HIGH SCORE:\n$9999999999") + 1) * sizeof(char)),
						 "GAME OVER!\n\nNEW HIGH SCORE:\n$%li", (long)Score);
	}
	else
	{
		string = (char*)malloc((strlen("GAME OVER!\n\nHIGH SCORE:\n$9999999999\nYOUR SCORE:\n$9999999999") + 1) * sizeof(char));
		snprintf(string, ((strlen("GAME OVER!\n\nHIGH SCORE:\n$9999999999\nYOUR SCORE:\n$9999999999") + 1) * sizeof(char)),
						 "GAME OVER!\n\nHIGH SCORE:\n$%li\nYOUR SCORE:\n$%li", HighScore, (long)Score);
	}
	toast_layer_show(message_layer, string, PUNISHMENT_DELAY, 0);
	free(string);
	
	// Delete mid-game Player Data Store
	if (persist_exists(PLAYER_DATA_KEY))
		persist_delete(PLAYER_DATA_KEY);
	
	app_timer_register(PUNISHMENT_DELAY + TOAST_LAYER_ANIM_DURATION, (void*)Play_Again, cell_index);
}

void Save_Game(void)
{
	persist_write_data(PLAYER_DATA_KEY, &Player, sizeof(Player));
	persist_write_int(PLAYER_SIZE_KEY, sizeof(Player));
	APP_LOG(APP_LOG_LEVEL_INFO, "Player Size: %i", sizeof(Player));
	return;
}

void Load_Game(MenuIndex *cell_index)
{
	if (value == 8)
		Player.MenuNumber = value;
	else
		Player.MenuNumber = 0;
	
	APP_LOG(APP_LOG_LEVEL_INFO, "Player MenuNumber: %i", Player.MenuNumber);
	
	menu_layer_reload_data(home_menu_layer);
	
	return;
}

void window_unload(Window *window) 
{	
	// Destroy the menu layer
	menu_layer_destroy(home_menu_layer);

	// Cleanup the menu icons
	for (unsigned int i = 0; i < ARRAY_LENGTH(menu_icons); i++)
	{
		gbitmap_destroy(menu_icons[i]);
	}	
	gbitmap_destroy(game_icon);
		
	// Unload Custom Fonts
	fonts_unload_custom_font(header_font);
	fonts_unload_custom_font(cell_font);
	fonts_unload_custom_font(subtitle_font);
}

static void destroy_ui(void)
{
	toast_layer_destroy(message_layer);
	number_window_destroy(number_window);
  window_destroy(window);
	
	// Save Player Data
	if ( Player.Day < 31)
		Save_Game();
}

void hide_number_window_layer(void)
{
  window_stack_pop(true);
}

int main(void)
{		
	srand(time(0));	
	window = window_create();
	p_NumWindowContext = malloc(sizeof(MenuCallback));
	number_window = number_window_create(NULL, (NumberWindowCallbacks) 
																			 {.selected 		= number_window_selected_callback,
																				.incremented 	= number_window_incremented_callback,
																				.decremented 	= number_window_decremented_callback
																			 }, p_NumWindowContext);
	
	// Create message layer
	message_layer = toast_layer_create(window);

	// Setup the window handlers
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});	

	window_stack_push(window, true);
	
	// Check for saved game
	if (persist_exists(PLAYER_DATA_KEY))
	{
		persist_read_data(PLAYER_DATA_KEY, &Player, persist_read_int(PLAYER_SIZE_KEY));
		if (Player.MenuNumber != 8)
		{
			value = Player.MenuNumber;
			confirm_header = malloc((strlen("\nLOAD SAVED GAME?") + 1) * sizeof(char));
			snprintf(confirm_header, (strlen("\nLOAD SAVED GAME?") + 1) * sizeof(char), "%s", "\nLOAD SAVED GAME?");

			//TODO: set temp_callbacks for possible menu 8,9 loading plus add confirm_header to persist

			p_MenuCallbackContext[0] = &Intro;
			p_MenuCallbackContext[1] = &Load_Game;
			Player.MenuNumber = 9;
		}
		
		menu_layer_reload_data(home_menu_layer);
	}
	else
		Intro(NULL);	
	
	app_event_loop();

	free(p_NumWindowContext);
	if (p_MenuCallbackContext[0] != NULL) free(p_MenuCallbackContext[0]);
	if (p_MenuCallbackContext[1] != NULL) free(p_MenuCallbackContext[1]);
	
	destroy_ui();
}

int LOG10(double val)
{
	int count = 0;
	val /= 10;
	while (val >= 1)
	{
		val /= 10;
		count++;
	}
	return count;
}

int EXP(int val)
{
	int temp = 1;
	for (int i = 0; i < val; i++)
		temp *= 10;	
	return temp;
}

void floatstrcat(char* str, double val, int precision)
{
	Y = val / EXP(LOG10(val) - LOG10(val) % 3);
	if (precision > 3) precision = 3;
	str += strlen(string);
	//  start with positive/negative
	if (Y < 0)
	{
		*(str++) = '-';
		Y = -Y;
	}
	//  integer value
	snprintf(str, 12, "%d", (int) Y);
	str += strlen(str);
	Y -= (int) Y;
	
	//  decimals
	if ((precision > 0) && (Y >= .001))
	{
		//  add period
		*(str++) = '.';
		//  loop through precision
		for (;precision > 0; precision--)
		{
			if (Y > 0)
			{
				Y *= 10;
				*(str++) = '0' + (int) (Y + ((precision == 1) ? .5 : 0));
				Y -= (int) val;
			}
			else
				break;
		}
	}
	// Add postfix unit
	*(str++) = postfix[LOG10(val) / 3];
	//  terminate
	*str = '\0';	
}

// Menu Header Draw function for Title only
void menu_header_simple_draw(GContext* ctx, const Layer *cell_layer, const char *title)
{
  graphics_context_set_text_color(ctx, GColorBlack);
	GRect titleOrigin = layer_get_bounds(cell_layer);
  graphics_draw_text(ctx, title, header_font, titleOrigin, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
}

// Menu Header Draw function for Icon and Title
void menu_header_simple_icon_draw(GContext* ctx, const Layer *cell_layer, const char *title, const GBitmap* bitmap)
{
  graphics_context_set_text_color(ctx, GColorBlack);
	GRect bitmap_bounds 		= bitmap->bounds;
	GRect title_bounds 			= layer_get_bounds(cell_layer);
	bitmap_bounds.origin.x 	+= 2;
	bitmap_bounds.origin.y	= (title_bounds.size.h - bitmap_bounds.size.h) / 2;
	graphics_draw_bitmap_in_rect(ctx, bitmap, bitmap_bounds);
	title_bounds.origin.x 	= bitmap_bounds.size.w + bitmap_bounds.origin.x * 2;
	title_bounds.origin.y 	-= 1;
  graphics_draw_text(ctx, title, header_font, title_bounds, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
}

// Menu Header Draw function for Icon, Title, and Subtitle
void menu_header_draw(GContext* ctx, const Layer *cell_layer, const char *title, const char* subtitle, const GBitmap* bitmap)
{
  graphics_context_set_text_color(ctx, GColorBlack);
	GRect bitmap_bounds 			= bitmap->bounds;
	GRect title_bounds 				= layer_get_bounds(cell_layer);
	GRect subtitle_bounds 		= layer_get_bounds(cell_layer);
	bitmap_bounds.origin.x 		+= 2;
	bitmap_bounds.origin.y		= (title_bounds.size.h - bitmap->bounds.size.h) / 2 + 3;
	graphics_draw_bitmap_in_rect(ctx, bitmap, bitmap_bounds);
	title_bounds.origin.x 		= bitmap_bounds.size.w + bitmap_bounds.origin.x * 2;
	title_bounds.origin.y			= ((layer_get_bounds(cell_layer).size.h - bitmap_bounds.size.h - 24) / 2);
	title_bounds.origin.y			-= 1;
	subtitle_bounds.origin.x 	= title_bounds.origin.x;
	subtitle_bounds.origin.y	= subtitle_bounds.size.h / 2;
  graphics_draw_text(ctx, title, header_font, title_bounds, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
	graphics_draw_text(ctx, subtitle, cell_font, subtitle_bounds, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
}

// Menu Header Draw function for long title only
void menu_header_long_draw(GContext* ctx, const Layer *cell_layer, const char *title)
{
  graphics_context_set_text_color(ctx, GColorBlack);
	GRect titleOrigin = layer_get_bounds(cell_layer);
	titleOrigin.size.h = menu_header_heights[Player.MenuNumber];
  graphics_draw_text(ctx, title, confirm_font, titleOrigin, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

// Menu Row Draw function for Title only
void menu_cell_simple_draw(GContext* ctx, const Layer *cell_layer, const char *title)
{
  graphics_context_set_text_color(ctx, GColorBlack);
	GRect titleOrigin = layer_get_bounds(cell_layer);
	titleOrigin.origin.y -= 3;
  graphics_draw_text(ctx, title, cell_font, titleOrigin, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
}

// Menu Row Draw function for Title only (Centered)
void menu_cell_simple_centered_draw(GContext* ctx, const Layer *cell_layer, const char *title)
{
  graphics_context_set_text_color(ctx, GColorBlack);
	GRect titleOrigin = layer_get_bounds(cell_layer);
	titleOrigin.origin.y -= 5;
  graphics_draw_text(ctx, title, header_font, titleOrigin, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
}

// Menu Row Draw function for Icon and Title
void menu_cell_simple_icon_draw(GContext* ctx, const Layer *cell_layer, const char *title, const GBitmap* bitmap)
{
  graphics_context_set_text_color(ctx, GColorBlack);
	GRect bitmap_bounds = bitmap->bounds;
	GRect titleOrigin = layer_get_bounds(cell_layer);
	bitmap_bounds.origin.x = (24 - bitmap->bounds.size.w) / 2;
	graphics_draw_bitmap_in_rect(ctx, bitmap, bitmap_bounds);
	titleOrigin.origin.x = 26;
	titleOrigin.origin.y -= 3;
  graphics_draw_text(ctx, title, cell_font, titleOrigin, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
}

//! Menu row drawing function to draw a basic cell with the title, subtitle, and icon. 
//! Call this function inside the `.draw_row` callback implementation, see \ref MenuLayerCallbacks.
//! @param ctx The destination graphics context
//! @param cell_layer The layer of the cell to draw
//! @param title Draws a title in larger text (18 points, Lucida Console font).
//! @param subtitle Draws a subtitle in smaller text (14 points, Lucida Console font).
//! @param icon Draws an icon to the left of the text.
void menu_cell_draw(GContext* ctx, const Layer *cell_layer, const char *title, const char* subtitle, const GBitmap* bitmap)
{
  graphics_context_set_text_color(ctx, GColorBlack);
	GRect bitmap_bounds 		= bitmap->bounds;
	GRect title_bounds 			= layer_get_bounds(cell_layer);
	//GRect subtitle_bounds 	= layer_get_bounds(cell_layer);
	bitmap_bounds.origin.x 	= (26 - bitmap->bounds.size.w) / 2;
	graphics_draw_bitmap_in_rect(ctx, bitmap, bitmap_bounds);
	title_bounds.origin.x 	= 26;
	title_bounds.origin.y 	-= 3;
  graphics_draw_text(ctx, title, cell_font, title_bounds, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
	//graphics_draw_text(ctx, subtitle, subtitle_font, subtitle_bounds, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
}