#include <stdio.h>
#include <math.h>

int main() {
    int data[20], hamming[30];
    int m, r = 0, i, j, k = 0;
    int error_pos = 0;

    printf("----- HAMMING CODE -----\n");

    // Input number of data bits
    printf("Enter number of data bits: ");
    scanf("%d", &m);

    printf("Enter data bits (one by one):\n");
    for(i = 1; i <= m; i++)
        scanf("%d", &data[i]);

    // Calculate number of parity bits required
    while(pow(2, r) < (m + r + 1))
        r++;

    int total = m + r;

    // Place data bits and parity positions
    for(i = 1, j = 1; i <= total; i++) {
        if(i == pow(2, k)) {
            hamming[i] = 0;  // parity bit placeholder
            k++;
        } else {
            hamming[i] = data[j];
            j++;
        }
    }

    // Calculate parity bits (even parity)
    for(i = 0; i < r; i++) {
        int pos = pow(2, i);
        int parity = 0;

        for(j = pos; j <= total; j++) {
            if(j & pos)
                parity ^= hamming[j];
        }
        hamming[pos] = parity;
    }

    printf("\nTransmitted Hamming Code: ");
    for(i = 1; i <= total; i++)
        printf("%d ", hamming[i]);


    printf("\n\nEnter received Hamming code:\n");
    for(i = 1; i <= total; i++)
        scanf("%d", &hamming[i]);

    // Detect error
    for(i = 0; i < r; i++) {
        int pos = pow(2, i);
        int parity = 0;

        for(j = pos; j <= total; j++) {
            if(j & pos)
                parity ^= hamming[j];
        }

        if(parity != 0)
            error_pos += pos;
    }
    printf("HI");
    printf("update");
    printf("kdfjdsk");

    if(error_pos == 0) {
        printf("\nNo error detected.\n");
    } else {
        printf("\nError detected at position: %d\n", error_pos);

        hamming[error_pos] ^= 1;

        printf("Corrected Hamming Code: ");
        for(i = 1; i <= total; i++)
            printf("%d ", hamming[i]);
        printf("\n");
    }

    return 0;
}
