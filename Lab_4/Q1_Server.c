#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

struct Request {
    int choice;
    char detail[50];
};

void handle_client(int client_fd) {
    struct Request req;
    char response[1024];
    
    // Receive data from client
    int bytes = recv(client_fd, &req, sizeof(req), 0);
    if (bytes <= 0) {
        close(client_fd);
        return;
    }

    printf("[CHILD] Received Choice: %d, Data: %s\n", req.choice, req.detail);

    pid_t pid = fork();

    if (pid == 0) { // Child process starts here
        switch(req.choice) {
            case 1:
                sprintf(response, "PID: %d | Name: Alice | Address: 101 Silicon Valley, CA", getpid());
                break;
            case 2:
                sprintf(response, "PID: %d | Dept: IT | Sem: 6 | Sec: B | Courses: Networks, Security", getpid());
                break;
            case 3:
                sprintf(response, "PID: %d | Subject: %s | Marks: 92/100", getpid(), req.detail);
                break;
            default:
                sprintf(response, "Invalid Choice");
        }
        
        send(client_fd, response, strlen(response), 0);
        printf("[CHILD %d] Response sent and child exiting.\n", getpid());
        close(client_fd);
        exit(0); 
    } else {
        // Parent process
        close(client_fd);
        waitpid(-1, NULL, WNOHANG); // Clean up zombie processes
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    int opt = 1;
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Allow immediate reuse of the port to avoid "Address already in use" errors
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_fd, 5);
    printf("Server listening on port 8080...\n");

    while (1) {
        client_fd = accept(server_fd, NULL, NULL);
        if (client_fd > 0) {
            printf("\n[PARENT] New client connected! Creating child task...\n");
            handle_client(client_fd);
        }
    }
    return 0;
}