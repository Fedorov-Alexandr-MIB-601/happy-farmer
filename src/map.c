#include "map.h"

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
			map[y][x] = '*';
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