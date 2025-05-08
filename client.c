#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#include "structs.h"
#include "crc32.h"
#include "error_simulator.h"

#define SERVER_PORT 12345
#define SERVER_IP "127.0.0.1"
#define ERROR_PROBABILITY 0.2

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server_addr;
    int slen = sizeof(server_addr);
    OperationPacket packet, recv_packet;
    int retry = 0, max_retries = 5;

    srand((unsigned int)time(NULL));
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
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    printf("Enter operation (+, -, *, /): ");
    scanf(" %c", &packet.operation);
    printf("Enter operand 1: ");
    scanf("%d", &packet.operand1);
    printf("Enter operand 2: ");
    scanf("%d", &packet.operand2);
    packet.result = 0;

    do {
        // Calculate CRC
        packet.crc = crc32((unsigned char*)&packet, sizeof(OperationPacket) - sizeof(unsigned int));

        // Copy to buffer and simulate error
        unsigned char buf[sizeof(OperationPacket)];
        memcpy(buf, &packet, sizeof(OperationPacket));
        simulate_error(buf, sizeof(OperationPacket), ERROR_PROBABILITY);

        // Send to server
        sendto(sock, (const char*)buf, sizeof(OperationPacket), 0, (struct sockaddr*)&server_addr, slen);

        // Receive response
        int recv_len = recvfrom(sock, (char*)&recv_packet, sizeof(OperationPacket), 0, (struct sockaddr*)&server_addr, &slen);
        if (recv_len == SOCKET_ERROR) {
            printf("recvfrom() failed with error code : %d\n", WSAGetLastError());
            closesocket(sock);
            WSACleanup();
            return 1;
        }

        // Check CRC
        unsigned int crc_check = crc32((unsigned char*)&recv_packet, sizeof(OperationPacket) - sizeof(unsigned int));
        if (crc_check != recv_packet.crc || recv_packet.result == ERROR_CRC) {
            printf("CRC error detected. Retrying...\n");
            retry++;
        } else {
            printf("Result: %d %c %d = %d\n", packet.operand1, packet.operation, packet.operand2, recv_packet.result);
            break;
        }
    } while (retry < max_retries);

    if (retry == max_retries) {
        printf("Failed to get a valid response after %d attempts.\n", max_retries);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}