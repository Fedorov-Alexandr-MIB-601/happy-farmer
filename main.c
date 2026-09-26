#include <stdio.h>

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

int main() 
{
	printf("Happy Farmer v0.01\n\n");
	
	init_variables();
	
	//Main loop
	while(1) 
	{
		print_action_selection();
		int user_selection = get_user_selection();
		printf("\nВы ввели %d\n\n", user_selection);
	}
	return 0;
}

int get_user_selection()
{
	while(1)
	{
		printf(">>>");
		int raw_input;
		if (scanf("%d", &raw_input) == 0)
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
