#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>

//функция для нахождения кольца с минимальной характеристикой
float findMinCh(float *mtx, int k, int rows, int cols) {
    float minCh = FLT_MAX;
	int minRow, minCol;
	
	if (k > rows || k > cols) {
		printf("Ошибка: характеристика кольца больше размеров матрицы.\n");
	}
	
	else if (k <= 0) {
		printf("Ошибка: характеристика кольца не может быть меньше или равна нулю.\n");
	}
	
	else {
		//проходим по всем кольцам заданного порядка
		for (int i = 0; i <= rows - k; i++) {
			for (int j = 0; j <= cols - k; j++) {
				float curCh = 0.0;

				//вычисление характеристики текущего кольца
				for (int m = i; m < i + k; m++) {
					for (int n = j; n < j + k; n++) {
						curCh += mtx[m * rows + n] * mtx[m * rows + n];
					}
				}
				
				for (int m = i + 1; m < i + k - 1; m++) {
					for (int n = j + 1; n < j + k - 1; n++) {
						curCh -= mtx[m * rows + n] * mtx[m * rows + n];
					}
				}
				
				//обновление минимальной характеристики и позиции кольца
				if (curCh < minCh) {
					minCh = curCh;
					minRow = i + 1;
					minCol = j + 1;
				}
			}
		}
		printf("Кольцо порядка %d с минимальной характеристикой (%.2f) начинается в строке %d и стоблце %d\n", k, minCh, minRow, minCol);
	}
}

int main(int argc, char *argv[]) {
    char *DEBUG = getenv("LAB2DEBUG");
    if (DEBUG) {
        fprintf(stderr, "Включен вывод отладочных сообщений\n");
    }

    if ((argc != 4 || strcmp(argv[1], "-m")) && (argc != 3)) {
        fprintf(stderr, "Usage: %s [-m] число_строк число_столбцов\n", argv[0]);
        return EXIT_FAILURE;
    }
    
	float temprows = atof(argv[argc - 2]);
	float tempcols = atof(argv[argc - 1]);
	
	float min_value = 0.0; //для заполнения массива случайными числами в диапазоне от 0 до 100
	float max_value = 100.0;
	
	//проверка на корректность введённых данных
	if (temprows <= 0 || tempcols <= 0 || ((float)((int) temprows) != temprows) || ((float)((int) tempcols) != tempcols)) {
		printf("Ошибка: неверные размеры матрицы\n");
		return EXIT_FAILURE;
	}
	
	int rows = (int) temprows;
	int cols = (int) tempcols;

	float *mtx = malloc(rows * cols * sizeof(float)); //способ представления матрицы - одномерный массив, содержащий строки матрицы
	
	//проверка на наличие аргумента -m
	int manual = 0;
	if (argc > 3 && strcmp(argv[1], "-m") == 0) {
		manual = 1;
	}
	
	if (manual) { //если -m есть - ввод с клавиатуры
		printf("Введите строки матрицы:\n");
		for (int i = 0; i < rows; i++) {
			char inp[100]; //массив, в который записываются введённые строки матрицы
			int cur = 0;
			float matrix[100] = {0.0}; //массив, в который записываются числа из временного массива
			int cnt = 0;
			if (fgets(inp, sizeof(inp), stdin) != NULL) {
				while (inp[cnt] != '\0') {
					char tmp[100]={' '}; //временный массив для записи отдельных элементов
					int tm = 0; //счётчик элементов
					do {
						tmp[tm] = inp[cnt];
						cnt++;
						tm++;
					} while ((inp[cnt] != ' ') && (inp[cnt] != '\0'));
					if ((atof(tmp) == 0.0) && (tmp[0] != '0')){ //проверка, являеются ли введённые данные числом
						printf("%s не является числом\n", tmp);
						return EXIT_FAILURE;
					}
					else {
						matrix[cur] = atof(tmp);
					}
					cur++;
					for (int j = 0; j < cols; j++) {
						mtx[i * rows + j] = matrix[j]; //записываем элементы из массива с элементами в строку в наш исходный массив
					}
				}
			}
			else {
				printf("Ошибка: введено недопустимое значение\n");
				return EXIT_FAILURE;
			}
		}
	}
	else { //если -m нет, заполняем массив случаными числами
		srand(time(NULL));
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				mtx[i * rows + j] = min_value + (max_value - min_value) * ((float) rand() / RAND_MAX);
			}
		}
	}
	
	printf("Введённая матрица:\n");

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%-6.2f ", mtx[i * rows + j]);
        }
        printf("\n");
    }
	
	int k; //характеристика кольца
	
	printf("Введите порядок кольца: ");
	int x = scanf("%d", &k);
	
	if (&x != NULL) {
		findMinCh(mtx, k, rows, cols);
	}
    else {
		printf("Ошибка: введено недопустимое значение\n");
		return EXIT_FAILURE;
	}
	
    return EXIT_SUCCESS;
}