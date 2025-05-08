#include "crc32.h"

unsigned int crc32(const unsigned char *buf, unsigned int len) {
    unsigned int crc = 0xFFFFFFFF;
    for (unsigned int i = 0; i < len; i++) {
        crc ^= buf[i];
        for (unsigned int j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
    }
    return ~crc;
}