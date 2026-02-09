#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 5

// Structure for route information
typedef struct {
    char source[20];
    char destination[20];
    int totalSeats;
    int bookedSeats;
} Route;

// Two routes
Route routes[2] = {
    {"CityA", "CityB", 10, 0},
    {"CityC", "CityD", 15, 0}
};

pthread_mutex_t lock;

void *clientHandler(void *arg) {
    int clientSocket = *(int *)arg;
    free(arg);
    char buffer[1024];

    sprintf(buffer,
        "Available Routes:\n"
        "1. %s -> %s | Available Seats: %d | Booked Seats: %d\n"
        "2. %s -> %s | Available Seats: %d | Booked Seats: %d\n",
        routes[0].source, routes[0].destination, routes[0].totalSeats - routes[0].bookedSeats, routes[0].bookedSeats,
        routes[1].source, routes[1].destination, routes[1].totalSeats - routes[1].bookedSeats, routes[1].bookedSeats
    );

    send(clientSocket, buffer, strlen(buffer), 0);

    int routeNumber;
    if (recv(clientSocket, &routeNumber, sizeof(routeNumber), 0) <= 0) {
        printf("Client disconnected unexpectedly.\n");
        close(clientSocket);
        pthread_exit(NULL);
    }

    int seatsRequested;
    if (recv(clientSocket, &seatsRequested, sizeof(seatsRequested), 0) <= 0) {
        printf("Client disconnected unexpectedly.\n");
        close(clientSocket);
        pthread_exit(NULL);
    }

    pthread_mutex_lock(&lock);

    if (routeNumber >= 1 && routeNumber <= 2) {
        Route *r = &routes[routeNumber - 1];
        if (r->bookedSeats + seatsRequested <= r->totalSeats) {
            r->bookedSeats += seatsRequested;
            sprintf(buffer, "Booking successful! %d seats booked for %s -> %s\n", seatsRequested, r->source, r->destination);
            printf("Client booked %d seats on route %d (%s -> %s). Updated: %d/%d booked.\n",
                   seatsRequested, routeNumber, r->source, r->destination, r->bookedSeats, r->totalSeats);
        } else {
            sprintf(buffer, "Booking failed! Only %d seats available for %s -> %s\n",
                    r->totalSeats - r->bookedSeats, r->source, r->destination);
            printf("Client attempted to book %d seats on route %d (%s -> %s) but only %d available.\n",
                   seatsRequested, routeNumber, r->source, r->destination, r->totalSeats - r->bookedSeats);
        }
    } else {
        strcpy(buffer, "Invalid route number!\n");
        printf("Client entered invalid route number: %d\n", routeNumber);
    }

    pthread_mutex_unlock(&lock);

    send(clientSocket, buffer, strlen(buffer), 0);
    close(clientSocket);
    pthread_exit(NULL);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    pthread_mutex_init(&lock, NULL);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started. Waiting for clients...\n");

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New client connected from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        pthread_t tid;
        int *pclient = malloc(sizeof(int));
        *pclient = new_socket;
        pthread_create(&tid, NULL, clientHandler, pclient);
        pthread_detach(tid);
    }

    pthread_mutex_destroy(&lock);
    close(server_fd);
    return 0;
}
