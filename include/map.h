#pragma once

//карта и игрок в ней
typedef struct
{
	int player_position_x, player_position_y;
	int map_height, map_width;
	char** map;
	unsigned char** creaky_floor;
} World;

//Генерирует случайную карту и возвращает указатель на неё
char** generate_map();

//Вычищает из памяти
void free_map(char** map, int height, int width);

void print_map(char** map, int height, int width, int player_position_x, int player_position_y);

//Возвращает указатель на мир с игроком
World* get_world(unsigned char generate_up_ladder);

void free_world(World* world);

void print_world(World* world);

//Пытается сдвинуть игрока, учитывая коллизию. Изменяет world
unsigned char try_move_player(World* world, int direction_x, int direction_y);

unsigned char is_cell_blocked(World* world, int x, int y);
unsigned char is_cell_empty(World* world, int x, int y);

void get_random_empty_cell(World* world, int* out_x, int* out_y);
void get_random_empty_cell_radius_square(World* world, int* out_x, int* out_y, int center_x, int center_y, int radius);

void set_cell(World* world, int x, int y, char cell);

void get_first_empty_cell(World* world, int* out_x, int* out_y);
void set_cell_to_random_free_pos(World* world, char cell);
void set_cell_to_random_free_pos_radius_square(World* world, char cell, int center_x, int center_y, int radius);

char* get_cell(World* world, int x, int y);
char* get_cell_player_standing(World* world);

unsigned char is_cell_creaky(World* world, int x, int y);