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

	APP_LOG(APP_LOG_LEVEL_DEBUG, "Event_Generator - Dice: %i", Dice);
	//Dice = 14;
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
				string = (char*)malloc((strlen("WILL YOU BUY A SATURDAY NIGHT SPECIAL for $400?") + 1) * sizeof(char));
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
			//toast_layer_show(message_layer, "THERE'S SOME WEED HERE THAT SMELLS LIKE GOOD STUFF!!", LONG_MESSAGE_DELAY, menu_header_heights[menu_number]);	
			X = 1;

			if (X == 1)
			{
				toast_layer_show(message_layer, "YOU HALLUCINATE ON THE WILDEST TRIP OF YOUR LIFE, STUMBLE ON TO THE SUBWAY TRACKS AND GET CREAMED BY A TRAIN.",
												 LONG_MESSAGE_DELAY, menu_header_heights[menu_number]);
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
				string = (char*)malloc((strlen("YOU FOUND %i UNITS OF COCAINE ON A DEAD DUDE IN THE SUBWAY!!!") + 1) * sizeof(char));
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

				string = (char*)malloc((strlen("OFFICER HARDASS AND %i OF HIS DEPUTIES ARE AFTER YOU!") + 1) * sizeof(char));
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
			toast_layer_show(message_layer, "YOU'VE BEEN KILLED!!!\n", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
		}
		return;
	}
}

void Cop_187(MenuIndex *cell_index)
{		
	toast_layer_show(message_layer, "HOLY SHIT! YOU KILLED ALL OF THEM!!!", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
	X = (rand() % 1250 + 750);
	Money.Cash += X;
	string = (char*)malloc((strlen("YOU FOUND $2000 ON OFFICER HARDASS!!") + 1) * sizeof(char));
	snprintf(string,  (strlen("YOU FOUND $2000 ON OFFICER HARDASS!!") + 1) * sizeof(char), "YOU FOUND %u DOLLARS ON OFFICER HARDASS!!", X);
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

	string = (char*)malloc((strlen("GAME OVER!\n\nYOUR SCORE:\n 100%%") + 1) * sizeof(char));
	snprintf(string, ((strlen("GAME OVER!\n\nYOUR SCORE:\n 100%%") + 1) * sizeof(char)),
					 "GAME OVER!\n\nYOUR SCORE:\n %i%%", Score);
	toast_layer_show(message_layer, string, LONG_MESSAGE_DELAY, 0);// V = Total Score
	free(string);
	//V = Input("PLAY AGAIN?",2,"YES","NO");

	//if (V==1)
	//	Intro();

	app_timer_register(5000, Exit, NULL);
}

void Exit(void *context)
{
	toast_layer_show(message_layer, "THANKS FOR PLAYING! REMEMBER TO WATCH YOUR BACK. PEACE!!!", LONG_MESSAGE_DELAY, 0);
	window_stack_pop_all(true);
	window_destroy(window);
}

void Intro(MenuIndex *cell_index)
{
	toast_layer_show(message_layer, "THE ORIGINAL\nFOR\npebbleOS\nVERSION 1.0\nBY A.CLYMER", SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
	
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
		
	Trenchcoat.Drug[TOTAL].Name					= "TOTAL";
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
	//APP_LOG(APP_LOG_LEVEL_INFO, "Input() = %d", value);
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
		case 9:		return 2;		break;
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
		case 9:		return MENU_CELL_BASIC_HT * 2;
		default:	return MENU_CELL_BASIC_HT; 
	}
}

// Here we draw what each header is
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data)
{
	int len = ((strlen("9999K") + 1) * sizeof(char));
	
	// Deterine which section we're working with
	switch(menu_number)
	{
		// Main Menu Header
		case 0:
		if (Day == 1)
			menu_header_simple_icon_draw(ctx, cell_layer, "DRUG WARS", game_icon);
		else
		{
		string = (char*)malloc((strlen(" DAY 00") + 1) * sizeof(char));
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
		string = (char*)malloc((strlen("FREESPACE 999") + 1) * sizeof(char));
		snprintf(string, ((strlen("FREESPACE 999") + 1) * sizeof(char)), "FREESPACE %u", Trenchcoat.Freespace);
		menu_header_draw(ctx, cell_layer, menu_items[menu_number].title, string, menu_icons[menu_number]);
		free(string);
		break;
		
		// Buy Menu Header
		case 3:
		string = (char*)malloc((strlen("BUY    $") * sizeof(char) + len));
		switch(LOG10(Money.Cash))
			{
				default:	snprintf(string, 17 * sizeof(char), "%s",					"BUY IT ALL!");					break;
				case 9:		snprintf(string, 17 * sizeof(char), "BUY $%uM", 	Money.Cash / 1000000); 	break;
				case 8:		snprintf(string, 17 * sizeof(char), "BUY  $%uM", 	Money.Cash / 1000000); 	break;
				case 7:		snprintf(string, 17 * sizeof(char), "BUY   $%uM", Money.Cash / 1000000); 	break;
				case 6:		snprintf(string, 17 * sizeof(char), "BUY $%uK",		Money.Cash / 1000);		 	break;
				case 5:		snprintf(string, 17 * sizeof(char), "BUY  $%uK", 	Money.Cash / 1000);			break;
				case 4:		snprintf(string, 17 * sizeof(char), "BUY   $%uK", Money.Cash / 1000);			break;
				case 3:		snprintf(string, 17 * sizeof(char), "BUY  $%u", 	Money.Cash);						break;
				case 2:		snprintf(string, 17 * sizeof(char), "BUY   $%u", 	Money.Cash);						break;
				case 1:		snprintf(string, 17 * sizeof(char), "BUY    $%u", Money.Cash);						break;
				case 0:		snprintf(string, 17 * sizeof(char), "BUY     $%u",Money.Cash);						break;
			}
		
		menu_header_simple_icon_draw(ctx, cell_layer, string, menu_icons[menu_number]);
		free(string);
		break;
	
		// Sell Menu Header
		case 4:
		string = (char*)malloc((strlen("SELL   $") * sizeof(char) + len));
		switch(LOG10(Money.Cash))
			{
				default:	snprintf(string, 17 * sizeof(char), "%s",					"FOR THE WIN!!!");			break;
				case 9:		snprintf(string, 17 * sizeof(char), "SELL$%uM", 	Money.Cash / 1000000); 	break;
				case 8:		snprintf(string, 17 * sizeof(char), "SELL $%uM", 	Money.Cash / 1000000); 	break;
				case 7:		snprintf(string, 17 * sizeof(char), "SELL  $%uM", Money.Cash / 1000000); 	break;
				case 6:		snprintf(string, 17 * sizeof(char), "SELL$%uK",		Money.Cash / 1000); 		break;
				case 5:		snprintf(string, 17 * sizeof(char), "SELL $%uK", 	Money.Cash / 1000);			break;
				case 4:		snprintf(string, 17 * sizeof(char), "SELL  $%uK", Money.Cash / 1000);			break;
				case 3:		snprintf(string, 17 * sizeof(char), "SELL $%u", 	Money.Cash);						break;
				case 2:		snprintf(string, 17 * sizeof(char), "SELL  $%u", 	Money.Cash);						break;
				case 1:		snprintf(string, 17 * sizeof(char), "SELL   $%u", Money.Cash);						break;
				case 0:		snprintf(string, 17 * sizeof(char), "SELL    $%u",Money.Cash);						break;
			}
		
		menu_header_simple_icon_draw(ctx, cell_layer, string, menu_icons[menu_number]);
		free(string);
		break;
	
		// Subway Menu Header
		case 5:
		string = (char*)malloc((strlen("STATEN ISLAND") + 1) * sizeof(char));
		strcpy(string, locations[CurrentCity]);
		menu_header_draw(ctx, cell_layer, menu_items[menu_number].title, string, menu_icons[menu_number]);
		free(string);
		break;
		
		// Loan Shark Menu Header
		case 6:
		string = (char*)malloc((strlen("DEBT   $") * sizeof(char) + len));
		switch(LOG10(Money.Debt))
			{
				default:	snprintf(string, 17 * sizeof(char), "%s",						"YOU'RE DEAD!!");				break;
				case 9:		snprintf(string, 17 * sizeof(char), "DEBT $%uM", 		Money.Debt / 1000000); 	break;
				case 8:		snprintf(string, 17 * sizeof(char), "DEBT  $%uM", 	Money.Debt / 1000000); 	break;
				case 7:		snprintf(string, 17 * sizeof(char), "DEBT   $%uM",	Money.Debt / 1000000); 	break;
				case 6:		snprintf(string, 17 * sizeof(char), "DEBT $%uK", 		Money.Debt / 1000); 		break;
				case 5:		snprintf(string, 17 * sizeof(char), "DEBT  $%uK", 	Money.Debt / 1000);			break;
				case 4:		snprintf(string, 17 * sizeof(char), "DEBT   $%uK",	Money.Debt / 1000);			break;
				case 3:		snprintf(string, 17 * sizeof(char), "DEBT  $%u", 		Money.Debt);						break;
				case 2:		snprintf(string, 17 * sizeof(char), "DEBT   $%u", 	Money.Debt);						break;
				case 1:		snprintf(string, 17 * sizeof(char), "DEBT    $%u",	Money.Debt);						break;
				case 0:		snprintf(string, 17 * sizeof(char), "DEBT     $%u", Money.Debt);						break;
			}
		
		menu_header_draw(ctx, cell_layer, menu_items[menu_number].title, string, menu_icons[menu_number]);
		free(string);
		break;
		
		// Bank Menu Header
		case 7:
		string = (char*)malloc((strlen("BALANCE $") * sizeof(char) + len));
		switch(LOG10(Money.Balance))
			{
				default:	snprintf(string, 17 * sizeof(char), "%s",							"YOU'RE DEAD!!");						break;
				case 9:		snprintf(string, 17 * sizeof(char), "BALANCE $%uM", 	Money.Balance / 1000000); 	break;
				case 8:		snprintf(string, 17 * sizeof(char), "BALANCE  $%uM", 	Money.Balance / 1000000); 	break;
				case 7:		snprintf(string, 17 * sizeof(char), "BALANCE   $%uM", Money.Balance / 1000000); 	break;
				case 6:		snprintf(string, 17 * sizeof(char), "BALANCE $%uK", 	Money.Balance / 1000);		 	break;
				case 5:		snprintf(string, 17 * sizeof(char), "BALANCE  $%uK", 	Money.Balance / 1000);			break;
				case 4:		snprintf(string, 17 * sizeof(char), "BALANCE   $%uK", Money.Balance / 1000);			break;
				case 3:		snprintf(string, 17 * sizeof(char), "BALANCE $%u", 		Money.Balance);							break;
				case 2:		snprintf(string, 17 * sizeof(char), "BALANCE  $%u", 	Money.Balance);							break;
				case 1:		snprintf(string, 17 * sizeof(char), "BALANCE   $%u", 	Money.Balance);							break;
				case 0:		snprintf(string, 17 * sizeof(char), "BALANCE    $%u", Money.Balance);							break;
			}
		
		menu_header_draw(ctx, cell_layer, menu_items[menu_number].title, string, menu_icons[menu_number]);
		free(string);
		break;
		
		// Chased Menu Header
		case 8:
		string = (char*)malloc((strlen(chased_menu[1]) + 1) * sizeof(char));
		snprintf(string, ((strlen(chased_menu[1]) + 1) * sizeof(char)), chased_menu[1], Cops);
		menu_header_draw(ctx, cell_layer, chased_menu[0], string, menu_icons[menu_number]);
		free(string);
		break;	
		
		// Congirm Menu Header
		case 9:
		menu_header_simple_draw(ctx, cell_layer, data);
		break;
	}
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data)
{
	int len = (strlen("9999K") + 1) * sizeof(char);
	switch(menu_number)
	{
		// Draw Main Menu
		default:
		string = (char*)malloc( (strlen(menu_items[cell_index->row + 1].title) + 1) * sizeof(char));
		snprintf(string, (strlen(menu_items[cell_index->row + 1].title) + 1) * sizeof(char), "%s", menu_items[cell_index->row + 1].title);
		break;

		// Draw Prices Menu
		case 1:
		string = (char*)malloc((strlen(Trenchcoat.Drug[cell_index->row].Name) + 9) * sizeof(char));
		snprintf(string,((strlen(Trenchcoat.Drug[cell_index->row].Name) + 9) * sizeof(char)), "%s  $%u",
						 Trenchcoat.Drug[cell_index->row].Name,
						 Trenchcoat.Drug[cell_index->row].Price);
		break;

		// Draw Trenchcoat Menu
		case 2:
		string = (char*)malloc(strlen(trenchcoat_items[cell_index->row]) * sizeof(char) + len);
		switch(cell_index->row)
		{			
			case 1:
			switch(LOG10(Money.Cash))
			{
				default:	snprintf(string, 17 * sizeof(char), "%s",							"MORE $ THAN DOG");			break;
				case 9:		snprintf(string, 17 * sizeof(char), "CASH    $%uM", 	Money.Cash / 1000000); 	break;
				case 8:		snprintf(string, 17 * sizeof(char), "CASH     $%uM", 	Money.Cash / 1000000); 	break;
				case 7:		snprintf(string, 17 * sizeof(char), "CASH      $%uM", Money.Cash / 1000000); 	break;
				case 6:		snprintf(string, 17 * sizeof(char), "CASH    $%uK", 	Money.Cash / 1000);		 	break;
				case 5:		snprintf(string, 17 * sizeof(char), "CASH     $%uK", 	Money.Cash / 1000);			break;
				case 4:		snprintf(string, 17 * sizeof(char), "CASH      $%uK", Money.Cash / 1000);			break;
				case 3:		snprintf(string, 17 * sizeof(char), "CASH     $%u", 	Money.Cash);						break;
				case 2:		snprintf(string, 17 * sizeof(char), "CASH      $%u", 	Money.Cash);						break;
				case 1:		snprintf(string, 17 * sizeof(char), "CASH       $%u", Money.Cash);						break;
				case 0:		snprintf(string, 17 * sizeof(char), "CASH        $%u", Money.Cash);						break;
			}
			break;

			case 2:
			value = Trenchcoat.Drug[TOTAL].Quantity;
			snprintf(string, ((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char)), trenchcoat_items[cell_index->row], value);
			break;

			case 3:
			value = Trenchcoat.Guns;
			snprintf(string, ((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char)), trenchcoat_items[cell_index->row], value);
			break;

			case 4:
			value = Damage;
			snprintf(string, ((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char)), trenchcoat_items[cell_index->row], value);
			break;

			case 5:
			value = Trenchcoat.Capacity;
			snprintf(string, ((strlen(trenchcoat_items[cell_index->row]) + 1) * sizeof(char)), trenchcoat_items[cell_index->row], value);
			break;
		}
		
		break;
		
		// Draw Buy/Sell Menu
		case 3:
		case 4:
		string = (char*)malloc((strlen(Trenchcoat.Drug[cell_index->row].Name) + 10) * sizeof(char));
		snprintf(string,((strlen(Trenchcoat.Drug[cell_index->row].Name) + 10) * sizeof(char)), "%s     %u",
						 Trenchcoat.Drug[cell_index->row].Name,
						 Trenchcoat.Drug[cell_index->row].Quantity);
		break;

		// Draw Subway Menu
		case 5:
		if (cell_index->row >= CurrentCity)
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
			snprintf(string, (strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char), chased_menu[cell_index->row + 2], Trenchcoat.Guns);
			break;

			case 1:
			string = (char*)malloc((strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char));
			snprintf(string, (strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char), chased_menu[cell_index->row + 2], Damage);
			break;

			default:
			string = (char*)malloc((strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char));
			strcpy(string, chased_menu[cell_index->row + 2]);
			break;
		}
		break;
		
		case 9:
		if (cell_index->row > 0)
		{
			string = (char*)malloc((strlen(confirm_menu[cell_index->row]) + 1) * sizeof(char));
			snprintf(string, (strlen(confirm_menu[cell_index->row]) + 1) * sizeof(char), "%s", confirm_menu[cell_index->row]);
		}	
		break;
	}
	
	if (menu_number > 0 && menu_number < 8 && cell_index->row == 0)
		menu_cell_simple_icon_draw(ctx, cell_layer, "BACK", menu_icons[0]);	
	else if (menu_number == 0)
		menu_cell_simple_icon_draw(ctx, cell_layer, string, menu_icons[cell_index->row + 1]);
	else
		menu_cell_simple_draw(ctx, cell_layer, string);
	
	free(string);
}

// Here we capture when a user selects a menu item
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{	
	int heap = heap_bytes_free();
	APP_LOG(APP_LOG_LEVEL_INFO, "Heap Bytes Free: %i", heap);
	
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
			if (Money.Cash / Trenchcoat.Drug[cell_index->row].Price < Trenchcoat.Freespace)
			{
				format = (char*)malloc((strlen("AFFORD 999") + 1) * sizeof(char));
				snprintf(format, (strlen("AFFORD 999") + 1) * sizeof(char), "AFFORD %i",
								 Money.Cash / Trenchcoat.Drug[cell_index->row].Price);
			}
			else
			{
				format = (char*)malloc((strlen("CARRY") + 1) * sizeof(char));
				snprintf(format, (strlen("CARRY 999") + 1) * sizeof(char), "CARRY %i",
								 Trenchcoat.Freespace);
			}				
			string = (char*)malloc((strlen("COCAINE\nYOU CAN AFFORD 999\nYOU HAVE 000") + 1) * sizeof(char));
			snprintf(string, ((strlen("COCAINE\nYOU CAN AFFORD 999\nYOU HAVE 000") + 1) * sizeof(char)),
							 "%s\nYOU CAN %s\nYOU HAVE %i", Trenchcoat.Drug[cell_index->row].Name, format,
							 Trenchcoat.Drug[cell_index->row].Quantity);
			toast_layer_show(message_layer, string, SHORT_MESSAGE_DELAY, menu_header_heights[menu_number]);
			free(format);
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
						string = (char*)malloc((strlen("BUY HOW MUCH ") + strlen(Trenchcoat.Drug[cell_index->row].Name) + 2) * sizeof(char));
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
						string = (char*)malloc((strlen("SELL HOW MUCH ") + strlen(Trenchcoat.Drug[cell_index->row].Name) + 2) * sizeof(char));
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
					
						APP_LOG(APP_LOG_LEVEL_DEBUG, "Current City: %i", CurrentCity);
					break;
				}
				else
				{
					if (cell_index->row == 1)
					{			
						string = (char*)malloc((strlen("HOW MUCH TO PAY?") + 1) * sizeof(char));
						strcpy(string, "HOW MUCH TO PAY?");
						Num_Input(string, Money.Debt < Money.Cash ? Money.Debt : Money.Cash - 100, 0, 100, cell_index);
						free(string);
					}
					if (cell_index->row == 2)
					{			
						string = (char*)malloc((strlen("HOW MUCH TO BORROW?") + 1) * sizeof(char));
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
						string = (char*)malloc((strlen("HOW MUCH TO WITHDRAW?") + 1) * sizeof(char));
						strcpy(string, "HOW MUCH TO WITHDRAW?");
						Num_Input(string, Money.Balance, 0, 100, cell_index);
						free(string);
					}
					if (cell_index->row == 2)
					{			
						string = (char*)malloc((strlen("HOW MUCH TO DEPOSIT?") + 1) * sizeof(char));
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
			
			case 9:
			APP_LOG(APP_LOG_LEVEL_INFO, "Yes/No");
			//data(void);
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
	
	window_stack_pop(true);
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
	*num_window_context = *cell_index;
	number_window_set_label(number_window, text);
	number_window_set_max(number_window, high);
	number_window_set_min(number_window, low);
	number_window_set_step_size(number_window, delta);
	number_window_set_value(number_window, 0);
  window_stack_push(number_window_get_window(number_window), true);
}

void BuyDrugs(int32_t howMany, MenuIndex *cell_index)
{	
	APP_LOG(APP_LOG_LEVEL_INFO,"Buying %li units of %s", howMany, Trenchcoat.Drug[cell_index->row].Name);
	Trenchcoat.Drug[cell_index->row].Quantity 	+= howMany;
	Trenchcoat.Freespace												-= howMany;
	Money.Cash 																	-= howMany * Trenchcoat.Drug[cell_index->row].Price;
}

void SellDrugs(int32_t howMany, MenuIndex *cell_index)
{
	APP_LOG(APP_LOG_LEVEL_INFO,"Selling %li units of %s", howMany, Trenchcoat.Drug[cell_index->row].Name);
	Trenchcoat.Drug[cell_index->row].Quantity 	-= howMany;
	Trenchcoat.Freespace												+= howMany;
	Money.Cash 																	+= howMany * Trenchcoat.Drug[cell_index->row].Price;
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
  window_stack_pop(true);
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
	num_window_context = malloc(sizeof(MenuIndex));
	number_window = number_window_create(NULL, (NumberWindowCallbacks) 
																			 {.selected 		= number_window_selected_callback,
																				.incremented 	= number_window_incremented_callback,
																				.decremented 	= number_window_decremented_callback
																			 }, num_window_context);
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

int LOG10(int val)
{
	int count = 0;
	val /= 10;
	while (val >= 1)
	{
		val /= 10;
		count++;
	}
	APP_LOG(APP_LOG_LEVEL_INFO, "LOG10 %i", count);
	return count;
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
	title_bounds.origin.y 	-= 3;
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
	title_bounds.origin.y			-= 3;
	subtitle_bounds.origin.x 	= title_bounds.origin.x;
	subtitle_bounds.origin.y	= subtitle_bounds.size.h / 2;
	title_bounds.origin.x			-= 2;
  graphics_draw_text(ctx, title, header_font, title_bounds, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
	graphics_draw_text(ctx, subtitle, cell_font, subtitle_bounds, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
}

// Menu Row Draw function for Title only
void menu_cell_simple_draw(GContext* ctx, const Layer *cell_layer, const char *title)
{
  graphics_context_set_text_color(ctx, GColorBlack);
	GRect titleOrigin = layer_get_bounds(cell_layer);
	titleOrigin.origin.y -= 3;
  graphics_draw_text(ctx, title, cell_font, titleOrigin, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
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