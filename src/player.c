#include "player.h"

#include <string.h>

void init_player(Player* player, char* username) {
	strncpy(player->username, username, MAX_TOKEN_LENGTH);
	player->hp = 100;
	player->max_hp = 100;
	player->stamina = 50;
	player->max_stamina = 50;
	player->gold = 0;
	player->x = -1;
	player->y = -1;
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		player->inventory[i] = 0;
	}
}