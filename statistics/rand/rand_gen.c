// rand_gen.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL)); // инициализация генератора
	
	int num = 1024 * 1024 * 5;
	
    for (size_t i = 0; i < num; i++) {
        unsigned char r = rand() % 256;
        fwrite(&r, 1, 1, stdout);
    }

    return 0;
}
