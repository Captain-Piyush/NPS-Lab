#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/chat_socket"
#define BUF_SIZE 100

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUF_SIZE];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(1);
    }

    unlink(SOCKET_PATH);   // remove old socket file

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    listen(server_fd, 5);
    printf("Server waiting for connection...\n");

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept");
        exit(1);
    }

    printf("Client connected\n");

    pid_t pid = fork();

    if (pid > 0) {
        /* Parent process: Sending */
        printf("\n[SERVER PARENT] PID=%d PPID=%d\n", getpid(), getppid());
        while (1) {
            printf("Server: ");
            fgets(buffer, BUF_SIZE, stdin);
            write(client_fd, buffer, strlen(buffer));
        }
    } else {
        /* Child process: Receiving */
        printf("\n[SERVER CHILD] PID=%d PPID=%d\n", getpid(), getppid());
        while (1) {
            int n = read(client_fd, buffer, BUF_SIZE);
            if (n <= 0) break;
            buffer[n] = '\0';
            printf("\nClient: %s", buffer);
        }
    }

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
