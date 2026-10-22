#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>



#define PARSER_EXIT_CODE_TOKEN_IS_TOO_LONG 1
#define PARSER_EXIT_CODE_WRONG_FORMAT 2
#define PARSER_EXIT_CODE_FAILED_OPEN_FILE 3
#define PARSER_EXIT_CODE_EMPTY_TOKEN 4


// Пункты выбора в меню
#define SELECTION_EXIT 0
#define SELECTION_CHECK_TIME 1
#define SELECTION_WORK 2
#define SELECTION_CHECK_INVENTORY 3
#define SELECTION_SET_ITEM 4
#define SELECTION_DROP_ITEM 5
#define SELECTION_INVENTORY_REVISION 6
#define SELECTION_INVENTORY_SEARCH 7
#define SELECTION_MAKE_SNAPSHOT 8
#define SELECTION_SETTINGS_PARSER 9





#define MAX_ITEMS_COUNT 10
#define MAX_TOKEN_LENGTH 64

char username[MAX_TOKEN_LENGTH];



//		----	ПЕРЕМЕННЫЕ		----
int current_day;
int current_hour;

#define INVENTORY_SIZE 10
int inventory[INVENTORY_SIZE];



char item_id_to_name[MAX_ITEMS_COUNT][MAX_TOKEN_LENGTH] = {
	"Пусто",
	"Дерево",
	"Камень",
	"Семена",
	"Лопата",
	"Лейка",
	"Лукошко",
	"Яблоко",
	"Морковка",
	"Ягода"
};
//






//		----	Функции сепарации логики		----
int setup();
int loop(); //Вернуть 0, если хочешь оборвать вечный цикл

//		----	Функции инициализации			----
void init_variables();
void ask_username();


//		----	Функции для помощи при выводе	----
void clear_screen() {
	system("cls");
}
void print_title() {
	printf("|-------------------------------|\n");
	printf("| H A P P Y   F A R M E R v0.02 |\n");
	printf("|-------------------------------|\n");
	printf("\n");
}
void redraw_to_header() {
	clear_screen();
	print_title();
}
void print_action_selection();
int get_int_input(const char* message);
int get_int_input_range(const char* message, int lowest_value, int highest_value);
void get_string_input(const char* message, char* buffer);
void get_confirm() {
	printf("\nОжидание подтверждения...");
	_getch();
}


char* get_military_time(int hour);
void print_id_to_item();
void show_time() {
	printf("День %d, %s", current_day, get_military_time(current_hour));
}


//		----	Функции для работы с файлами	----
int parse_config(const char* filepath, char** keys, char** values, int* pairs_count);

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

int main() 
{
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001); //Тимофей Владиславович сказал, это надо чтобы было без кракозябр в выводе
	redraw_to_header();
	
	if (setup() != 0) {
		return 1;
	}
	
	//Вот это я смешную конструкцию придумал 🤣
	while (loop());


	return 0;
}

int load_items() {
	printf("Загружаю предметы...\n");
	char** keys;
	char** values;
	int pairs_count;

	int exit_code = parse_config("assets/items.txt", &keys, &values, &pairs_count);
	if (exit_code) {
		printf("Ошибка загрузки предметов, %d\n", exit_code);
		return 1;
	}

	for (int i = 0; i < pairs_count; i++)
	{
		int index = atoi(keys[i]);

		//Пришлось добавить проверку, потому что мне почему-то никто не сказал, что я залез в чужую память, например в 10 индекс, странно
		//Кажется ошбки не было, потому что я залез не в память чужой программы, а своей же
		if (index >= 0 && index < MAX_ITEMS_COUNT) 
			strcpy(item_id_to_name[index], values[i]);
		
		free(keys[i]);
	}
	free(keys);
	for (int i = 0; i < pairs_count; i++)
	{
		free(values[i]);
	}
	free(values);
	printf("Предметы успешно загружены...\n");
	return 0;
}

int setup() {
	if (load_items()) {
		return 1;
	}
	init_variables();

	redraw_to_header();
	ask_username();

	get_confirm();
	return 0;
}


const int START_DAY = 1;
const int START_HOUR = 8;
void init_variables()
{
	current_day = START_DAY;
	current_hour = START_HOUR;
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		inventory[i] = rand() % MAX_ITEMS_COUNT;
	}
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



void ask_username() {
	get_string_input("Привет, меня зовут Ёжик. А как зовут тебя?\nНапиши своё имя и фамилию, чтобы мы тебя узнавали:", (char*)username);
	if (strcmp(username, "") == 0)
	{
		printf("Значит будешь Анонимом.\n");
		strcpy(username, "Аноним");
	}
	else
		printf("Ого! Я тебя узнал!\n");
}


int loop() {
	redraw_to_header();
	print_action_selection();
	int user_selection = get_int_input_range("", 0, 9);
	redraw_to_header();

	switch (user_selection)
	{
	case SELECTION_EXIT:
		printf("Выход из программы...\n");
		return 0;

	case SELECTION_CHECK_TIME:
		printf("Текущее время: ");
		show_time();
		printf("\n\n");
		break;

	case SELECTION_WORK:
		menu_work();
		break;

	case SELECTION_CHECK_INVENTORY:
		menu_check_inventory();
		break;
	case SELECTION_SET_ITEM:
		menu_set_item();
		break;
	case SELECTION_DROP_ITEM:
		menu_drop_item();
		break;
	case SELECTION_INVENTORY_REVISION:
		menu_inventory_revision();
		break;
	case SELECTION_INVENTORY_SEARCH:
		menu_inventory_search();
		break;
	case SELECTION_MAKE_SNAPSHOT:
		menu_make_snapshot();
		break;
	case SELECTION_SETTINGS_PARSER:
		menu_settings_parser();
		break;
	default:
		printf("Такого действия нет!\n");
		break;
	}
	get_confirm();
	return 1;
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
		return raw_input;
	}
	return -1;
}

int get_int_input_range(const char* message, int lowest_value, int highest_value)
{
	while(1)
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
		return raw_input;
	}
	return -1;
}

void get_string_input_old(const char* message, char* destination_buffer) {
	if (destination_buffer == NULL) return;

	printf("%s\n>>>", message);

	if (fgets(destination_buffer, MAX_TOKEN_LENGTH, stdin) != NULL) {
		// Если символ перевода строки НЕ поместился — чистим остаток потока stdin
		if (strchr(destination_buffer, '\n') == NULL) {
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
		}
		// Удаляем сам '\n'
		remove_newlines(destination_buffer);
	}
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
	if (fgets(destination_buffer, (int)max_size, stdin) == NULL) {
		destination_buffer[0] = '\0';
	}
#endif

	// Убираем \r и \n на конце
	remove_newlines(destination_buffer);
}


void print_action_selection()
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

char* get_military_time(int hour) {
	hour %= 24;
	static char result[6] = "";
	
	if (hour < 10) {
		strcat(result, "0");
	}
	snprintf(result, sizeof(result), "%02d:00", hour);
	return result;

}
void print_id_to_item() {
	for (int i = 1; i < MAX_ITEMS_COUNT; i++)
	{
		printf("[%d] - %s\n", i, item_id_to_name[i]);
	}

}

const int HOURS_IN_DAY = 24;
void feed_forward_time(int hours_to_skip) {
	current_hour += hours_to_skip;
	int days_passed = current_hour / HOURS_IN_DAY;
	current_hour %= HOURS_IN_DAY;
	current_day += days_passed;
}

int set_item(int slot_index, int item_id) {
	if (slot_index < 0 || slot_index > INVENTORY_SIZE)
		return 0;
	if (item_id < 0 || item_id > MAX_ITEMS_COUNT)
		return 0;

	inventory[slot_index] = item_id;
	return 1;
}

int clear_item(int slot_index) {
	return set_item(slot_index, NULL);
}

int count_items(int item_id) {
	int items_count = 0;
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		if (inventory[i] == item_id)
			items_count++;
	}
	return items_count;
}


void menu_work() {
	int work_hours = 0;
	while (1) {
		work_hours = get_int_input("Сколько часов вы хотите работать?");
		if (work_hours < 0) {
			printf("Вы путешественник во времени?\n");
			continue;
		}
		break;
	}
	if (work_hours == 0) {
		printf("Вы решили не работать...\n\n");
	}
	else {
		printf("Начинаю работать в ");
		show_time();
		printf("\n");
		printf("Работаю %d часов...\n", work_hours);
		feed_forward_time(work_hours);
		printf("Закончил работать в ");
		show_time();
		printf("\n\n");
	}
}

void menu_check_inventory() {
	printf("Вот что у меня в карманах:\n");
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		printf("Слот %d: %s\n", i, item_id_to_name[ inventory[i] ]);
	}
	printf("\n");
}




void menu_set_item() {
	int slot_index = get_int_input_range("Введите индекс слота для установки предмета", 0, INVENTORY_SIZE - 1);
	int old_item = inventory[slot_index];

	print_id_to_item();
	int item_id = get_int_input_range("Введите айдишник желаемого предмета", 1, MAX_ITEMS_COUNT - 1);
	
	if (old_item == 0)
		printf("Устанавливаю предмет %s в слот %d...", item_id_to_name[item_id], slot_index);
	else
		printf("Заменяю предмет %s в слоте %d на %s...", item_id_to_name[old_item], slot_index, item_id_to_name[item_id]);
	
	set_item(slot_index, item_id);
	printf("\n\n");
}

void menu_drop_item() {
	int slot_index = get_int_input_range("Введите индекс слота из которого нужно выбросить предмет", 0, INVENTORY_SIZE - 1);
	int old_item = inventory[slot_index];

	if (old_item == 0)
		printf("В этом слоте уже ничего не было...");
	else
		printf("Выбрасываю предмет %s из слота %d...", item_id_to_name[old_item], slot_index);

	clear_item(slot_index);
	printf("\n\n");
}

void menu_inventory_revision() {
	print_id_to_item();
	int item_id = get_int_input_range("Введите айдишник предмета для ревизии", 1, MAX_ITEMS_COUNT - 1);

	int items_count = count_items(item_id);
	
	if (items_count == 0) {
		printf("У меня нет такого предмета((\n");
	}
	else if(items_count == 1) {
		for (int i = 0; i < INVENTORY_SIZE; i++)
		{
			if (inventory[i] == item_id) {
				printf("Единственный предмет '%s' лежит в слоте %d\n", item_id_to_name[item_id], i);
				break;
			}
		}
		
	}
	else {
		printf("Количество предметов '%s' - %d\n", item_id_to_name[item_id], items_count);
		printf("Они лежат в слотах:");
		for (int i = 0; i < INVENTORY_SIZE; i++)
		{
			if (inventory[i] == item_id) {
				printf(" %d,", i);
			}
		}
	}
	printf("\n\n");
	

}
int get_item_index_by_name(char* item_name) {
	for (int i = 0; i < MAX_ITEMS_COUNT; i++)
	{
		if (strcmp(item_id_to_name[i], item_name) == 0) {
			return i;
		}
	}
	return -1;
}
void menu_inventory_search() {
	char search_bar[MAX_TOKEN_LENGTH];
	print_id_to_item();
	get_string_input("Введите название искомого предмета", search_bar);
	int item_id = get_item_index_by_name(search_bar);
	if (item_id == -1) {
		printf("Такого предмета не существует!\n");
		return;
	}
	int items_count = count_items(item_id);

	if (items_count == 0) {
		printf("У меня нет такого предмета(\n");
		return;
	}

	printf("Предметы типа '%s' лежат в слотах:", search_bar);
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		if (inventory[i] == item_id) {
			printf(" %d,", i);
		}
	}
}



void menu_make_snapshot() {
	//На экран и в файл
	FILE* fp = fopen("diary.txt", "a");
	FILE* stream = stdout;
	for (int i = 0; i < 2; i++) {
		fprintf(stream, "---- Зпись от %d дня в %s ----\n", current_day, get_military_time(current_hour));
		fprintf(stream, "Фермер по имени   %s   имеет у себя в рюкзаке:\n", username);
		for (int item_id = 1; item_id < MAX_ITEMS_COUNT; item_id++)
		{
			int items_count = count_items(item_id);
			if (items_count == 0) continue;
			fprintf(stream, "- %s \tв количестве %d шт.\n", item_id_to_name[item_id], items_count);
		}
		fprintf(stream, "\n\n\n");

		if (i == 1) break;
		stream = fp;
	}
	
	fclose(fp);
	

}





void menu_settings_parser() {
	printf("Изучаю настройки...\n");

	char** keys;
	char** values;
	int pairs_count;
	int exit_code = parse_config("input.txt", &keys, &values, &pairs_count);
	if (exit_code == PARSER_EXIT_CODE_EMPTY_TOKEN) {
		printf("Ошибка: Пустой ключ или значение");
		return;
	}

	FILE* fp = fopen("output.txt", "w");
	fprintf(stdout, "---- Чистая Таблица Настроек: ----\n\n");
	fprintf(fp, "---- Чистая Таблица Настроек: ----\n\n");
	for (int i = 0; i < pairs_count; i++)
	{
		fprintf(stdout, "Параметр: '%s'\t| Значение: '%s'\n", keys[i], values[i]);
		fprintf(fp, "Параметр: '%s'\t| Значение: '%s'\n", keys[i], values[i]);

		free(keys[i]);
	}
	free(keys);
	for (int i = 0; i < pairs_count; i++)
	{
		free(values[i]);
	}
	free(values);
	fclose(fp);
}


//Всё что ниже по-хорошему надобно в отдельный файл

char* get_string_from_file(FILE* file) {
	//Размер файла в байтах
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);
	//

	char* result = malloc(size + 1); // +1 для теремнатора
	fread(result, sizeof(char), size, file);
	result[size] = '\0';
	return result;
}

//возвращает 0 если всё ок, иначе код ошибки
int check_config(const char* config, int* pairs_count) {
	int equals_counter = 0; //По сути количество пар ключ/значение
	int newline_counter = 0;
	int token_length_counter = 0;

	
	unsigned char comment_ignore = 0;

	int i = 0;
	while (config[i] != '\0') {
		if (token_length_counter == MAX_TOKEN_LENGTH) {
			return PARSER_EXIT_CODE_TOKEN_IS_TOO_LONG;
		}

		if (comment_ignore == TRUE)
		{
			if (config[i] == '\n') {
				comment_ignore = FALSE;
			}
		}
		else if (config[i] == ' ' && token_length_counter == 0) {
			//Пропускаем пробельчики слева
		}
		else if (config[i] == '#') {
			comment_ignore = TRUE;
			token_length_counter = 0;
		}
		else if (config[i] == '=') {
			if (newline_counter == equals_counter) {
				equals_counter++;
				token_length_counter = 0;
			}
			else {
				return PARSER_EXIT_CODE_WRONG_FORMAT;
			}
		}
		else if (config[i] == '\n') {
			if (equals_counter > newline_counter) {
				newline_counter++;
				token_length_counter = 0;
			}
			else {
				//printf("Ошибка: ожидалось '=' но получил 'newline'\n");
				//return ?;
				//По идее можно просто игнорить
			}
		}
		else {
			if (config[i] != '\n' && config[i] != '\r') {
				token_length_counter++;
			}
		}

		i++;
	}
	*pairs_count = equals_counter;
	return 0;
}

//Изначально я хотел передавать по двум звёздочкам **, но что-то у меня не получилось и пришлось использовать три *** 😥
int parse_config(const char* filepath, char*** keys, char*** values, int* pairs_count) { 
	FILE* file = fopen(filepath, "r");
	if (file == NULL) {
		return PARSER_EXIT_CODE_FAILED_OPEN_FILE;
	}
	else {

		char* content = get_string_from_file(file);
		
		int _pairs_count = -1;
		//Проверка корректности нотации
		int exit_code = check_config(content, &_pairs_count);
		if (exit_code) return exit_code;
		
		*pairs_count = _pairs_count;

		*keys = malloc(sizeof(char*) * _pairs_count);
		for (int i = 0; i < _pairs_count; i++)
		{
			(*keys)[i] = malloc(sizeof(char) * MAX_TOKEN_LENGTH);
		}
		*values = malloc(sizeof(char*) * _pairs_count);
		for (int i = 0; i < _pairs_count; i++)
		{
			(*values)[i] = malloc(sizeof(char) * MAX_TOKEN_LENGTH);
		}

		unsigned char comment_ignore = 0;
		int equals_counter = 0;
		int newline_counter = 0;
		int token_length_counter = 0;
		int i = 0;
		while (content[i] != '\0') {
			if (comment_ignore == TRUE)
			{
				if (content[i] == '\n') {
					comment_ignore = FALSE;
				}
			}
			else if (content[i] == ' ' && token_length_counter == 0) {
				//Пропускаем пробельчики слева
			}
			else if (content[i] == '#') {
				comment_ignore = TRUE;

				if (newline_counter + 1 == equals_counter) { //Правая часть
					if (token_length_counter == 0) return PARSER_EXIT_CODE_EMPTY_TOKEN;
					(*values)[newline_counter][token_length_counter] = '\0';
					remove_back_spaces((*values)[newline_counter]);
					newline_counter++;
				}
				token_length_counter = 0;
				

			}
			else if (content[i] == '=') {
				if (token_length_counter == 0) return PARSER_EXIT_CODE_EMPTY_TOKEN;
				(*keys)[newline_counter][token_length_counter] = '\0';
				remove_back_spaces((*keys)[newline_counter]);
				equals_counter++;
				token_length_counter = 0;
				
			}
			else if (content[i] == '\n') {
				if (equals_counter > newline_counter) {
					if (token_length_counter == 0) return PARSER_EXIT_CODE_EMPTY_TOKEN;
					(*values)[newline_counter][token_length_counter] = '\0';
					remove_back_spaces((*values)[newline_counter]);
					newline_counter++;
					token_length_counter = 0;
					if (newline_counter == _pairs_count) {
						break;
					}
				}
			}
			else {
				if (content[i] != '\n' && content[i] != '\r') {
					if (newline_counter == equals_counter) { //Левая часть
						(*keys)[newline_counter][token_length_counter] = content[i];
					}
					else if (newline_counter + 1 == equals_counter) { //Правая часть
						(*values)[newline_counter][token_length_counter] = content[i];
					}
					token_length_counter++;
				}

				
			}


			i++;
		}


		free(content);

		fclose(file);
	}
	
	return 0;
}