#include <stdio.h>
#include <locale.h>

#define SELECTION_EXIT 0
#define SELECTION_CHECK_TIME 1
#define SELECTION_WORK 2
#define SELECTION_CHECK_INVENTORY 3
#define SELECTION_SET_ITEM 4
#define SELECTION_DROP_ITEM 5
#define SELECTION_INVENTORY_REVISION 6






const int HOURS_IN_DAY = 24;

const int START_DAY = 1;
int current_day;

const int START_HOUR = 8;
int current_hour;

#define INVENTORY_SIZE 10
int inventory[INVENTORY_SIZE];


const int UNIQUE_ITEMS_COUNT = 10;
/* item_ids
 * 
 * 0 - Пусто
 * 1 - Дерево
 * 2 - Камень
 * 3 - Семена
 * 4 - Лопата
 * 5 - Лейка
 * 6 - Лукошко
 * 7 - Яблоко
 * 8 - Морковка
 * 9 - Ягода
 */
const char item_id_to_name[][64] = {
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
#define ITEM_EMPTY 0
#define ITEM_WOOD 1
#define ITEM_STONE 2
#define ITEM_SEEDS 3
#define ITEM_SHOVEL 4
#define ITEM_WATERING_CAN 5
#define ITEM_BASKET 6
#define ITEM_APPLE 7
#define ITEM_CARROT 8
#define ITEM_BERRY 9



void init_variables() 
{
	 current_day = START_DAY;
	 current_hour = START_HOUR;

	 inventory[0] = ITEM_SHOVEL;
	 inventory[1] = ITEM_WATERING_CAN;
	 inventory[2] = ITEM_SEEDS;
	 inventory[3] = ITEM_SEEDS;

	 inventory[7] = ITEM_CARROT;
	 inventory[8] = ITEM_CARROT;
	 inventory[9] = ITEM_APPLE;
}

//		----	Функции для помощи при выводе	----
void print_action_selection();
int get_int_input(char* message);
int get_int_input_range(char* message, int lowest_value, int highest_value);
char* get_military_time(int hour);
void print_id_to_item();
void show_time() {
	printf("День %d, %s", current_day, get_military_time(current_hour));
}

//		----	Геймплейные функции				----
void feed_forward_time(int hours_to_skip);
int set_item(int slot_index, int item_id);
int count_items(int item_id);

//		----	Функции выбора из меню			----
void menu_work();
void menu_check_inventory();
void menu_set_item();
void menu_drop_item();
void menu_inventory_revision();

int main() 
{   
	setlocale(LC_ALL, "ru_RU.UTF-8"); //нейронка сказала, это надо чтобы было без кракозябр в выводе

	printf("Happy Farmer v0.01\n\n");
	
	init_variables();
	
	//Main loop
	while(1) 
	{

		print_action_selection();
		int user_selection = get_int_input_range("", 0, 6);
		
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
		default:
			printf("Такого действия нет!\n");
			break;
		}

	}
	return 0;
}


int get_int_input(char* message)
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

int get_int_input_range(char* message, int lowest_value, int highest_value)
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

void print_action_selection()
{
	printf("Выберите действие:\n");
	printf("[0] - Выход\n");
	printf("[1] - Посмотреть на часы\n");
	printf("[2] - Поработать\n");
	printf("[3] - Посмотреть инвентарь\n");
	printf("[4] - Положить предмет в слот\n");
	printf("[5] - Выбросить предмет\n");
	printf("[6] - Ревизия ресурсов\n");
	
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
	for (int i = 1; i < UNIQUE_ITEMS_COUNT; i++)
	{
		printf("[%d] - %s\n", i, item_id_to_name[i]);
	}

}

void feed_forward_time(int hours_to_skip) {
	current_hour += hours_to_skip;
	int days_passed = current_hour / HOURS_IN_DAY;
	current_hour %= 24;
	current_day += days_passed;
}

int set_item(int slot_index, int item_id) {
	if (slot_index < 0 || slot_index > INVENTORY_SIZE)
		return 0;
	if (item_id < 0 || item_id > UNIQUE_ITEMS_COUNT)
		return 0;

	inventory[slot_index] = item_id;
	return 1;
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
	int item_id = get_int_input_range("Введите айдишник желаемого предмета", 1, UNIQUE_ITEMS_COUNT - 1);
	
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

	set_item(slot_index, ITEM_EMPTY);
	printf("\n\n");
}

void menu_inventory_revision() {
	print_id_to_item();
	int item_id = get_int_input_range("Введите айдишник предмета для ревизии", 1, UNIQUE_ITEMS_COUNT - 1);

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