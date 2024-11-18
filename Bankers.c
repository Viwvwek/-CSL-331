#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

void main() {
    int processes, resources, i, j, count, flag, ml = 0, g = 0, ls = 0;
    
    // Input the number of processes and resources
    printf("Enter number of processes: ");
    scanf("%d", &processes);
    printf("Enter number of resources: ");
    scanf("%d", &resources);

    int max[processes][resources], alloc[processes][resources], need[processes][resources];
    int avail[resources], finish[processes], work[resources];

    // Input the max demand matrix
    printf("\nEnter the maximum demand matrix:\n");
    for (i = 0; i < processes; i++) {
        printf("Enter the maximum resources for Process %d:\n", i + 1);
        for (j = 0; j < resources; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // Input the allocation matrix
    printf("\nEnter the allocation matrix:\n");
    for (i = 0; i < processes; i++) {
        printf("Enter the allocated resources for Process %d:\n", i + 1);
        for (j = 0; j < resources; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    // Calculate the need matrix (Need = Max - Alloc)
    for (i = 0; i < processes; i++) {
        for (j = 0; j < resources; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    // Input the available resources
    printf("\nEnter the available resources:\n");
    for (i = 0; i < resources; i++) {
        scanf("%d", &avail[i]);
    }

    // Initialize finish array to 0 (not finished)
    for (i = 0; i < processes; i++) {
        finish[i] = 0;
    }

    // Start of the Banker's Algorithm (safety check)
    count = 0; // Count the number of processes that have finished
    while (count < processes) {
        flag = 0;  // Flag to check if a process can finish in the current iteration
        for (i = 0; i < processes; i++) {
            if (finish[i] == 0) {  // Process not finished
                // Check if all resources needed by this process can be allocated
                int canFinish = 1;
                for (j = 0; j < resources; j++) {
                    if (need[i][j] > avail[j]) {
                        canFinish = 0;  // Process cannot finish due to insufficient resources
                        break;
                    }
                }

                // If the process can finish, allocate resources to it
                if (canFinish) {
                    for (j = 0; j < resources; j++) {
                        avail[j] += alloc[i][j];  // Add allocated resources back to available pool
                    }
                    finish[i] = 1;  // Mark this process as finished
                    count++;         // Increment finished process count
                    printf("Process %d can finish. Available resources: [", i + 1);
                    for (j = 0; j < resources; j++) {
                        printf("%d ", avail[j]);
                    }
                    printf("]\n");
                    flag = 1;  // Mark that progress was made
                    break;     // Exit inner loop to restart the check with updated resources
                }
            }
        }
        
        // If no process can finish, break out of the loop
        if (flag == 0) {
            printf("\nSystem is in an unsafe state.\n");
            return;
        }
    }

    printf("\nSystem is in a safe state.\n");
}
