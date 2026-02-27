#include <stdio.h>
#include <string.h>

unsigned int crc12(unsigned char *data, int len) {
    unsigned int crc = 0;
    unsigned int poly = 0x80F;

    for(int i = 0; i < len; i++) {
        crc ^= (data[i] << 4);
        for(int j = 0; j < 8; j++) {
            if(crc & 0x800)
                crc = (crc << 1) ^ poly;
            else
                crc <<= 1;
        }
    }
    return crc & 0xFFF;
}

unsigned int crc16(unsigned char *data, int len) {
    unsigned int crc = 0;
    unsigned int poly = 0x8005;

    for(int i = 0; i < len; i++) {
        crc ^= (data[i] << 8);
        for(int j = 0; j < 8; j++) {
            if(crc & 0x8000)
                crc = (crc << 1) ^ poly;
            else
                crc <<= 1;
        }
    }
    return crc & 0xFFFF;
}

unsigned int crc_ccitt(unsigned char *data, int len) {
    unsigned int crc = 0xFFFF;
    unsigned int poly = 0x1021;

    for(int i = 0; i < len; i++) {
        crc ^= (data[i] << 8);
        for(int j = 0; j < 8; j++) {
            if(crc & 0x8000)
                crc = (crc << 1) ^ poly;
            else
                crc <<= 1;
        }
    }
    return crc & 0xFFFF;
}

int main() {
    char data[100];
    int choice;
    unsigned int crc;

    printf("SENDER SIDE:\n");
    printf("Enter data (characters): ");
    scanf("%s", data);

    printf("\nChoose CRC Type:\n");
    printf("1. CRC-12\n");
    printf("2. CRC-16\n");
    printf("3. CRC-CCITT\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    int len = strlen(data);

    switch(choice) {
        case 1: crc = crc12((unsigned char*)data, len); break;
        case 2: crc = crc16((unsigned char*)data, len); break;
        case 3: crc = crc_ccitt((unsigned char*)data, len); break;
        default: printf("Invalid choice\n"); return 0;
    }

    printf("\nData to transmit: %s", data);
    printf("\nGenerated CRC: %X", crc);
    printf("\nTransmitted Frame: %s%X\n", data, crc);

    return 0;
}
