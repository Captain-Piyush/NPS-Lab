#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr = { .sin_family = AF_INET, .sin_port = htons(PORT) };
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed"); return 1;
    }

    char buffer[1024];
    printf("Enter filename: "); scanf("%s", buffer);
    send(sock, buffer, strlen(buffer), 0);

    memset(buffer, 0, 1024);
    read(sock, buffer, 1024);
    if (strcmp(buffer, "File not present") == 0) {
        printf("Server: %s. Terminating.\n", buffer);
        close(sock); return 0;
    }

    int choice;
    while (1) {
        printf("\n1.Search 2.Replace 3.Reorder 4.Exit\nChoice: ");
        scanf("%d", &choice);
        sprintf(buffer, "%d", choice);
        send(sock, buffer, strlen(buffer), 0);

        if (choice == 4) break;
        if (choice == 1) {
            printf("String to search: "); scanf("%s", buffer);
            send(sock, buffer, strlen(buffer), 0);
        } else if (choice == 2) {
            printf("Find: "); scanf("%s", buffer); send(sock, buffer, 100, 0);
            printf("Replace with: "); scanf("%s", buffer); send(sock, buffer, 100, 0);
        }
        
        memset(buffer, 0, 1024);
        read(sock, buffer, 1024);
        printf("Server: %s\n", buffer);
    }
    close(sock);
    return 0;
}