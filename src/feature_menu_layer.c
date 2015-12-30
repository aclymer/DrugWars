#include "external_variables.h"
#include "instructions_layer.h"
#include "feature_menu_layer.h"
#include "ToastLayer.h"
#include <math.h>

static Window 		  *window;
static TextLayer	  *number_window_value_text_layer;
static TextLayer	  *number_window_title_text_layer;
static NumberWindow	*number_window;
ToastLayer 			    *message_layer;
SETTINGS_DATA 		  Settings;
PLAYER_DATA 		    Player;

// Main Game Event Generation fxn
void Event_Generator(MenuIndex *cell_index)
{
  Player.Dice++;//Player.Dice       = (rand() % 21);

  Player.Trenchcoat.Drug[COCAINE].Price 	= (rand() % 12001	+ 16000	)		    ;
  Player.Trenchcoat.Drug[HEROINE].Price 	= (rand() % 7001	+ 5000	)		    ;
  Player.Trenchcoat.Drug[ACID]   .Price 	= (rand() % 35		+ 10	  ) * 100	;
  Player.Trenchcoat.Drug[WEED]   .Price 	= (rand() % 43		+ 33	  ) * 10	;
  Player.Trenchcoat.Drug[SPEED]  .Price	 	= (rand() % 16		+ 7		  ) * 10	;
  Player.Trenchcoat.Drug[LUDES]  .Price	 	= (rand() % 5		  + 1		  ) * 10	;

  switch(Player.Dice)
  {
    case 1:
    toast_layer_show(message_layer, "WEED PRICES HAVE BOTTOMED OUT!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
    Player.Trenchcoat.Drug[LUDES].Price = rand() % 9 + 1;
    break;

    case 2:
    toast_layer_show(message_layer, "WEED PRICES HAVE BOTTOMED OUT!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
    Player.Trenchcoat.Drug[WEED].Price = (rand() % 28 + 13	) * 10	;
    break;

    case 3:
    toast_layer_show(message_layer, "PIGS ARE SELLING CHEAP HEROINE FROM LAST WEEK'S RAID!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
    Player.Trenchcoat.Drug[HEROINE].Price = rand() % 1151 + 850;
    break;

    case 4:
    case 5:
    toast_layer_show(message_layer, "ADDICTS ARE BUYING HEROINE AT OUTRAGEOUS PRICES!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
    Player.Trenchcoat.Drug[HEROINE].Price = rand() % 25001 + 18000;
    break;

    case 6:
    case 7:
    toast_layer_show(message_layer, "PIGS MADE A BIG COKE BUST!\nPRICES ARE OUTRAGEOUS!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
    Player.Trenchcoat.Drug[COCAINE].Price = rand() % 30001 + 28000;
    break;

    case 8:
    if (Settings.vibrate) vibes_short_pulse();
    for (X = 1; X < 8; X++)
    {
      if (Player.Trenchcoat.Drug[X].Quantity > 0)
        break;
    }
    if (X == 7)
    {
      string = malloc((strlen(messages[2]) + 6) * sizeof(char));
      snprintf(string, (strlen(messages[2]) + 6) * sizeof(char),
               messages[2],
               (int) (Player.Money.Cash / 3 + 1 / 2));
    }
    else if (X < 7)
    {
      string = malloc((strlen(messages[3]) + 13) * sizeof(char));
      snprintf(string, (strlen(messages[3]) + 13) * sizeof(char),  messages[3],
               (int) (Player.Money.Cash / 3 + 1 / 2),
               (int) (Player.Trenchcoat.Drug[X].Quantity / 3 + 1 / 2),
               drug_names[X]);

      Player.Trenchcoat.Drug[X].Quantity -= (int) (Player.Trenchcoat.Drug[X].Quantity / 3 + 1 / 2);
    }
    toast_layer_show(message_layer, string, PUNISHMENT_DELAY, menu_header_heights[Player.MenuNumber]);
    safe_free(string);
    Player.Money.Cash -= (int) (Player.Money.Cash / 3 + 1 / 2);
    UpdateFreespace(cell_index);
    break;

    case 9:
    case 10:
    case 11:
    if (Player.Trenchcoat.Drug[TOTAL].Quantity >= 30)
    {		
      if (Player.Dice == 9)	Player.Cops = 1;
      if (Player.Dice == 10)	Player.Cops = 3;
      if (Player.Dice == 11)	Player.Cops = 4;
      if (Settings.vibrate) vibes_double_pulse();

      string = malloc((strlen(messages[(Player.Cops > 1 ? 4 : 5)]) + 1) * sizeof(char));
      if (Player.Cops > 1)
        snprintf(string, (strlen(messages[4]) + 1) * sizeof(char), messages[4], Player.Cops - 1);
      else
        strcpy(string, messages[5]);
      Player.MenuNumber = 8;
      toast_layer_show(message_layer, string, SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
      safe_free(string);
      menu_layer_reload_data(home_menu_layer);
    }	
    break;

    case 12:
    case 13:
    X = rand() % 3 + 1;
    // If there is room for ammo and the gun (if you don't already have the gun), then offer
    if (Player.Money.Cash >= Player.Trenchcoat.Guns[X].Price + 100
        && Player.Trenchcoat.Freespace >= Player.Trenchcoat.Guns[X].Capacity
        + (Player.Trenchcoat.Guns[X].Quantity > 0 ? 0 : 5))
    {
      confirm_header = malloc((strlen(messages[6]) + strlen(gun_names[X]) + 1) * sizeof(char));
      snprintf(confirm_header,
               (strlen(messages[6]) + strlen(gun_names[X]) + 1) * sizeof(char),
               (Player.Trenchcoat.Guns[X].Quantity > 0 ? messages[6] : messages[7]),
               gun_names[X],
               Player.Trenchcoat.Guns[X].Price);
      p_MenuCallbackContext[0] = NULL;
      p_MenuCallbackContext[1] = &Buy_Gun;
      Player.Trenchcoat.Guns[TOTAL].Damage = X;
      Player.MenuNumber = 9;
      menu_layer_reload_data(home_menu_layer);
    }
    break;

    case 14:
    toast_layer_show(message_layer, "THERE'S SOME WEED HERE THAT SMELLS DANK!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);	
    confirm_header = malloc((strlen("WANNA SMOKE IT?") + 1) * sizeof(char));
    strcpy(confirm_header, "WANNA SMOKE IT?");
    p_MenuCallbackContext[0] = NULL;
    p_MenuCallbackContext[1] = &Smoke_It;
    Player.MenuNumber = 9;
    menu_layer_reload_data(home_menu_layer);
    break;

    case 15:
    if (Player.Money.Cash >= 300)
    {
      confirm_header = malloc((strlen(messages[8]) + 2) * sizeof(char));
      snprintf(confirm_header, (strlen(messages[8]) + 2) * sizeof(char), messages[8], Player.Trenchcoat.Capacity * 2);
      p_MenuCallbackContext[0] = NULL;
      p_MenuCallbackContext[1] = &Buy_Trenchcoat;
      Player.MenuNumber = 9;
      menu_layer_reload_data(home_menu_layer);
    }
    break;

    case 16:
    if (Player.Trenchcoat.Freespace >= 8)
    {
      X = rand() % 6 + 1; // [1-6]
      int Y = rand() % 8 + 1; // [1-8]
      string = (char*)malloc((strlen(messages[9]) + 6) * sizeof(char));
      snprintf(string,
               (strlen(messages[9]) + 6) * sizeof(char),
               messages[9],
               Y,
               drug_names[X]
              );				
      toast_layer_show(message_layer, string, LONG_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
      safe_free(string);
      Player.Trenchcoat.Drug[X].Quantity += Y;
      UpdateFreespace(cell_index);
      return;
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
  string = malloc((strlen(messages[0]) + 1) * sizeof(char));
  snprintf(string, (strlen(messages[0]) + 1) * sizeof(char), messages[0], version);
  toast_layer_show(message_layer, string, SHORT_MESSAGE_DELAY, 0);
  app_timer_register(SHORT_MESSAGE_DELAY, &Show_Instructions, NULL);
  safe_free(string);
  
  Player.Cops									                = 0;
  Player.Health								                = 50;
  Player.Day									                = 1;
  Player.CurrentCity													= BRONX;
  Player.Damage																= 0;

  Player.Money.Cash 							 						= 2000;
  Player.Money.Balance				          		  = 0;
  Player.Money.Debt 							 					  = 5000;

  Player.Trenchcoat.Price						          = 200;
  Player.Trenchcoat.Capacity	 				        = 100;
  Player.Trenchcoat.Freespace					        = 100;	

  Player.Trenchcoat.Drug[TOTAL].Price			    = 0;
  Player.Trenchcoat.Drug[TOTAL].Quantity		  = 0;

  Player.Trenchcoat.Drug[COCAINE].Price		    = rand() % 12001 + 16000;
  Player.Trenchcoat.Drug[COCAINE].Quantity	  = 0;

  Player.Trenchcoat.Drug[HEROINE].Price		    = rand() % 7001 + 5000;
  Player.Trenchcoat.Drug[HEROINE].Quantity	  = 0;

  Player.Trenchcoat.Drug[ACID].Price			    = (rand() % 33 + 10) * 100;
  Player.Trenchcoat.Drug[ACID].Quantity		    = 0;

  Player.Trenchcoat.Drug[WEED].Price			    = (rand() % 43 + 30) * 10;
  Player.Trenchcoat.Drug[WEED].Quantity		    = 0;

  Player.Trenchcoat.Drug[SPEED].Price			    = (rand() % 16 + 7) * 10;
  Player.Trenchcoat.Drug[SPEED].Quantity	  	= 0;

  Player.Trenchcoat.Drug[LUDES].Price			    = (rand() % 5 + 1) * 10;
  Player.Trenchcoat.Drug[LUDES].Quantity	  	= 0;

  Player.Trenchcoat.Guns[TOTAL].Quantity	  	= 0;

  Player.Trenchcoat.Guns[1].Price			      	= 350;
  Player.Trenchcoat.Guns[1].Damage		      	= 25;
  Player.Trenchcoat.Guns[1].Capacity					= 7;
  Player.Trenchcoat.Guns[1].Quantity					= 0;
  Player.Trenchcoat.Guns[1].Ammo							= 0;

  Player.Trenchcoat.Guns[2].Price				      = 400;
  Player.Trenchcoat.Guns[2].Damage			      = 25;
  Player.Trenchcoat.Guns[2].Capacity		    	= 6;
  Player.Trenchcoat.Guns[2].Quantity		    	= 0;
  Player.Trenchcoat.Guns[2].Ammo			      	= 0;

  Player.Trenchcoat.Guns[3].Price			      	= 500;
  Player.Trenchcoat.Guns[3].Damage			      = 25;
  Player.Trenchcoat.Guns[3].Capacity		    	= 9;
  Player.Trenchcoat.Guns[3].Quantity		    	= 0;
  Player.Trenchcoat.Guns[3].Ammo				      = 0;

  UpdateFreespace(cell_index);
  Show_Instructions(NULL);
  return;
}

// Display Instruction Layer
void Show_Instructions(void *data)
{
  confirm_header = malloc((strlen("\nWOULD YOU LIKE TO SEE THE INSTRUCTIONS?") + 1) * sizeof(char));
  strcpy(confirm_header, "\nWOULD YOU LIKE TO SEE THE INSTRUCTIONS?");
  p_MenuCallbackContext[0]                    = NULL;
  p_MenuCallbackContext[1]                    = &show_instructions_layer;
  Player.MenuNumber                           = 9;
  menu_layer_reload_data(home_menu_layer);
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
  APP_LOG(APP_LOG_LEVEL_DEBUG, "free %u", heap_bytes_free());
  APP_LOG(APP_LOG_LEVEL_DEBUG, "heap %u", heap_bytes_used());
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
      menu_header_simple_icon_draw(ctx, cell_layer, "DRUGWARS", game_icon);    
    else if (Player.Day == NUM_DAYS[Settings.days])
      menu_header_simple_icon_draw(ctx, cell_layer, "LAST DAY", game_icon);
    else
    {
      string = (char*)malloc((strlen("DAY 00/00") + 1) * sizeof(char));
      snprintf(string, ((strlen("DAY 00/00") + 1) * sizeof(char)), "DAY %i/%i", Player.Day, NUM_DAYS[Settings.days]);
      menu_header_simple_icon_draw(ctx, cell_layer, string, game_icon);
      safe_free(string);
    }
    break;

    // Prices Menu Header
    case 1:
    menu_header_simple_icon_draw(ctx, cell_layer, menu_items[Player.MenuNumber], menu_icons[Player.MenuNumber]);
    break;

    // Trenchcoat Menu Header
    case 2:
    string = (char*)malloc((strlen("CAPACITY 999") + 1) * sizeof(char));
    snprintf(string, ((strlen("CAPACITY 999") + 1) * sizeof(char)), "CAPACITY %i", Player.Trenchcoat.Capacity);
    menu_header_draw(ctx, cell_layer, menu_items[Player.MenuNumber], string, menu_icons[Player.MenuNumber]);
    safe_free(string);
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
    safe_free(string);
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
    safe_free(string);
    break;

    // Subway Menu Header
    case 5:
    string = (char*)malloc((strlen("STATEN ISLAND") + 1) * sizeof(char));
    strcpy(string, locations[Player.CurrentCity]);
    menu_header_draw(ctx, cell_layer, menu_items[Player.MenuNumber], string, menu_icons[Player.MenuNumber]);
    safe_free(string);
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
    menu_header_draw(ctx, cell_layer, menu_items[Player.MenuNumber], string, menu_icons[Player.MenuNumber]);
    safe_free(string);
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
    menu_header_draw(ctx, cell_layer, menu_items[Player.MenuNumber], string, menu_icons[Player.MenuNumber]);
    safe_free(string);
    break;

    // Chased Menu Header
    case 8:
    string = (char*)malloc((strlen(chased_menu[0]) + 2) * sizeof(char));
    snprintf(string, ((strlen(chased_menu[0]) + 2) * sizeof(char)), chased_menu[0], Player.Cops);
    if (Player.Cops != 1)
      strcat(string, "S");
    menu_header_draw(ctx, cell_layer, string, chased_menu[1], menu_icons[Player.MenuNumber]);
    safe_free(string);
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
  strval = (char*)malloc(11 * sizeof(char));
  switch(Player.MenuNumber)
  {
    // Draw Main Menu
    default:
    string = (char*)malloc( (strlen(menu_items[cell_index->row + 1]) + 1) * sizeof(char));
    snprintf(string, (strlen(menu_items[cell_index->row + 1]) + 1) * sizeof(char), "%s", menu_items[cell_index->row + 1]);
    break;

    // Draw Prices Menu
    case 1:
    string = (char*)malloc((strlen(drug_names[cell_index->row]) + 1) * sizeof(char));
    snprintf(string,((strlen(drug_names[cell_index->row]) + 1) * sizeof(char)), "%s", drug_names[cell_index->row]);
    snprintf(strval, 7 * sizeof(char), "$%i", (int) Player.Trenchcoat.Drug[cell_index->row].Price);
    break;

    // Draw Trenchcoat Menu
    case 2:
    string = (char*)malloc(strlen(trenchcoat_items[cell_index->row]) * sizeof(char) + len);
    strcpy(string, trenchcoat_items[cell_index->row]);
    switch(cell_index->row)
    {			
      case 1:
      snprintf(strval, 4 * sizeof(char), "%3i", Player.Trenchcoat.Drug[TOTAL].Quantity);
      break;

      case 2:
      snprintf(strval, 4 * sizeof(char), "%1i", Player.Trenchcoat.Guns[0].Quantity);
      break;

      case 3:
      snprintf(strval, (6 * sizeof(char)), "%2u",	Player.Trenchcoat.Guns[0].Ammo);
      break;

      case 4:
      snprintf(strval, 11 * sizeof(char), "%2i OF 50", Player.Damage);
      break;

      case 5:
      snprintf(strval, 4 * sizeof(char), "%3u", Player.Trenchcoat.Freespace);
      break;
    }

    break;

    // Draw Buy/Sell Menu
    case 3:
    case 4:
    string = (char*)malloc((strlen(drug_names[cell_index->row]) + 11) * sizeof(char));
    snprintf(string, ((strlen(drug_names[cell_index->row]) + 1) * sizeof(char)), "%s", drug_names[cell_index->row]);
    snprintf(strval, 6 * sizeof(char), "%5u", Player.Trenchcoat.Drug[cell_index->row].Quantity);
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
      snprintf(strval, (6 * sizeof(char)), "%2i",	Player.Trenchcoat.Guns[0].Ammo);
      break;

      case 1:
      string = (char*)malloc((strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char));
      snprintf(string, (strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char), chased_menu[cell_index->row + 2], Player.Trenchcoat.Guns[0].Quantity);
      snprintf(strval, 6 * sizeof(char), "%1i", Player.Trenchcoat.Guns[0].Quantity);
      break;

      case 2:
      string = (char*)malloc((strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char));
      snprintf(string, (strlen(chased_menu[cell_index->row + 2]) + 1) * sizeof(char), chased_menu[cell_index->row + 2], Player.Damage);
      snprintf(strval, 9 * sizeof(char), "%2i OF 50", Player.Damage);
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

  switch (Player.MenuNumber)
  {
    case 0:	
      menu_cell_simple_icon_draw(ctx, cell_layer, string, menu_icons[cell_index->row + 1]);
    break;
    case 1:
    case 2:
    case 3:
    case 4: 
    if (cell_index->row == 0)
    {
      menu_cell_simple_icon_draw(ctx, cell_layer, "BACK", menu_icons[0]);
      break;
    }
    else
      menu_cell_value_draw(ctx, cell_layer, string, strval);
    break;
    case 8:
      if (cell_index->row < 3)
        menu_cell_value_draw(ctx, cell_layer, string, strval);
      else
        menu_cell_simple_draw(ctx, cell_layer, string);
    break;
    case 9:
      menu_cell_simple_centered_draw(ctx, cell_layer, string);
    break;
    default:
      if (cell_index->row == 0)
      {
        menu_cell_simple_icon_draw(ctx, cell_layer, "BACK", menu_icons[0]);
        break;
      }
      else
        menu_cell_simple_draw(ctx, cell_layer, string);
    break;
  }

  safe_free(strval);
  safe_free(string);
}

static void menu_selection_will_change_callback(MenuLayer *menu_layer, MenuIndex *new_cell_index, MenuIndex old_cell_index, void *data)
{
  if (toast_layer_is_visible(message_layer))
    *new_cell_index = old_cell_index;
}

// Here we capture when a user selects a menu item
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{	
  if (toast_layer_is_visible(message_layer))
    return;
  
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
        format = (char*)malloc((strlen("CARRY 999") + 1) * sizeof(char));
        snprintf(format, (strlen("CARRY 999") + 1) * sizeof(char), "CARRY %i",
                 Player.Trenchcoat.Freespace);
      }				
      string = (char*)malloc((strlen("COCAINE\nYOU CAN AFFORD 999\nYOU HAVE 000") + 1) * sizeof(char));
      snprintf(string, ((strlen("COCAINE\nYOU CAN AFFORD 999\nYOU HAVE 000") + 1) * sizeof(char)),
               "%s\nYOU CAN %s\nYOU HAVE %3i", drug_names[cell_index->row], format,
               Player.Trenchcoat.Drug[cell_index->row].Quantity);
      toast_layer_show(message_layer, string, SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
      safe_free(format);
      safe_free(string);
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
          Num_Input("BUY HOW MUCH?",  (Player.Money.Cash / Player.Trenchcoat.Drug[cell_index->row].Price > Player.Trenchcoat.Freespace ?
                                      Player.Trenchcoat.Freespace :
                                      Player.Money.Cash / Player.Trenchcoat.Drug[cell_index->row].Price), 0, 1, 0, cell_index
                   );
          return;
        }
      }
      break;

      // Sell Menu
      case 4:
      if (cell_index->row < 7)
      {
        if (Player.Trenchcoat.Drug[cell_index->row].Quantity > 0)
          Num_Input("SELL HOW MUCH?", Player.Trenchcoat.Drug[cell_index->row].Quantity, 0, 1, 0, cell_index);
        else
          toast_layer_show(message_layer, "YOU DON'T HAVE ANYTHING TO SELL!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
        
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

      if (Player.Day > NUM_DAYS[Settings.days])
        Game_Over(cell_index);

      Player.MenuNumber = 0;
      Event_Generator(cell_index);
      menu_layer_set_selected_index(menu_layer, MenuIndex(0, 0), MenuRowAlignCenter, false);
      if (Settings.autosave)
        Save_Game();
      break;

      // Loan Shark
      case 6:			
      if (Player.CurrentCity != BRONX)
      {
        toast_layer_show(message_layer, "THE LOAN SHARK IS IN THE BRONX", LONG_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);

        break;
      }
      else
      {
        if (cell_index->row == 1)
        {			
          int high = (Player.Money.Debt < (Player.Money.Cash - 100) ? Player.Money.Debt : Player.Money.Cash - 100);
          uint8_t delta = EXP(LOG10(high) - 2);
          Num_Input("HOW MUCH TO PAY?", high, 0, (delta > Player.Money.Debt ? Player.Money.Debt : delta), high, cell_index);
        }
        if (cell_index->row == 2)
        {		
          if (Player.Money.Debt >= 50000)
            toast_layer_show(message_layer, "YOU THINK HE'S CRAZY BRO!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
          else
            Num_Input("HOW MUCH TO BORROW?", 50000 - Player.Money.Debt, 0, (1000 > 50000 - Player.Money.Debt ? 50000 - Player.Money.Debt : 1000), 0, cell_index);
          
        }
      }
      break;

      // Bank Menu
      case 7:
      if (Player.CurrentCity != BROOKLYN)
        toast_layer_show(message_layer, "THE BANK IS IN BROOKLYN!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
      else
      {
        if (cell_index->row == 1)
          Num_Input("HOW MUCH TO WITHDRAW?", Player.Money.Balance, 0, EXP(LOG10(Player.Money.Balance) - 2) , 0, cell_index);
        
        if (cell_index->row == 2)
          Num_Input("HOW MUCH TO DEPOSIT?", Player.Money.Cash, 0, EXP(LOG10(Player.Money.Cash / 2) - 2), Player.Money.Cash / 2, cell_index);

      }
      break;

      // Chased Menu
      case 8:
      if (cell_index->row == 3)
      {
        X = rand() % 3;
        if (X == 2)
        {
          Player.MenuNumber = 0;
          if (Player.Cops < 2)
            toast_layer_show(message_layer, "YOU LOST HIM IN AN ALLEY!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
          else
            toast_layer_show(message_layer, "YOU LOST THEM IN AN ALLEY!\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
          
          if (Player.Day > NUM_DAYS[Settings.days])
            Game_Over(cell_index);
          else {
            Player.Cops = 0;
          }
        }
        else {
          if (Player.Cops < 2)
            toast_layer_show(message_layer, "YOU CAN'T SHAKE HIM!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
          else
            toast_layer_show(message_layer, "YOU CAN'T SHAKE THEM!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
          
          app_timer_register(SHORT_MESSAGE_DELAY + TOAST_LAYER_ANIM_DURATION, (void*)Being_Shot, cell_index);
        }
      }
      if (cell_index->row == 4)
      {
        if (Player.Trenchcoat.Guns[0].Quantity == 0)
          toast_layer_show(message_layer, "YOU DON'T HAVE ANY GUNS!\nYOU HAVE TO RUN!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
        else if (Player.Trenchcoat.Guns[0].Ammo == 0)
          toast_layer_show(message_layer, "YOU DON'T HAVE ANY AMMO!\nYOU HAVE TO RUN!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);					
        else
        {
          for(X = 1; X < 4; X++) {  
            if (Player.Trenchcoat.Guns[X].Ammo > 0)
              break;
          }
          Player.Trenchcoat.Guns[X].Ammo--;
          UpdateFreespace(cell_index);
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
              toast_layer_show(message_layer, "YOU KILLED ONE!", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
              app_timer_register(SHORT_MESSAGE_DELAY + TOAST_LAYER_ANIM_DURATION, (void*)Being_Shot, cell_index);
            }
          }
          else
          {
            toast_layer_show(message_layer, "YOU MISSED!\n\n", SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
            app_timer_register(SHORT_MESSAGE_DELAY + TOAST_LAYER_ANIM_DURATION, (void*)Being_Shot, cell_index);
          }
        }
      }
      if (Player.Cops < 1 && Player.Money.Cash >= 1200 && Player.Damage > 0)
      {
        int max_meds = (Player.Damage * 1000 + 200 > Player.Money.Cash ? Player.Money.Cash : Player.Damage * 1000);
        confirm_header = malloc((strlen("WILL YOU PAY $50000 FOR A DOCTOR TO SEW YOU UP?") + 1) * sizeof(char));
        snprintf(confirm_header,
                 (strlen("WILL YOU PAY $50000 FOR A DOCTOR TO SEW YOU UP?") + 1) * sizeof(char),
                 "WILL YOU PAY $%i FOR A DOCTOR TO SEW YOU UP?",
                 max_meds);
        p_MenuCallbackContext[0] = NULL;
        p_MenuCallbackContext[1] = &Doctor;
        Player.MenuNumber = 9;
      }
      break;

      case 9:
      Player.MenuNumber = 0;
      safe_free(confirm_header);
      if (p_MenuCallbackContext[cell_index->row] != NULL)
      {
        p_MenuCallbackContext[cell_index->row](cell_index);
        p_MenuCallbackContext[0] = NULL;
        p_MenuCallbackContext[1] = NULL;
      }
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
  int value = number_window_get_value(number_window);

  switch(Player.MenuNumber)
  {
    case 6:
    case 7:
    memset(number_window_value_text, 0, 10 * sizeof(char));
    if (value > 9999)
    {
      *(number_window_value_text) = '$';
      floatstrcat(number_window_value_text, value, 3);
    }
    else
    {
      snprintf(number_window_value_text, 10 * sizeof(char), "$%i", value);
    }
    text_layer_set_text(number_window_value_text_layer, number_window_value_text);
    break;
    default:
    memset(number_window_value_text, 0, 10 * sizeof(char));
    if (value > 9999)
    {
      *(number_window_value_text) = 32;
      floatstrcat(number_window_value_text, value, 3);
    }
    else
    {
      snprintf(number_window_value_text, 10 * sizeof(char), "%i", value);
    }
    text_layer_set_text(number_window_value_text_layer, number_window_value_text);
    break;
  }
}

void Num_Input(char *text, int high, int low, int delta, int set, MenuIndex *cell_index)
{
  *p_NumWindowContext 		= *cell_index;
  text_layer_set_text(number_window_title_text_layer, text);	
  number_window_set_max(number_window, high);
  number_window_set_min(number_window, low);
  number_window_set_step_size(number_window, delta);
  number_window_set_value(number_window, set);
  number_window_incremented_callback(number_window, (void*)cell_index);	

  Window *number_window_root = number_window_get_window(number_window);
  Layer *number_window_root_layer = window_get_root_layer(number_window_root);

  layer_add_child(number_window_root_layer, text_layer_get_layer(number_window_title_text_layer));
  layer_add_child(number_window_root_layer, text_layer_get_layer(number_window_value_text_layer));
  window_set_background_color(number_window_root, PBL_IF_COLOR_ELSE(GColorLimerick, GColorWhite));
  window_stack_push(number_window_root, true);
}

void BuyDrugs(int howMany, MenuIndex *cell_index)
{	
  Player.Trenchcoat.Drug[cell_index->row].Quantity 	+= howMany;
  Player.Money.Cash 									-= howMany * Player.Trenchcoat.Drug[cell_index->row].Price;
  UpdateFreespace(cell_index);
}

void SellDrugs(int howMany, MenuIndex *cell_index)
{
  Player.Trenchcoat.Drug[cell_index->row].Quantity 	-= howMany;
  Player.Money.Cash 									+= howMany * Player.Trenchcoat.Drug[cell_index->row].Price;
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
  if (Settings.vibrate) vibes_short_pulse();
  toast_layer_show(message_layer, "YOU HALLUCINATE, STUMBLE ON TO THE TRACKS AND GET HIT BY A TRAIN!",
                   LONG_MESSAGE_DELAY, menu_header_heights[0]);
  app_timer_register(LONG_MESSAGE_DELAY + TOAST_LAYER_ANIM_DURATION, (void*)Game_Over, cell_index);
}

void Being_Shot(MenuIndex *cell_index)
{
  format = malloc((strlen("YOU'VE BEEN KILLED!") + 1) * sizeof(char));
  X = rand() % 2;
  if (X == 0)
    if (Player.Cops < 2)
    strcpy(format, "HE MISSED!");
    else
    strcpy(format, "THEY MISSED!");		
    else
  {
    Player.Damage += 3 + 3 * (int)(rand() % Player.Cops);
    if (Player.Damage < 50)	
      strcpy(format, "YOU'VE BEEN HIT!");	
    else
    {
      strcpy(format, "YOU'VE BEEN KILLED!");
      app_timer_register(SHORT_MESSAGE_DELAY + TOAST_LAYER_ANIM_DURATION, (void*)Game_Over, cell_index);
    }
    if (Settings.vibrate) vibes_short_pulse();
  }

  string = malloc((strlen("THEY'RE FIRING AT YOU!\nYOU'VE BEEN KILLED!") + 1) * sizeof(char));
  if (Player.Cops < 2)
    snprintf(string, (strlen("HE'S FIRING AT YOU!\nYOU'VE BEEN KILLED!") + 1) * sizeof(char), "HE'S FIRING AT YOU!\n%s", format);
  else
    snprintf(string, (strlen("THEY'RE FIRING AT YOU!\nYOU'VE BEEN KILLED!") + 1) * sizeof(char), "THEY'RE FIRING AT YOU!\n%s", format);
  safe_free(format);
  toast_layer_show(message_layer, string, SHORT_MESSAGE_DELAY, menu_header_heights[Player.MenuNumber]);
  safe_free(string);
  return;
}

void Cop_187(MenuIndex *cell_index)
{
  if (Settings.vibrate) vibes_short_pulse();
  X = (rand() % 1250 + 750);
  Player.Money.Cash += X;
  string = (char*)malloc((strlen("\nHOLY SHIT! YOU KILLED ALL OF THEM AND FOUND $2000 ON OFFICER HARDASS!") + 1) * sizeof(char));
  if (Player.Dice == 9)
    snprintf(string,  (strlen("\nHOLY SHIT! YOU KILLED HIM AND FOUND $2000 ON HIS CARCASS!") + 1) * sizeof(char), "\nHOLY SHIT! YOU KILLED HIM AND FOUND $%u ON HIS CARCASS!", X);
  else
    snprintf(string,  (strlen("\nHOLY SHIT! YOU KILLED ALL OF THEM AND FOUND $2000 ON OFFICER HARDASS!") + 1) * sizeof(char), "\nHOLY SHIT! YOU KILLED ALL OF THEM AND FOUND $%u ON OFFICER HARDASS!", X);
  toast_layer_show(message_layer, string, LONG_MESSAGE_DELAY, 0);
  safe_free(string);
  if (Player.Money.Cash >= 1200 && Player.Damage > 0)
  {
    int max_meds = (Player.Damage * 1000 + 200 > Player.Money.Cash ? Player.Money.Cash : Player.Damage * 1000);
    confirm_header = malloc((strlen("WILL YOU PAY $50000 FOR A DOCTOR TO SEW YOU UP?") + 1) * sizeof(char));
    snprintf(confirm_header,
             (strlen("WILL YOU PAY $50000 FOR A DOCTOR TO SEW YOU UP?") + 1) * sizeof(char),
             "WILL YOU PAY $%i FOR A DOCTOR TO SEW YOU UP?",
             max_meds);
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
  if (Player.Money.Cash > Player.Damage * 1000 + 200)
  {
    Player.Damage 		= 0;
    Player.Money.Cash -= Player.Damage * 1000;
  }
  else
  {
    Player.Damage 		-= (int) ((Player.Money.Cash - 200) / 1000);
    Player.Money.Cash = 200;
  }
}

void Exit(MenuIndex *cell_index)
{
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
    - Player.Trenchcoat.Guns[TOTAL].Ammo
    - Player.Trenchcoat.Guns[TOTAL].Quantity * 5;
}

void Play_Again(MenuIndex *cell_index)
{
  confirm_header = malloc((strlen("\nPLAY AGAIN?") + 1) * sizeof(char));
  strcpy(confirm_header, "\nPLAY AGAIN?");
  Player.MenuNumber = 9;
  p_MenuCallbackContext[0] = &Exit;
  p_MenuCallbackContext[1] = &Intro;
  menu_layer_reload_data(home_menu_layer);
}

void Game_Over(MenuIndex *cell_index)
{
  Score = Player.Money.Cash + Player.Money.Balance - Player.Money.Debt;
  if (Score <= 0) Score = 0;
  if (persist_exists(HIGH_SCORE_KEY))
    persist_read_data(HIGH_SCORE_KEY, &high_scores, sizeof(high_scores));
  else
    persist_write_data(HIGH_SCORE_KEY, high_scores, sizeof(high_scores));

  if (Score > high_scores[Settings.days])
  {
    high_scores[Settings.days] = Score;
    persist_write_data(HIGH_SCORE_KEY, high_scores, sizeof(high_scores));
    string = (char*)malloc((strlen("GAME OVER!\n\nNEW HIGH SCORE:\n$9999999999") + 1) * sizeof(char));
    snprintf(string, ((strlen("GAME OVER!\n\nNEW HIGH SCORE:\n$9999999999") + 1) * sizeof(char)),
             "GAME OVER!\n\nNEW HIGH SCORE:\n$%i", Score);
  }
  else
  {
    string = (char*)malloc((strlen("GAME OVER!\n\nHIGH SCORE:\n$9999999999\nYOUR SCORE:\n$9999999999") + 1) * sizeof(char));
    snprintf(string, ((strlen("GAME OVER!\n\nHIGH SCORE:\n$9999999999\nYOUR SCORE:\n$9999999999") + 1) * sizeof(char)),
             "GAME OVER!\n\nHIGH SCORE:\n$%i\nYOUR SCORE:\n$%i", high_scores[Settings.days], Score);
  }
  
  psleep(40);
  toast_layer_show(message_layer, string, PUNISHMENT_DELAY, 0);
  safe_free(string);

  // Delete mid-game Player Data Store
  if (persist_exists(PLAYER_DATA_KEY))
    persist_delete(PLAYER_DATA_KEY);

  app_timer_register(PUNISHMENT_DELAY + TOAST_LAYER_ANIM_DURATION, (void*)Play_Again, cell_index);
}

void Save_Game(void)
{
  persist_write_data(PLAYER_DATA_KEY, &Player, sizeof(Player));
  psleep(100);
  persist_write_int(PLAYER_SIZE_KEY, sizeof(Player));
  psleep(100);
  return;
}

static void check_for_saved_game(void)
{	
  // Check for saved game
  if (persist_exists(PLAYER_DATA_KEY) && persist_exists(PLAYER_SIZE_KEY))
  {
    persist_read_data(PLAYER_DATA_KEY, &Player, persist_read_int(PLAYER_SIZE_KEY));
    if (Player.MenuNumber != 8)
    {
      value = Player.MenuNumber;
      confirm_header = malloc((strlen("\nLOAD SAVED GAME?") + 1) * sizeof(char));
      snprintf(confirm_header, (strlen("\nLOAD SAVED GAME?") + 1) * sizeof(char), "%s", "\nLOAD SAVED GAME?");
      p_MenuCallbackContext[0] = &Intro;
      p_MenuCallbackContext[1] = &Load_Game;
      Player.MenuNumber = 9;
    }

    menu_layer_reload_data(home_menu_layer);
  }
  else
    Intro(NULL);	
}

void Load_Game(MenuIndex *cell_index)
{
  if (value == 8)
    Player.MenuNumber = value;
  else
    Player.MenuNumber = 0;

  menu_layer_reload_data(home_menu_layer);

  return;
} 

// This initializes the menu upon window load
void window_load(Window *window)
{
  // Here we load the bitmap assets
  // resource_init_current_app must be called before all asset loading
  menu_icons[0]                                     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RETURN_ICON);
  menu_icons[1]                                     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PRICES_ICON);
  menu_icons[2]                                     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TRENCHCOAT_ICON);
  menu_icons[3]                                     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BUY_ICON);
  menu_icons[4]                                     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SELL_ICON);
  menu_icons[5]                                     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SUBWAY_ICON);
  menu_icons[6]                                     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOAN_SHARK_ICON);
  menu_icons[7]                                     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BANK_ICON);
  menu_icons[8]                                     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DWPD_ICON);
  game_icon										                      = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_GAME_ICON);

  // Set all the callbacks for the menu layer
  menu_layer_set_callbacks(home_menu_layer, NULL, (MenuLayerCallbacks)
                           {
                             .get_num_sections 	    = menu_get_num_sections_callback,
                             .get_num_rows 		      = menu_get_num_rows_callback,
                             .get_header_height     = menu_get_header_height_callback,
                             .get_cell_height	      = menu_get_cell_height_callback,
                             .draw_header 		      = menu_draw_header_callback,
                             .draw_row 			        = menu_draw_row_callback,
                             .select_click 		      = menu_select_callback,
                             .selection_will_change = menu_selection_will_change_callback,
                           });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(home_menu_layer, window);
}

void window_unload(Window *window) 
{
  // Save Player Data
  if (Player.Day <= NUM_DAYS[Settings.days] && Player.Day > 1)
    Save_Game();
  
  if (Player.Day > NUM_DAYS[Settings.days] || Player.Damage >= 50)
    persist_delete(PLAYER_DATA_KEY);
  
  text_layer_destroy(number_window_title_text_layer);
  text_layer_destroy(number_window_value_text_layer);
  
  toast_layer_destroy(message_layer);
  number_window_destroy(number_window);
  menu_layer_destroy(home_menu_layer);

  // Cleanup the menu icons  
  gbitmap_destroy(menu_icons[0]);
  gbitmap_destroy(menu_icons[1]);
  gbitmap_destroy(menu_icons[2]);
  gbitmap_destroy(menu_icons[3]);
  gbitmap_destroy(menu_icons[4]);
  gbitmap_destroy(menu_icons[5]);
  gbitmap_destroy(menu_icons[6]);
  gbitmap_destroy(menu_icons[7]);
  gbitmap_destroy(menu_icons[8]);  
  gbitmap_destroy(game_icon);
  
  safe_free(p_NumWindowContext);
}

static void destroy_ui(void)
{
  window_destroy(window);
}

static void create_ui(void)
{
  window 				= window_create();

  if (window == NULL) return; 
  
  window_set_background_color(window, PBL_IF_COLOR_ELSE(GColorLimerick, GColorWhite));
  
  GRect	bounds = layer_get_frame(window_get_root_layer(window));

  p_NumWindowContext 	= malloc(sizeof(MenuCallback));

  // Create the menu layer
  home_menu_layer 	= menu_layer_create(bounds);
  
  menu_layer_pad_bottom_enable(home_menu_layer, false);
  menu_layer_set_normal_colors(home_menu_layer, PBL_IF_COLOR_ELSE(GColorCeleste, GColorWhite), PBL_IF_COLOR_ELSE(GColorLimerick, GColorBlack));
  menu_layer_set_highlight_colors(home_menu_layer, PBL_IF_COLOR_ELSE(GColorLimerick, GColorLightGray), PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack));

  // Add it to the window for display
  layer_add_child(window_get_root_layer(window), menu_layer_get_layer(home_menu_layer));

  number_window 		= number_window_create(NULL, (NumberWindowCallbacks) 
                                           {.selected 	  = number_window_selected_callback,
                                            .incremented 	= number_window_incremented_callback,
                                            .decremented 	= number_window_incremented_callback
                                           }, p_NumWindowContext);

  // Create Number window title text overlay
  number_window_title_text_layer		= text_layer_create(GRect(0,10,115,133));  
  text_layer_set_background_color(number_window_title_text_layer, PBL_IF_COLOR_ELSE(GColorLimerick, GColorWhite));  
  text_layer_set_font(number_window_title_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(number_window_title_text_layer, GTextAlignmentCenter);

  // Create Number window value text overlay
  number_window_value_text_layer = text_layer_create(GRect(0,70,115,60));  
  text_layer_set_background_color(number_window_value_text_layer, PBL_IF_COLOR_ELSE(GColorLimerick, GColorWhite));  
  text_layer_set_font(number_window_value_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(number_window_value_text_layer, GTextAlignmentCenter);

  number_window_value_text = malloc(10 * sizeof(char));

  // Create message layer
  message_layer 			= toast_layer_create(window);

  // Setup the window handlers
  window_set_window_handlers(window, (WindowHandlers)
                             {
                               .load = window_load,
                               .unload = window_unload,
                             });

  if (persist_exists(SETTINGS_DATA_KEY))
    persist_read_data(SETTINGS_DATA_KEY, &Settings, sizeof(SETTINGS_DATA));
  else
  {
    Settings.vibrate 	= false;
    Settings.autosave	= false;
    Settings.light		= false;
    Settings.days		  = 0;
    
    persist_write_data(SETTINGS_DATA_KEY, &Settings, sizeof(SETTINGS_DATA));
    psleep(40);
  }
  
  window_stack_push(window, true); 
}

static void inbox_received_handler(DictionaryIterator *iter, void *context)
{  
  APP_LOG(APP_LOG_LEVEL_INFO, "Rx: ");
  for (int i = 0; i < 5; i++)
  {
    Tuple *new_tuple = dict_find(iter, i);  
  
    if (new_tuple)
    {
      switch (i)
      {
        case VERSION:
        APP_LOG(APP_LOG_LEVEL_INFO, "v%s", new_tuple->value->cstring);
        version = malloc((strlen(new_tuple->value->cstring) + 1) * sizeof(char));
        strcpy(version, new_tuple->value->cstring);
        break;
    
        case VIBRATE:
        APP_LOG(APP_LOG_LEVEL_INFO, "Vibe:%u", new_tuple->value->uint8);
        ((SETTINGS_DATA*) context)->vibrate = (bool) new_tuple->value->uint8;
        break;
    
        case LIGHT:
        APP_LOG(APP_LOG_LEVEL_INFO, "Light:%u", new_tuple->value->uint8);
        ((SETTINGS_DATA*) context)->light = (bool) new_tuple->value->uint8;
        light_enable(Settings.light);
        break;
    
        case DAYS:
        APP_LOG(APP_LOG_LEVEL_INFO, "Days:%u", new_tuple->value->uint8);
        ((SETTINGS_DATA*) context)->days = (int) new_tuple->value->uint8;
        break;
    
        case AUTOSAVE:
        APP_LOG(APP_LOG_LEVEL_INFO, "Save:%u", new_tuple->value->uint8);
        ((SETTINGS_DATA*) context)->autosave = (bool) new_tuple->value->uint8;
        break;
      }
    }
  }
  persist_write_data(SETTINGS_DATA_KEY, &Settings, sizeof(Settings));
}

void check_version(void)
{
  app_message_register_inbox_received(inbox_received_handler); 
  app_message_open(128, 128);
  app_message_set_context(&Settings);
  version = malloc( 5 * sizeof(char));
  snprintf(version, 5 * sizeof(char), "%i.%i", __pbl_app_info.process_version.major, __pbl_app_info.process_version.minor);
   
  return;
}

int main(void)
{
  srand(time(0));	
  create_ui();
  check_version();
  check_for_saved_game();
  light_enable(Settings.light);
  app_event_loop();
  destroy_ui();
  safe_free(version);
  safe_free(string);
  safe_free(format);
  safe_free(confirm_header);
  
  return 0;
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
  double Y = val / EXP(LOG10(val) - LOG10(val) % 3);
  if (precision > 3) precision = 3;
  str += strlen(str);

  //  start with positive/negative
  if (Y < 0)
  {
    *(str++) = '-';
    Y = -Y;
  }

  //  integer value
  snprintf(str, 10 * sizeof(char), "%d", (int) Y);
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
      if (Y > 0.0)
      {
        Y *= 10.0;
        int d = (int) (Y + (precision == 1 ? 0.5 : 0.0)) + '0';
        *(str++) = (char) d;
        Y -= d;
      }
      else
        break;
    }
  }

  // Add postfix unit
  if (val > 999)
    *(str++) = postfix[LOG10(val) / 3];

  //  terminate
  *str = '\0';	
}

// Menu Header Draw function for Title only
void menu_header_simple_draw(GContext* ctx, const Layer *cell_layer, const char *title)
{
  graphics_context_set_text_color(ctx, GColorBlack);
  GRect titleOrigin = layer_get_bounds(cell_layer);
  graphics_draw_text(ctx, title, HEADER_FONT, titleOrigin, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
}

// Menu Header Draw function for Icon and Title
void menu_header_simple_icon_draw(GContext* ctx, const Layer *cell_layer, const char *title, const GBitmap* bitmap)
{
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  GRect bitmap_bounds 		  = gbitmap_get_bounds(bitmap);
  GRect title_bounds 			  = layer_get_bounds(cell_layer);
  bitmap_bounds.origin.x 		+= (24 - bitmap_bounds.size.w) / 2;
  bitmap_bounds.origin.y		= (title_bounds.size.h - bitmap_bounds.size.h) / 2;
  graphics_draw_bitmap_in_rect(ctx, bitmap, bitmap_bounds);
  title_bounds.origin.x 		= 26 - (26 - bitmap_bounds.size.w) / 2;
  title_bounds.size.w			  -= bitmap_bounds.size.w + bitmap_bounds.origin.x * 2;
  title_bounds.origin.y		  -= 1;
  graphics_draw_text(ctx, title, HEADER_FONT, title_bounds, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorDarkCandyAppleRed, GColorBlack));
  graphics_draw_line(ctx,  (GPoint){.x = 0  , .y = title_bounds.origin.y + title_bounds.size.h},
                           (GPoint){.x = 144, .y = title_bounds.origin.y + title_bounds.size.h});
}

// Menu Header Draw function for Icon, Title, and Subtitle
void menu_header_draw(GContext* ctx, const Layer *cell_layer, const char *title, const char* subtitle, const GBitmap* bitmap)
{
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  GRect bitmap_bounds 		  = gbitmap_get_bounds(bitmap);
  GRect title_bounds 			  = layer_get_bounds(cell_layer);
  // Set Bitmap bounds
  bitmap_bounds.origin.x 		+= 2;
  bitmap_bounds.origin.y		= (title_bounds.size.h - bitmap_bounds.size.h) / 2 + 3;
  graphics_draw_bitmap_in_rect(ctx, bitmap, bitmap_bounds);
  // Set Title bounds
  title_bounds.size.w			  -= bitmap_bounds.size.w + bitmap_bounds.origin.x * 2;
  title_bounds.origin.x 		+= bitmap_bounds.size.w + bitmap_bounds.origin.x * 2;
  title_bounds.origin.y		  = ((layer_get_bounds(cell_layer).size.h - bitmap_bounds.size.h - 24) / 2) + TEXT_ORIGIN_Y_SHIFT;
  // Set SubTitle bounds
  GRect subtitle_bounds 		= title_bounds;
  subtitle_bounds.origin.x 	= title_bounds.origin.x;
  subtitle_bounds.origin.y	= title_bounds.size.h / 2 - 5;
  subtitle_bounds.size.w		= title_bounds.size.w;
  graphics_draw_text(ctx, title, HEADER_FONT, title_bounds, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx, subtitle, CELL_FONT, subtitle_bounds, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
  GRect bar_bounds 			  = layer_get_bounds(cell_layer);
  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorDarkCandyAppleRed, GColorBlack));
  graphics_draw_line(ctx,  (GPoint){.x = 0  , .y = bar_bounds.origin.y + bar_bounds.size.h - 1},
                           (GPoint){.x = 144, .y = bar_bounds.origin.y + bar_bounds.size.h - 1});
}

// Menu Header Draw function for long title only
void menu_header_long_draw(GContext* ctx, const Layer *cell_layer, const char *title)
{
  graphics_context_set_text_color(ctx, GColorBlack);
  GRect titleOrigin         = layer_get_bounds(cell_layer);
  titleOrigin.size.h        = menu_header_heights[Player.MenuNumber];
  graphics_draw_text(ctx, title, CONFIRM_FONT, titleOrigin, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  GRect title_bounds 			  = layer_get_bounds(cell_layer);
  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorDarkCandyAppleRed, GColorBlack));
  graphics_draw_line(ctx,  (GPoint){.x = 0  , .y = title_bounds.origin.y + title_bounds.size.h - 1},
                           (GPoint){.x = 144, .y = title_bounds.origin.y + title_bounds.size.h - 1});
}

// Menu Row Draw function for Title only
void menu_cell_simple_draw(GContext* ctx, const Layer *cell_layer, const char *title)
{
  graphics_context_set_text_color(ctx, GColorBlack);
  GRect titleOrigin      = layer_get_bounds(cell_layer);
  titleOrigin.origin.y   += TEXT_ORIGIN_Y_SHIFT;
  graphics_draw_text(ctx, title, CELL_FONT, titleOrigin, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
}

// Menu Row Draw function Rt_Aligned
void menu_cell_value_draw(GContext* ctx, const Layer *cell_layer, const char *title, const char* value)
{
  menu_cell_simple_draw(ctx, cell_layer, title);
  graphics_context_set_text_color(ctx, GColorBlack);
  GRect valueOrigin      = layer_get_bounds(cell_layer);
  valueOrigin.origin.y   += TEXT_ORIGIN_Y_SHIFT;
  graphics_draw_text(ctx, value, CELL_FONT, valueOrigin, GTextOverflowModeFill, GTextAlignmentRight, NULL);
}

// Menu Row Draw function for Title only (Centered)
void menu_cell_simple_centered_draw(GContext* ctx, const Layer *cell_layer, const char *title)
{
  graphics_context_set_text_color(ctx, GColorBlack);
  GRect titleOrigin      = layer_get_bounds(cell_layer);
  titleOrigin.origin.x   = 26;
  titleOrigin.size.w     = 144 - titleOrigin.origin.x * 2;
  titleOrigin.origin.y   += TEXT_ORIGIN_Y_SHIFT;
  graphics_draw_text(ctx, title, CONFIRM_FONT, titleOrigin, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
}

// Menu Row Draw function for Icon and Title
void menu_cell_simple_icon_draw(GContext* ctx, const Layer *cell_layer, const char *title, const GBitmap* bitmap)
{
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  GRect bitmap_bounds 	  = gbitmap_get_bounds(bitmap);
  GRect titleOrigin 		  = layer_get_bounds(cell_layer);
  bitmap_bounds.origin.x 	= (24 - bitmap_bounds.size.w) / 2;
  bitmap_bounds.origin.y	+= 1;
  graphics_draw_bitmap_in_rect(ctx, bitmap, bitmap_bounds);
  titleOrigin.origin.x 	  = 26;
  titleOrigin.origin.y 	  += TEXT_ORIGIN_Y_SHIFT;
  graphics_draw_text(ctx, title, CELL_FONT, titleOrigin, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
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
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  GRect bitmap_bounds 	  = gbitmap_get_bounds(bitmap);
  GRect title_bounds 		  = layer_get_bounds(cell_layer);
  bitmap_bounds.origin.x 	= (26 - bitmap_bounds.size.w) / 2;
  graphics_draw_bitmap_in_rect(ctx, bitmap, bitmap_bounds);
  title_bounds.origin.x 	= 26;
  title_bounds.origin.y 	+= TEXT_ORIGIN_Y_SHIFT;
  graphics_draw_text(ctx, title, CELL_FONT, title_bounds, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
}