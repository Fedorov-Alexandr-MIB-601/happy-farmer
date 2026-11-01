#pragma once

//карта и игрок в ней
struct World
{
	int player_position_x, player_position_y;
	int map_height, map_width;
	char** map;
};

//Генерирует случайную карту и возвращает указатель на неё
char** generate_map();

//Вычищает из памяти
void free_map(char** map, int height, int width);

void print_map(char** map, int height, int width, int player_position_x, int player_position_y);

//Возвращает указатель на мир с игроком
struct World* get_world();

void free_world(struct World* world);

void print_world(struct World* world);

//Пытается сдвинуть игрока, учитывая коллизию. Изменяет world
int try_move_player(struct World* world, int direction_x, int direction_y);

int is_cell_blocked(struct World* world, int x, int y);

void get_random_empty_cell(struct World* world, int* out_x, int* out_y);

void set_cell(struct World* world, int x, int y, char cell);

void set_cell_to_random_free_pos(struct World* world, char cell);

char* get_cell(struct World* world, int x, int y);
char* get_cell_player_standing(struct World* world);