#include <stdio.h>
#include <stdlib.h> // For abs() and qsort()

// Comparison function for sorting
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void main() {
    int n, i, head, direction, seek_time = 0;

    printf("Enter the number of requests: ");
    scanf("%d", &n);

    int requests[n + 1]; // Array to store requests + initial head

    printf("Enter the request sequence:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }

    printf("Enter the initial head position: ");
    scanf("%d", &head);

    printf("Enter the direction (0 for left, 1 for right): ");
    scanf("%d", &direction);

    // Include the initial head position in the list of requests
    requests[n] = head;

    // Sort the requests
    qsort(requests, n + 1, sizeof(int), compare);

    // Find the index of the head in the sorted array
    int head_index = 0;
    for (i = 0; i <= n; i++) {
        if (requests[i] == head) {
            head_index = i;
            break;
        }
    }

    printf("\nSeek sequence: ");

    // Scan to the right
    if (direction == 1) {
        // Process requests to the right of the head
        for (i = head_index; i <= n; i++) {
            printf("%d -> ", requests[i]);
            if (i != head_index) // Calculate seek time after moving
                seek_time += abs(requests[i] - requests[i - 1]);
        }
        // Process requests to the left of the head
        for (i = head_index - 1; i >= 0; i--) {
            printf("%d -> ", requests[i]);
            seek_time += abs(requests[i] - requests[i + 1]);
        }
    }
    // Scan to the left
    else {
        // Process requests to the left of the head
        for (i = head_index; i >= 0; i--) {
            printf("%d -> ", requests[i]);
            if (i != head_index) // Calculate seek time after moving
                seek_time += abs(requests[i] - requests[i + 1]);
        }
        // Process requests to the right of the head
        for (i = head_index + 1; i <= n; i++) {
            printf("%d -> ", requests[i]);
            seek_time += abs(requests[i] - requests[i - 1]);
        }
    }

    printf("\b\b  \n"); // Remove trailing "->"
    printf("\nTotal seek time: %d\n", seek_time);
}
