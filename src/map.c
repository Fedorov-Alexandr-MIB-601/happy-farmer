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