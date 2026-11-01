#include "map.h"

#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

char** generate_map(int* out_height, int* out_width) {
	char** map;
	int height = (rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE)) + MAP_MIN_SIZE;
	int width = (rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE)) + MAP_MIN_SIZE;

	// 1. Выделяем память под массив указателей (строк)
	map = (char**)malloc(height * sizeof(char*));
	// 2. В цикле выделяем память под каждую строку (столбцы)
	for (int i = 0; i < height; i++) {
		map[i] = (char*)malloc((width) * sizeof(char));
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (y == 0 || y == height - 1 || x == 0 || x == width - 1)
				map[y][x] = '#';
			else {
				int probability = rand() % 100;
				if (probability < 5) {
					map[y][x] = 'R';
				}
				else if (probability >= 5 && probability < 10) {
					map[y][x] = 'T';
				}
				else {
					map[y][x] = '.';
				}
			}
		}
	}

	*out_height = height;
	*out_width = width;
	return map;
}

void free_map(char** map, int height, int width) {
	for (int y = 0; y < height; y++) {
		free(map[y]);
	}
	free(map);
}

void print_map(char** map, int height, int width, int player_position_x, int player_position_y) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (x == player_position_x && y == player_position_y)
				putc('@', stdout);
			else
				putc(map[y][x], stdout);
		}
		printf("\n");
	}
}

void set_cell(struct World* world, int x, int y, char cell) {
	*get_cell(world, x, y) = cell;

	
}

void set_cell_to_random_free_pos(struct World* world, char cell) {
	int pos_x;
	int pos_y;
	get_random_empty_cell(world, &pos_x, &pos_y);
	set_cell(world, pos_x, pos_y, cell);
}

struct World* get_world() {
	struct World* world = malloc(sizeof(struct World));
	
	world->map = generate_map(&(world->map_height), &(world->map_width));

	set_cell_to_random_free_pos(world, 'D');
	set_cell_to_random_free_pos(world, 'U');


	get_random_empty_cell(world, &(world->player_position_x), &(world->player_position_y));

	return world;
}

void free_world(struct World* world) {
	free_map(world->map, world->map_height, world->map_width);
	free(world);
}

void print_world(struct World* world) {
	print_map(world->map, world->map_height, world->map_width, world->player_position_x, world->player_position_y);
}

int is_cell_blocked(struct World* world, int x, int y) {
	if (*get_cell(world, x, y) == '#') return TRUE;
	if (*get_cell(world, x, y) == 'R') return TRUE;
	if (*get_cell(world, x, y) == 'T') return TRUE;
	return FALSE;
}

int is_cell_empty(struct World* world, int x, int y) {
	if (*get_cell(world, x, y) == '.') return TRUE;
	return FALSE;
}

int try_move_player(struct World* world, int direction_x, int direction_y) {
	int pos_x = world->player_position_x;
	int pos_y = world->player_position_y;

	//Вообще из-за стен по краям этого никогда не случится, но всё же
	if (pos_x + direction_x < 0) return FALSE;
	if (pos_x + direction_x >= world->map_width) return FALSE;
	if (pos_y + direction_y < 0) return FALSE;
	if (pos_y + direction_y >= world->map_height) return FALSE;

	if (!is_cell_blocked(world, pos_x + direction_x, pos_y + direction_y)) {
		world->player_position_x = pos_x + direction_x;
		world->player_position_y = pos_y + direction_y;
		return TRUE;
	}
	
	return FALSE;

}

void get_random_empty_cell(struct World* world, int* out_x, int* out_y) {
	while (TRUE) {
		int test_x = rand() % world->map_width;
		int test_y = rand() % world->map_height;
		if (is_cell_empty(world, test_x, test_y)) {
			*out_x = test_x;
			*out_y = test_y;
			return;
		}
	}
}

char* get_cell(struct World* world, int x, int y) {
	if (x < 0 || x >= world->map_width || y < 0 || y >= world->map_height)
		return NULL;

	return &(world->map[y][x]);

}

char* get_cell_player_standing(struct World* world) {
	return get_cell(world, world->player_position_x, world->player_position_y);
}