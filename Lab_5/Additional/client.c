#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    read(sock, buffer, sizeof(buffer));
    printf("%s", buffer);

    int routeNumber, seats;
    printf("Enter route number (1 or 2): ");
    scanf("%d", &routeNumber);
    printf("Enter number of seats to book: ");
    scanf("%d", &seats);

    send(sock, &routeNumber, sizeof(routeNumber), 0);
    send(sock, &seats, sizeof(seats), 0);

    // Receive booking status
    memset(buffer, 0, sizeof(buffer));
    read(sock, buffer, sizeof(buffer));
    printf("%s\n", buffer);

    close(sock);
    return 0;
}
