#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void sort_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char *data = malloc(size + 1);
    fread(data, 1, size, fp);
    data[size] = '\0';
    fclose(fp);

    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (data[i] > data[j]) {
                char temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
    fp = fopen(filename, "w");
    fprintf(fp, "%s", data);
    fclose(fp);
    free(data);
}

int main() {
    signal(SIGPIPE, SIG_IGN);
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    printf("TCP Server running on port %d...\n", PORT);

    new_socket = accept(server_fd, NULL, NULL);

    // Step 1: Handle Filename
    memset(buffer, 0, BUFFER_SIZE);
    read(new_socket, buffer, BUFFER_SIZE);
    char filename[100];
    strcpy(filename, buffer);

    FILE *f = fopen(filename, "r+");
    if (!f) {
        send(new_socket, "File not present", 17, 0);
        close(new_socket);
        close(server_fd);
        return 0;
    }
    send(new_socket, "File found", 11, 0);

    // Step 2: Menu Loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        if (read(new_socket, buffer, BUFFER_SIZE) <= 0) break;
        int choice = atoi(buffer);

        if (choice == 1) { // Search
            char target[100], word[100];
            read(new_socket, target, 100);
            int count = 0; rewind(f);
            while (fscanf(f, "%s", word) != EOF) {
                if (strcmp(word, target) == 0) count++;
            }
            if (count > 0) {
                sprintf(buffer, "Occurrences: %d", count);
                send(new_socket, buffer, strlen(buffer), 0);
            } else send(new_socket, "String not found", 17, 0);
        } 
        else if (choice == 2) { // Replace
            char s1[100], s2[100], word[100];
            read(new_socket, s1, 100);
            read(new_socket, s2, 100);
            FILE *tmp = fopen("temp.txt", "w");
            rewind(f);
            int found = 0;
            while (fscanf(f, "%s", word) != EOF) {
                if (strcmp(word, s1) == 0) { fprintf(tmp, "%s ", s2); found = 1; }
                else fprintf(tmp, "%s ", word);
            }
            fclose(f); fclose(tmp);
            remove(filename); rename("temp.txt", filename);
            f = fopen(filename, "r+");
            if (found) send(new_socket, "String replaced", 16, 0);
            else send(new_socket, "String not found", 17, 0);
        }
        else if (choice == 3) { // Reorder
            fclose(f);
            sort_file(filename);
            f = fopen(filename, "r+");
            send(new_socket, "File reordered by ASCII", 24, 0);
        }
        else if (choice == 4) break;
    }
    close(new_socket); close(server_fd);
    return 0;
}