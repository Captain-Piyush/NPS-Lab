#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH "/tmp/socket_server"
#define CLIENT_PATH "/tmp/socket_client"

int main() {
    int sockfd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[256];

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    unlink(CLIENT_PATH);

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strcpy(client_addr.sun_path, CLIENT_PATH);
    bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_PATH);

    printf("--- CLIENT STARTED ---\n(Type 'exit' to quit)\n");

    while (1) {
        printf("\nClient (You): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; 

        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (strcmp(buffer, "exit") == 0) break;

        // Wait for server to finish printing before sending next string
        int n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, NULL, NULL);
        buffer[n] = '\0';
        printf("%s\n", buffer);
    }

    close(sockfd);
    unlink(CLIENT_PATH);
    return 0;
}