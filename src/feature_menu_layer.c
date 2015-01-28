#define DEBUG 1
#include "feature_menu_layer.h"
#include "ToastLayer.h"
#include <time.h>
#include <math.h>
#include <stdarg.h>

Window 				*window;
NumberWindow	*number_window;
ToastLayer 		*message_layer;

void Event_Generator(MenuIndex *cell_index)
{
	Trenchcoat.Drug[COCAINE].Price 		= (rand() % 12001	+ 16000	)				;
	Trenchcoat.Drug[HEROINE].Price 		= (rand() % 7001	+ 5000	)				;
	Trenchcoat.Drug[ACID].Price 			= (rand() % 35		+ 10		) * 100	;
	Trenchcoat.Drug[WEED].Price 			= (rand() % 43		+ 33		) * 10	;
	Trenchcoat.Drug[SPEED].Price	 		= (rand() % 16		+ 7			) * 10	;
	Trenchcoat.Drug[LUDES].Price	 		= (rand() % 5			+ 1			) * 10	;
	Dice 															= (rand() % 21		+ 0			)				;

	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Event_Generator - Dice: %i", Dice);

	switch(Dice)
	{
		case 1:
		{
			toast_layer_show(message_layer, "RIVAL DEALERS ARE SELLING CHEAP LUDES!", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
			Trenchcoat.Drug[LUDES].Price = rand() % 9 + 1;
			break;
		}

		case 2:
		{
			toast_layer_show(message_layer, "WEED PRICES HAVE BOTTOMED OUT!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
			Trenchcoat.Drug[WEED].Price = (rand() % 28 + 13	) * 10	;
			break;
		}

		case 3:
		{
			toast_layer_show(message_layer, "PIGS ARE SELLING CHEAP HEROINE FROM LAST WEEK'S RAID!!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
			Trenchcoat.Drug[HEROINE].Price = rand() % 1151 + 850;
			break;
		}

		case 4:
		case 5:
		{
			toast_layer_show(message_layer, "ADDICTS ARE BUYING HEROINE AT OUTRAGEOUS PRICES!!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
			Trenchcoat.Drug[HEROINE].Price = rand() % 25001 + 18000;
			break;
		}

		case 6:
		case 7:
		{
			toast_layer_show(message_layer, "PIGS MADE A BIG COKE BUST!\nPRICES ARE OUTRAGEOUS!!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
			Trenchcoat.Drug[COCAINE].Price = rand() % 20000 + 80000;
			break;
		}

		case 8:
		{
			toast_layer_show(message_layer, "YOU WERE MUGGED IN THE SUBWAY!!!\n", PUNISHMENT_DELAY, menu_header_heights[menu_number]);
			Money.Cash *= 0.666667;
			break;
		}

		case 12:
		case 13:
		{
			if (Money.Cash >= 500 && Trenchcoat.Freespace >= 5)
			{
				string = malloc((strlen("WILL YOU BUY A SATURDAY NIGHT SPECIAL for $400?") + 1) * sizeof(char));
				snprintf(string, ((strlen("WILL YOU BUY A SATURDAY NIGHT SPECIAL for $400?") + 1) * sizeof(char)), "WILL YOU BUY A ");
				switch(rand() % 3)
				{
					case 0:		strcat(string, "BARRETTA"); 							break;
					case 1:		strcat(string, "SATURDAY NIGHT SPECIAL");	break;
					case 2:		strcat(string, ".44 MAGNUM");							break;
				}
				strcat(string, "\nFOR $400?");
				toast_layer_show(message_layer, string, SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
				free(string);
				X = 1;
				if (X == 1)
				{
					Trenchcoat.Guns			++;//I = I + 1;
					Money.Cash 					-= 400;//Z = Z - 400;
					Trenchcoat.Capacity -= 5;//T = T - 5;
					UpdateFreespace(cell_index);
				}
			}
			break;
		}
		
		case 14:
			toast_layer_show(message_layer, "THERE'S SOME WEED HERE THAT SMELLS LIKE GOOD STUFF!!", LONG_MESSAGE_DELAY, menu_header_heights[menu_number]);	
			X = 1;

			if (X == 1 && rand() % 2)
			{
				toast_layer_show(message_layer, "YOU HALLUCINATE ON THE WILDEST TRIP OF YOUR LIFE, STUMBLE ON TO THE SUBWAY TRACKS AND GET CREAMED BY A TRAIN.", LONG_MESSAGE_DELAY, menu_header_heights[menu_number]);
				psleep(PUNISHMENT_DELAY * 1000);
				Game_Over();
			}
		break;
	
		case 15:
			if (Money.Cash >= 300)
			{
				toast_layer_show(message_layer, "WILL YOU BUY A NEW TRENCHCOAT WITH MORE POCKETS FOR $200?", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
				X = 1;
				if (X == 1)
				{
					Trenchcoat.Capacity += 20;
					Money.Cash -= 200;
					UpdateFreespace(cell_index);
				}	
			}
		break;
		
		case 16:
			if (Trenchcoat.Freespace >= 8)
			{
				X = rand() % 7 + 1; // [1-6]
				int Y = rand() % 8 + 1;
				string = malloc((strlen("YOU FOUND %i UNITS OF COCAINE ON A DEAD DUDE IN THE SUBWAY!!!") + 1) * sizeof(char));
				snprintf(string,
								 (strlen("YOU FOUND %i UNITS OF COCAINE ON A DEAD DUDE IN THE SUBWAY!!!") + 1) * sizeof(char),
								 "YOU FOUND %i UNITS OF %s\nON A DEAD DUDE IN THE SUBWAY!!!",
								 Y, Trenchcoat.Drug[X].Name
								);				
				toast_layer_show(message_layer, string, LONG_MESSAGE_DELAY, menu_header_heights[menu_number]);
				free(string);
				Trenchcoat.Drug[X].Quantity += Y;
				UpdateFreespace(cell_index);
				return;																									// goto void Home() {;
			}
		break;
		
		case 17:
			toast_layer_show(message_layer, "THE MARKET IS FLOODED WITH CHEAP HOME-MADE ACID!!!", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
			Trenchcoat.Drug[ACID].Price = (rand() % 51 + 20) * 10;		
			return;																									// goto void Home() {;
		break;
		
		case 9:
		case 10:
		case 11:
			if (Trenchcoat.Capacity - Trenchcoat.Freespace >= 50)
			{		
				if (Dice == 9)	Cops = 1;
				if (Dice == 10)	Cops = 3;
				if (Dice == 11)	Cops = 4;

				string = malloc((strlen("OFFICER HARDASS AND %i OF HIS DEPUTIES ARE AFTER YOU!") + 1) * sizeof(char));
				snprintf(string, (strlen("OFFICER HARDASS AND %i OF HIS DEPUTIES ARE AFTER YOU!") + 1) * sizeof(char), "OFFICER HARDASS AND %i OF HIS DEPUTIES ARE AFTER YOU!", Cops);
				toast_layer_show(message_layer, string, SHORT_MESSAGE_DELAY, 0);
				free(string);
				menu_number = 8;
				menu_layer_reload_data(home_menu_layer);
			}	
		break;
	}
	
	return;
}

void Being_Shot(MenuIndex *cell_index)
{
	X = (rand() % 2);

	toast_layer_show(message_layer, "THEY'RE FIRING AT YOU!!!\n\n", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
	psleep(LONG_MESSAGE_DELAY);
	if (X == 0)
	{
		toast_layer_show(message_layer, "THEY MISSED!!!\n\n", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);				
		return;
	}
	else
	{
		toast_layer_show(message_layer, "YOU'VE BEEN HIT!!!\n\n", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
		Damage += 3;
		if (Damage >= 50)
		{
			psleep(LONG_MESSAGE_DELAY);
			toast_layer_show(message_layer, "YOU'VE BEEN KILLED!!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
			psleep(SHORT_MESSAGE_DELAY);
			Game_Over();
		}
		return;
	}
}

void Cop_187(MenuIndex *cell_index)
{		
	toast_layer_show(message_layer, "HOLY SHIT! YOU KILLED ALL OF THEM!!!", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
	X = (rand() % 1250 + 750);
	Money.Cash += X;
	string = malloc((strlen("YOU FOUND $2000 ON OFFICER HARDASS!!\n") + 1) * sizeof(char));
	snprintf(string,  (strlen("YOU FOUND $2000 ON OFFICER HARDASS!!\n") + 1) * sizeof(char), "YOU FOUND %u DOLLARS ON OFFICER HARDASS!!\n", X);
	psleep(SHORT_MESSAGE_DELAY);
	toast_layer_show(message_layer, string, LONG_MESSAGE_DELAY, menu_header_heights[menu_number]);
	free(string);
	if (Money.Cash >= 1200)
	{
		toast_layer_show(message_layer, "WILL YOU PAY $1000 DOLLARS FOR A DOCTOR TO SEW YOU UP?", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
		X = 1;
		if (X == 1)
		{
			Money.Cash -= 1000;
			Damage = 0;
		}
	}
}

void Subway(MenuIndex *cell_index)
{
	Day++;
	Money.Debt 		*= 1.1;
	Money.Balance *= 1.06;

	if (Day == 31)
		Game_Over();

	return;
}

void Game_Over(void)
{
	toast_layer_show(message_layer, "GAME OVER!", PUNISHMENT_DELAY, 0);
	Score += Money.Balance - Money.Debt;

	if (Score < 0)
		Score = 0;
	else
	{
		Score /= 315;
		Score *= 10;
		if (Score > 100)
			Score = 100;
	}

	string	= malloc((strlen("YOUR SCORE:\n 100%") + 1) * sizeof(char));
	snprintf(string, ((strlen("YOUR SCORE:\n 100%") + 1) * sizeof(char)), "YOUR SCORE:\n %i", Score);
	toast_layer_show(message_layer, string, LONG_MESSAGE_DELAY, 0);// V = Total Score
	free(string);
	//V = Input("PLAY AGAIN?",2,"YES","NO");

	//if (V==1)
	//	Intro();

	toast_layer_show(message_layer, "THANKS FOR PLAYING! REMEMBER TO WATCH YOUR BACK. PEACE!!!", LONG_MESSAGE_DELAY, 0);
	psleep(5000);
	window_stack_pop_all(true);
	window_destroy(window);
}

void Intro(MenuIndex *cell_index)
{
	toast_layer_show(message_layer, "'THE ORIGINAL'\nVERSION 1.0\n\nBY A.CLYMER\n\n", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
	
	Cops																= 0;
	Health															= 50;
	Day																	= 1;
	CurrentCity													= BRONX;
	
	Money.Cash 													= 2000;
	Money.Balance												= 0;
	Money.Debt 													= 5000;
	
	Trenchcoat.Price										= 200;
	Trenchcoat.Capacity	 								= 100;
	Trenchcoat.Freespace								= 100;	
		
	Trenchcoat.Drug[TOTAL].Name					= "...BACK ";
	Trenchcoat.Drug[TOTAL].Quantity			= 0;
	
	Trenchcoat.Drug[COCAINE].Name				= "COCAINE";
	Trenchcoat.Drug[COCAINE].Price			= rand() % 12001 + 16000;
	Trenchcoat.Drug[COCAINE].Quantity		= 0;
	
	Trenchcoat.Drug[HEROINE].Name				= "HEROINE";
	Trenchcoat.Drug[HEROINE].Price			= rand() % 7001 + 5000;
	Trenchcoat.Drug[HEROINE].Quantity		= 0;

	Trenchcoat.Drug[ACID].Name					= "ACID   ";
	Trenchcoat.Drug[ACID].Price					= (rand() % 33 + 10) * 100;
	Trenchcoat.Drug[ACID].Quantity			= 0;

	Trenchcoat.Drug[WEED].Name					= "WEED   ";
	Trenchcoat.Drug[WEED].Price					= (rand() % 43 + 30) * 10;
	Trenchcoat.Drug[WEED].Quantity			= 0;

	Trenchcoat.Drug[SPEED].Name					= "SPEED  ";
	Trenchcoat.Drug[SPEED].Price				= (rand() % 16 + 7) * 10;
	Trenchcoat.Drug[SPEED].Quantity			= 0;

	Trenchcoat.Drug[LUDES].Name					= "LUDES  ";
	Trenchcoat.Drug[LUDES].Price				= (rand() % 5 + 1) * 10;
	Trenchcoat.Drug[LUDES].Quantity			= 0;

	//Num_Input("ORIGINAL GAME FOR IBM BY:\nJOHN E. DELL\nINSTRUCTIONS ?\n (1) Yes / (2) No",2,1,1);
	//if (DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "Input() = %d", value);
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
	switch(menu_number)
	{
		case 0: 	return 7;		break;
		case 1: 	return 7;		break;
		case 2: 	return 6;		break;	
		case 3:
		case 4: 	return 7;		break;
		case 5: 	return 6;		break;
		case 6:		return 3;		break;
		case 7:		return 3;		break;
		case 8:		return 4;		break;
		default: 	return 1;		break;
	}
}

// A callback is used to specify the height of the section header
int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
	// This is a define provided in pebble.h that you may use for the default height
	return menu_header_heights[menu_number];
}

// A callback is used to specify the height of the cell
int16_t menu_get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{
	// This is a define provided in pebble.h that you may use for the default height
	switch(menu_number)
	{
		default:	return MENU_CELL_BASIC_HT; 
	}
}

// Here we draw what each header is
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data)
{
	int value = 0;
	// Deterine which section we're working with
	switch(menu_number)
	{
		// Main Menu Header
		case 0:
		if (Day == 1)
			menu_header_simple_icon_draw(ctx, cell_layer, "DRUG WARS", game_icon);
		else
		{
		string = malloc((strlen(" DAY 00") + 1) * sizeof(char));
		snprintf(string, ((strlen(" DAY 00") + 1) * sizeof(char)), " DAY %i", Day);
		menu_header_simple_icon_draw(ctx, cell_layer, string, game_icon);
		free(string);
		}
		break;
		
		// Prices Menu Header
		case 1:
		menu_header_simple_icon_draw(ctx, cell_layer, menu_items[menu_number].title, menu_icons[menu_number]);
		break;
		
		// Trenchcoat Menu Header
		case 2:
		string = malloc((strlen("FREESPACE   999") + 1) * sizeof(char));
		snprintf(string, ((strlen("FREESPACE   999") + 1) * sizeof(char)), "FREESPACE   %u", Trenchcoat.Freespace);
		menu_cell_basic_draw(ctx, cell_layer, menu_items[menu_number].title, string, NULL);
		free(string);
		break;
		
		// Buy Menu Header
		case 3:
		string = malloc((strlen("BUY $9999K") + 1) * sizeof(char));
		value = money2String(Money.Cash,"BUY $%u");
		snprintf(string, ((strlen("BUY $9999K") + 1) * sizeof(char)), format, value);
		menu_header_simple_icon_draw(ctx, cell_layer, string);
		free(format);
		free(string);
		break;
	
		// Sell Menu Header
		case 4:
		string = malloc((strlen("SELL $9999K") + 1) * sizeof(char));
		value = money2String(Money.Cash,"SELL $%u");
		snprintf(string, ((strlen("SELL $9999K") + 1) * sizeof(char)), format, value);
		menu_header_simple_icon_draw(ctx, cell_layer, string);
		free(format);
		free(string);
		break;
	
		// Subway Menu Header
		case 5:
		string = malloc((strlen("STATEN ISLAND") + 1) * sizeof(char));
		strcpy(string, locations[CurrentCity]);
		menu_cell_basic_draw(ctx, cell_layer, menu_items[menu_number].title, string, menu_icons[menu_number]);
		free(string);
		break;
		
		// Loan Shark Menu Header
		case 6:
		string = malloc((strlen("DEBT: $99999M") + 1) * sizeof(char));
		value = money2String(Money.Debt,"DEBT: $%u");
		snprintf(string, ((strlen("DEBT: $99999M") + 1) * sizeof(char)), format, value);
		menu_cell_basic_draw(ctx, cell_layer, menu_items[menu_number].title, string, menu_icons[menu_number]);
		free(string);
		break;
		
		// Bank Menu Header
		case 7:
		string = malloc((strlen("BALANCE: $99999M") + 1) * sizeof(char));
		value = money2String(Money.Balance,"BALANCE: $%u");
		snprintf(string, ((strlen("BALANCE: $99999M") + 1) * sizeof(char)), format, value);
		menu_cell_basic_draw(ctx, cell_layer, menu_items[menu_number].title, string, menu_icons[menu_number]);
		free(format);
		free(string);
		break;
		
		// Chased Menu Header
		case 8:
		string = malloc((strlen(chased_menu[1]) + 1) * sizeof(char));
		snprintf(string, ((strlen(chased_menu[1]) + 1) * sizeof(char)), chased_menu[1], Cops);
		menu_cell_basic_draw(ctx, cell_layer, chased_menu[0], string, NULL);
		free(string);
		break;		
	}
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data)
{	
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "menu_draw_row_callback(menu[%i], row[%i])", menu_number, cell_index->row);
	
	uint32_t value = 0;
	switch(menu_number)
	{
		// Draw Main Menu
		case 0:
		string = malloc((strlen(menu_items[cell_index->row + 1].title) + 1) * sizeof(char));
		strcpy(string, menu_items[cell_index->row + 1].title);
		break;

		// Draw Prices Menu
		case 1:
		string 	= malloc((strlen(Trenchcoat.Drug[cell_index->row].Name) + 9) * sizeof(char));
		snprintf(string,((strlen(Trenchcoat.Drug[cell_index->row].Name) + 9) * sizeof(char)), "%s: $%u",
						 Trenchcoat.Drug[cell_index->row].Name,
						 Trenchcoat.Drug[cell_index->row].Price);
		break;

		// Draw Trenchcoat Menu
		case 2:
		string = malloc((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char));
		switch(cell_index->row)
		{
			case 1:
			format = malloc((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char));
			value = money2String(Money.Cash, (char*) trenchcoat_items[cell_index->row]);
			break;

			case 2:
			format = malloc((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char));
			value = Trenchcoat.Drug[TOTAL].Quantity;
			strcpy(format, trenchcoat_items[cell_index->row]);
			break;

			case 3:
			format = malloc((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char));
			value = Trenchcoat.Guns;
			strcpy(format, trenchcoat_items[cell_index->row]);
			break;

			case 4:
			format = malloc((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char));
			value = Damage;
			strcpy(format, trenchcoat_items[cell_index->row]);
			break;

			case 5:
			format = malloc((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char));
			value = Trenchcoat.Capacity;
			strcpy(format, trenchcoat_items[cell_index->row]);
			break;
		}
		snprintf(string, ((strlen(trenchcoat_items[cell_index->row]) + strlen(format)) * sizeof(char)), format, value);
		free(format);
		break;

		// Draw Buy/Sell Menu
		case 3: case 4:
		string 	= malloc((strlen(Trenchcoat.Drug[cell_index->row].Name) + 9) * sizeof(char));
		snprintf(string,((strlen(Trenchcoat.Drug[cell_index->row].Name) + 9) * sizeof(char)), "%s: %u",
						 Trenchcoat.Drug[cell_index->row].Name,
						 Trenchcoat.Drug[cell_index->row].Quantity);
		break;

		// Draw Jet Menu
		case 5:
		if (cell_index->row >= CurrentCity)
		{
			string = malloc((strlen(locations[cell_index->row + 1]) + 1) * sizeof(char));
			strcpy(string, locations[cell_index->row + 1]);
		}
		else
		{
			string = malloc((strlen(locations[cell_index->row]) + 1) * sizeof(char));
			strcpy(string, locations[cell_index->row]);
		}
		break;

		// Draw Loan Shark Menu
		case 6:
		string = malloc((strlen(loan_menu[cell_index->row]) + 1) * sizeof(char));
		strcpy(string, loan_menu[cell_index->row]);
		break;

		// Draw Bank Menu
		case 7:
		string = malloc((strlen(bank_menu[cell_index->row]) + 1) * sizeof(char));
		strcpy(string, bank_menu[cell_index->row]);
		break;

		// Draw Chased Menu
		case 8:
		switch(cell_index->row)
		{
			case 0:	
			string = malloc((strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char));
			snprintf(string, (strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char), chased_menu[cell_index->row + 2], Trenchcoat.Guns);
			break;

			case 1:
			string = malloc((strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char));
			snprintf(string, (strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char), chased_menu[cell_index->row + 2], Damage);
			break;

			default:
			string = malloc((strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char));
			strcpy(string, chased_menu[cell_index->row + 2]);
			break;
		}
		break;
	}
	
	if (menu_number > 0 && menu_number < 8 && cell_index->row == 0)
		menu_cell_simple_icon_draw(ctx, cell_layer, " BACK", menu_icons[0]);	
	else if (menu_number == 0)
		menu_cell_simple_icon_draw(ctx, cell_layer, string, menu_icons[cell_index->row + 1]);
	else
		menu_cell_simple_draw(ctx, cell_layer, string);
	
	free(string);
}

// Here we capture when a user selects a menu item
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{	
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "menu_select_layer(menu[%u], row[%i])", menu_number, cell_index->row);
	
	if (menu_number > 0 && menu_number < 8 && cell_index->row == 0)
	{
		int temp = menu_number - 1;
		menu_number = 0;
		menu_layer_reload_data(menu_layer);
		menu_layer_set_selected_index(menu_layer, MenuIndex(0, temp), MenuRowAlignCenter, false);
	}
	else
	{
		// Use the menu_number to specify which item will receive the select action
		switch (menu_number)
		{
			// Main Menu
			case 0:
				menu_number = cell_index->row + 1;
				menu_layer_reload_data(menu_layer);
				menu_layer_set_selected_index(home_menu_layer, MenuIndex(cell_index->section, 0), MenuRowAlignTop, false);
			return;
			
			// Prices Menu
			case 1:
				string = malloc((strlen("You have 100\n on hand.") + 1) * sizeof(char));
				snprintf(string, ((strlen("You have 100\n on hand.") + 1) * sizeof(char)), "You have %i\n on hand.", Trenchcoat.Drug[cell_index->row].Quantity);
				toast_layer_show(message_layer, string, SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
				free(string);
				return;
			break;
		
			// Buy Menu
			case 3:
				if (cell_index->row < 7)
				{
					if (Trenchcoat.Freespace < 1)
						toast_layer_show(message_layer, "YOU'RE GONNA NEED A BIGGER COAT!", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
					else if (Money.Cash < Trenchcoat.Drug[cell_index->row].Price)
						toast_layer_show(message_layer, "YOU CAN'T AFFORD THAT SHIT!", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
					else
					{
						string = malloc((strlen("BUY HOW MUCH ") + strlen(Trenchcoat.Drug[cell_index->row].Name) + 2) * sizeof(char));
						strcpy(string, "BUY HOW MUCH ");
						strcat(string, Trenchcoat.Drug[cell_index->row].Name);
						strcat(string, "?");
						Num_Input(string, (Money.Cash / Trenchcoat.Drug[cell_index->row].Price > Trenchcoat.Freespace ?
											Trenchcoat.Freespace :
											Money.Cash / Trenchcoat.Drug[cell_index->row].Price), 0, 1, cell_index
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
					if (Trenchcoat.Drug[cell_index->row].Quantity > 0)
					{
						string = malloc((strlen("SELL HOW MUCH ") + strlen(Trenchcoat.Drug[cell_index->row].Name) + 2) * sizeof(char));
						strcpy(string, "SELL HOW MUCH ");
						strcat(string, Trenchcoat.Drug[cell_index->row].Name);
						strcat(string, "?");
						Num_Input(string, Trenchcoat.Drug[cell_index->row].Quantity, 0, 1, cell_index);
						free(string);
					}
					else
						toast_layer_show(message_layer, "YOU DON'T HAVE ANYTHING TO SELL!!!", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
					return;
				}
			break;
			
			// Jet Menu
			case 5:
				if (cell_index->row >= CurrentCity)
					CurrentCity = cell_index->row + 1;
				else
					CurrentCity = cell_index->row;
				Subway(cell_index);
				menu_number = 0;
				Event_Generator(cell_index);
			break;
			
			// Loan Shark
			case 6:			
				if (CurrentCity != BRONX)
				{
					toast_layer_show(message_layer, "THE LOAN SHARK ONLY DEALS IN THE BRONX.", LONG_MESSAGE_DELAY, menu_header_heights[menu_number]);
					if (DEBUG)
						APP_LOG(APP_LOG_LEVEL_DEBUG, "Current City: %i", CurrentCity);
					break;
				}
				else
				{
					if (cell_index->row == 1)
					{			
						string = malloc((strlen("HOW MUCH TO PAY?") + 1) * sizeof(char));
						strcpy(string, "HOW MUCH TO PAY?");
						Num_Input(string, Money.Debt < Money.Cash ? Money.Debt : Money.Cash - 100, 0, 100, cell_index);
						free(string);
					}
					if (cell_index->row == 2)
					{			
						string = malloc((strlen("HOW MUCH TO BORROW?") + 1) * sizeof(char));
						strcpy(string, "HOW MUCH TO BORROW?");
						Num_Input(string, 50000, 0, 1000, cell_index);
						free(string);
					}
				}
			break;

			// Bank Menu
			case 7:
				if (CurrentCity != BROOKLYN)
				{
					toast_layer_show(message_layer, "THE BANK IS IN BROOKLYN!!!", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
					break;
				}
				else
				{
					if (cell_index->row == 1)
					{			
						string = malloc((strlen("HOW MUCH TO WITHDRAW?") + 1) * sizeof(char));
						strcpy(string, "HOW MUCH TO WITHDRAW?");
						Num_Input(string, Money.Balance, 0, 100, cell_index);
						free(string);
					}
					if (cell_index->row == 2)
					{			
						string = malloc((strlen("HOW MUCH TO DEPOSIT?") + 1) * sizeof(char));
						strcpy(string, "HOW MUCH TO DEPOSIT?");
						Num_Input(string, Money.Cash, 0, 100, cell_index);
						free(string);
					}
				}
			break;
			
			case 8:
				if (cell_index->row == 2)
				{
					X = rand() % 2;
					APP_LOG(APP_LOG_LEVEL_DEBUG, "Chased rand() = %i", X);
					if (X == 0)
					{
						toast_layer_show(message_layer, "YOU LOST THEM IN AN ALLEY!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
						if (Day == 31)
							return; 													// goto Home();
						else
						{
							Cops = 0;
							menu_number = 0;
							break;
						}
					}
					else
					{
						toast_layer_show(message_layer, "YOU CAN'T SHAKE THEM!!!", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
						Being_Shot(cell_index);
					}
				}
				if (cell_index->row == 3)
				{
					if (Trenchcoat.Guns == 0)
					{
						toast_layer_show(message_layer, "YOU DON'T HAVE ANY GUNS!!!\nYOU HAVE TO RUN!", SHORT_MESSAGE_DELAY, 0);
						break;
					}
					else							
						X = rand() % 2;

					if (X == 0)
						toast_layer_show(message_layer, "YOU MISSED!!!\n\n", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
					else
					{
						toast_layer_show(message_layer, "YOU KILLED ONE!!!", LONG_MESSAGE_DELAY, menu_header_heights[menu_number]);
						Cops--;
						if (Cops < 0)
							Cop_187(cell_index);
					}	
				}
			break;
			
			default:
				menu_number = 0;
			break;
		}
		
		menu_layer_reload_data(menu_layer);
	}
}

void number_window_selected_callback(struct NumberWindow *number_window, void *context)
{
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Num_Input(%d)", (int)number_window_get_value(number_window));
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Menu[%i]\tRow[%i]\tSection[%i]", menu_number, ((MenuIndex*)context)->row, ((MenuIndex*)context)->section);

	switch(menu_number)
	{
		case 3:		BuyDrugs(number_window_get_value(number_window), (MenuIndex*)context);	break;
		case 4:		SellDrugs(number_window_get_value(number_window), (MenuIndex*)context);	break;
		
		case 6:
			if (((MenuIndex*)context)->row == 1)
			{
				Money.Cash -= number_window_get_value(number_window);
				Money.Debt -= number_window_get_value(number_window);
			}
			else if (((MenuIndex*)context)->row == 2)
			{
				Money.Debt += number_window_get_value(number_window);
				Money.Cash += number_window_get_value(number_window);
			}
		break;
		
		case 7:
			if (((MenuIndex*)context)->row == 1)
			{
				Money.Balance -= number_window_get_value(number_window);
				Money.Cash += number_window_get_value(number_window);
			}
			else if (((MenuIndex*)context)->row == 2)
			{
				Money.Balance += number_window_get_value(number_window);	
				Money.Cash -= number_window_get_value(number_window);		
			}
		break;
		
	}
	window_stack_pop(false);
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Window_stack_pop(%d)", ((MenuIndex*)context)->row);
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
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BUY_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TRENCHCOAT_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BUY_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SELL_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SUBWAY_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SELL_ICON);
	menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BANK_ICON);
	game_icon										 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_GAME_ICON);
	
	// Load custom font	
	header_font 	= fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LUCIDA_CONSOLE_REG_24));
	cell_font 		= fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LUCIDA_CONSOLE_REG_18));
	subtitle_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LUCIDA_CONSOLE_REG_14));
	
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

void Num_Input(char *text, int high, int low, int delta, MenuIndex *cell_index)
{
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_INFO, "Row[%i], %s [%d Max.]", cell_index->row, text, high);
	
	number_window = number_window_create(NULL, (NumberWindowCallbacks) {.selected 		= number_window_selected_callback,
																																			.incremented 	= number_window_incremented_callback,
																																			.decremented 	= number_window_decremented_callback
																																		 }, cell_index);
	number_window_set_label(number_window, text);
	number_window_set_max(number_window, high);
	number_window_set_min(number_window, low);
	number_window_set_step_size(number_window, delta);
	
	//layer_mark_dirty(menu_layer_get_layer(home_menu_layer));
	
  window_stack_push(number_window_get_window(number_window), true);
}

void BuyDrugs(int32_t howMany, MenuIndex *cell_index)
{
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_INFO,"Buying %li units of %s", howMany, Trenchcoat.Drug[cell_index->row].Name);
	Trenchcoat.Drug[cell_index->row].Quantity 	+= howMany;
	Trenchcoat.Freespace												-= howMany;
	Money.Cash 																	-= howMany * Trenchcoat.Drug[cell_index->row].Price;
}

void SellDrugs(int32_t howMany, MenuIndex *cell_index)
{
	if (DEBUG)
		APP_LOG(APP_LOG_LEVEL_INFO,"Selling %li units of %s", howMany, Trenchcoat.Drug[cell_index->row].Name);
	Trenchcoat.Drug[cell_index->row].Quantity 	-= howMany;
	Trenchcoat.Freespace												+= howMany;
	Money.Cash 																	+= howMany * Trenchcoat.Drug[cell_index->row].Price;
}

bool TrenchcoatAdd(ushort quantity, ITEMS type, MenuIndex *cell_index)
{
	if (Trenchcoat.Freespace < quantity)
		toast_layer_show(message_layer, "TOO BAD YOU DON'T HAVE ENOUGH ROOM IN YOUR COAT!!!", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
	
	else if (type > 0 && type <= LUDES)
		Trenchcoat.Drug[type].Quantity += value;

	else if (type == GUNS)
		Trenchcoat.Guns += quantity;
	
	else if (type == CAPACITY)
		Trenchcoat.Capacity += quantity;
	
	UpdateFreespace(cell_index);
	
	return true;
}

void window_unload(Window *window) {
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
}

void hide_number_window_layer(void)
{
  window_stack_pop(false);
}

void UpdateFreespace(MenuIndex *cell_index)
{
	Trenchcoat.Drug[TOTAL].Quantity = 0;
	for (short i = 1; i < 7; i++)
	{
		Trenchcoat.Drug[TOTAL].Quantity += Trenchcoat.Drug[i].Quantity;
	}	
	Trenchcoat.Freespace = Trenchcoat.Capacity - Trenchcoat.Drug[TOTAL].Quantity;
}

int main(void)
{		
	srand(time(0));	
	window = window_create();
	
	// Create message layer
	message_layer = toast_layer_create(window);

	// Setup the window handlers
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});	

	window_stack_push(window, true);
	
	Intro(NULL);	
	
	app_event_loop();

	destroy_ui();
}