#pragma once
#include "constants.h"

typedef enum {
    LIGHTMODE_DISABLED,
    LIGHTMODE_PALE,
    LIGHTMODE_NORMAL,
    LIGHTMODE_BRIGHT
} LightMode;

typedef struct {
    int kerosene;
    int max_kerosene;
    LightMode light_mode;
} Lantern;

typedef struct {
    char username[MAX_TOKEN_LENGTH];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int x, y; // Координаты на карте приключения
    int inventory[INVENTORY_SIZE];
    Lantern lantern;
} Player;



float get_lightness_multiplier(Lantern* lantern);

int get_kerosene_usage(LightMode light_mode);
void lantern_decrement_kerosine(Lantern* lantern);

void init_player(Player* player, char* username);