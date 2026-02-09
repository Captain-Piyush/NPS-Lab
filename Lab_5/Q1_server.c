#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    char messages[2][50];
    int client_sockets[2];
    int client_count = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_fd, 5);
    printf("Server started...\n");

    while (1) {
        new_socket = accept(server_fd,
                            (struct sockaddr *)&client_addr,
                            &addr_len);

        /* If more than 2 clients connect */
        if (client_count >= 2) {
            send(new_socket, "terminate session", 18, 0);
            close(new_socket);
            printf("More than 2 clients connected. Server terminating.\n");
            break;
        }

        int bytes = recv(new_socket,
                          messages[client_count],
                          sizeof(messages[client_count]) - 1,
                          0);

        messages[client_count][bytes] = '\0';   // null terminate

        client_sockets[client_count] = new_socket;

        printf("Received \"%s\" from %s:%d\n",
               messages[client_count],
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        client_count++;

        /* When two clients are connected */
        if (client_count == 2) {

            /* Open file containing "Manipal" */
            FILE *fp = fopen("data.txt", "r");
            if (fp == NULL) {
                perror("File error");
                exit(1);
            }

            char word[50];
            fgets(word, sizeof(word), fp);
            fclose(fp);

            word[strcspn(word, "\n")] = '\0';   // remove newline

            printf("\nFinal Output:\n");
            printf("%s %s %s\n", word, messages[0], messages[1]);

            printf("\nClient Socket Addresses:\n");
            for (int i = 0; i < 2; i++) {
                printf("Socket Descriptor: %d\n", client_sockets[i]);
                close(client_sockets[i]);
            }
        }
    }

    close(server_fd);
    return 0;
}
