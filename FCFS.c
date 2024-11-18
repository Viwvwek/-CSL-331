#include <stdio.h>
#include <stdlib.h> // For abs()

void main() {
    int n, i, head, seek_time = 0;

    printf("Enter the number of requests: ");
    scanf("%d", &n);

    int requests[n];

    printf("Enter the request sequence:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }

    printf("Enter the initial head position: ");
    scanf("%d", &head);

    printf("\nSeek sequence: %d", head);

    // Calculate total seek time
    for (i = 0; i < n; i++) {
        seek_time += abs(requests[i] - head); // Distance between head and request
        head = requests[i];                  // Move head to the current request
        printf(" -> %d", head);              // Print seek sequence
    }

    printf("\n\nTotal seek time: %d\n", seek_time);
}
