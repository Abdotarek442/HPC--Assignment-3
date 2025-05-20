#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

double calculate_standard_deviation(int arr[], int n) {
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    double mean = sum / n;
    
    printf("Mean: %.6f\n", mean);
    
    double sum_squared_diff = 0.0;
    #pragma omp parallel for reduction(+:sum_squared_diff)
    for (int i = 0; i < n; i++) {
        double diff = arr[i] - mean;
        sum_squared_diff += diff * diff;
    }
    double variance = sum_squared_diff / n;
    
    printf("Variance: %.6f\n", variance);
    
    double std_dev = sqrt(variance);
    
    return std_dev;
}

int main() {
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);
    
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    int choice;
    printf("Choose input method:\n");
    printf("1. Enter values manually\n");
    printf("2. Generate random values\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        printf("Enter %d integers:\n", n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }
    } else if (choice == 2) {
        printf("Generating %d random integers...\n", n);
        srand(time(NULL));  
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 100;  // Random numbers between 0-99
        }
    } else {
        printf("Invalid choice. Exiting.\n");
        free(arr);
        return 1;
    }

    
    int num_threads = 4; 
    omp_set_num_threads(num_threads);
    printf("Using %d OpenMP threads\n", num_threads);
    
    double std_dev = calculate_standard_deviation(arr, n);
    
    printf("Standard Deviation: %.6f\n", std_dev);
    
    return 0;
}