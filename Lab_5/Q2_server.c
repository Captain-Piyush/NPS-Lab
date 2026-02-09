#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 5000
#define MAX 100

int isAnagram(char str1[], char str2[]) {
    int count[256] = {0};

    if (strlen(str1) != strlen(str2)) return 0;

    for (int i = 0; str1[i] && str2[i]; i++) {
        count[(unsigned char)str1[i]]++;
        count[(unsigned char)str2[i]]--;
    }

    for (int i = 0; i < 256; i++) {
        if (count[i] != 0) return 0;
    }
    return 1;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char str1[MAX], str2[MAX], msg[MAX];

    // Create socket
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
        new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Display connection info
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        char timeStr[50];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);

        printf("[%s] Connected to client: %s:%d\n",
               timeStr,
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        recv(new_socket, str1, sizeof(str1), 0);
        str1[strcspn(str1, "\n")] = '\0';
        recv(new_socket, str2, sizeof(str2), 0);
        str2[strcspn(str2, "\n")] = '\0';

        if (isAnagram(str1, str2))
            strcpy(msg, "The strings are anagrams.");
        else
            strcpy(msg, "The strings are NOT anagrams.");

        send(new_socket, msg, strlen(msg)+1, 0);
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
