#pragma once
#include "constants.h"

typedef struct {
    char name[MAX_TOKEN_LENGTH];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int x, y; // Координаты на карте приключения Zachem?
    int inventory[MAX_ITEMS_COUNT];
} Player;
