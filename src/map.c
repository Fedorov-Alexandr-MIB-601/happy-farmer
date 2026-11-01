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
			else
				map[y][x] = '.';
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


struct World* get_world() {
	struct World* world = malloc(sizeof(struct World));
	
	world->map = generate_map(&(world->map_height), &(world->map_width));
	world->player_position_x = 3;
	world->player_position_y = 3;
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
	if (world->map[y][x] == '#') return TRUE;
	if (world->map[y][x] == 'R') return TRUE;
	if (world->map[y][x] == 'T') return TRUE;
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