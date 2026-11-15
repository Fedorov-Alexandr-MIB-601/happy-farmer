#include "utils.h"

#include "constants.h"
#include "structures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#ifdef _WIN32
#include <Windows.h>
#endif

void clear_screen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void print_title() {
	printf("|-------------------------------|\n");
	printf("| H A P P Y   F A R M E R v0.04 |\n");
	printf("|-------------------------------|\n\n");
}

void redraw_to_header() {
	clear_screen();
	print_title();
}

void get_confirm() {
	printf("\nОжидание подтверждения...");
	getchar();
}

void print_action_selection(char* username)
{

	printf("Что ты выберешь, %s?\n", username);
	printf("[0]  - Выход\n");
	printf("[1]  - Создать персонажа\n");
	printf("[2]  - Карточка персонажа\n");
	printf("[3]  - Управление временем\n");
	printf("[4]  - Посмотреть инвентарь\n");
	printf("[5]  - Положить предмет в слот\n");
	printf("[6]  - Выбросить предмет\n");
	printf("[7]  - Поиск предмета в рюкзаке по названию\n");
	printf("[8]  - Записать состояние в дневник фермера\n");
	printf("[9]  - Парсер файла настроек\n");
	printf("[10] - Отправиться в приключение\n");

}

void print_time_submenu_selection(char* username) {
	printf("Что ты выберешь, %s?\n", username);
	printf("[0] - Назад\n");
	printf("[1] - Посмотреть на часы\n");
	printf("[2] - Ждать\n");
	printf("[3] - Лечь спать\n");

}


int get_int_input(const char* message)
{
	while (1)
	{
		printf(message);
		printf("\n>>>");
		int raw_input;
		if (scanf("%d", &raw_input) != 1)
		{
			printf("Кажется, это не число...\n");
			scanf("%*s");
			continue;
		}
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
		return raw_input;
	}
	return -1;
}

int get_int_input_range(const char* message, int lowest_value, int highest_value)
{
	while (1)
	{
		printf(message);
		printf("\n>>>");
		int raw_input;
		if (scanf("%d", &raw_input) != 1)
		{
			printf("Кажется, это не число...\n");
			scanf("%*s");
			continue;
		}

		if (raw_input < lowest_value || raw_input > highest_value) {
			printf("Число дожно быть в диапазоне от %d до %d включительно!\n", lowest_value, highest_value);
			continue;
		}
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
		return raw_input;
	}
	return -1;
}


void get_string_input(const char* message, char* destination_buffer) {
	if (destination_buffer == NULL) return;


	printf("%s\n>>> ", message);
	//Опять что-ли себе на комп линукс поставить???
#ifdef _WIN32
	// На Windows читаем в UTF-16, чтобы консоль не ломала кириллицу
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	wchar_t wbuffer[512];
	DWORD charsRead = 0;

	// Читаем из консоли Windows
	if (ReadConsoleW(hStdin, wbuffer, sizeof(wbuffer) / sizeof(wchar_t) - 1, &charsRead, NULL)) {
		wbuffer[charsRead] = L'\0';

		// Конвертируем прочитанную широкую строку (UTF-16) в UTF-8
		WideCharToMultiByte(CP_UTF8, 0, wbuffer, -1, destination_buffer, MAX_TOKEN_LENGTH, NULL, NULL);
	}
	else {
		// Запасной вариант (если ввод был перенаправлен из файла, а не из реальной консоли)
		if (fgets(destination_buffer, MAX_TOKEN_LENGTH, stdin) == NULL) {
			destination_buffer[0] = '\0';
		}
	}
#else
	// На Linux / macOS fgets работает с UTF-8 идеально
	if (fgets(destination_buffer, MAX_TOKEN_LENGTH, stdin) == NULL) {
		destination_buffer[0] = '\0';
	}
#endif

	// Убираем \r и \n на конце
	remove_newlines(destination_buffer);
}

char* get_military_time(int hour) {
	hour %= 24;
	static char result[6] = "";

	if (hour < 10) {
		strcat(result, "0");
	}
	snprintf(result, sizeof(result), "%02d:00", hour);
	return result;

}


void remove_newlines(char* str) {
	if (str == NULL)
		return;

	int original = 0;
	int edited = 0;

	while (str[original] != '\0') {
		if (str[original] != '\n' && str[original] != '\r') {
			str[edited] = str[original];
			edited++;
		}
		original++;
	}
	str[edited] = '\0';
}

//Смешной каламбур
void remove_back_spaces(char* str) {
	if (str == NULL)
		return;

	int i = strlen(str) - 1;

	while (str[i] == ' ') {
		str[i] = '\0';
		i--;
		if (i < 0) {
			break;
		}
	}
}


int rand_top(int max) {
	return rand() % max;
}

int rand_range(int min, int max) {
	return (rand_top(max - min)) + min;
}

void print_character_not_exist() {
	redraw_to_header();
	printf("\033[31mПерсонаж не создан!\033[0m\n");
	printf("Пожалуйста, создайте персонажа чтобы продолжить!\n");
}


void show_time(GameState* game_state) {
	printf("День %d, %s", game_state->current_day, get_military_time(game_state->current_hour));
}
void print_id_to_item(GameState* game_state) {
	for (int i = 1; i < MAX_ITEMS_COUNT; i++)
	{
		printf("[%d] - %s\n", i, game_state->item_id_to_name[i]);
	}

}