#include <stdio.h>
#include <string.h>

#define MAX 100

int main() {
    char data[MAX];
    int i, count = 0, parityBit, choice;

    printf("SENDER SIDE:\n");

    printf("Enter binary data: ");
    scanf("%s", data);

    printf("Choose Parity Type:\n");
    printf("1. Even Parity\n");
    printf("2. Odd Parity\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    // Count number of 1's
    for(i = 0; data[i] != '\0'; i++) {
        if(data[i] == '1')
            count++;
    }

    // Generate parity bit
    if(choice == 1) {   // Even parity
        if(count % 2 == 0)
            parityBit = 0;
        else
            parityBit = 1;
    }
    else {              // Odd parity
        if(count % 2 == 0)
            parityBit = 1;
        else
            parityBit = 0;
    }

    printf("\nData to be transmitted: %s", data);
    printf("\nParity bit: %d", parityBit);

    printf("\n\nTransmitted data (Data + Parity): %s%d\n", data, parityBit);

    return 0;
}
