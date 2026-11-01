#pragma once
//		----	Функции сепарации логики		----
int setup();
int loop(); //Вернуть 0, если хочешь оборвать вечный цикл

//		----	Функции инициализации			----
void init_variables();
void ask_username();

//		----	Вывод	----
void show_time();
void print_id_to_item();

//		----	Функции для работы с файлами	----
int parse_config(const char* filepath, char*** keys, char*** values, int* pairs_count);

//		----	Геймплейные функции				----
void feed_forward_time(int hours_to_skip);
int set_item(int slot_index, int item_id);
int clear_item(int slot_index);
int count_items(int item_id);

//		----	Функции выбора из меню			----
void menu_work();
void menu_check_inventory();
void menu_set_item();
void menu_drop_item();
void menu_inventory_revision();
void menu_inventory_search();
void menu_make_snapshot();
void menu_settings_parser();
void menu_selection();