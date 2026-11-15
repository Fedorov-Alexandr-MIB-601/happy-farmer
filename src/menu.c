#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <string.h>
#include "parser.h"

void feed_forward_time(GameState* game_state, int hours_to_skip) {
	game_state->current_hour += hours_to_skip;
	int days_passed = game_state->current_hour / HOURS_IN_DAY;
	game_state->current_hour %= HOURS_IN_DAY;
	game_state->current_day += days_passed;
}

int set_item(Player* player, int slot_index, int item_id) {
	if (slot_index < 0 || slot_index > INVENTORY_SIZE)
		return 0;
	if (item_id < 0 || item_id > MAX_ITEMS_COUNT)
		return 0;

	player->inventory[slot_index] = item_id;
	return 1;
}

int clear_item(Player* player, int slot_index) {
	return set_item(player, slot_index, 0);
}

int count_items(Player* player, int item_id) {
	int items_count = 0;
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		if (player->inventory[i] == item_id)
			items_count++;
	}
	return items_count;
}




void menu_check_time(GameState* game_state) {
	printf("Текущее время: ");
	show_time(game_state);
	printf("\n\n");
}

void menu_skip_time(GameState* game_state) {
	if (game_state->current_hour == 2) {
		printf("Уже 02:00 на часах, кажется, пора баиньки\n");
		return;
	}
	int work_hours = 0;
	while (1) {
		work_hours = get_int_input("Сколько часов вы хотите пропустить?");
		if (work_hours < 0) {
			printf("Вы путешественник во времени?\n");
			continue;
		}
		break;
	}
	if (work_hours == 0) {
		printf("Вы решили не тратить время впустую...\n\n");
	}
	else {
		printf("Начинаю филонить в ");
		show_time(game_state);
		printf("\n");

		int possible_work_time = HOURS_IN_DAY + 2 - game_state->current_hour;

		if (work_hours > possible_work_time)
			work_hours = possible_work_time;
			

		printf("Бездельничаю %d часов...\n", work_hours);
		feed_forward_time(game_state, work_hours);
		printf("Закончил дармоедить в ");
		show_time(game_state);
		printf("\n\n");
	}
}

void menu_check_inventory(GameState* game_state) {
	printf("Вот что у меня в карманах:\n");
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		printf("Слот %d: %s\n", i, game_state->item_id_to_name[game_state->player->inventory[i]]);
	}
	printf("\n");
}




void menu_set_item(GameState* game_state) {
	int slot_index = get_int_input_range("Введите индекс слота для установки предмета", 0, INVENTORY_SIZE - 1);
	int old_item = game_state->player->inventory[slot_index];

	print_id_to_item(game_state);
	int item_id = get_int_input_range("Введите айдишник желаемого предмета", 1, MAX_ITEMS_COUNT - 1);

	if (old_item == 0)
		printf("Устанавливаю предмет %s в слот %d...", game_state->item_id_to_name[item_id], slot_index);
	else
		printf("Заменяю предмет %s в слоте %d на %s...", game_state->item_id_to_name[old_item], slot_index, game_state->item_id_to_name[item_id]);

	set_item(game_state->player, slot_index, item_id);
	printf("\n\n");
}

void menu_drop_item(GameState* game_state) {
	int slot_index = get_int_input_range("Введите индекс слота из которого нужно выбросить предмет", 0, INVENTORY_SIZE - 1);
	int old_item = game_state->player->inventory[slot_index];

	if (old_item == 0)
		printf("В этом слоте уже ничего не было...");
	else
		printf("Выбрасываю предмет %s из слота %d...", game_state->item_id_to_name[old_item], slot_index);

	clear_item(game_state->player, slot_index);
	printf("\n\n");
}

void menu_inventory_revision(GameState* game_state) {
	print_id_to_item(game_state);
	int item_id = get_int_input_range("Введите айдишник предмета для ревизии", 1, MAX_ITEMS_COUNT - 1);

	int items_count = count_items(game_state->player, item_id);

	if (items_count == 0) {
		printf("У меня нет такого предмета((\n");
	}
	else if (items_count == 1) {
		for (int i = 0; i < INVENTORY_SIZE; i++)
		{
			if (game_state->player->inventory[i] == item_id) {
				printf("Единственный предмет '%s' лежит в слоте %d\n", game_state->item_id_to_name[item_id], i);
				break;
			}
		}

	}
	else {
		printf("Количество предметов '%s' - %d\n", game_state->item_id_to_name[item_id], items_count);
		printf("Они лежат в слотах:");
		for (int i = 0; i < INVENTORY_SIZE; i++)
		{
			if (game_state->player->inventory[i] == item_id) {
				printf(" %d,", i);
			}
		}
	}
	printf("\n\n");


}
int get_item_index_by_name(GameState* game_state, char* item_name) {
	for (int i = 0; i < MAX_ITEMS_COUNT; i++)
	{
		if (strcmp(game_state->item_id_to_name[i], item_name) == 0) {
			return i;
		}
	}
	return -1;
}
void menu_inventory_search(GameState* game_state) {
	char search_bar[MAX_TOKEN_LENGTH];
	print_id_to_item(game_state);
	get_string_input("Введите название искомого предмета", search_bar);
	int item_id = get_item_index_by_name(game_state, search_bar);
	if (item_id == -1) {
		printf("Такого предмета не существует!\n");
		return;
	}
	int items_count = count_items(game_state->player, item_id);

	if (items_count == 0) {
		printf("У меня нет такого предмета(\n");
		return;
	}

	printf("Предметы типа '%s' лежат в слотах:", search_bar);
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		if (game_state->player->inventory[i] == item_id) {
			printf(" %d,", i);
		}
	}
}



void menu_make_snapshot(GameState* game_state) {
	//На экран и в файл
	FILE* fp = fopen("diary.txt", "a");
	FILE* stream = stdout;
	for (int i = 0; i < 2; i++) {
		fprintf(stream, "---- Зпись от %d дня в %s ----\n", game_state->current_day, get_military_time(game_state->current_hour));
		fprintf(stream, "Фермер по имени   %s   имеет у себя в рюкзаке:\n", game_state->player->username);
		for (int item_id = 1; item_id < MAX_ITEMS_COUNT; item_id++)
		{
			int items_count = count_items(game_state->player, item_id);
			if (items_count == 0) continue;
			fprintf(stream, "- %s \tв количестве %d шт.\n", game_state->item_id_to_name[item_id], items_count);
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
	int exit_code = parse_config("assets/input.txt", &keys, &values, &pairs_count);
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



void menu_adventure(GameState* game_state) {
	int frame_counter = 0;
	char last_input = 0;

	Player* player = game_state->player;
	game_state->adventure_depth = 1;

	unsigned char generate_up_ladder = TRUE;

	while (TRUE) {
		unsigned char exit = FALSE;


		World* world = get_world(player, generate_up_ladder);
		if (generate_up_ladder == FALSE)
			generate_up_ladder = TRUE;

		while (TRUE) {



			redraw_to_header();
			printf("  <>---- Подземелье ----<>\n");
			print_adventure_hud(game_state);
			printf("\n\n");

			print_world(world);

			//printf("\nframe %d\n", frame_counter);
			//printf("\nlast-input %c\n", last_input);

			char player_cell = *get_cell_player_standing(world);

			if (player_cell == ',') {
				if (rand_top(100) < 50) {
					printf("\n\033[31mПол скрипит!\033[0m\n");
				}
				else {
					generate_up_ladder = FALSE;
					game_state->adventure_depth++;
					break;
				}
			}
			else if (player_cell == '.') {
				if (is_cell_creaky(world, world->player->x, world->player->y)) {
					printf("\n\033[31mОсторожно, скрипучий пол!\033[0m\n");
					*get_cell(world, world->player->x, world->player->y) = ',';
				}
			}


			printf("\n[w] - Вверх   [s] - Вниз   [a] - Влево   [d] - Вправо   ");
			if (player_cell == 'U') {
				printf("[q] - Выход");
			}
			printf("\n>>>");

			frame_counter++;

			last_input = getchar();
			while (last_input == '\n') {
				last_input = getchar();
			}

			if (last_input == 'q' && *get_cell_player_standing(world) == 'U') {


				int c;
				while ((c = getchar()) != '\n' && c != EOF); //чистим чистим чистим

				exit = TRUE;
				break;
			}

			switch (last_input) {
			case 'w':
				if (try_move_player(world, 0, -1))
					player->stamina--;
				break;
			case 'a':
				if (try_move_player(world, -1, 0))
					player->stamina--;
				break;
			case 's':
				if (try_move_player(world, 0, 1))
					player->stamina--;
				break;
			case 'd':
				if (try_move_player(world, 1, 0))
					player->stamina--;
				break;

			}

			if (*get_cell_player_standing(world) == 'D') {
				int reward = BASE_GOLD_MULITPLIER * game_state->adventure_depth;
				player->gold += reward;

				game_state->adventure_depth++;
				break;
			}


		}

		free_world(world);
		if (exit) {
			break;
		}
	}

}


void menu_time_management(GameState* game_state) {
	if (game_state->player == NULL)
		print_time_submenu_selection("Ноунейм");
	else
		print_time_submenu_selection(game_state->player->username);
	int user_selection = get_int_input_range("", 0, 3);
	redraw_to_header();
	switch (user_selection) {
	case TIME_MENU_SELECTION_EXIT:
		printf("Возврат назад...\n");
		return;

	case TIME_MENU_SELECTION_CHECK_TIME:
		menu_check_time(game_state);
		break;

	case TIME_MENU_SELECTION_SKIP_TIME:
		menu_skip_time(game_state);
		break;
	case TIME_MENU_SELECTION_SLEEP:
		break;
	}
}

unsigned char do_you_want_to_delete_character() {
	printf("Персонаж уже существует.\n");
	printf("Вы уверены, что хотите удалить его и создать нового?\n");
	printf("[0] - Оставить страго персонажа\n");
	printf("[1] - Удалить и создать нового\n");
	int user_selection = get_int_input_range("", 0, 1);
	if (user_selection) {
		return 1;
	}
	printf("Оставляю старого персонажа...\n");
	return 0;
}

void make_character(GameState* game_state) {
	printf("---- Создание персонажа ----\n");
	Player* player = malloc(sizeof(Player));


	char username[MAX_TOKEN_LENGTH];

	get_string_input("Введите имя:", (char*)username);
	if (strcmp(username, "") == 0)
	{
		printf("Значит будешь Анонимом.\n");
		strcpy(username, "Аноним");
	}
	printf("Приветсвую тебя, %s!\n", username);


	init_player(player, username);

	game_state->player = player;
	printf("Персонаж создан!");
}

void menu_make_character(GameState* game_state) {
	if (game_state->player != NULL) {
		if (!do_you_want_to_delete_character()) {
			return;
		}
		
		redraw_to_header();

		printf("Память структуры игрока по адресу %p освобождена\n\n", game_state->player);
		free(game_state->player);
	}
	make_character(game_state);

}



void menu_print_character(GameState* game_state) {
	// красиво выводит его характеристики, текущую дату и время, а также содержимое инвентаря с названиями предметов.
	Player* player = game_state->player;
	printf("---- Карточка персонажа ----\n");
	
	printf("- День: %d\n", game_state->current_day);
	printf("- Время: %s\n", get_military_time(game_state->current_hour));

	printf("\n");

	printf("- Имя: \033[36m%s\033[0m\n", player->username);

	printf("  - Здоровье: ");
	print_colored_fraction(player->hp, player->max_hp);
	printf("\n");
	printf("  - Выносливость: ");
	print_colored_fraction(player->stamina, player->max_stamina);
	printf("\n");
	printf("\n");
	printf("  - Золото: \033[33m%dG\033[0m\n", player->gold);

	printf("\n\n- Инвентарь:\n");
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		printf("  - Слот %d: %s\n", i, game_state->item_id_to_name[player->inventory[i]]);
	}
	printf("\n\n\n");
}