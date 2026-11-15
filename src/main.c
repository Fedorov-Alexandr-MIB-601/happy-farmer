#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "constants.h"
#include "utils.h"
#include "parser.h"
#include "map.h"
#include "player.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <time.h>



int main() 
{
#ifdef _WIN32
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001); //Тимофей Владиславович сказал, это надо чтобы было без кракозябр в выводе
#endif

	srand(time(NULL));

	redraw_to_header();
	
	 

	
	GameState game_state;
	game_state.player = NULL;


	if (setup(&game_state) != 0) {
		return 1;
	}
	
	//Вот это я смешную конструкцию придумал 🤣
	while (loop(&game_state));

	
	printf("Память структуры игрока по адресу %p освобождена\n", game_state.player);
	free(game_state.player);

	return 0;
}

int load_items(GameState* game_state) {
	printf("Загружаю предметы...\n");
	char DEFAULT_NAMES[MAX_ITEMS_COUNT][MAX_TOKEN_LENGTH] = {
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

	for (int i = 0; i < MAX_ITEMS_COUNT; i++)
	{
		strcpy(game_state->item_id_to_name[i], DEFAULT_NAMES[i]);
	}

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
			strcpy(game_state->item_id_to_name[index], values[i]);
		
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

int setup(GameState* game_state) {
	if (load_items(game_state)) {
		return 1;
	}
	init_variables(game_state);

	//redraw_to_header();
	//ask_username();

	//get_confirm();
	return 0;
}



void init_variables(GameState* game_state)
{
	game_state->current_day = START_DAY;
	game_state->current_hour = START_HOUR;
	
}





void ask_username(Player* player) {
	get_string_input("Привет, меня зовут Ёжик. А как зовут тебя?\nНапиши своё имя и фамилию, чтобы мы тебя узнавали:", (char*)(player->username));
	if (strcmp(player->username, "") == 0)
	{
		printf("Значит будешь Анонимом.\n");
		strcpy(player->username, "Аноним");
	}
	else
		printf("Ого! Я тебя узнал!\n");
}




int loop(GameState* game_state) {
	redraw_to_header();

	Player* player = game_state->player;
	if (is_player_null(player))
		print_action_selection("Ноунейм");
	else
		print_action_selection(player->username);
	int user_selection = get_int_input_range("", 0, 10);
	redraw_to_header();

	switch (user_selection)
	{
	case SELECTION_EXIT:
		printf("Выход из программы...\n");
		return 0;

	case SELECTION_MAKE_CHARACTER:
		menu_make_character(game_state);
		break;

	case SELECTION_CHARACTER_CARD:
		if (is_player_null(player))
			print_character_not_exist();
		else
			menu_print_character(game_state);
		break;
	case SELECTION_TIME_MANAGMENT:
		menu_time_management(game_state);
		break;
		
	case SELECTION_CHECK_INVENTORY:
		if (is_player_null(player)) 
			print_character_not_exist();
		else
			menu_check_inventory(game_state);
		break;
	case SELECTION_SET_ITEM:
		if (is_player_null(player))
			print_character_not_exist();
		else
			menu_set_item(game_state);
		break;
	case SELECTION_DROP_ITEM:
		if (is_player_null(player))
			print_character_not_exist();
		else
			menu_drop_item(game_state);
		break;
	case SELECTION_INVENTORY_SEARCH:
		if (is_player_null(player))
			print_character_not_exist();
		else
			menu_inventory_search(game_state);
		break;
	case SELECTION_MAKE_SNAPSHOT:
		if (is_player_null(player))
			print_character_not_exist();
		else
			menu_make_snapshot(game_state);
		break;
	case SELECTION_SETTINGS_PARSER:
		menu_settings_parser();
		break;
	case SELECTION_ADVENTURE:
		if (is_player_null(player))
			print_character_not_exist();
		else
			menu_adventure(game_state);
		break;
	default:
		printf("Такого действия нет!\n");
		break;
	}
	get_confirm();
	return 1;
}







unsigned char is_player_null(Player* player) {
	return player == NULL;
}


