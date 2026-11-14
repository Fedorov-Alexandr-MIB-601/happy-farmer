#pragma once
#include "map.h"
#include "player.h"
#include "constants.h"
#include "structures.h"


//		----	Геймплейные функции				----
void feed_forward_time(GameState* game_state, int hours_to_skip);
int set_item(Player* player, int slot_index, int item_id);
int clear_item(Player* player, int slot_index);
int count_items(Player* player, int item_id);

//		----	Функции выбора из меню			----
void menu_check_time(GameState* game_state);
void menu_skip_time(GameState* game_state);
void menu_check_inventory(GameState* game_state);
void menu_set_item(GameState* game_state);
void menu_drop_item(GameState* game_state);
void menu_inventory_revision(GameState* game_state);
void menu_inventory_search(GameState* game_state);
void menu_make_snapshot(GameState* game_state);
void menu_settings_parser();
void menu_adventure(GameState* game_state);


void menu_time_management(GameState* game_state);