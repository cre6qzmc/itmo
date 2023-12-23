#include <stdlib.h>
#include <stdio.h>
#include <time.h>

unsigned short hu_max = 65535;

//проверяем, являются ли введённые данные числом
int check(char *argv[]) {
    char *a = argv[1];
    int len = 0, zero = (*a == '0'), n = ((*a >= '0') && (*a <= '9'));
    while (*a) {
        if (!(*a >= '0' && *a <= '9')) {
            n = 0;
            break;
        }
        ++a;
        ++len;
    }
    if (zero && (len != 1)) {
    	n = 0;
    }
    return n;
}

//функция для вывода числа в двоичной форме, с разделением байтов
void bin(unsigned short a) {

    //битовыми сдвигами проходим по каждому биту числа слева направо, выводя их значения и разделяя биты пробелами
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            if (a & ((unsigned short) 1 << (16 - 1))) {
                printf("1");
            } else printf("0");
            a <<= 1;
        }
        printf(" ");
    }
}

//функция, выполняющее основное задание
unsigned short doublets(unsigned short num) {

	int cnt_00 = 0, cnt_01 = 0, cnt_10 = 0, cnt_11 = 0; //счётчики для каждого дублета
	int curr_d = 8; //текущий дублет (т. к. число двухбайтовое - дублетов 8)
	unsigned short sorted = 0; 
	
	//считаем, сколько у нас дублетов каждого вида в введённом числе
	while (num > 0) {
		if (num % 4 == 3) {
			cnt_11 += 1;
		}
		else if (num % 4 == 2) {
			cnt_10 += 1;
		}
		else if (num % 4 == 1) {
			cnt_01 += 1;
		}
		num >>= 2;
	}
	
	cnt_00 = 8 - (cnt_01 + cnt_10 + cnt_11);
	
	// образуем новое число: в самое начало у нас идут дублеты со значением "11", далее - "10", ...
	while (cnt_11 > 0) {
			sorted += ((unsigned short) 1 << (curr_d * 2 - 1)) + ((unsigned short) 1 << (curr_d * 2 - 2));
			curr_d--;
			cnt_11--;
	}
	while (cnt_10 > 0) {
			sorted += ((unsigned short) 1 << (curr_d * 2 - 1));
			curr_d--;
			cnt_10--;
	}
	while (cnt_01 > 0) {
			sorted += ((unsigned short) 1 << (curr_d * 2 - 2));
			curr_d--;
			cnt_01--;
	}
	
	return sorted;
}


int main(int argc, char *argv[]) {
    // Проверка запуска с переменной среды, включающей отладочный вывод.
    // Пример запуска с установкой переменной LAB1DEBUG в 1:
    // $ LAB1DEBUG=1 ./lab1abcNXXXXX 123
    char *DEBUG = getenv("LAB1DEBUG");
    if (DEBUG) {
        fprintf(stderr, "Включен вывод отладочных сообщений\n");
    }
    
    if (argc != 2) {
        srand(time(NULL));
        unsigned short a = 0 + rand() % (hu_max + 1);
        printf("Случайное число: %hu\n", a);
		bin(a);
		printf("\n");
		a = doublets(a);
		bin(a);
		printf("\n");
    }
	else if (check(argv)) {
		char *end;
        unsigned long long a = strtoul(argv[1], &end, 10);
		if (a > hu_max) {
			printf("Число не входит в диапазон входных данных\n");
		}
		else {
			bin(a);
			printf("\n");
			a = doublets(a);
			bin(a);
			printf("\n");
		}
    } 
	else printf("Ошибка: '%s' не является числом.\n", argv[1]);
    
    return EXIT_SUCCESS;
}



