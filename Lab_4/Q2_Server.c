#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


struct DNSEntry {
    char hostname[50];
    char ip[20];
};

struct DNSEntry database[] = {
    {"google.com", "142.250.190.46"},
    {"github.com", "140.82.121.4"},
    {"openai.com", "13.107.246.40"},
    {"localhost", "127.0.0.1"}
};

int db_size = 4;

void perform_lookup(int client_fd) {
    char buffer[100];
    char result[100] = "Host Not Found";
    
    int n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0) return;
    buffer[n] = '\0';

    printf("Lookup request received for: %s\n", buffer);


    for (int i = 0; i < db_size; i++) {
        if (strcmp(buffer, database[i].hostname) == 0) {
            strcpy(result, database[i].ip);
            break;
        }
    }

    send(client_fd, result, strlen(result), 0);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9000);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("DNS Server is running on port 9000...\n");

    while (1) {
        client_fd = accept(server_fd, NULL, NULL);
        if (client_fd > 0) {
            perform_lookup(client_fd);
            close(client_fd);
        }
    }

    return 0;
}