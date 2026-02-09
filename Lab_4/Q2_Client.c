#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char hostname[100], ip_address[100];

 
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Enter Hostname to lookup: ");
    scanf("%s", hostname);

    // 3. Send Hostname to Server
    send(sock_fd, hostname, strlen(hostname), 0);

    // 4. Receive IP Address
    int n = recv(sock_fd, ip_address, sizeof(ip_address) - 1, 0);
    if (n > 0) {
        ip_address[n] = '\0';
        printf("Server Response (IP): %s\n", ip_address);
    } else {
        printf("No response from server.\n");
    }

    close(sock_fd);
    return 0;
}