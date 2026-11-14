#pragma once

#include "constants.h"
#include "player.h"
#include "map.h"

//		----	Структуры	----
typedef struct {
	int current_day;
	int current_hour;
	Player* player;
	World* world;
	char item_id_to_name[MAX_ITEMS_COUNT][MAX_TOKEN_LENGTH];
} GameState;


//		----	Функции сепарации логики		----
int setup(GameState* game_state);
int loop(GameState* game_state); //Вернуть 0, если хочешь оборвать вечный цикл

//		----	Функции инициализации			----
void init_variables(GameState* game_state);
void ask_username(Player* player);

//		----	Вывод	----
void show_time(GameState* game_state);
void print_id_to_item(GameState* game_state);

//		----	Функции для работы с файлами	----
int parse_config(const char* filepath, char*** keys, char*** values, int* pairs_count);

//		----	Геймплейные функции				----
void feed_forward_time(GameState* game_state, int hours_to_skip);
int set_item(Player* player, int slot_index, int item_id);
int clear_item(Player* player, int slot_index);
int count_items(Player* player, int item_id);

//		----	Функции выбора из меню			----
void menu_work(GameState* game_state);
void menu_check_inventory(GameState* game_state);
void menu_set_item(GameState* game_state);
void menu_drop_item(GameState* game_state);
void menu_inventory_revision(GameState* game_state);
void menu_inventory_search(GameState* game_state);
void menu_make_snapshot(GameState* game_state);
void menu_settings_parser();
void menu_adventure(GameState* game_state);


unsigned char is_player_null(Player* player);
void print_character_not_exist();