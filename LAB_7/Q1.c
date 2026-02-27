#include <stdio.h>

int main() {
    int bucket_size, out_rate, packet_size, n;
    int bucket_content = 0;
    int last_time = 0;

    printf("Enter Bucket Size (Capacity): ");
    scanf("%d", &bucket_size);
    printf("Enter Output (Leak) Rate: ");
    scanf("%d", &out_rate);
    printf("Enter size of each incoming packet: ");
    scanf("%d", &packet_size);
    printf("Enter number of packets to simulate: ");
    scanf("%d", &n);

    int arrival_times[n];
    printf("Enter the arrival times for %d packets (space-separated): ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arrival_times[i]);
    }

    printf("\nTime\tPacket\tLeaked\tContent\tStatus\n");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        int current_time = arrival_times[i];
        
        int time_gap = current_time - last_time;
        int leaked = time_gap * out_rate;
        
        bucket_content -= leaked;
        if (bucket_content < 0) bucket_content = 0;

        printf("%d\t%d B\t%d B\t", current_time, packet_size, leaked);

        // Check for overflow (Bucket Capacity)
        if (bucket_content + packet_size <= bucket_size) {
            bucket_content += packet_size;
            printf("%d B\tCONFORMING (Accepted)\n", bucket_content);
        } else {
            // Content stays the same because the packet was dropped
            printf("%d B\tNON-CONFORMING (Dropped)\n", bucket_content);
        }

        last_time = current_time;
    }

    return 0;
}