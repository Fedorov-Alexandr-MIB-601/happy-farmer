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


init_player(Player* player, char* username);