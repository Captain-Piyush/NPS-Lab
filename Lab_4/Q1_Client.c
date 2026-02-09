#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct Request {
    int choice;
    char detail[50];
};

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    struct Request req;
    char buffer[1024];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("1. Reg Number\n2. Student Name\n3. Subject Code\nEnter choice: ");
    scanf("%d", &req.choice);
    printf("Enter detail: ");
    scanf("%s", req.detail);

    send(client_fd, &req, sizeof(req), 0);
    
    recv(client_fd, buffer, sizeof(buffer), 0);
    printf("\n--- Server Response ---\n%s\n", buffer);

    close(client_fd);
    return 0;
}