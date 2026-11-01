#pragma once
#include "constants.h"

//карта и игрок в ней
struct World
{
	int player_position_x;
	int player_position_y;
	char** map;
};

//Генерирует случайную карту и возвращает указатель на неё
char** generate_map();

//Вычищает из памяти
void free_map(char** map, int height, int width);

//Возвращает указатель на мир с игроком
struct World* get_world();

//Пытается сдвинуть игрока, учитывая коллизию. Изменяет world
int try_move_player(struct World* world, int direction_x, int direction_y);

