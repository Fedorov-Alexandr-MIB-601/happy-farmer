#include "parser.h"

#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#ifdef _WIN32
#include <Windows.h>
#endif


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