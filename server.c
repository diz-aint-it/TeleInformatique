#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "structs.h"
#include "crc32.h"

#define SERVER_PORT 12345

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server_addr, client_addr;
    int slen = sizeof(client_addr);
    OperationPacket packet;
    int recv_len;

    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return 1;
    }

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    while (1) {
        recv_len = recvfrom(sock, (char*)&packet, sizeof(OperationPacket), 0, (struct sockaddr*)&client_addr, &slen);
        if (recv_len == SOCKET_ERROR) {
            printf("recvfrom() failed with error code : %d\n", WSAGetLastError());
            continue;
        }

        unsigned int crc_check = crc32((unsigned char*)&packet, sizeof(OperationPacket) - sizeof(unsigned int));
        if (crc_check != packet.crc) {
            packet.result = ERROR_CRC;
        } else {
            switch (packet.operation) {
                case OP_ADD:
                    packet.result = packet.operand1 + packet.operand2;
                    break;
                case OP_SUB:
                    packet.result = packet.operand1 - packet.operand2;
                    break;
                case OP_MUL:
                    packet.result = packet.operand1 * packet.operand2;
                    break;
                case OP_DIV:
                    if (packet.operand2 == 0)
                        packet.result = ERROR_CRC;
                    else
                        packet.result = packet.operand1 / packet.operand2;
                    break;
                default:
                    packet.result = ERROR_CRC;
            }
        }
        packet.crc = crc32((unsigned char*)&packet, sizeof(OperationPacket) - sizeof(unsigned int));
        sendto(sock, (const char*)&packet, sizeof(OperationPacket), 0, (struct sockaddr*)&client_addr, slen);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}