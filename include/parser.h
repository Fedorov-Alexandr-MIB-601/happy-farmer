#pragma once
#include <stdio.h>

char* get_string_from_file(FILE* file);

//возвращает 0 если всё ок, иначе код ошибки
int check_config(const char* config, int* pairs_count);

//Изначально я хотел передавать по двум звёздочкам **, но что-то у меня не получилось и пришлось использовать три *** 😥
int parse_config(const char* filepath, char*** keys, char*** values, int* pairs_count);