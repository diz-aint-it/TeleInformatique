#include <stdlib.h>
#include <time.h>
#include "error_simulator.h"

void simulate_error(unsigned char *buf, int len, double error_probability) {
    if (((double)rand() / RAND_MAX) < error_probability) {
        int byte_pos = rand() % len;
        int bit_pos = rand() % 8;
        buf[byte_pos] ^= (1 << bit_pos);
    }
}