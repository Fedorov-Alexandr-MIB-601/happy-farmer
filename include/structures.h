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
	int adventure_depth;
	char item_id_to_name[MAX_ITEMS_COUNT][MAX_TOKEN_LENGTH];
} GameState;