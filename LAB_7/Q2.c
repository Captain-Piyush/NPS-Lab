#include <stdio.h>

int main() {
    double bucket_size, refill_rate, packet_size;
    int n;
    
    printf("--- Token Bucket Configuration ---\n");
    printf("Enter Bucket Capacity (Max Tokens): ");
    scanf("%lf", &bucket_size);
    printf("Enter Token Refill Rate (tokens/sec): ");
    scanf("%lf", &refill_rate);
    printf("Enter Tokens required per packet: ");
    scanf("%lf", &packet_size);
    printf("Enter number of packets to simulate: ");
    scanf("%d", &n);

    double arrival_times[n];
    printf("Enter the arrival times for %d packets: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%lf", &arrival_times[i]);
    }

    double tokens = bucket_size; 
    double last_refill_time = 0.0;

    printf("\n%-10s %-12s %-12s %-12s %-15s\n", "Time(s)", "Added", "Before", "After", "Status");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        double current_time = arrival_times[i];
        
        double time_passed = current_time - last_refill_time;
        double added_tokens = time_passed * refill_rate;
        
        double tokens_before = tokens + added_tokens;
        
        if (tokens_before > bucket_size) {
            tokens_before = bucket_size;
        }

        printf("%-10.2f %-12.2f %-12.2f ", current_time, added_tokens, tokens_before);

        if (tokens_before >= packet_size) {
            tokens = tokens_before - packet_size;
            printf("%-12.2f %-15s\n", tokens, "ACCEPTED");
        } else {
            tokens = tokens_before;
            printf("%-12.2f %-15s\n", tokens, "DROPPED");
        }

        last_refill_time = current_time;
    }

    return 0;
}