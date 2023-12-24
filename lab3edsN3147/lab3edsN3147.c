#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_RESET "\x1b[0m"
#define CROSSED_TEXT "\x1b[9m"
#define RESET_TEXT "\x1b[0m"

#define MAX_STRING 1000
#define MAX_ARRAY_LENGTH 500

char *pattern = "[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9]"; //регулярное выражение для номера карты

regex_t regex_pattern; //структура для представления регулярного выражения

regmatch_t match; //структура для хранения информации о вхождениях подстроки в рассматриваемую строку

int value; //идентификатор вхождения подстроки в рассматриваемую строку

int sline = 1; //номер строки, с которой начинаем поиск
int eline = -1; //номер строки, до которой продолжается поиск
int color = 0; //флаг для опции -c
int line = 0; // флаг для опции -n

int console_input = 0; //идентификатор ввода

FILE *fr, *fw; //файлы ввода/вывода

char str[MAX_STRING]; //строки для ввода и обработки данных
char *concatenated_string = NULL;

//функция - алгоритм Луна для проверки контрольной суммы найденной строки
int luhn(char *card) {
    int sum = 0;
    for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int digit = card[i * 5 + j] - '0';
			if (j % 2 == 0) { //если текущая цифра на четной позиции, удваиваем ее
				digit *= 2;
			}
			sum += digit - (9 * (digit > 9));
        }
	}
    return (sum % 10 == 0); //если сумма делится на 10 без остатка, номер валиден
}

//функция для проверки, являются ли введённые данные числом
int check(char *a) {
    int len = 0;
	int n = 1;
    while (*a) {
        if (!((*a - '0' >= 0 && *a - '0' <= 9))) {
			n = 0;
			break;
		}
        ++a;
        ++len;
    }
    if ((*a - '0' == 0) && (len != 1)) {
		n = 0;
	}
    return n;
}

//функция для проверки, являются ли введённые данные файлами для ввода/вывода
int check_file (char *str, char *mode) {
	if (!strcmp(mode, "r")) {
		if ((fr = fopen(str, "r")) == NULL) {
			fprintf(stderr, "Ошибка: не удалось открыть файл %s для чтения.\n", str);
			return EXIT_FAILURE;
		}
	} else fw = fopen(str, "w");
}

//функция для проверки, нужно ли устанавливать разрыв
void check_n (int *curr, int gaps, int* ind_gap, int wocolor) {
	if (*curr < gaps && wocolor == ind_gap[*curr]) {
    	*curr = *curr + 1;
    	if (fw != NULL) {
    		fprintf(fw, "\n");
    	} 
		else {
			printf("\n");
		}
    }
}

//вывод отформатированой строки
void print_modifyed_string(int* ind, char* str, int cur_ind, int* ind_gap, int gaps) {
    int wocolor = 0;
    int curr = 0;
   	for (int i = 0; i < cur_ind; i++) {
   		while (wocolor < ind[i]) {
    		if (gaps) {
    			check_n(&curr, gaps, ind_gap, wocolor);
			}
    		if (fw != NULL) {
				fprintf(fw, "%c", (char)(str[wocolor]));
			}
    		else {
				printf("%c", (char)(str[wocolor]));
			}
        	++wocolor;
        }
        if (fw != NULL) {
			if (color) {
				fprintf(fw, COLOR_MAGENTA);
			}
			else {
				fprintf(fw, CROSSED_TEXT);
			}
		}
		else {
			if (color) {
				printf(COLOR_MAGENTA);
			}
			else {
				printf(CROSSED_TEXT);
			}
		}
        for(int j = 0; j < 19; j++) {
        	if (gaps) {
        		check_n(&curr, gaps, ind_gap, wocolor);
        	}
        	if (fw != NULL) {
				fprintf(fw, "%c", (char) (str[wocolor]));
			}
    		else {
				printf("%c", (char) (str[wocolor]));
			}
        	++wocolor;
        }
        if (fw != NULL) {
			if (color) {
				fprintf(fw, COLOR_RESET);
			}
			else {
				fprintf(fw, RESET_TEXT);
			}
		}
		else {
			if (color) {
				printf(COLOR_RESET);
			}
			else {
				printf(RESET_TEXT);
			}
		}
    }
    while (wocolor < (int) strlen(str)) {
        if (gaps) {
        	check_n(&curr, gaps, ind_gap, wocolor);
        }
        if (fw != NULL) {
    		fprintf(fw, "%c", (char)(str[wocolor]));
    	} 
		else {
			printf("%c", (char)(str[wocolor]));
		}
        ++wocolor;
    }
    if (fw != NULL) {
		fprintf(fw, "\n");
	} 
	else {
		printf("\n");
	}
}

void analyze_string(char* str, int* ind_gap, int gaps, int lcnt) {
	value = regexec(&regex_pattern, str, 0, NULL, 0);
    if (!value) {
        int shift = 0;
        int ind[MAX_ARRAY_LENGTH];
        int cur_ind = 0;			
        // поиск вхождений подстроки
    	while (regexec(&regex_pattern, str + shift, 1, &match, REG_NOTBOL) == 0) {
        	if (match.rm_so == -1) {
				break;
        	}
        	char card[19];
        	for (int i = shift + match.rm_so; i < (int) (shift + match.rm_so + 19); i++) {
        		card[i - shift - match.rm_so] = (char)(str[i]);
        	}
        	// если найденная подстрока удовлетворяет алгоритму Луна, необходимо запомнить индекс начала такой подстроки
        	if (luhn(card)) {
        		if (((lcnt >= sline) || (sline == 1)) && ((lcnt <= eline) || (eline == -1))) {
        			ind[cur_ind] = shift + match.rm_so;
        			cur_ind++;
        		}
        	}
        	// обновление смещения для продолжения поиска
        	shift += match.rm_eo;
    	}
    	print_modifyed_string(ind, str, cur_ind, ind_gap, gaps);
	} 
	else {
		print_modifyed_string(NULL, str, 0, ind_gap, gaps);
	}
}

int solve() {
	int lcnt = 0;
	// компиляция регулярного выражения
    value = regcomp(&regex_pattern, pattern, 0);
	if (line) {
    	if (!console_input) {
    		while (fgets(str, sizeof(str), fr) != NULL) {
				lcnt++;
    			int length = strlen(str);
    			if (str[length - 1] == '\n') {
            			str[length - 1] = '\0';
            			length--;
        		}
        		analyze_string(str, NULL, 0, lcnt);
    		}
    	}
		else {
    		while (fgets(str, sizeof(str), stdin) != NULL) {
				lcnt++;
    			int length = strlen(str);
    			if (str[length - 1] == '\n') {
            		str[length - 1] = '\0';
            		length--;
        		}
    			analyze_string(str, NULL, 0, lcnt);
    		}
		}
	} 
	else {
		//обработка случая неединственной строки
    	int total_length = 0;
    	int ind_gap[MAX_ARRAY_LENGTH];
    	int gaps = 0;
    	if (!console_input) {
    		while (fgets(str, sizeof(str), fr) != NULL) {
				lcnt++;
    			int length = strlen(str);
    			// удаление символа переноса строки
    			if (str[length - 1] == '\n') {
            		str[length - 1] = '\0';
            		length--;
        		}
        		// динамическое выделение памяти для дополненной строки
    			char *temp = realloc(concatenated_string, total_length + length);	
    			// обработка ошибки выделения памяти
        		if (temp == NULL) {
            		fprintf(stderr, "Ошибка при выделении памяти\n");
            		free(concatenated_string);
            		return EXIT_FAILURE;
            	}
            	// дополнение общей строки
            	concatenated_string = temp;
        		strcpy(concatenated_string + total_length, str);
				total_length += length;
				// установка "разрыва" строки
        		ind_gap[gaps] = ((int)total_length);
        		gaps++;
            }
        }
		else {
        	while (fgets(str, sizeof(str), stdin) != NULL) {
				lcnt++;
    			int length = strlen(str);
    			// удаление символа переноса строки
    			if (str[length - 1] == '\n') {
            		str[length - 1] = '\0';
            		length--;
        		}
        		// динамическое выделение памяти для дополненной строки
    			char *temp = realloc(concatenated_string, total_length + length);
    			// обработка ошибки выделения памяти
        		if (temp == NULL) {
            		fprintf(stderr, "Ошибка при выделении памяти\n");
            		free(concatenated_string);
            		return EXIT_FAILURE;
            	}
            	// дополнение общей строки
            	concatenated_string = temp;
        		strcpy(concatenated_string + total_length, str);
        		total_length += length;
        		// установка "разрыва" строки
        		ind_gap[gaps] = ((int) total_length);
        		gaps++;
        	}
        }
		analyze_string(concatenated_string, ind_gap, gaps, lcnt);
    }
}

int check_opt(char *str) {
	int lenght_str = strlen(str);
	if (str[0] == '-') {
		switch (lenght_str) {
			case 1: //случай, когда после - ничего не введено
				fprintf(stderr, "Usage: ./lab3edsN3147 [опции] [имя_вход_файла [имя_выход_файла]]\n");
				exit(EXIT_FAILURE);
			case 2: //проверка опций -n, -c, -v
				if (strcmp(str, "-v") == 0) {
					fprintf(stderr, "Ошибка: опция '-v' не может быть использована с другими опциями\n");
					exit(EXIT_FAILURE);
				}
				else if (strcmp(str, "-c") == 0) {
					if (color) {
						fprintf(stderr, "Ошибка: опция -c используется несколько раз\n");
    	    			exit(EXIT_FAILURE);
					}
					else {
						color = 1;
						break;
					}
				}
				else if (strcmp(str, "-n") == 0) {
					if (line) {
						fprintf(stderr, "Ошибка: опция -n используется несколько раз\n");
    	    			exit(EXIT_FAILURE);
					}
					else {
						line = 1;
						break;
					}
				}
				else {
					fprintf(stderr, "Ошибка: неподдерживаемая опция %s\n", str);
    	    		exit(EXIT_FAILURE);
				}
			default: //проверка опций -b=M, -e=N
				if (str[1] == 'b' || str[1] == 'e') {
					if (str[2] == '=') {
						char val[strlen(str) - 2];
						for (int i = 3; i < strlen(str); i++) {
							val[i - 3] = str[i];
						}
						val[strlen(str) - 3] = '\0';
						if (!check(val)) {
							fprintf(stderr, "Ошибка: %s не является натуральным числом\n", val);
							exit(EXIT_FAILURE);
						}
						else {
							if (str[1] == 'b') {
								if (sline != 1) {
									fprintf(stderr, "Ошибка: опция -b используется несколько раз\n");
									exit(EXIT_FAILURE);
								}
								else {
									sline = strtoul(val, NULL, 10);
									break;
								}
							}
							else if (str[1] == 'e') {
								if (eline != -1) {
									fprintf(stderr, "Ошибка: опция -e используется несколько раз\n");
									exit(EXIT_FAILURE);
								}
								else {
									eline = strtoul(val, NULL, 10);
									break;
								}
							}
						}	
					}
					else {
						fprintf(stderr, "Ошибка: неподдерживаемая опция %s\n", str);
						exit(EXIT_FAILURE);
					}
				}
				else {
					fprintf(stderr, "Ошибка: неподдерживаемая опция %s\n", str);
    	    		exit(EXIT_FAILURE);
				}
		}
	}
	else {
		if (fr == NULL) {
			check_file(str, "r");
		}
		else {
			check_file(str, "w");
		}
	}
}
			

int main(int argc, char *argv[]) {
	char *DEBUG = getenv("LAB3DEBUG");
    if (DEBUG) {
        fprintf(stderr, "Включен вывод отладочных сообщений\n");
    }
	
	if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        printf("Щенникова Елена Дмитриевна, гр. N3147\nВариант: 4-1-4-5\n");
        return EXIT_SUCCESS;
    }
	
	switch(argc) {
    	case 1:
    		// определение идентификатора ввода
    	    console_input = 1;
    	    solve();
    	    break;
    	case 2:
    	case 3:
    	case 4:
    	case 5:
    	case 6:
    	case 7:
    	case 8:
			// проверка введённых опций
    		for (int i = 1; i < argc; i++) {
				check_opt(argv[i]);
    			if ((i < argc - 2 && ((fr != NULL))) || (i != argc - 1 && fw != NULL)) {
					fprintf(stderr, "Usage: lab3edsN3147 [опции] [имя_вход_файла [имя_выход_файла]]\n");
					return EXIT_FAILURE;
    			}
    		}
			if (fr != NULL) {
				console_input = 0;
			}
    		else {
				console_input = 1;
			}
    		solve();
    		break;
    	default: 
    		fprintf(stderr, "Usage: lab3edsN3147 [опции] [имя_вход_файла [имя_выход_файла]]\n");
    	    return EXIT_FAILURE;
    	}
    //очистка памяти
    regfree(&regex_pattern);
    if (fr != NULL) fclose(fr);
    if (fw != NULL) fclose(fw);
    free(concatenated_string);
    return EXIT_SUCCESS;
}