#include <stdio.h>
#include <locale.h>

#define SELECTION_EXIT 0
#define SELECTION_CHECK_TIME 1
#define SELECTION_WORK 2
#define SELECTION_CHECK_INVENTORY 3
#define SELECTION_SET_ITEM 4
#define SELECTION_DROP_ITEM 5
#define SELECTION_INVENTORY_REVISION 6







const int START_DAY = 1;
int current_day;

const int START_HOUR = 8;
int current_hour;

#define INVENTORY_SIZE 10
int inventory[INVENTORY_SIZE];

/* item_ids
 * 
 * 0 - Пусто
 * 1 - Дерево
 * 2 - Камень
 * 3 - Семена
 * 4 - Лопата
 * 5 - Потом придумаю
 * 6 - Потом придумаю
 * 7 - Потом придумаю
 * 8 - Потом придумаю
 * 9 - Потом придумаю
 */
void init_variables() 
{
	 current_day = START_DAY;
	 current_hour = START_HOUR;
	 
}

void print_action_selection();
int get_user_selection();
char* get_military_time(int hour);

int main() 
{   
	setlocale(LC_ALL, "ru_RU.UTF-8"); //нейронка сказала, это надо чтобы было без кракозябр в выводе

	printf("Happy Farmer v0.01\n\n");
	
	init_variables();
	
	//Main loop
	while(1) 
	{
		print_action_selection();
		int user_selection = get_user_selection();
		
		switch (user_selection) 
		{
		case SELECTION_EXIT:
			printf("Выход из программы...\n");
			return 0;
			break;
		case SELECTION_CHECK_TIME:
			printf("Текущее время: День %d, %s \n\n", current_day, get_military_time(current_hour));
			break;
		case SELECTION_WORK:
			printf("2");
			break;
		case SELECTION_CHECK_INVENTORY:
			printf("3");
			break;
		case SELECTION_SET_ITEM:
			printf("4");
			break;
		case SELECTION_DROP_ITEM:
			printf("5");
			break;
		case SELECTION_INVENTORY_REVISION:
			printf("6");
			break;
		default:
			printf("Такого действия нет!");
			break;
		}
	}
	return 0;
}

int get_user_selection()
{
	while(1)
	{
		printf(">>>");
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
	printf("\n");
	
}

char* get_military_time(int hour) {
	hour %= 24;
	//if (hour < 10)
	static char result[6] = "";
	
	if (hour < 10) {
		strcat(result, "0");
	}
	snprintf(result, sizeof(result), "%02d:00", hour);
	return result;

}
