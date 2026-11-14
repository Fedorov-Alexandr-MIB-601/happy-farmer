#pragma once

#include "constants.h"
#include "player.h"
#include "map.h"

#include "menu.h"

//		----	Функции сепарации логики		----
int setup(GameState* game_state);
int loop(GameState* game_state); //Вернуть 0, если хочешь оборвать вечный цикл

//		----	Функции инициализации			----
void init_variables(GameState* game_state);
void ask_username(Player* player);





unsigned char is_player_null(Player* player);
