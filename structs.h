#ifndef STRUCTS_H
#define STRUCTS_H

#define OP_ADD '+'
#define OP_SUB '-'
#define OP_MUL '*'
#define OP_DIV '/'

#define ERROR_CRC -99999

typedef struct {
    char operation; // '+', '-', '*', '/'
    int operand1;
    int operand2;
    int result;     // Result or error code
    unsigned int crc;
} OperationPacket;

#endif