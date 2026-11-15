#pragma once
#include "constants.h"

typedef struct {
    char username[MAX_TOKEN_LENGTH];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int x, y; // Координаты на карте приключения
    int inventory[INVENTORY_SIZE];
} Player;

typedef enum {
    DISABLED,
    PALE,
    NORMAL,
    BRIGHT
} LightMode;

typedef struct {
    int kerosene;
    int max_kerosene;
    LightMode light_mode;
} Lantern;

void init_player(Player* player, char* username);