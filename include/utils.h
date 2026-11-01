#pragma once

//		----	Функции для помощи при выводе и вводе	----
void clear_screen();
void print_title();
void redraw_to_header();
void get_confirm();

void print_action_selection(char* username);
int get_int_input(const char* message);
int get_int_input_range(const char* message, int lowest_value, int highest_value);
void get_string_input(const char* message, char* buffer);
char* get_military_time(int hour);

void remove_newlines(char* str);
void remove_back_spaces(char* str);

int rand_top(int max);
int rand_range(int min, int max);