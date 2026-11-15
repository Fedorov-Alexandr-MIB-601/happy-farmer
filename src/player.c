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
	Lantern lantern;
	lantern.kerosene = 100;
	lantern.max_kerosene = 100;
	lantern.light_mode = LIGHTMODE_NORMAL;
	player->lantern = lantern;
}

float get_lightness_multiplier(Lantern* lantern) {
	if (lantern->kerosene <= 0) {
		return 3.0;
	}
	switch (lantern->light_mode) {
	case LIGHTMODE_DISABLED:
		return 3.0;
	case LIGHTMODE_PALE:
		return 1.0;
	case LIGHTMODE_NORMAL:
		return 0.5;
	case LIGHTMODE_BRIGHT:
		return 0.1;
	}

	return 1.0;

}

int get_kerosene_usage(Lantern* lantern) {
	switch (lantern->light_mode) {
	case LIGHTMODE_DISABLED:
		return 0;
	case LIGHTMODE_PALE:
		return 1;
	case LIGHTMODE_NORMAL:
		return 2;
	case LIGHTMODE_BRIGHT:
		return 3;
	}
	return 0;
}

void lantern_decrement_kerosine(Lantern* lantern) {
	lantern->kerosene -= get_kerosene_usage(lantern);
	if (lantern->kerosene < 0) {
		lantern->kerosene = 0;
		lantern->light_mode = LIGHTMODE_DISABLED;
	}
	
}
