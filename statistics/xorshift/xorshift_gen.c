// xorshift_gen.c
#include <stdio.h>
#include <stdint.h>

uint32_t xorshift32(uint32_t *state) {
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

int main() {
    uint32_t state = 123456789; // любое ненулевое начальное значение
    for (int i = 0; i < 12500000; i++) {
        uint32_t r = xorshift32(&state);
        fwrite(&r, sizeof(r), 1, stdout);
    }
    return 0;
}
