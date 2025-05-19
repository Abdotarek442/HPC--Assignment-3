#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void matrix_vector_mult(int n, int **A, int *v, int *r) {
    #pragma omp parallel for default(none) shared(A, v, r, n)
    for (int i = 0; i < n; i++) {
        r[i] = 0;
        for (int j = 0; j < n; j++) {
            r[i] += A[i][j] * v[j];
        }
    }
}

int main() {
    int n;

    printf("Enter the size of the matrix/vector (n): ");
    scanf("%d", &n);

    // Allocate memory for matrix
    int **A = (int **)malloc(n * sizeof(int *));
    if (!A) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    for (int i = 0; i < n; i++) {
        A[i] = (int *)malloc(n * sizeof(int));
        if (!A[i]) {
            printf("Memory allocation failed for row %d.\n", i);
            for (int k = 0; k < i; k++) free(A[k]); // Free previously allocated rows
            free(A);
            return 1;
        }
    }

    // Allocate memory for vector and result
    int *v = (int *)malloc(n * sizeof(int));
    int *r = (int *)malloc(n * sizeof(int));

    if (!v || !r) {
        printf("Memory allocation failed.\n");
        for (int i = 0; i < n; i++) free(A[i]);
        free(A);
        free(v);
        free(r);
        return 1;
    }

    // Input matrix
    printf("Enter elements of %dx%d matrix:\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    // Input vector
    printf("Enter elements of vector (%d elements):\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &v[i]);
    }

    // Perform matrix-vector multiplication
    matrix_vector_mult(n, A, v, r);

    // Output result
    printf("Resulting vector r:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", r[i]);
    }
    printf("\n");

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    free(v);
    free(r);

    return 0;
}
