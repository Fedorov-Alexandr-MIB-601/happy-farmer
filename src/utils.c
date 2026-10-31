#include <stdio.h>
#include "utils.h"

#include "constants.h"

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
	printf("| H A P P Y   F A R M E R v0.02 |\n");
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
	printf("[0] - Выход\n");
	printf("[1] - Посмотреть на часы\n");
	printf("[2] - Поработать\n");
	printf("[3] - Посмотреть инвентарь\n");
	printf("[4] - Положить предмет в слот\n");
	printf("[5] - Выбросить предмет\n");
	printf("[6] - Ревизия ресурсов\n");
	printf("[7] - Поиск предмета в рюкзаке по названию\n");
	printf("[8] - Записать состояние в дневник фермера\n");
	printf("[9] - Парсер файла настроек\n");

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
