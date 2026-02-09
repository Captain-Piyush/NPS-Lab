#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH "/tmp/socket_server"

void swap(char *x, char *y) {
    char temp = *x; *x = *y; *y = temp;
}

void permute(char *a, int l, int r) {
    if (l == r) printf("  [Permutation]: %s\n", a);
    else {
        for (int i = l; i <= r; i++) {
            swap((a + l), (a + i));
            permute(a, l + 1, r);
            swap((a + l), (a + i));
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[256];
    socklen_t client_len = sizeof(struct sockaddr_un);

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    unlink(SERVER_PATH);
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_PATH);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("--- SERVER STARTED ---\nWaiting for strings from client...\n");

    while (1) {
        int n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr *)&client_addr, &client_len);
        buffer[n] = '\0';

        if (strcmp(buffer, "exit") == 0) break;

        printf("\nClient sent: %s\n", buffer);
        permute(buffer, 0, strlen(buffer) - 1);

        // Half-Duplex: Send acknowledgement so client can send next message
        char *msg = "Server: Permutations printed. Over.";
        sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&client_addr, client_len);
    }

    close(sockfd);
    unlink(SERVER_PATH);
    return 0;
}