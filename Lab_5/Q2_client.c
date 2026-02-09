#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define MAX 100

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char str1[MAX], str2[MAX], msg[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Enter first string: ");
    fgets(str1, MAX, stdin);
    str1[strcspn(str1, "\n")] = '\0';
    send(sock, str1, strlen(str1)+1, 0);

    printf("Enter second string: ");
    fgets(str2, MAX, stdin);
    str2[strcspn(str2, "\n")] = '\0';
    send(sock, str2, strlen(str2)+1, 0);

    recv(sock, msg, sizeof(msg), 0);
    printf("Server says: %s\n", msg);

    close(sock);
    return 0;
}
