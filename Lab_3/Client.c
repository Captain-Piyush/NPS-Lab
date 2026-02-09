#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/chat_socket"
#define BUF_SIZE 100

int main() {
    int sock_fd;
    struct sockaddr_un addr;
    char buffer[BUF_SIZE];

    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(1);
    }

    pid_t pid = fork();

    if (pid > 0) {
        /* Parent process: Sending */
        printf("\n[CLIENT PARENT] PID=%d PPID=%d\n", getpid(), getppid());
        while (1) {
            printf("Client: ");
            fgets(buffer, BUF_SIZE, stdin);
            write(sock_fd, buffer, strlen(buffer));
        }
    } else {
        /* Child process: Receiving */
        printf("\n[CLIENT CHILD] PID=%d PPID=%d\n", getpid(), getppid());
        while (1) {
            int n = read(sock_fd, buffer, BUF_SIZE);
            if (n <= 0) break;
            buffer[n] = '\0';
            printf("\nServer: %s", buffer);
        }
    }

    close(sock_fd);
    return 0;
}
