#include <stdio.h>
#include <string.h>

#define MAX 100

int main() {
    char received[MAX];
    int i, count = 0, choice;

    printf("RECEIVER SIDE:\n");

    printf("Enter received data (including parity bit at end): ");
    scanf("%s", received);

    printf("Enter parity type used:\n");
    printf("1. Even Parity\n");
    printf("2. Odd Parity\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    // Count number of 1's including parity bit
    for(i = 0; received[i] != '\0'; i++) {
        if(received[i] == '1')
            count++;
    }

    // Check parity condition
    if(choice == 1) {  // Even parity
        if(count % 2 == 0)
            printf("\nNo Error! Data received correctly.\n");
        else
            printf("\nError detected! Data is corrupted.\n");
    }
    else {             // Odd parity
        if(count % 2 != 0)
            printf("\nNo Error! Data received correctly.\n");
        else
            printf("\nError detected! Data is corrupted.\n");
    }

    return 0;
}
