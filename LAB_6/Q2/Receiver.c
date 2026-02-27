#include <stdio.h>
#include <string.h>

unsigned int crc12(unsigned char *data, int len);
unsigned int crc16(unsigned char *data, int len);
unsigned int crc_ccitt(unsigned char *data, int len);

// Same CRC functions as sender (copy exactly)
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
    unsigned int received_crc, calculated_crc;
    int choice;

    printf("RECEIVER SIDE:\n");
    printf("Enter received data: ");
    scanf("%s", data);

    printf("Enter received CRC (in HEX): ");
    scanf("%X", &received_crc);

    printf("\nChoose CRC Type Used:\n");
    printf("1. CRC-12\n");
    printf("2. CRC-16\n");
    printf("3. CRC-CCITT\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    int len = strlen(data);

    switch(choice) {
        case 1: calculated_crc = crc12((unsigned char*)data, len); break;
        case 2: calculated_crc = crc16((unsigned char*)data, len); break;
        case 3: calculated_crc = crc_ccitt((unsigned char*)data, len); break;
        default: printf("Invalid choice\n"); return 0;
    }

    if(calculated_crc == received_crc)
        printf("\nData received correctly. No error detected.\n");
    else
        printf("\nData is corrupted! Error detected.\n");

    return 0;
}
